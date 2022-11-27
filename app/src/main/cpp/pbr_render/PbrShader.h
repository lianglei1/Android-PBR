#ifndef PbrShader_H
#define PbrShader_H

#include <glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GLES3/gl3.h>



class PbrShader
{
public:

	PbrShader(const std::string vertexPath, const std::string fragmentPath);
	~PbrShader();

	//Delete the copy constructor/assignment.
	PbrShader(const PbrShader &) = delete;
	PbrShader &operator=(const PbrShader &) = delete;

	PbrShader(PbrShader &&other) : mID(other.mID)
	{
		other.mID = 0; //Use the "null" texture for the old object.
	}

	PbrShader &operator=(PbrShader &&other)
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
	void use() const;

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
	GLuint mID = 0;

	void release();
};

#endif