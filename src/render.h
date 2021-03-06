#pragma once
#include "camera/camera.h"
#include "platform/loader.h"
#include "shader/shaderSource.h"
#include "shader/shader.h"
#include "scene/scene.h"
using namespace scene_version_1;

class Render;

struct ShaderCache {
    bool loaded = false;
    ShaderSource circle;
    ShaderSource line;
    void load(const ShaderLoader& loader);
};

struct Shaders {
    CircleShader circle;
    LineShader line;
    void create(const ShaderCache& cache);
    void link(const Render& render);
    void destroy();
};

class Render {
    ShaderCache shaderCache;
public:
    Shaders shaders;
    Camera camera;
    Scene scene;

    void loadResources(Platform& platform);
    void initResources();
    void destroy();
    void initScene();
    void draw();
    void reshape(int width, int height);
    void reloadShaders(Platform& platform);
};