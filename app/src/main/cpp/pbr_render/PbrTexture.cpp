#include "PbrTexture.h"
#include <stb_image.h>
#include <iostream>
#include <vector>
#include "../util/LogUtil.h"

PbrTexture::PbrTexture() {
    glGenTextures(1, &mID);
}

PbrTexture::PbrTexture(const std::string PbrTexturePath, bool srgb) {
    glGenTextures(1, &mID);
    loadTexture(PbrTexturePath.c_str(), srgb);
}

PbrTexture::~PbrTexture() {
    release();
}

GLuint PbrTexture::getId() const {
    return mID;
}

void PbrTexture::bind(GLenum PbrTextureUnit) const {
    glActiveTexture(PbrTextureUnit);
    glBindTexture(GL_TEXTURE_2D, mID);
}

void PbrTexture::loadTexture(const GLchar *PbrTexturePath, bool srgb) {
    glBindTexture(GL_TEXTURE_2D, mID);
    // set the PbrTexture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set PbrTexture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create PbrTexture and generate mipmaps
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(PbrTexturePath, &width, &height, &nrChannels, 4);
    if (data) {
        GLenum format = GL_RGBA;

        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            //TODO: debug why GL_RGBA 才能正常显示
            format = GL_RGBA;
            //  format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        if (srgb) {
            format = GL_SRGB8_ALPHA8;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {

        LOGCATI("Failed to load PbrTexture%s", PbrTexturePath);
    }

    stbi_image_free(data);
}

void PbrTexture::release() {
    glDeleteTextures(1, &mID);
    mID = 0;
}