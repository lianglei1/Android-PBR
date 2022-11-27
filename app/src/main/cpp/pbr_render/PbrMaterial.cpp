#include "PbrMaterial.h"

PbrMaterial::PbrMaterial() {

}

void PbrMaterial::use(const std::shared_ptr<PbrShader> PbrShader, unsigned int &textureUnit) const {
    PbrShader->setVec2("textureScale", mTextureScale);

    if (mAlbedoMap != nullptr) {
        PbrShader->setInt("albedoMap", textureUnit);
        mAlbedoMap->bind(GL_TEXTURE0 + textureUnit);
        textureUnit++;
    }

    if (mNormalMap != nullptr) {
        PbrShader->setInt("normalMap", textureUnit);
        mNormalMap->bind(GL_TEXTURE0 + textureUnit);
        textureUnit++;
    }

    if (mMetallicMap != nullptr) {
        PbrShader->setInt("metallicMap", textureUnit);
        mMetallicMap->bind(GL_TEXTURE0 + textureUnit);
        textureUnit++;
    }

    if (mRoughnessMap != nullptr) {
        PbrShader->setInt("roughnessMap", textureUnit);
        mRoughnessMap->bind(GL_TEXTURE0 + textureUnit);
        textureUnit++;
    }

    if (mAoMap != nullptr) {
        PbrShader->setInt("aoMap", textureUnit);
        mAoMap->bind(GL_TEXTURE0 + textureUnit);
        textureUnit++;
    }

    if (mDisplacementMap != nullptr) {
        PbrShader->setInt("displacementMap", textureUnit);
        mDisplacementMap->bind(GL_TEXTURE0 + textureUnit);
        textureUnit++;
    }

    glActiveTexture(GL_TEXTURE0);
}

void PbrMaterial::setAlbedoMap(std::shared_ptr<PbrTexture> albedoMap) {
    mAlbedoMap = albedoMap;
}

void PbrMaterial::setNormalMap(std::shared_ptr<PbrTexture> normalMap) {
    mNormalMap = normalMap;
}

void PbrMaterial::setMetallicMap(std::shared_ptr<PbrTexture> metallicMap) {
    mMetallicMap = metallicMap;
}

void PbrMaterial::setRoughnessMap(std::shared_ptr<PbrTexture> roughnessMap) {
    mRoughnessMap = roughnessMap;
}

void PbrMaterial::setAoMap(std::shared_ptr<PbrTexture> aoMap) {
    mAoMap = aoMap;
}

void PbrMaterial::setDisplacementMap(std::shared_ptr<PbrTexture> displacementMap) {
    mDisplacementMap = displacementMap;
}

void PbrMaterial::setTextureScale(float scaleX, float scaleY) {
    this->mTextureScale = glm::vec2(scaleX, scaleY);
}