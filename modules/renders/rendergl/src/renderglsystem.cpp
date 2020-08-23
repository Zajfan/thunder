#include "renderglsystem.h"

#include "agl.h"

#include <components/camera.h>
#include <components/scene.h>

#include <resources/pipeline.h>


#include "resources/atexturegl.h"
#include "resources/arendertexturegl.h"

#include "commandbuffergl.h"

#include <log.h>

#define MAX_RESOLUTION 8192

void _CheckGLError(const char* file, int line) {
    GLenum err ( glGetError() );

    while ( err != GL_NO_ERROR ) {
        std::string error;
        switch ( err ) {
            case GL_INVALID_OPERATION:  error="GL_INVALID_OPERATION";      break;
            case GL_INVALID_ENUM:       error="GL_INVALID_ENUM";           break;
            case GL_INVALID_VALUE:      error="GL_INVALID_VALUE";          break;
            case GL_OUT_OF_MEMORY:      error="OUT_OF_MEMORY";          break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error="GL_INVALID_FRAMEBUFFER_OPERATION";  break;
        }
        Log(Log::DBG) << error.c_str() <<" - " << file << ":" << line;
        err = glGetError();
    }
    return;
}

RenderGLSystem::RenderGLSystem(Engine *engine) :
        System(),
        m_pEngine(engine) {
    PROFILE_FUNCTION();

    System *system = m_pEngine->resourceSystem();

    ATextureGL::registerClassFactory(system);
    ARenderTextureGL::registerClassFactory(system);
    AMaterialGL::registerClassFactory(system);
    AMeshGL::registerClassFactory(system);

    CommandBufferGL::registerClassFactory(m_pEngine);
}

RenderGLSystem::~RenderGLSystem() {
    PROFILE_FUNCTION();

    System *system = m_pEngine->resourceSystem();

    ATextureGL::unregisterClassFactory(system);
    ARenderTextureGL::unregisterClassFactory(system);
    AMaterialGL::unregisterClassFactory(system);
    AMeshGL::unregisterClassFactory(system);

    CommandBufferGL::unregisterClassFactory(m_pEngine);
}

/*!
    Initialization of render.
*/
bool RenderGLSystem::init() {
    PROFILE_FUNCTION();

#ifndef THUNDER_MOBILE
    if(!gladLoadGL()) {
        CheckGLError();
        Log(Log::ERR) << "[ Render::RenderGLSystem ] Failed to initialize OpenGL context";
        return false;
    }
    glEnable        (GL_TEXTURE_CUBE_MAP_SEAMLESS);
    CheckGLError();
#endif

    int32_t targets;
    glGetIntegerv	(GL_MAX_DRAW_BUFFERS, &targets);
    CheckGLError();

    int32_t texture;
    glGetIntegerv	(GL_MAX_TEXTURE_SIZE, &texture);
    CheckGLError();

    texture = MIN(texture, MAX_RESOLUTION);

    Pipeline::setShadowPageSize(texture, texture);

    CommandBufferGL::setInited();

    return true;
}

const char *RenderGLSystem::name() const {
    return "RenderGL";
}

/*!
    Main drawing procedure.
*/
void RenderGLSystem::update(Scene *scene) {
    PROFILE_FUNCTION();

    PROFILER_RESET(POLYGONS);
    PROFILER_RESET(DRAWCALLS);

    Camera *camera  = Camera::current();
    if(camera && CommandBufferGL::isInited()) {
        int32_t target;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &target);

        Pipeline *pipe  = camera->pipeline();
        pipe->setTarget(target);
        pipe->analizeScene(scene);
        pipe->draw(*camera);
        pipe->finish();
    }
}

bool RenderGLSystem::isThreadSafe() const {
    return false;
}
