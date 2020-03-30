#ifndef CAMERA_H
#define CAMERA_H

#include <array>

#include "nativebehaviour.h"

class Pipeline;
class CameraPrivate;

class NEXT_LIBRARY_EXPORT Camera : public Component {
    A_REGISTER(Camera, Component, Components)

    A_PROPERTIES(
        A_PROPERTY(float, Fov,  Camera::fov, Camera::setFov),
        A_PROPERTY(float, Near, Camera::nearPlane, Camera::setNear),
        A_PROPERTY(float, Far,  Camera::farPlane, Camera::setFar),
        A_PROPERTY(float, Size, Camera::orthoSize, Camera::setOrthoSize),
        A_PROPERTY(float, Focal_Distance, Camera::focal, Camera::setFocal),
        A_PROPERTYEX(Vector4, Background_Color, Camera::color, Camera::setColor, "editor=Color"),
        A_PROPERTY(bool, Orthographic, Camera::orthographic, Camera::setOrthographic)
    )
    A_NOMETHODS()

public:
    Camera ();
    ~Camera ();

    Pipeline *pipeline ();

    void setPipeline (Pipeline *pipeline);

    void matrices (Matrix4 &view, Matrix4 &projection) const;

    Matrix4 projectionMatrix () const;

    static bool project (const Vector3 &worldSpace, const Matrix4 &modelView, const Matrix4 &projection, Vector3 &screenSpace);
    static bool unproject (const Vector3 &screenSpace, const Matrix4 &modelView, const Matrix4 &projection, Vector3 &worldSpace);

    Ray castRay (float x, float y);

    float ratio () const;
    void setRatio (float ratio);

    float nearPlane () const;
    void setNear (const float value);

    float farPlane () const;
    void  setFar (const float value);

    float focal () const;
    void setFocal (const float focal);

    float fov () const;
    void setFov (const float value);

    Vector4 color () const;
    void setColor (const Vector4 &color);

    float orthoSize () const;
    void setOrthoSize (const float size);

    bool orthographic () const;
    void setOrthographic (const bool mode);

    static Camera *current ();
    static void setCurrent (Camera *current);

    static array<Vector3, 8> frustumCorners (const Camera &camera);
    static array<Vector3, 8> frustumCorners (bool ortho, float sigma, float ratio, const Vector3 &position, const Quaternion &rotation, float nearPlane, float farPlane);
    static Object::ObjectList frustumCulling(ObjectList &list, const array<Vector3, 8> &frustum);

private:
#ifdef NEXT_SHARED
    bool drawHandles(bool selected) override;
#endif

private:
    CameraPrivate *p_ptr;

};

#endif // CAMERA_H
