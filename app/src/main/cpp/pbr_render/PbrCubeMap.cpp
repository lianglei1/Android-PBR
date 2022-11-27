#include "PbrCubeMap.h"
#include <stb_image.h>
#include <iostream>

PbrCubeMap::PbrCubeMap()
{
	glGenTextures(1, &mID);
}

PbrCubeMap::~PbrCubeMap()
{
	release();
}

GLuint PbrCubeMap::getId() const
{
	return mID;
}

void PbrCubeMap::bind(GLenum textureUnit) const
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
}

void PbrCubeMap::release()
{
	glDeleteTextures(1, &mID);
	mID = 0;
}