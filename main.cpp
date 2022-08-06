#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

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

void load_shader(std::string path, std::string& storage)
{
    std::fstream shader;

    shader.open(path);
    if(shader.is_open())
    {
        std::string current_line;
        while(std::getline(shader, current_line))
        {
            storage += current_line + "\n";
        }
        shader.close();
        return;
    }

    std::cout << "[ERROR] Shader Couldn't be loaded...\n";
}

void shader_verification(uint shader_id)
{
    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        std::cout << "[SHADER LOADING ERROR] " << info_log << "\n";
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
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
    };

    // Indices
    uint indices[] = {
        0, 1, 2,    // First Triangle
        2, 3, 0     // Second Triangle
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
    glEnableVertexAttribArray(0);

    // ============= SHADERS ===================
    std::string vertex_shader_str;
    load_shader("shaders/vertex_shader.glsl", vertex_shader_str);
    const char* vertex_shader_source = vertex_shader_str.c_str();

    uint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    shader_verification(vertex_shader);

    std::string fragment_shader_str;
    load_shader("shaders/fragment_shader.glsl", fragment_shader_str);
    const char* fragment_shader_source = fragment_shader_str.c_str();

    uint fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    shader_verification(fragment_shader);

    // Linking Shaders
    uint shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Execution
    while(!glfwWindowShouldClose(window))
    {
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.03f,0.0f,0.1f,0.03f);

        //========= Rendering =============
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Activating the Shader
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
