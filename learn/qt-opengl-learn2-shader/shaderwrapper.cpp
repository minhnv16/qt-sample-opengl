#include "shaderwrapper.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <memory>
#include <sys/stat.h>

ShaderWrapper::ShaderWrapper()
{
    printf(__FUNCTION__);
    return;
}

ShaderWrapper::ShaderWrapper(const char *vertexPath, const char *fragmentPath)
{

    GLFWwindow* window = 0;
    if (GLFW_FALSE == glfwInit()) {
        return ;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int nWidth = 800, nHeight = 800;
    window = glfwCreateWindow(nWidth, nHeight, "Test OpenGL", NULL /* glfwGetPrimaryMonitor()*/, NULL);
    if (!window) {
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (GLEW_OK != err) {
        glfwTerminate();
        return;
    }

    printf(__FUNCTION__);
    struct stat stVertex;
    stat(vertexPath, &stVertex);
    size_t sizeVertex = stVertex.st_size;
    printf("%ld\n", sizeVertex);


    FILE *fileVertex = NULL;
    std::shared_ptr<char> pSharedVertex((char*)malloc(sizeVertex));


    printf("start Read shader\n");

    fileVertex = fopen(vertexPath,"rb");
    if(fileVertex){
        fread(pSharedVertex.get(), sizeVertex, 1, fileVertex);
    }
    else{
        printf("Read shader Failed\n");
        return;

    }
    printf("Read fileVertex OK\n");


    struct stat stFragment;
    stat(fragmentPath, &stFragment);
    size_t sizeFragment = stFragment.st_size;

    FILE *fileFragment = NULL;
    std::shared_ptr<char> pSharedFragment((char*) malloc(sizeFragment));
    printf("start Read shader\n");

    fileFragment = fopen(fragmentPath,"rb");

    if(fileFragment){
        fread(pSharedFragment.get(), sizeFragment, 1, fileFragment);
        printf("end fread\n");

    }
    else{
        printf("Read shader Failed\n");
        return;

    }
    printf("Read fileFragment OK\n");

    char* vShaderCode = pSharedVertex.get();
    char* fShaderCode = pSharedFragment.get();


    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    {
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);


        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return;
        };

    }
    // similiar for Fragment Shader
    {
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);


        // print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return;
        };

    }
    printf("Compile shader OK");

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);


}

void ShaderWrapper::use()
{
    glUseProgram(ID);
}

void ShaderWrapper::setBool(const string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ShaderWrapper::setInt(const string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderWrapper::setFloat(const string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
