# PBR 材质渲染器

一个简单的PBR/IBL渲染器 纯 C++/OpenGL ES3实现

![image](https://github.com/lianglei1/Android-PBR/blob/main/art/device-2022-11-27-140433.png)
![image](https://github.com/lianglei1/Android-PBR/blob/main/art/device-2022-11-27-135639.png)
![image](https://github.com/lianglei1/Android-PBR/blob/main/art/device-2022-11-27-140000.png)
参考：
https://github.com/FrMarchand/pbr-material-viewer

https://github.com/JoshuaSenouf/gl-engine

https://github.com/ragnraok/AndroidPBR

### 细节
* 通过HDR贴图生成cubmap贴图实现基于环境的光照，图片资源来自
 ([hdrlabs免费资源网站](http://www.hdrlabs.com/sibl/archive.html)).

* 手动调整材质贴图缩放比列和displacement 贴图高度大小。

* 触摸旋转相机360°观察材质


## 开始项目

项目需要kotlin、C++能力和opengl高级编程知识。
* kotlin学习网站：https://www.kotlincn.net/
* C++学习网站：https://www.runoob.com/cplusplus/cpp-tutorial.html
* PBR知识理论：https://learnopengl-cn.github.io/07%20PBR/01%20Theory/
* PBR光照：https://learnopengl-cn.github.io/07%20PBR/02%20Lighting/
* 漫反射辐照：https://learnopengl-cn.github.io/07%20PBR/03%20IBL/01%20Diffuse%20irradiance/
* 镜面IBL：https://learnopengl-cn.github.io/07%20PBR/03%20IBL/02%20Specular%20IBL/

![image](https://github.com/lianglei1/Android-PBR/blob/main/art/PBR%E6%B8%B2%E6%9F%93%E6%B5%81%E7%A8%8B.png)

### 配置

* Android Studio  2020.3.1.RC1
* NDK 21.2.6472646
* OpenGL 3.0 ES
* CMake 3.4+


```bash
 git clone https://github.com/lianglei1/Android-PBR.git
```



## 依赖库
* [glm](https://glm.g-truc.net/0.9.9/index.html) - C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
* [stb-image](https://github.com/nothings/stb) - Image loading/decoding from file/memory


