#include "texturebase.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/type_ptr.hpp"


namespace l3_textures{


using namespace std;

TextureBase::TextureBase()
{
}
const char *TextureBase::GetTexturePath()
{
    return "../qt-opengl-learn3-texture/wall.jpg";
}

const char *TextureBase::GetVertexShaderSource()
{
    const char * VertexShaderSource = R"GLSL(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;
        uniform mat4 transform;
        out vec3 ourColor;
        out vec2 TexCoord;
        out vec3 oPos;

        void main()
        {
            gl_Position = transform * vec4(aPos, 1.0);
            //gl_Position = vec4(aPos, 1.0);
            oPos = aPos;
            ourColor = aColor;
            TexCoord = aTexCoord;
        }
    )GLSL";
    return VertexShaderSource;
}

const char *TextureBase::GetFragmentShaderSource()
{
    const char * FragmentShaderSource = R"GLSL(
        #version 330 core
        out vec4 FragColor;

        in vec3 ourColor;
        in vec2 TexCoord;
        in vec3 oPos;
        uniform sampler2D ourTexture;

        void main()
        {
            if(abs(oPos.x) >= 0.45f
                || abs(oPos.y) >= 0.45f
            )
            {
                FragColor = vec4(ourColor, 1.0f);
                //FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
            }
            else{
                FragColor = texture(ourTexture, TexCoord);

            }
        }
    )GLSL";
    return FragmentShaderSource;
}


int TextureBase::initVertex()
{
    // positions                           // colors                // texture coords
    vertices.push_back(VertexAtt(Position(+0.5f, +0.5f, 0.0f), Color(1.0f, 0.0f, 0.0f), TextureCoord(1.0f, 1.0f)));   // top right
    vertices.push_back(VertexAtt(Position(+0.5f, -0.5f, 0.0f), Color(0.0f, 1.0f, 0.0f), TextureCoord(1.0f, 0.0f)));   // bottom right
    vertices.push_back(VertexAtt(Position(-0.5f, -0.5f, 0.0f), Color(0.0f, 0.0f, 1.0f), TextureCoord(0.0f, 0.0f)));   // bottom left
    vertices.push_back(VertexAtt(Position(-0.5f, +0.5f, 0.0f), Color(1.0f, 1.0f, 0.0f), TextureCoord(0.0f, 1.0f)));    // top left

    return 0;
}

int TextureBase::initGL()
{
    initVertex();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexAtt), &vertices[0], GL_DYNAMIC_DRAW );


    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(int), &indices[0], GL_STATIC_DRAW);

    //unsigned int vertexShader;
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char * VertexShaderSource = GetVertexShaderSource();
    glShaderSource(m_vertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(m_vertexShader);

    int success;
    char infoLog[512] = {0};
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    //fragment shader
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* FragmentShaderSource = GetFragmentShaderSource();
    glShaderSource(m_fragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(m_fragmentShader);

    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(m_fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;

    }


    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glLinkProgram(m_shaderProgram);
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK\n" << infoLog << std::endl;
        return -1;
    }



    unsigned int aPosLocation = glGetAttribLocation(m_shaderProgram, "aPos");
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)0);
    glEnableVertexAttribArray(aPosLocation);

    unsigned int aColor = glGetAttribLocation(m_shaderProgram, "aColor");
    glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)(sizeof(Position)));
    glEnableVertexAttribArray(aColor);


    unsigned int aTexCoord = glGetAttribLocation(m_shaderProgram, "aTexCoord");
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)(sizeof(Position)+sizeof(Color)));
    glEnableVertexAttribArray(aTexCoord);

    //begin loading image
    int width, height, nrChannels;
    glGenTextures(1, &m_nTexture);
    glBindTexture(GL_TEXTURE_2D, m_nTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data = stbi_load(
        GetTexturePath(),
        &width, &height, &nrChannels, 0);
    printf("w=%d h=%d\n", width, height);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
             0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    //end loading image

    return 0;
}

void TextureBase::drawingGL()
{
    unsigned int transformLoc = 0;
    glm::mat4 transform = glm::mat4(1.0f);

    glUseProgram(m_shaderProgram);
    transformLoc = glGetUniformLocation(m_shaderProgram, "transform");
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindTexture(GL_TEXTURE_2D, m_nTexture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void TextureBase::terminateGL()
{

}




}
