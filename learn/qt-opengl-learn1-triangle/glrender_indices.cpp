#include "glrender_indices.h"
#include <math.h>



void GlRender_indices::MoveVertexData()
{
    for(int i=1;i<vertices.size();i++){
        vertices[i].pos.x -= 0.1f;
        vertices[i].pos.y -= 0.1f;

    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexAtt), &vertices[0], GL_STATIC_DRAW);
}

void GlRender_indices::MoveMatrixTransformData()
{

}

GlRender_indices::GlRender_indices(GLFWwindow *window)
{
    m_window = window;
}

int GlRender_indices::initVertex2()
{
    float width = 2.0f/numOfTriangleBottom;
    float height = width;
    VertexAtt pt0(Position(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f));
    vertices.push_back(pt0);


    for(int i=0;i<numOfTriangleBottom;i++){
        int numOfTriangThisLine = numOfTriangleBottom - i;

        float fBottom = -1.0f+i*height;
        float fTop = fBottom + height;

        for(int j=0;j<numOfTriangThisLine;j++){

            indices.push_back(0);

            //printf("%d, %d\n", i,j);
            float fLeft = -1.0f + j*width+i*width/2.0f;
            float fRight = fLeft + width;
            VertexAtt pt1(Position(fLeft, fBottom, 0.0f), Color(1.0f, 0.0f, 0.0f));
            VertexAtt pt2(Position(fRight, fBottom, 0.0f), Color(0.0f, 1.0f, 0.0f));
            VertexAtt pt3(Position(0.5f*(fLeft+fRight), fTop, 0.0f), Color(0.0f, 0.0f, 1.0f));


            indices.push_back(vertices.size());
            vertices.push_back(pt1);
            indices.push_back(vertices.size());
            vertices.push_back(pt2);
            indices.push_back(vertices.size());
            vertices.push_back(pt3);



        }
    }

    return 0;
}

int GlRender_indices::initGL()
{
    initVertex2();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexAtt), &vertices[0], GL_DYNAMIC_DRAW );

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(int), &indices[0], GL_STATIC_DRAW);


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

void GlRender_indices::drawingGL()
{
    glUseProgram(shaderProgram);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_LINE_LOOP, indices.size(), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GlRender_indices::terminateGL()
{
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

}

void GlRender_indices::use()
{
    glUseProgram(shaderProgram);

}

void GlRender_indices::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);

}

void GlRender_indices::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);

}

void GlRender_indices::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);

}
