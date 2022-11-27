#include "PbrMesh.h"

PbrMesh::PbrMesh() {}

PbrMesh::~PbrMesh() {}

void PbrMesh::draw(const std::shared_ptr<PbrShader> shader) {
    shader->use();

    // draw PbrMesh
    glBindVertexArray(mVAO);
    glDrawElements(mPrimitive, mIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void
PbrMesh::setupMesh(const std::vector<PbrVertex> &vertices, const std::vector<unsigned int> &indices) {
    mIndexCount = indices.size();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PbrVertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0],
                 GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PbrVertex), (void *) 0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PbrVertex),
                          (void *) offsetof(PbrVertex, Normal));
    // vertex tangent
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PbrVertex),
                          (void *) offsetof(PbrVertex, Tangent));
    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(PbrVertex),
                          (void *) offsetof(PbrVertex, TexCoords));

    glBindVertexArray(0);
}