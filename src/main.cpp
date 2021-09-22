#include <iostream>
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <src/model/color.h>
#include "render.h"
#include "ui.h"

Render render;

void reshape(GLFWwindow*, int w, int h) {
    render.reshape(w, h);
}
void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    using namespace ui::keyboard;
    auto keyEvent = KeyEvent(key, action, mods);

    if (keyEvent.is(ESC)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (keyEvent.is(R)) {
        std::cout << "Reload shaders" << std::endl;
        Platform platform;
        render.reloadShaders(platform);
    }
}
void mouseClick(GLFWwindow*, int button, int action, int mods) {
    using namespace ui::mouse;
    auto mouseEvent = MouseEvent(button, action, mods);
    if (mouseEvent.is(Action::PRESS, Button::LEFT)) {

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<float> dist(50.f, 120.f);

        float radius = dist(mt);
        auto cursor = mouseEvent.getCursor();

        auto circle = CircleModel(cursor.x, static_cast<float>(render.camera.viewSize.y) - cursor.y, radius);
        render.circles.push_back(circle);
    }
}
void mouseMove(GLFWwindow*, double x, double y) {
    using namespace ui::mouse;
    auto mouseEvent = MouseEvent(x, y);
}

int main() {

    if (!glfwInit()) {
        return -1;
    }

    const auto WIDTH = 1280;
    const auto HEIGHT = 860;
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Delone", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init glad" << std::endl;
        return -1;
    } else {
        printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    }

    glfwSetWindowPos(window, 600, 75);
    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseClick);
    glfwSetCursorPosCallback(window, mouseMove);
    glfwSwapInterval(1);
    glfwSetTime(0.0);

    Platform platform;
    render.load(platform);
    render.init();
    render.reshape(WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(window)) {
        render.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    render.destroy();
    glfwTerminate();
    return 0;
}
