#include "glrender.h"




GlRender::GlRender(GLFWwindow *window)
{
    m_window = window;
    vertices.push_back(VertexAtt(Position(-0.5f, -0.5f, 0.0f), Color(1.0f, 0.0f, 0.0f)));
    vertices.push_back(VertexAtt(Position(+0.5f, -0.5f, 0.0f), Color(0.0f, 1.0f, 0.0f)));
    vertices.push_back(VertexAtt(Position(+0.0f, +0.5f, 0.0f), Color(0.0f, 0.0f, 1.0f)));

    vertices.push_back(VertexAtt(Position(+0.8f, +0.8f, 0.0f), Color(1.0f, 0.0f, 0.0f)));
    vertices.push_back(VertexAtt(Position(+0.8f, +0.9f, 0.0f), Color(0.0f, 1.0f, 0.0f)));
    vertices.push_back(VertexAtt(Position(+0.7f, +0.9f, 0.0f), Color(0.0f, 0.0f, 1.0f)));

}

int GlRender::initGL()
{
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
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 3);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GlRender::terminateGL()
{
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

}
