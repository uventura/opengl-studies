#include "Window.hpp"

Window::Window(const uint width, const uint height, std::string title)
{
    /* Init GLFW */
    if(!glfwInit())
    {
        std::cout << "[GLFW ERROR] Couldn't init\n";
        return;
    }

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        std::cout << "[WINDOW ERROR] Failed to build...\n";
        glfwTerminate();
        return;
    }

}

void Window::frameBufferCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::inputProcess()
{
    // Close Window Key
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void Window::use()
{
    // Loading Context
    glfwMakeContextCurrent(window);
    if(!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "[ERROR] Failed to load GLAD\n";
        return;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Shader Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    // Viewport Settings
    glfwSetFramebufferSizeCallback(window, Window::frameBufferCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::clear()
{
    // Clear Screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.03f,0.0f,0.1f,0.03f);
}

void Window::update()
{
    // Swap buffers
    glfwSwapBuffers(window);

    // Get Input Events
    glfwPollEvents();

    // Input Analysis
    inputProcess();
}

void Window::close()
{
    glfwTerminate();
}
