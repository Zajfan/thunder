#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "baselight.h"

class PointLightPrivate;

class NEXT_LIBRARY_EXPORT PointLight : public BaseLight {
    A_REGISTER(PointLight, BaseLight, Components)

    A_PROPERTIES(
        A_PROPERTY(float, Attenuation_Radius, PointLight::radius, PointLight::setRadius),
        A_PROPERTY(float, Source_Radius,      PointLight::sourceRadius, PointLight::setSourceRadius),
        A_PROPERTY(float, Source_Length,      PointLight::sourceLength, PointLight::setSourceLength)
    )

public:
    PointLight ();
    ~PointLight () override;

    float radius () const;
    void setRadius (float radius);

    float sourceRadius () const;
    void setSourceRadius (float radius);

    float sourceLength () const;
    void setSourceLength (float length);

private:
    void draw (ICommandBuffer &buffer, uint32_t layer) override;

    void shadowsUpdate (const Camera &camera, Pipeline *pipeline, ObjectList &components) override;

    AABBox bound () const override;

#ifdef NEXT_SHARED
    bool drawHandles(ObjectList &selected) override;
#endif

private:
    PointLightPrivate *p_ptr;

};

#endif // POINTLIGHT_H
