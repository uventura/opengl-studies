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

// Camera Definitions
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

// Time
float delta_time = 0.0f;
float last_time = 0.0f;

// Mouse
float last_x = 400, last_y = 300;
bool first_mouse = true;
float yaw = 0.0f, pitch = 0.0f;
float fov = 45.0f;

// Functions

void processInput(GLFWwindow* window)
{
    const float camera_speed = 4.5f * delta_time;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos += camera_speed * camera_front;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos -= camera_speed * camera_front;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if(first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffest = last_y - ypos;

    last_x = xpos;
    last_y = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffest *= sensitivity;

    yaw += xoffset;
    pitch += yoffest;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}

void zoomCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if(fov < 1.0f)
        fov = 1.0f;
    if(fov > 45.0f)
        fov = 45.0f;
}

int main( void )
{
    Window window(640, 480, "OpenGL Studies");
    window.use();

    // Data
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // Generating a VAO
    uint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    uint VBO; // Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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
    ImGui_ImplGlfw_InitForOpenGL(window.id(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

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
    unsigned char* tex_data = stbi_load("textures/texture02.jpg", &tex_width, &tex_height, &tex_channels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_data);

    // ====== GLM EXAMPLE =========
    float rotation[3] = {-55.0f, 0.0f, 0.0f};

    // Coordinate System
    glm::mat4 view = glm::mat4(1.0f);
    // view = glm::translate(view, glm::vec3(1.0f, 0.0f, -3.0f));

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -1.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -5.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -2.5f),
        glm::vec3( 1.3f, -2.0f, -0.5f),
        glm::vec3( 1.5f, 2.0f, -1.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    // // Camera
    // glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    // glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);
    // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    // glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));
    // glm::vec3 camera_up = glm::cross(camera_direction, camera_right);

    // Euler Angles
    // glm::vec3 direction;
    
    // float yaw;
    // float pitch;

    // // Yaw
    // direction.x = cos(glm::radians(yaw));
    // direction.z = sin(glm::radians(yaw));
    // direction.y = sin(glm::radians(pitch));

    // // Pitch


    // Execution
    while(!window.shouldClose())
    {
        window.clear();

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_time;
        last_time = current_frame;

        //========= DEAR IMGUI =============

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SliderFloat("Rotation X", &rotation[0], -360.0f, 360.0f);
        ImGui::SliderFloat("Rotation Y", &rotation[1], -360.0f, 360.0f);
        ImGui::SliderFloat("Rotation Z", &rotation[2], -360.0f, 360.0f);

        ImGui::Render();

        // Transform Action
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), 640.0f/480.0f, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

        //========= Rendering =============
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        // const float radius = 10.0f;
        // float cam_x = sin(glfwGetTime()) * radius;
        // float cam_z = cos(glfwGetTime()) * radius;

        // view = glm::lookAt(
        //     glm::vec3(cam_x, 0.0f, cam_z),
        //     glm::vec3(0.0f, 0.0f, 0.0f),
        //     glm::vec3(0.0f, 1.0f, 0.0f)
        // );
        view = glm::lookAt(
            camera_pos,
            camera_pos + camera_front,
            camera_up
        );

        for(uint i=0; i<10; ++i)
        {
            // Uniform
            shader.use();

            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f - i;

            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            shader.setUniformMatrix4fv("model", glm::value_ptr(model));
            shader.setUniformMatrix4fv("view", glm::value_ptr(view));
            shader.setUniformMatrix4fv("projection", glm::value_ptr(projection));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        // Render Imgui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.update();
        processInput(window.id());

        glfwSetCursorPosCallback(window.id(), mouseCallback);
        glfwSetScrollCallback(window.id(), zoomCallback);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.close();
    // return 0;

}
