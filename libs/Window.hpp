#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class Window
{
    private:
        GLFWwindow* window;

        static void frameBufferCallback(GLFWwindow* window, int width, int height);
        void inputProcess();

    public:
        Window(const uint width, const uint height, std::string title);

        inline GLFWwindow* getWindow(){return window;}

        void use();
        bool shouldClose();
        void clear();
        void update();
        void close();
};
