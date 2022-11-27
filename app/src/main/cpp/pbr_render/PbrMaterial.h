#ifndef PbrMaterial_H
#define PbrMaterial_H

#include "PbrTexture.h"
#include "PbrCubeMap.h"
#include "PbrShader.h"

class PbrMaterial
{
public:
	PbrMaterial();
	void use( const std::shared_ptr<PbrShader> shader, unsigned int &textureUnit) const;
	void setAlbedoMap(std::shared_ptr<PbrTexture> albedoMap);
	void setNormalMap(std::shared_ptr<PbrTexture> normalMap);
	void setMetallicMap(std::shared_ptr<PbrTexture> metallicMap);
	void setRoughnessMap(std::shared_ptr<PbrTexture> roughnessMap);
	void setAoMap(std::shared_ptr<PbrTexture> aoMap);
	void setDisplacementMap(std::shared_ptr<PbrTexture> displacementMap);
	void setTextureScale(float scaleX, float scaleY);

private:
	std::shared_ptr<PbrTexture> mAlbedoMap = nullptr;
	std::shared_ptr<PbrTexture> mNormalMap = nullptr;
	std::shared_ptr<PbrTexture> mMetallicMap = nullptr;
	std::shared_ptr<PbrTexture> mRoughnessMap = nullptr;
	std::shared_ptr<PbrTexture> mAoMap = nullptr;
	std::shared_ptr<PbrTexture> mDisplacementMap = nullptr;
	glm::vec2 mTextureScale = glm::vec2(1.0f, 1.0f);
};

#endif//PbrMaterial_H