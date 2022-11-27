#ifndef PbrMesh_H
#define PbrMesh_H

#include <vector>
#include <glm.hpp>
#include "PbrShader.h"

#include <GLES3/gl3.h>


struct PbrVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec2 TexCoords;
};

class PbrMesh {
public:
    PbrMesh();

    virtual ~PbrMesh() = 0;

    virtual void draw(const std::shared_ptr<PbrShader> shader);

private:
    unsigned int mVAO, mVBO, mEBO;
    int mIndexCount = 0;

protected:
    GLenum mPrimitive = GL_TRIANGLES;

    void
    setupMesh(const std::vector<PbrVertex> &vertices, const std::vector<unsigned int> &indices);
};

#endif //PbrMesh_H