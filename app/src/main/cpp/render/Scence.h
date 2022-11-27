//
// Created by 梁磊 on 2022/11/26.
//

#ifndef PBRDEMO_SCENCE_H
#define PBRDEMO_SCENCE_H

#include "ScenceBase.h"
#include <string>
#include <map>
#include <vector>

#include "../pbr_render/PbrCamera.h"
#include "../pbr_render/PbrSkybox.h"
#include "../pbr_render/PbrTexture.h"
#include "../pbr_render/PbrSphere.h"
#include "../pbr_render/PbrQuad.h"
#include "../pbr_render/PbrCubemapgenerator.h"
#include <memory>
#include <GLES3/gl3.h>


class Scence : public ScenceBase{

public:
    Scence();

    ~Scence();

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void Destroy();

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    void createFbo(int width, int height);
private:
    // settings
    const unsigned int SCR_WIDTH = 2000;
    const unsigned int SCR_HEIGHT = 1024;

// camera
    PbrCamera *camera;
    unsigned int hdrFBO, colorBuffer, renderBuffer;

// Skybox
    std::shared_ptr<PbrSkybox> skybox = nullptr;
    std::shared_ptr<PbrCubeMap> environmentMap = nullptr;
    std::shared_ptr<PbrCubeMap> irradianceMap = nullptr;
    std::shared_ptr<PbrCubeMap> preFilterMap = nullptr;
    std::shared_ptr<PbrTexture> brdfLUT = nullptr;

// Geometry
    std::shared_ptr<PbrSphere> sphere = nullptr;
    std::shared_ptr<PbrSphere> light = nullptr;
    std::shared_ptr<PbrQuad> quad = nullptr;

// Material
    std::shared_ptr<PbrTexture> albedoMap = nullptr;
    std::shared_ptr<PbrTexture> normalMap = nullptr;
    std::shared_ptr<PbrTexture> metallicMap = nullptr;
    std::shared_ptr<PbrTexture> roughnessMap = nullptr;
    std::shared_ptr<PbrTexture> aoMap = nullptr;
    std::shared_ptr<PbrTexture> displacementMap = nullptr;
    std::shared_ptr<PbrShader> shaderSingleColor = nullptr;
    std::shared_ptr<PbrShader> shaderScreen = nullptr;
    std::shared_ptr<PbrShader> shaderWireframe = nullptr;
    std::shared_ptr<PbrShader> shaderSkybox = nullptr;
    std::shared_ptr<PbrShader> shaderPBR = nullptr;
    int textureScale[2] = {1, 1};
    float lightPos[3] = {2.0, 0.0, 2.0};
    float rotationAngle = 0.1;
    float displacementAmount = 0.1f;
    bool lightEnabled = false;
    bool hasInit = false;

    int mScreenWidth = SCR_WIDTH;
    int mScreenHeight = SCR_HEIGHT;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;
    glm::mat4 m_MVPMatrix;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;


};


#endif //PBRDEMO_SCENCE_H
