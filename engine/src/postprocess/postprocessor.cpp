#include "postprocess/postprocessor.h"

#include "resources/mesh.h"
#include "resources/material.h"
#include "resources/rendertexture.h"
#include "resources/pipeline.h"

#include "commandbuffer.h"

#include "filters/blur.h"

static Blur *s_pBlur = nullptr;

PostProcessor::PostProcessor() :
        m_Enabled(false),
        m_pResultTexture(nullptr),
        m_pMaterial(nullptr) {

    m_pMesh = Engine::loadResource<Mesh>(".embedded/plane.fbx/Plane001");
}

PostProcessor::~PostProcessor() {

}

RenderTexture *PostProcessor::draw(RenderTexture *source, Pipeline *pipeline) {
    if(m_Enabled && m_pMaterial && m_pResultTexture) {
        m_pMaterial->setTexture("rgbMap", source);

        ICommandBuffer *buffer = pipeline->buffer();

        buffer->setRenderTarget({m_pResultTexture});
        buffer->drawMesh(Matrix4(), m_pMesh, ICommandBuffer::UI, m_pMaterial);

        return m_pResultTexture;
    }
    return source;
}

void PostProcessor::resize(int32_t width, int32_t height) {
    if(m_pResultTexture) {
        m_pResultTexture->resize(width, height);
    }
}

void PostProcessor::setSettings(const PostProcessSettings &) {

}

uint32_t PostProcessor::layer() const {
    return ICommandBuffer::TRANSLUCENT;
}

void PostProcessor::setEnabled(bool value) {
    m_Enabled = value;
}

Blur *PostProcessor::blur() {
    if(s_pBlur == nullptr) {
        s_pBlur = new Blur();
    }
    return s_pBlur;
}
