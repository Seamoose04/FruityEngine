#include "Renderer.h"
#include <glad/glad.h>
#include <iostream>
#include "game/Properties/Camera.h"

Renderer::Renderer(int width, int height)
    : _width(width), _height(height),
    _clearColor(0.0f, 0.0f, 0.0f, 1.0f),
    _hdrBuffer(width, height, GL_RGBA16F),
	_msaaBuffer(width, height, GL_RGBA16F, 4)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenBuffers(1, &_lightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, _lightUBO);

	int bufferSize = MAX_POINT_LIGHTS * 32 + 32;
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, _lightUBO);

    PostProcessEffect::InitQuad();

    std::cout << "Renderer initialized (" << width << "x" << height << ")\n";
}

Renderer::~Renderer() {
	glDeleteBuffers(1, &_lightUBO);
}

void Renderer::BeginFrame() {
	_camera = nullptr;
	_renderQueue.clear();
	_pointLights.clear();

    _msaaBuffer.Bind();
    glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame() {
	_FlushQueue();

    _msaaBuffer.Unbind();
	_msaaBuffer.ResolveTo(_hdrBuffer);

    if (_postProcessOutput) {
        _postProcessOutput->Execute();
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, _width, _height);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Renderer::SetCamera(const Camera* camera, const glm::vec3& position) {
	if (_camera != nullptr) {
		std::cerr << "[Renderer] Error: SetCamera called more than once per frame. Only one camera is allowed." << std::endl;
		return;
	}
	_camera = camera;
	_cameraPos = position;
}

void Renderer::SubmitMesh(const Mesh& mesh, std::shared_ptr<Material> material, const glm::mat4& modelMatrix) {
	_renderQueue.push_back({ &mesh, material, modelMatrix });
}

void Renderer::SubmitPointLight(const PointLight& light) {
	if ((int)_pointLights.size() >= MAX_POINT_LIGHTS) {
        std::cerr << "[Renderer] Warning: MAX_POINT_LIGHTS (" << MAX_POINT_LIGHTS << ") exceeded. Light ignored.\n";
        return;
    }
    _pointLights.push_back(&light);
}

void Renderer::SetAmbientLight(const AmbientLight& light) {
	_ambientLight = light;
}

void Renderer::_UploadLightUBO() {
	glBindBuffer(GL_UNIFORM_BUFFER, _lightUBO);

	int count = (int)_pointLights.size();
	for (int i = 0; i < count; i++) {
		const PointLight* l = _pointLights[i];
		int offset = i * 32;

		glm::vec4 posAndRadius(l->position, l->radius);
		glm::vec4 colorAndIntensity(l->color, l->intensity);

		glBufferSubData(GL_UNIFORM_BUFFER, offset, 16, &posAndRadius);
		glBufferSubData(GL_UNIFORM_BUFFER, offset + 16, 16, &colorAndIntensity);
	}

	int ambientOffset = MAX_POINT_LIGHTS * 32;
	glm::vec4 ambientData(_ambientLight.color, _ambientLight.intensity);
	glBufferSubData(GL_UNIFORM_BUFFER, ambientOffset, 16, &ambientData);

	glBufferSubData(GL_UNIFORM_BUFFER, ambientOffset + 16, 4, &count);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::_FlushQueue() {
	if (_camera == nullptr) {
		std::cerr << "[Renderer] Error: No camera set this frame. Nothing will be drawn." << std::endl;
		return;
	}

	_UploadLightUBO();

	for (const RenderCommand& cmd : _renderQueue) {
		glm::mat4 mvp = _camera->GetProjection() * _camera->GetView() * cmd.modelMatrix;
		cmd.material->Apply();
		cmd.material->GetShader()->SetMat4("u_MVP", mvp);
		cmd.material->GetShader()->SetMat4("u_Model", cmd.modelMatrix);
		cmd.material->GetShader()->SetVec3("u_CameraPos", _cameraPos);
		cmd.mesh->Bind();
		glDrawElements(GL_TRIANGLES, cmd.mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::SetPostProcessGraph(SceneEffect* sceneNode, RenderToScreenEffect* outputNode) {
    sceneNode->SetHDRBuffer(&_hdrBuffer);

    _postProcessOutput = outputNode;
    std::cout << "Post-process graph registered" << std::endl;
}

void Renderer::ClearPostProcessGraph() {
    _postProcessOutput = nullptr;
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    _clearColor = glm::vec4(r, g, b, a);
}

void Renderer::Resize(int w, int h) {
	_width = w;
	_height = h;
    _hdrBuffer.Resize(w, h);
	_msaaBuffer.Resize(w, h);

    if (_postProcessOutput) {
        _postProcessOutput->Resize(w, h);
    }
}

int Renderer::GetWidth() const {
    return _width;
}

int Renderer::GetHeight() const {
    return _height;
}
