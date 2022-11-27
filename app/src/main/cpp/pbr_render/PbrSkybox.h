#ifndef PbrSkybox_H
#define PbrSkybox_H

#include <memory>
#include "PbrCubemap.h"
#include "PbrShader.h"
#include "PbrMesh.h"

class PbrSkybox : public PbrMesh
{
public:
	PbrSkybox();
	void draw(const std::shared_ptr<PbrShader>  shader);
	void setEnvironmentMap(std::shared_ptr<PbrCubeMap> environmentMap);

private:
	std::shared_ptr<PbrCubeMap> mEnvironmentMap = nullptr;
};

#endif //PbrSkybox_H