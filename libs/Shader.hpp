#include <string>
#include <fstream>
#include <iostream>

#include <glad/gl.h>

class Shader
{
    private:
        uint shader_program;

        void loadShaderFile(std::string path, std::string& storage);
        uint shaderConfiguration(std::string path, uint shader_type);
        void shaderCheck(uint shader_id);
        void shaderProgramCheck(uint shader_program_id);

    public:
        Shader(std::string vertex_shader_src, std::string fragment_shader_src);
        void use();

        inline uint id(){return shader_program;};
};
