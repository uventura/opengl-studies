#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <cmath>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Shader.hpp"

int main( void )
{
    Window window(640, 480, "OpenGL Studies");
    window.use();

    // Data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.1f, 0.2f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 0.0f, 0.3f, 1.0f, 0.2f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 0.0f, 0.3f, 0.1f, 1.0f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 0.2f, 1.0f, 0.3f, 0.0f, 1.0f, // 3
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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

    //============ TEXTURES ============
    uint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float tex_coords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
    };

    int tex_width, tex_height, tex_channels;
    unsigned char* tex_data = stbi_load("textures/texture01.jpg", &tex_width, &tex_height, &tex_channels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_data);

    // ====== GLM EXAMPLE =========
    float rotation = 0.0f;
    float scale = 1.0f;
    float previous_rot = rotation;
    float previous_sca = scale;

    glm::mat4 transf = glm::mat4(1.0f);

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
        ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f);
        ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);

        ImGui::Render();

        // Transform Action
        transf = glm::rotate(transf, glm::radians(rotation-previous_rot), glm::vec3(0.0f, 0.0f, 1.0f));

        if(scale > previous_sca)
            transf = glm::scale(transf, glm::vec3(scale+previous_sca, scale+previous_sca, scale+previous_sca));
        else if(scale < previous_sca)
            transf = glm::scale(transf, glm::vec3(scale, scale, scale));
        else
            transf = glm::scale(transf, glm::vec3(1.0f, 1.0f, 1.0f));

        previous_rot = rotation;
        previous_sca = scale;

        //========= Rendering =============

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        // Uniform
        int vertex_location = glGetUniformLocation(shader.id(), "deltaPos");
        int trans = glGetUniformLocation(shader.id(), "transform");

        glUniform3f(vertex_location, move, move, move);
        glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(transf));
        shader.use();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

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
    // return 0;

}
