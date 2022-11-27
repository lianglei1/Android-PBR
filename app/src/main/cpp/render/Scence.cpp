//
// Created by 梁磊 on 2022/11/26.
//

#include "Scence.h"
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "stb_image.h"
#include "../pbr_render/PbrShaderStrings.h"

Scence::Scence() {
    camera = new PbrCamera(glm::vec3(0.0f, 0.0f, 3.0f));
}

Scence::~Scence() {

    if (camera) {
        delete camera;
        camera = nullptr;
    }

}


void Scence::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {
    m_AngleX = static_cast<int>(rotateX);
    m_AngleY = static_cast<int>(rotateY);
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
}

void Scence::Init() {
    if (hasInit)
        return;
    hasInit = true;
    //TODO 先把 model 文件夹拷贝到 /sdcard/Android/data/com.byteflow.app/files/Download 路径下，然后可以选择你要加载的模型
    std::string path(DEFAULT_OGL_ASSETS_DIR);
    std::string shaderPath = path + "/pbrshaders";
    std::string texturePath = path + "/textures";
    std::string modelPath = path + "/model/";
    //----------
    // Shader(s)
    //----------
    shaderSingleColor = std::make_shared<PbrShader>(PbrShaderStrings::shadersinglecolorVert,
                                                    PbrShaderStrings::shadersinglecolorFrag);
    shaderScreen = std::make_shared<PbrShader>(PbrShaderStrings::shaderscreenVert,
                                               PbrShaderStrings::shaderscreenFrag);
    shaderSkybox = std::make_shared<PbrShader>(PbrShaderStrings::shaderskyboxVert,
                                               PbrShaderStrings::shaderskyboxFrag);
    shaderPBR = std::make_shared<PbrShader>(PbrShaderStrings::shaderpbrVert,
                                            PbrShaderStrings::shaderpbrFrag);
    std::shared_ptr<PbrCubemapgenerator> cubeMapGenerator = std::make_shared<PbrCubemapgenerator>();

    // Initialize geometry
    sphere = std::make_shared<PbrSphere>();
    light = std::make_shared<PbrSphere>();
    quad = std::make_shared<PbrQuad>();

    // load and create textures
    // -------------------------

    /*   albedoMap = std::make_shared<PbrTexture>(texturePath + "/malbedo.png", false);
       normalMap = std::make_shared<PbrTexture>(texturePath + "/mnormal.png");
       metallicMap = std::make_shared<PbrTexture>(texturePath + "/mmetallic.png");
       roughnessMap = std::make_shared<PbrTexture>(texturePath + "/mroughness.png");
       aoMap = std::make_shared<PbrTexture>(texturePath + "/mao.png");
       displacementMap = std::make_shared<PbrTexture>(texturePath + "/height.png");*/

    // ------------------------- plastic---------------------------------------------------
     albedoMap = std::make_shared<PbrTexture>(texturePath + "/albedo.png", false);
     normalMap = std::make_shared<PbrTexture>(texturePath + "/normal.png");
     metallicMap = std::make_shared<PbrTexture>(texturePath + "/metal.png");
     roughnessMap = std::make_shared<PbrTexture>(texturePath + "/rough.png");
     aoMap = std::make_shared<PbrTexture>(texturePath + "/ao.png");
     displacementMap = std::make_shared<PbrTexture>(texturePath + "/height.png");
    // ------------------------- metal---------------------------------------------------

    /*albedoMap = std::make_shared<PbrTexture>(texturePath + "/rusted-steel_albedo.png", false);
    normalMap = std::make_shared<PbrTexture>(texturePath + "/rusted-steel_normal-dx.png");
    metallicMap = std::make_shared<PbrTexture>(texturePath + "/rusted-steel_metallic.png");
    roughnessMap = std::make_shared<PbrTexture>(texturePath + "/rusted-steel_roughness.png");
    aoMap = std::make_shared<PbrTexture>(texturePath + "/rusted-steel_ao.png");
    displacementMap = std::make_shared<PbrTexture>(texturePath + "/rusted-steel_height.png");*/

    /*environmentMap = cubeMapGenerator->generateEnvironmentMap(
            texturePath + "/appart.hdr");*/
    environmentMap = cubeMapGenerator->generateEnvironmentMap(
            texturePath + "/default_env.hdr");
    irradianceMap = cubeMapGenerator->generateIrradianceMap(environmentMap, shaderPath);
    preFilterMap = cubeMapGenerator->generatePreFilterMap(environmentMap, shaderPath);
    brdfLUT = cubeMapGenerator->generateBrdfLUT(shaderPath);
    sphere->setAlbedoMap(albedoMap);
    sphere->setNormalMap(normalMap);
    sphere->setMetallicMap(metallicMap);
    sphere->setRoughnessMap(roughnessMap);
    sphere->setAoMap(aoMap);
    sphere->setDisplacementMap(displacementMap);
    sphere->setTextureScale(1, 1);

    sphere->setIrradianceMap(irradianceMap);
    sphere->setPreFilterMap(preFilterMap);
    sphere->setBrdfLUT(brdfLUT);

    skybox = std::make_shared<PbrSkybox>();
    skybox->setEnvironmentMap(environmentMap);

    shaderScreen->use();
    shaderScreen->setInt("screenTexture", 0);
    createFbo(mScreenWidth, mScreenHeight);
    glViewport(0, 0, mScreenWidth, mScreenHeight);


}

