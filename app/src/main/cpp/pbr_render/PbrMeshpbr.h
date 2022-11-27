#ifndef PbrMeshpbr_H
#define PbrMeshpbr_H

#include "PbrMesh.h"
#include "PbrMaterial.h"

class PbrMeshpbr : public PbrMesh {
public:
    PbrMeshpbr();

    virtual ~PbrMeshpbr() = 0;

    void draw(const std::shared_ptr<PbrShader> shader);

    void setAlbedoMap(std::shared_ptr <PbrTexture> albedoMap);

    void setNormalMap(std::shared_ptr <PbrTexture> normalMap);

    void setMetallicMap(std::shared_ptr <PbrTexture> metallicMap);

    void setRoughnessMap(std::shared_ptr <PbrTexture> roughnessMap);

    void setAoMap(std::shared_ptr <PbrTexture> aoMap);

    void setDisplacementMap(std::shared_ptr <PbrTexture> displacementMap);

    void setTextureScale(float scaleX, float scaleY);

    void setBrdfLUT(std::shared_ptr <PbrTexture> brdfLUT);

    void setIrradianceMap(std::shared_ptr <PbrCubeMap> irradianceMap);

    void setPreFilterMap(std::shared_ptr <PbrCubeMap> preFilterMap);

private:
    PbrMaterial mMaterial;
    std::shared_ptr <PbrTexture> mBrdfLUT;
    std::shared_ptr <PbrCubeMap> mIrradianceMap;
    std::shared_ptr <PbrCubeMap> mPreFilterMap;
};

#endif//PbrMeshpbr_H