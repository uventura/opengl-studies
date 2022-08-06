#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main( void )
{
    GLFWwindow* window;

    /* Init GLFW */
    if(!glfwInit())
        return -1;

    const uint width = 640;
    const uint height = 480;

    window = glfwCreateWindow(width, height, "OpenGL Studies", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Loading Context
    glfwMakeContextCurrent(window);
    if(!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "[ERROR] Failed to load GLAD\n";
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Shader Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    // Viewport Settings
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.03f,0.0f,0.1f,0.03f);

        /* Swap buffers */
        glfwSwapBuffers(window);

        // Get Input Events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
