#ifndef PbrCubemapgenerator_H
#define PbrCubemapgenerator_H


#include "PbrCubeMap.h"
#include "PbrTexture.h"
#include "PbrQuad.h"
#include "PbrSkybox.h"
#include <memory>


class PbrCubemapgenerator {
public:
    PbrCubemapgenerator();

    ~PbrCubemapgenerator();

    std::shared_ptr<PbrCubeMap> generateEnvironmentMap(const std::string imagePath);

    std::shared_ptr<PbrCubeMap>
    generateIrradianceMap(const std::shared_ptr<PbrCubeMap> environmentMap,
                          const std::string texturePath);

    std::shared_ptr<PbrCubeMap>
    generatePreFilterMap(const std::shared_ptr<PbrCubeMap> environmentMap,
                         const std::string texturePath);

    std::shared_ptr<PbrTexture> generateBrdfLUT(const std::string path);


private:
    std::shared_ptr<PbrQuad> mQuad = nullptr;//std::make_shared<PbrQuad>();
    std::shared_ptr<PbrSkybox> mSkybox = nullptr;// std::make_shared<PbrSkybox>();
};

#endif//PbrCubemapgenerator_H