#include "gltexture.h"
#include "GLEW/glew.h"
#include "exception.h"

GLTexture::GLTexture(unsigned int width, unsigned int height, void* data, Format format)
{
    check_argument(width  > 0);
    check_argument(height > 0);
    check_argument(data != NULL);

    glGenTextures(1, &handle);
    bind();

    check_assert(format == Format::RGB565);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &handle);
}

void GLTexture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, handle);
}
