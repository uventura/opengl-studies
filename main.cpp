#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include "Shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void input_process(GLFWwindow* window)
{
    // Close Window Key
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
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

    // Data
    float vertices[] = {
        // First Square
        -0.5f, -0.5f, 0.0f, // 0
         0.5f, -0.5f, 0.0f, // 1
         0.5f,  0.5f, 0.0f, // 2
        -0.5f,  0.5f, 0.0f, // 3
    };

    // Indices
    uint indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    // Generating a VAO
    uint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Element Buffer Object
    uint EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    uint VBO; // Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // ============= SHADERS ===================
    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    // Execution
    while(!glfwWindowShouldClose(window))
    {
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.03f,0.0f,0.1f,0.03f);

        //========= Rendering =============

        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);

        shader.use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(0);

        /* Swap buffers */
        glfwSwapBuffers(window);

        // Get Input Events
        glfwPollEvents();

        // Input Analysis
        input_process(window);
    }

    glfwTerminate();
    return 0;
}
