#include "shaderwrapper.h"

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <memory>
#include <sys/stat.h>
namespace l2_shader_wrap{

int main(){
    GLFWwindow* window = 0;
    if (GLFW_FALSE == glfwInit()) {
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int nWidth = 800, nHeight = 800;
    window = glfwCreateWindow(nWidth, nHeight, "Test OpenGL", NULL /* glfwGetPrimaryMonitor()*/, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (GLEW_OK != err) {
        glfwTerminate();
        return 1;
    }


    std::shared_ptr<ShaderWrapper> ourShader(new ShaderWrapper("/home/minh/Desktop/vertex.sh", "/home/minh/Desktop/frag.sh"));
    return 0;
}
}
