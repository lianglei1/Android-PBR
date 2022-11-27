#ifndef PBRCUBEMAP_H
#define PBRCUBEMAP_H

#include <string>
#include <vector>
#include <GLES3/gl3.h>


class PbrCubeMap
{
public:
	PbrCubeMap();
	~PbrCubeMap();

	//Delete the copy constructor/assignment.
	PbrCubeMap(const PbrCubeMap &) = delete;
	PbrCubeMap &operator=(const PbrCubeMap &) = delete;

	PbrCubeMap(PbrCubeMap &&other) : mID(other.mID)
	{
		other.mID = 0; //Use the "null" texture for the old object.
	}

	PbrCubeMap &operator=(PbrCubeMap &&other)
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
	void bind(GLenum textureUnit) const;

private:
	GLuint mID = 0;

	void release();
};

#endif //PBRCUBEMAP_H