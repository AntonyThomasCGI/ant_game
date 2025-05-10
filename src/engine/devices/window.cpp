
#include <iostream>

#include "window.hpp"



void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    win->framebufferResized(width, height);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    win->keyPressed(key, scancode, action, mode);
}


Window::Window(unsigned int width, unsigned int height)
{
    createWindow(width, height);
}


Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}


std::pair<uint32_t, const char**> Window::getInstanceExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    return std::make_pair(glfwExtensionCount, glfwExtensions);
}


VkResult Window::createSurface(VkInstance &instance, VkSurfaceKHR *surface)
{
    return glfwCreateWindowSurface(instance, window, nullptr, surface);
}


void Window::createWindow(unsigned int width, unsigned int height)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, "Ant Game", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, keyCallback);
}


void Window::getFramebufferSize(int &width, int &height)
{
    glfwGetFramebufferSize(window, &width, &height);
}

void Window::update()
{
    if (glfwWindowShouldClose(window)) {
        shouldClose();
    }
    glfwPollEvents();
}
