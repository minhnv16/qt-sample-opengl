#ifndef TEXTUREBASE_H
#define TEXTUREBASE_H

#include "common.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <math.h>

namespace l3_textures{



class TextureBase
{

public:

public:
    std::vector<VertexAtt> vertices;
    std::vector<int> indices;

    unsigned int VAO {0}; //vertex array object
    unsigned int VBO {0}; //vertex buffer object
    unsigned int EBO {0}; //element buffer object

    //vertex shader
    virtual const char* GetVertexShaderSource();
    unsigned int m_vertexShader {0};

    //fragment shader
    virtual const char * GetFragmentShaderSource();
    unsigned int m_fragmentShader {0};

    //program shader
    unsigned int m_shaderProgram {0};

    //texture
    unsigned int m_nTexture;

public:
    TextureBase();

    virtual int initVertex();
    virtual int initGL();
    virtual void drawingGL();
    virtual void terminateGL();

    virtual const char* GetTexturePath();

};


}
#endif // TEXTUREBASE_H
