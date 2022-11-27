#ifndef PbrTexture_H
#define PbrTexture_H

#include <string>
#include <GLES3/gl3.h>


class PbrTexture
{
public:
	PbrTexture();
	PbrTexture(const std::string PbrTexturePath, bool srgb = false);
	~PbrTexture();

	//Delete the copy constructor/assignment.
	PbrTexture(const PbrTexture &) = delete;
	PbrTexture &operator=(const PbrTexture &) = delete;

	PbrTexture(PbrTexture &&other) : mID(other.mID)
	{
		other.mID = 0; //Use the "null" PbrTexture for the old object.
	}

	PbrTexture &operator=(PbrTexture &&other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			release();
			//ID is now 0.
			std::swap(mID, other.mID);
		}
	}

	GLuint getId() const;
	void bind(GLenum PbrTextureUnit) const;

private:
	GLuint mID = 0;

	void loadTexture(const GLchar* PbrTexturePath, bool srgb);
	void release();
};

#endif //PbrTexture_H