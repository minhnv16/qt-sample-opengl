#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include "GLFW/glfw3.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include "gamemodel.h"
#include "gamerender.h"

void GameRender::OnModelChange(void *pModel)
{
    m_vecVertices.clear();
    m_indices.clear();

    VertexAtt pt0(Position(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), TextureCoord(0.0f, 0.0f));
    m_vecVertices.push_back(pt0);

    GameModel* pGameModel = dynamic_cast<GameModel*>((GameModel*)pModel);
    std::vector<Object> &m_vecObject = pGameModel->m_vecObject;

    for(int i=0;i<m_vecObject.size();i++){
        Object obj = m_vecObject[i];
        m_indices.push_back(0);
        int nIndexLastArrayVertex = m_vecVertices.size();

        for(int j=0;j<obj.vecVertex.size();j++){
            m_indices.push_back(m_vecVertices.size());
            m_vecVertices.push_back(obj.vecVertex[j]);
        }

        m_vecObject[i].nIndexInVertices = nIndexLastArrayVertex;

    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vecVertices.size()*sizeof(VertexAtt), &m_vecVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(int), &m_indices[0], GL_STATIC_DRAW);

}

GameRender::GameRender()
{

}

void GameRender::initVecTexture(int nNumberTexture)
{

    for(int i=0;i<nNumberTexture;i++)
    {
        unsigned int texture;

        //begin loading image
        int width, height, nrChannels;


        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

        char szImagePath[512] = {0};
        sprintf(szImagePath, "../learn/qt-opengl-learn3-texture/texture/%d.jpg", i);
        unsigned char *data = stbi_load(
            szImagePath,
            &width, &height, &nrChannels, 0);
        printf("w=%d h=%d\n", width, height);
        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else{
            std::cout << "Failed to load texture" << std::endl;
            return;
        }
        stbi_image_free(data);
        //end loading image
        m_vecTexture.push_back(texture);
    }



}

void GameRender::OnSelectedId(int selectedId)
{
    glUseProgram(m_shaderProgram);
    unsigned int indexLoc = glGetUniformLocation(m_shaderProgram, "aIndexSelect");
    glUniform1i(indexLoc, selectedId);
}

void GameRender::OnUpdateObject(Object obj)
{
    glUseProgram(m_shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

}


int GameRender::InitRenderer()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        glfwTerminate();
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    //unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //vertext buffer object
    //unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, m_vecVertices.size()*sizeof(VertexAtt), &m_vecVertices[0], GL_STATIC_DRAW);

    //unsigned int EBO;

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()* sizeof(int), &m_indices[0], GL_STATIC_DRAW);


    //vertex shader

    const char * VertexShaderSource = R"GLSL(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;
        uniform float aTime;
        out vec3 ourColor;
        out vec2 TexCoord;
        out float oIndex;

        void rotate_point(float cx, float cy, float angle,inout float x,inout float y)
        {
            float s = sin(angle);
            float c = cos(angle);

            // translate point back to origin:
            x -= cx;
            y -= cy;

            // rotate point
            float xnew = x * c - y * s;
            float ynew = x * s + y * c;

            // translate point back:
            x = xnew + cx;
            y = ynew + cy;

        }

        void main()
        {
            vec3 tmpPos = aPos;
            float angle = 0.0f;
            angle = aTime * aColor.z / 3;

            rotate_point(aColor.x, aColor.y, angle, tmpPos.x, tmpPos.y);
            rotate_point(0.0f, 0.0f, aTime, tmpPos.x, tmpPos.y);
            gl_Position = vec4(tmpPos, 1.0);

            ourColor = aColor;
            TexCoord = aTexCoord;
            oIndex = aColor.z;
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
        out vec4 FragColor;

        in vec2 TexCoord;
        in float oIndex;

        uniform sampler2D texture0;
        uniform sampler2D texture1;
        uniform sampler2D texture2;

        uniform int aIndexSelect;

        void main()
        {
            int nIndex = int(oIndex) % 2;
            if(nIndex == 0){
                //FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.6);
                FragColor = mix(texture(texture1, TexCoord), texture(texture0, TexCoord), 0.6);
            }
            else{
                //FragColor = mix(texture(texture2, TexCoord), texture(texture1, TexCoord), 0.6);
                FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.6);
            }
            if(aIndexSelect == oIndex){
                FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
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

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
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

    glUseProgram(m_shaderProgram);

    unsigned int aTimeLoc = glGetUniformLocation(m_shaderProgram, "aTime");
    //glUniform1f(aTimeLoc, glfwGetTime());

    unsigned int indexLoc = glGetUniformLocation(m_shaderProgram, "aIndexSelect");
    glUniform1i(indexLoc, -1);

    //

    initVecTexture(3);

    glfwSwapInterval(0);
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    for(int i=0;i<m_vecTexture.size();i++){
        char szTexure[512] = {0};
        sprintf(szTexure, "texture%d", i);
        int nTexturePos = glGetUniformLocation(m_shaderProgram, szTexure);
        glUniform1i(nTexturePos, i);
    }

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0);

    glLineWidth(5);

    return 0;
}
int nbFrames = 0;
double lastTime = glfwGetTime();

int GameRender::DrawWindow()
{

    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 1.0)
    { // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%f ms/frame\n", 1000.0f / double(nbFrames));
        printf("fps=%d\n", nbFrames);
        nbFrames = 0;
        lastTime = currentTime;
    }

    unsigned int aTimeLoc = glGetUniformLocation(m_shaderProgram, "aTime");
    glUniform1f(aTimeLoc, currentTime);


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    for(int i=0;i<m_vecTexture.size();i++){
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, m_vecTexture[i]);
    }

    //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawElements(GL_TRIANGLE_FAN, m_indices.size(), GL_UNSIGNED_INT, 0);


    return 0;

}




