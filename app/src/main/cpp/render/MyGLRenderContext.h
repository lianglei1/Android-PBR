

#ifndef PBR_MYGLRENDERCONTEXT_H
#define PBR_MYGLRENDERCONTEXT_H

#include "stdint.h"
#include <GLES3/gl3.h>
#include "ScenceBase.h"


class MyGLRenderContext
{
	MyGLRenderContext();

	~MyGLRenderContext();

public:

	void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

	void OnSurfaceCreated();

	void OnSurfaceChanged(int width, int height);

	void OnDrawFrame();

	static MyGLRenderContext* GetInstance();
	static void DestroyInstance();

private:
	static MyGLRenderContext *m_pContext;
	ScenceBase *scenceBase;
	int m_ScreenW;
	int m_ScreenH;

};


#endif //PBR_MYGLRENDERCONTEXT_H
