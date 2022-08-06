#include "Shader.hpp"

Shader::Shader(std::string vertex_shader_src, std::string fragment_shader_src)
{
    // Shaders Configuration
    uint vertex_shader = shaderConfiguration(vertex_shader_src, GL_VERTEX_SHADER);
    uint fragment_shader = shaderConfiguration(fragment_shader_src, GL_FRAGMENT_SHADER);

    // Init Shader Program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    shaderProgramCheck(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::use()
{
    glUseProgram(shader_program);
}

void Shader::loadShaderFile(std::string path, std::string& storage)
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

uint Shader::shaderConfiguration(std::string path, uint shader_type)
{
    std::string file_storage;
    loadShaderFile(path, file_storage);

    const char* file_char = file_storage.c_str();

    uint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &file_char, NULL);
    glCompileShader(shader);

    shaderCheck(shader);

    return shader;
}

void Shader::shaderCheck(uint shader_id)
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

void Shader::shaderProgramCheck(uint shader_program_id)
{
    int success;
    char info_log[512];
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "[SHADER PROGRAM ERROR] " << info_log << "\n";
    }
}
