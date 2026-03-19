#pragma once

#include "glm/glm.hpp"
#include "Mesh.h"
#include "Framebuffer.h"
#include "effects/Scene.h"
#include "effects/RenderToScreen.h"
#include "graphics/Material.h"
#include "lights/AmbientLight.h"
#include "lights/PointLight.h"

class Camera;

struct RenderCommand {
	const Mesh* mesh;
	std::shared_ptr<Material> material;
	glm::mat4 modelMatrix;
};

class Renderer {
public:
	static constexpr int MAX_POINT_LIGHTS = 8;

    Renderer(int width, int height);
    ~Renderer();

    void BeginFrame();
    void EndFrame();

    void SetClearColor(float r, float g, float b, float a);
    void Resize(int width, int height);

	void SetCamera(const Camera* camera, const glm::vec3& position);
	void SubmitMesh(const Mesh& mesh, std::shared_ptr<Material> material, const glm::mat4& modelMatrix);
	void SubmitPointLight(const PointLight& light);
	void SetAmbientLight(const AmbientLight& light);

    void SetPostProcessGraph(SceneEffect* screenNode, RenderToScreenEffect* outputNode);
    void ClearPostProcessGraph();

    int GetWidth() const;
    int GetHeight() const;

private:
	void _FlushQueue();
	void _UploadLightUBO();

    int _width, _height;
    glm::vec4 _clearColor;
    Framebuffer _hdrBuffer;
	Framebuffer _msaaBuffer;
    RenderToScreenEffect* _postProcessOutput = nullptr;

	const Camera* _camera = nullptr;
	glm::vec3 _cameraPos = glm::vec3(0.0f);
	std::vector<RenderCommand> _renderQueue;
	std::vector<const PointLight*> _pointLights;
	AmbientLight _ambientLight;

	unsigned int _lightUBO = 0;
};
