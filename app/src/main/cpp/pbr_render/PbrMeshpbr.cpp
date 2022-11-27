#include "PbrMeshpbr.h"

PbrMeshpbr::PbrMeshpbr() {

}

PbrMeshpbr::~PbrMeshpbr() {

}

void PbrMeshpbr::draw(const std::shared_ptr<PbrShader> shader) {
    shader->use();
    unsigned int textureUnit = 0;
    mMaterial.use(shader, textureUnit);

    if (mBrdfLUT != nullptr) {
        shader->setInt("brdfLUT", textureUnit);
        mBrdfLUT->bind(GL_TEXTURE0 + textureUnit);
        textureUnit++;
    }

    if (mIrradianceMap != nullptr) {
        mIrradianceMap->bind(GL_TEXTURE0 + textureUnit);
        shader->setInt("irradianceMap", textureUnit);
        textureUnit++;
    }

    if (mPreFilterMap != nullptr) {
        mPreFilterMap->bind(GL_TEXTURE0 + textureUnit);
        shader->setInt("preFilterMap", textureUnit);
        textureUnit++;
    }

    PbrMesh::draw(shader);
}

void PbrMeshpbr::setAlbedoMap(std::shared_ptr<PbrTexture> albedoMap) {
    mMaterial.setAlbedoMap(albedoMap);
}

void PbrMeshpbr::setNormalMap(std::shared_ptr<PbrTexture> normalMap) {
    mMaterial.setNormalMap(normalMap);
}

void PbrMeshpbr::setMetallicMap(std::shared_ptr<PbrTexture> metallicMap) {
    mMaterial.setMetallicMap(metallicMap);
}

void PbrMeshpbr::setRoughnessMap(std::shared_ptr<PbrTexture> roughnessMap) {
    mMaterial.setRoughnessMap(roughnessMap);
}

void PbrMeshpbr::setAoMap(std::shared_ptr<PbrTexture> aoMap) {
    mMaterial.setAoMap(aoMap);
}

void PbrMeshpbr::setDisplacementMap(std::shared_ptr<PbrTexture> displacementMap) {
    mMaterial.setDisplacementMap(displacementMap);
}

void PbrMeshpbr::setTextureScale(float scaleX, float scaleY) {
    mMaterial.setTextureScale(scaleX, scaleY);
}

void PbrMeshpbr::setBrdfLUT(std::shared_ptr<PbrTexture> brdfLUT) {
    mBrdfLUT = brdfLUT;
}

void PbrMeshpbr::setIrradianceMap(std::shared_ptr<PbrCubeMap> irradianceMap) {
    mIrradianceMap = irradianceMap;
}

void PbrMeshpbr::setPreFilterMap(std::shared_ptr<PbrCubeMap> preFilterMap) {
    mPreFilterMap = preFilterMap;
}
