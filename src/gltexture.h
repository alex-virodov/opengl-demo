#ifndef __GLTEXTURE_H__
#define __GLTEXTURE_H__

class GLTexture
{
public:
    enum class Format
    {
        RGB565,
        // todo: others
    };

    GLTexture(unsigned int width, unsigned int height, void* data, Format format);
    ~GLTexture();

    void bind() const;

private:
    // note: should be GLuint, but want to avoid #include'ing gl.h
    unsigned int handle;
};

#endif