#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <cmath>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    //============= SETUP IMGUI =============
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    bool show_demo_window = true;

    // Execution
    while(!glfwWindowShouldClose(window))
    {
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.03f,0.0f,0.1f,0.03f);

        //========= DEAR IMGUI =============

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();

        //========= Rendering =============

        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);

        //======== Using Uniforms ========
        float current_time = glfwGetTime();
        float red_value = (sin(current_time) / 2.0f) + 0.5f;
        float blue_value = (cos(current_time) / 2.0f) + 0.5f;

        int vertex_color_location = glGetUniformLocation(shader.id(), "our_color");
        shader.use();
        glUniform4f(vertex_color_location, red_value, 0.0f, blue_value, 1.0f);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(0);

        // Render Imgui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap buffers */
        glfwSwapBuffers(window);

        // Get Input Events
        glfwPollEvents();

        // Input Analysis
        input_process(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
