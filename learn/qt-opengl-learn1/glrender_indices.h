#ifndef GLRENDER_INDICES_H
#define GLRENDER_INDICES_H


#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <vector>
class GlRender_indices
{
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
    struct VertexAtt{
        Position pos;
        Color color;
        VertexAtt(Position _pos, Color _color)
        {
            pos = _pos;
            color = _color;
        };
        void multi(float _factor){
            pos.x*=_factor;
            pos.y*=_factor;
        };
        void move_left(float x_pos){
            pos.x -= x_pos;
        };
    };


    std::vector<VertexAtt> vertices;
    std::vector<int> indices;


public:
    GlRender_indices(GLFWwindow* window);

    GLFWwindow* m_window = 0;

    unsigned int VAO {0};
    unsigned int VBO {0};

    //vertex shader
    const char * VertexShaderSource = R"GLSL(
        #version 330 core
        in vec3 aPos;
        in vec3 iColor;
        out vec4 oColor;
        void main(){
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
            oColor = vec4(iColor.x, iColor.y, iColor.z, 1.0f);
        }
    )GLSL";
    unsigned int vertexShader {0};

    //fragment shader
    const char * FragmentShaderSource = R"GLSL(
        #version 330 core
        in vec4 oColor;
        out vec4 FragColor;
        void main(){
            FragColor = oColor;
        }
    )GLSL";
    unsigned int fragmentShader {0};
    unsigned int shaderProgram {0};

    int initVertex2();

    int initGL();
    void drawingGL();
    void terminateGL();
};

#endif // GLRENDER_INDICES_H
