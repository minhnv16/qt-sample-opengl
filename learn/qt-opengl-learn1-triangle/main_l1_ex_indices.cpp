#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
namespace l1_ex{
using namespace std;
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
};



VertexAtt vertices[]={
    VertexAtt(Position(-0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f)),

    VertexAtt(Position(-0.5f, -0.5f, 0.0f), Color(1.0f, 0.0f, 0.0f)),
    VertexAtt(Position(+0.5f, -0.5f, 0.0f), Color(0.0f, 1.0f, 0.0f)),
    VertexAtt(Position(+0.0f, +0.5f, 0.0f), Color(0.0f, 0.0f, 1.0f)),

    VertexAtt(Position(+0.8f, +0.8f, 0.0f), Color(1.0f, 0.0f, 0.0f)),
    VertexAtt(Position(+0.8f, +0.9f, 0.0f), Color(0.0f, 1.0f, 0.0f)),
    VertexAtt(Position(+0.7f, +0.9f, 0.0f), Color(0.0f, 0.0f, 1.0f)),

    VertexAtt(Position(-0.8f, -0.8f, 0.0f), Color(1.0f, 0.0f, 0.0f)),
    VertexAtt(Position(-0.8f, -0.9f, 0.0f), Color(0.0f, 1.0f, 0.0f)),
    VertexAtt(Position(-0.7f, -0.9f, 0.0f), Color(0.0f, 0.0f, 1.0f)),

};



int main()
{
    GLFWwindow* window = 0;
    if (GLFW_FALSE == glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int nWidth = 800, nHeight = 800;
    window = glfwCreateWindow(nWidth, nHeight, "Test OpenGL", NULL /* glfwGetPrimaryMonitor()*/, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    //init glew
    GLenum err = glewInit();

    if (GLEW_OK != err) {
        glfwTerminate();
        return -1;
    }
    //init_vertices();



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
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512] = {0};
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }


    //fragment shader
    const char * FragmentShaderSource = R"GLSL(
        #version 330 core
        in vec4 oColor;
        out vec4 FragColor;
        void main(){
            FragColor = oColor;
        }
    )GLSL";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;

    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK\n" << infoLog << std::endl;
        return -1;

    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);




    //start init VAO,VBO,EBO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //vertext buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int indices[] = {
        1, 2, 3,
        0,
        4, 5, 6,
        0,
        7, 8, 9,
        0
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //end init VAO,VBO,EBO



    unsigned int aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)0);
    glEnableVertexAttribArray(aPosLocation);

    unsigned int iColor = glGetAttribLocation(shaderProgram, "iColor");
    glVertexAttribPointer(iColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)(sizeof(Position)));
    glEnableVertexAttribArray(iColor);






    glUseProgram(shaderProgram);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0);




    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 1, 3);
        //glDrawArrays(GL_TRIANGLES, 4, 3);
       // glDrawArrays(GL_TRIANGLES, 7, 3);

        glDrawElements(GL_LINE_LOOP, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
}//namespace l1