void Scence::Draw(int screenW, int screenH) {

    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render scene
    // ------------
    UpdateMVPMatrix(m_MVPMatrix, m_AngleX, m_AngleY, (float)screenW / screenH);

    // camera/view transformation
    glm::mat4 view = camera->getViewMatrix();


    // projection matrix
    glViewport(0, 0, mScreenWidth, mScreenHeight);
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera->mZoom),
                                  (float) mScreenWidth / (float) mScreenHeight, 0.1f,
                                  100.0f);


    //  rotationAngle += 0.01f;
    if (lightEnabled) {
        shaderSingleColor->use();
        shaderSingleColor->setMat4("view", view);
        shaderSingleColor->setMat4("projection", projection);
        shaderSingleColor->setVec3("color", 100.0, 100.0, 100.0);

        glm::mat4 model = glm::mat4(1.0f);
        //  model = glm::rotate(model, rotationAngle, glm::vec3(0.0, 1.0, 0.0));
        model = glm::translate(model, glm::vec3(lightPos[0], lightPos[1], lightPos[2]));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        shaderSingleColor->setMat4("model", model);
        light->draw(shaderSingleColor);
    }
    // Render object
    shaderPBR->use();
    shaderPBR->setMat4("view", view);
    shaderPBR->setMat4("projection", projection);
    shaderPBR->setVec3("eyePos", camera->mPosition);

    glm::mat4 model = glm::mat4(1.0f);

    // model = glm::rotate(model, rotationAngle, glm::vec3(0.0, 1.0, 0.0));

    model = glm::rotate(model, rotationAngle, glm::vec3(0.0, 1.0, 0.0));
    shaderPBR->setMat4("model", model);
    glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(model)));
    shaderPBR->setMat3("normalMat", normalMat);
    shaderPBR->setFloat("displacementAmount", displacementAmount);
    shaderPBR->setBool("lightEnabled", lightEnabled);
    shaderPBR->setVec3("lightPos", lightPos[0], lightPos[1], lightPos[2]);
    sphere->draw(shaderPBR);

    // Render skybox
    // -------------
    shaderSkybox->use();
    view = glm::mat4(glm::mat3(camera->getViewMatrix()));
    shaderSkybox->setMat4("view", view);
    shaderSkybox->setMat4("projection", projection);
    skybox->draw(shaderSkybox);
    // Render quad with scene's visuals as its texture image
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // Draw Screen quad
    shaderScreen->use();
    shaderScreen->setVec2("screenSize", glm::vec2(mScreenWidth, mScreenHeight));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    quad->draw(shaderScreen);
    glEnable(GL_DEPTH_TEST);



}

void Scence::Destroy() {


}

void Scence::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio) {

    if (firstMouse)
    {
        lastX = static_cast<float>(angleX);
        lastY = static_cast<float>(angleY);
        firstMouse = false;
    }
    float xoffset = static_cast<float>(angleX) - lastX;
    float yoffset = lastY - static_cast<float>(angleY); // reversed since y-coordinates go from bottom to top

    lastX = static_cast<float>(angleX);
    lastY = static_cast<float>(angleY);

    camera->processMouseMovement(yoffset, xoffset);


}

void Scence::createFbo(int width, int height) {
    glDeleteTextures(1, &colorBuffer);
    glDeleteRenderbuffers(1, &renderBuffer);

    glDeleteFramebuffers(1, &hdrFBO);

    // HDR framebuffer configuration
    // --------------------------
    glGenFramebuffers(1, &hdrFBO);
    // create a color attachment texture
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,
                          height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // Attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              renderBuffer); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


}
