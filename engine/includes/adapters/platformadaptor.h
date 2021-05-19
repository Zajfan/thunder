#ifndef PLATFORMADAPTER_H
#define PLATFORMADAPTER_H

#include <cstdint>
#include <string>

#include <amath.h>

#include "input.h"

extern int thunderMain(Engine *engine);

class PlatformAdaptor {
public:
    virtual ~PlatformAdaptor            () {}

    virtual bool                        init                        () = 0;

    virtual void                        update                      () = 0;

    virtual bool                        start                       () = 0;

    virtual void                        stop                        () = 0;

    virtual void                        destroy                     () = 0;

    virtual bool                        isValid                     () = 0;

    virtual uint32_t                    screenWidth                 () = 0;

    virtual uint32_t                    screenHeight                () = 0;

    virtual bool                        key                         (Input::KeyCode code) { A_UNUSED(code); return false; }
    virtual bool                        keyPressed                  (Input::KeyCode code) { A_UNUSED(code); return false; }
    virtual bool                        keyReleased                 (Input::KeyCode code) { A_UNUSED(code); return false; }

    virtual string                      inputString                 () = 0;
    virtual void                        setKeyboardVisible          (bool visible) { A_UNUSED(visible); }

    virtual Vector4                     mousePosition               () { return Vector4(); }

    virtual Vector4                     mouseDelta                  () { return Vector4(); }

    virtual bool                        mouseButton                 (Input::MouseButton code) { A_UNUSED(code); return false; }
    virtual bool                        mousePressed                (Input::MouseButton code) { A_UNUSED(code); return false; }
    virtual bool                        mouseReleased               (Input::MouseButton code) { A_UNUSED(code); return false; }
    virtual void                        setMousePosition            (int32_t x, int32_t y) { A_UNUSED(x); A_UNUSED(y); }

    virtual uint32_t                    joystickCount               () { return 0; }
    virtual uint32_t                    joystickButtons             (uint32_t index) { A_UNUSED(index); return 0; }
    virtual Vector4                     joystickThumbs              (uint32_t index) { A_UNUSED(index); return Vector4(); }
    virtual Vector2                     joystickTriggers            (uint32_t index) { A_UNUSED(index); return Vector2(); }

    virtual uint32_t                    touchCount                  () { return 0; }
    virtual uint32_t                    touchState                  (uint32_t index) { A_UNUSED(index); return 0; }
    virtual Vector4                     touchPosition               (uint32_t index) { A_UNUSED(index); return 0; }

    virtual void                       *pluginLoad                  (const char *name) { A_UNUSED(name); return nullptr; }

    virtual bool                        pluginUnload                (void *plugin) { A_UNUSED(plugin); return false; }

    virtual void                       *pluginAddress               (void *plugin, const string &name) { A_UNUSED(plugin); A_UNUSED(name); return nullptr; }

    virtual string                      locationLocalDir            () { return string(); }

    virtual void                        syncConfiguration           (VariantMap &map) const { A_UNUSED(map); }

};

#endif // PLATFORMADAPTER_H
