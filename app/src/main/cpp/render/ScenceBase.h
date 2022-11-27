#ifndef ScenceBase_H
#define ScenceBase_H

#define DEFAULT_OGL_ASSETS_DIR "/sdcard/Android/data/com.lianglei.PbrDemo/files/Download"

#include "stdint.h"
#include <GLES3/gl3.h>


class ScenceBase {
public:
    ScenceBase() {

    }
    virtual ~ScenceBase() {

    }

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY){

    }

    virtual void Init() = 0;

    virtual void Draw(int screenW, int screenH) = 0;

    virtual void Destroy() = 0;

protected:

};


#endif //ScenceBase_H
