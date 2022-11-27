
#include "MyGLRenderContext.h"
#include "../util/LogUtil.h"
#include "Scence.h"

MyGLRenderContext *MyGLRenderContext::m_pContext = nullptr;

MyGLRenderContext::MyGLRenderContext() {
    scenceBase = new Scence();
}

MyGLRenderContext::~MyGLRenderContext() {

    if (scenceBase) {
        delete scenceBase;
        scenceBase = nullptr;
    }

}

void
MyGLRenderContext::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {
    LOGCATE("MyGLRenderContext::UpdateTransformMatrix [rotateX, rotateY, scaleX, scaleY] = [%f, %f, %f, %f]",
            rotateX, rotateY, scaleX, scaleY);
    if (scenceBase) {
        scenceBase->UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    }
}

void MyGLRenderContext::OnSurfaceCreated() {
    LOGCATE("MyGLRenderContext::OnSurfaceCreated");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MyGLRenderContext::OnSurfaceChanged(int width, int height) {
    LOGCATE("MyGLRenderContext::OnSurfaceChanged [w, h] = [%d, %d]", width, height);
    glViewport(0, 0, width, height);
    m_ScreenW = width;
    m_ScreenH = height;
}

void MyGLRenderContext::OnDrawFrame() {
    LOGCATE("MyGLRenderContext::OnDrawFrame");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);\
     if (scenceBase) {
        scenceBase->Init();
        scenceBase->Draw(m_ScreenW, m_ScreenH);
    }

}

MyGLRenderContext *MyGLRenderContext::GetInstance() {
    LOGCATE("MyGLRenderContext::GetInstance");
    if (m_pContext == nullptr) {
        m_pContext = new MyGLRenderContext();
    }
    return m_pContext;
}

void MyGLRenderContext::DestroyInstance() {
    LOGCATE("MyGLRenderContext::DestroyInstance");
    if (m_pContext) {
        delete m_pContext;
        m_pContext = nullptr;
    }

}



