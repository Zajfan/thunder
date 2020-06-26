#ifndef TEXTURE_H
#define TEXTURE_H

#include <amath.h>

#include "resource.h"

class TexturePrivate;

class NEXT_LIBRARY_EXPORT Texture : public Resource {
    A_REGISTER(Texture, Resource, Resources)

public:
    enum TextureType {
        Flat,
        Cubemap
    };

    enum FormatType {
        R8,
        RGB8,
        RGBA8,
        RGB10A2,
        RGB16Float,
        R11G11B10Float,
        Depth,
        RGBA32Float
    };

    enum CompressionType {
        Uncompressed,
        DXT1,
        DXT5,
        ETC2
    };

    enum FilteringType {
        None,
        Bilinear,
        Trilinear
    };

    enum WrapType {
        Clamp,
        Repeat,
        Mirrored
    };

    typedef deque<uint8_t *> Surface;
    typedef deque<Surface>   Sides;

public:
    Texture ();
    ~Texture ();

    virtual void *nativeHandle ();

    virtual void readPixels (int32_t x, int32_t y, int32_t width, int32_t height);
    uint32_t getPixel (int32_t x, int32_t y) const;

    int32_t width () const;
    void setWidth (int32_t width);

    int32_t height () const;
    void setHeight (int32_t height);

    Vector2Vector shape () const;
    void setShape (const Vector2Vector &shape);

    bool isCompressed () const;
    bool isCubemap () const;

    Surface &surface (uint32_t face);
    void addSurface (const Surface &surface);

    void setDirty ();

    void resize (int32_t width, int32_t height);

    FormatType format () const;
    void setFormat (FormatType type);

    WrapType wrap () const;
    void setWrap (WrapType type);

    FilteringType filtering () const;
    void setFiltering (FilteringType type);

    void loadUserData (const VariantMap &data) override;

private:
    TexturePrivate *p_ptr;

protected:
    TextureType type () const;
    void setType (TextureType type);

    void clear ();

    Sides *getSides ();

    int32_t size (int32_t width, int32_t height) const;
    int32_t sizeDXTc (int32_t width, int32_t height) const;
    int32_t sizeRGB (int32_t width, int32_t height) const;

    uint8_t components () const;
};

#endif // TEXTURE_H
