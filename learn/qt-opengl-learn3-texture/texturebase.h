#ifndef TEXTUREBASE_H
#define TEXTUREBASE_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
namespace l3_textures{
class TextureBase
{

public:
    struct Position{
        float x, y, z;
        Position(float _x, float  _y, float  _z){
            x=_x,y=_y,z=_z;
        };
        Position(){};
    };
    struct Color{
        float r, g, b;
        Color(float _r, float _g, float _b){
            r=_r,g=_g,b=_b;
        };
        Color(){};
    };
    struct TextureCoord{
        float x, y;
        TextureCoord(float _x, float  _y){
            x=_x,y=_y;
        };
        TextureCoord(){};
    };
    struct VertexAtt{
        Position pos;
        Color color;
        TextureCoord texCord;
        VertexAtt(Position _pos, Color _color, TextureCoord _texCord)
        {
            pos = _pos;
            color = _color;
            texCord = _texCord;
        };
    };



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
