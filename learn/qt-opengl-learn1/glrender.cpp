#include "glrender.h"
#include <math.h>



GlRender::GlRender(GLFWwindow *window)
{
    m_window = window;
}

int GlRender::initVertex1()
{
    int nNum = 12;
    for(int i=0;i<nNum;i++){
        VertexAtt pt1(Position(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f));
        VertexAtt pt2(Position(+1.0f, 0.0f, 0.0f), Color(0.0f, 1.0f, 0.0f));
        VertexAtt pt3(Position(+0.5f, +1.0f, 0.0f), Color(0.0f, 0.0f, 1.0f));

        pt1.multi(2.0f/nNum);
        pt2.multi(2.0f/nNum);
        pt3.multi(2.0f/nNum);


        float bottomWidth = 2.0f/nNum;

        pt1.move_left(1 - 2.0f/nNum*i);
        pt2.move_left(1 - 2.0f/nNum*i);
        pt3.move_left(1 - 2.0f/nNum*i);

        vertices.push_back(pt1);
        vertices.push_back(pt2);
        vertices.push_back(pt3);

        printf("");
    }
    return 0;
}
int GlRender::initVertex2()
{
    float width = 2.0f/numOfTriangleBottom;
    float height = width;


    for(int i=0;i<numOfTriangleBottom;i++){
        int numOfTriangThisLine = numOfTriangleBottom - i;

        float fBottom = -1.0f+i*height;
        float fTop = fBottom + height;

        for(int j=0;j<numOfTriangThisLine;j++){

            //printf("%d, %d\n", i,j);
            float fLeft = -1.0f + j*width+i*width/2.0f;
            float fRight = fLeft + width;
            VertexAtt pt1(Position(fLeft, fBottom, 0.0f), Color(1.0f, 0.0f, 0.0f));
            VertexAtt pt2(Position(fRight, fBottom, 0.0f), Color(0.0f, 1.0f, 0.0f));
            VertexAtt pt3(Position(0.5f*(fLeft+fRight), fTop, 0.0f), Color(0.0f, 0.0f, 1.0f));

            vertices.push_back(pt1);
            vertices.push_back(pt2);
            vertices.push_back(pt3);

        }
    }

    return 0;
}

int GlRender::initGL()
{
    initVertex2();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexAtt), &vertices[0], GL_STATIC_DRAW);


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

    unsigned int aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)0);
    glEnableVertexAttribArray(aPosLocation);

    unsigned int iColor = glGetAttribLocation(shaderProgram, "iColor");
    glVertexAttribPointer(iColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)(sizeof(Position)));
    glEnableVertexAttribArray(iColor);

    return 0;
}

void GlRender::drawingGL()
{
    glUseProgram(shaderProgram);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    int nNum = vertices.size()/3;
    for(int i=0;i<nNum;i++){
        glDrawArrays(GL_TRIANGLES, i*3, 3);

    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GlRender::terminateGL()
{
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

}
