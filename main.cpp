#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <cmath>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.hpp"
#include "Shader.hpp"

int main( void )
{
    Window window(640, 480, "OpenGL Studies");
    window.use();

    // Data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.1f, 0.2f, // 0
         0.5f, -0.5f, 0.0f, 0.3f, 1.0f, 0.2f, // 1
         0.0f,  0.5f, 0.0f, 0.3f, 0.1f, 1.0f, // 2
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    float move = 0.0f;

    // Execution
    while(!window.shouldClose())
    {
        window.clear();

        //========= DEAR IMGUI =============

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SliderFloat("Move Triangle", &move, -1.0f, 1.0f);

        ImGui::Render();

        //========= Rendering =============

        glBindVertexArray(VAO);

        int vertex_location = glGetUniformLocation(shader.id(), "deltaPos");
        glUniform3f(vertex_location, move, move, move);
        shader.use();

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(0);

        // Render Imgui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.update();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.close();
    return 0;
}
