#include "Scene.h"

Framebuffer* SceneEffect::GetOutput() {
    return _hdrBuffer;
}

void SceneEffect::SetHDRBuffer(Framebuffer* buffer) {
    _hdrBuffer = buffer;
}