#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/type_ptr.hpp"

#include "stb_image.h"
#include "texturebase.h"
#include "texturewall.h"

namespace l3_textures{
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



VertexAtt vertices[] = {
    // positions                           // colors                // texture coords
    VertexAtt(Position(0.5f,  0.5f, 0.0f), Color(1.0f, 0.0f, 0.0f), TextureCoord(1.0f, 1.0f)),   // top right
    VertexAtt(Position(0.5f, -0.5f, 0.0f), Color(0.0f, 1.0f, 0.0f), TextureCoord(1.0f, 0.0f)),   // bottom right
    VertexAtt(Position(-0.5f, -0.5f, 0.0f), Color(0.0f, 0.0f, 1.0f), TextureCoord(0.0f, 0.0f)),   // bottom left
    VertexAtt(Position(-0.5f,  0.5f, 0.0f), Color(1.0f, 1.0f, 0.0f), TextureCoord(0.0f, 1.0f))    // top left
};


int main_1texture()
{
    GLFWwindow* window = 0;
    if (GLFW_FALSE == glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int nWidth = 800, nHeight = 800;
    window = glfwCreateWindow(nWidth, nHeight, __FUNCTION__, NULL /* glfwGetPrimaryMonitor()*/, NULL);
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

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //vertext buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //vertex shader
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
            oPos = aPos;
            ourColor = aColor;
            TexCoord = aTexCoord;
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

        in vec3 ourColor;
        in vec2 TexCoord;
        in vec3 oPos;
        uniform sampler2D ourTexture;

        void main()
        {
            if(abs(oPos.x) >= 0.45f
                || abs(oPos.y) >= 0.45f
            ){
                FragColor = vec4(ourColor, 1.0f);
                //FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
            }
            else{
                FragColor = texture(ourTexture, TexCoord);

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



    unsigned int aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)0);
    glEnableVertexAttribArray(aPosLocation);

    unsigned int aColor = glGetAttribLocation(shaderProgram, "aColor");
    glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)(sizeof(Position)));
    glEnableVertexAttribArray(aColor);


    unsigned int aTexCoord = glGetAttribLocation(shaderProgram, "aTexCoord");
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)(sizeof(Position)+sizeof(Color)));
    glEnableVertexAttribArray(aTexCoord);


    //begin loading image
    int width, height, nrChannels;
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data = stbi_load(
        "../qt-opengl-learn3-texture/wall.jpg",
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

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");



    glfwSwapInterval(0);
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    glUseProgram(shaderProgram);
    glLineWidth(5);


    while (!glfwWindowShouldClose(window))
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

        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        //glDrawArrays(GL_LINE_LOOP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}


int main_2texture()
{
    GLFWwindow* window = 0;
    if (GLFW_FALSE == glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int nWidth = 800, nHeight = 800;
    window = glfwCreateWindow(nWidth, nHeight, __FUNCTION__, NULL /* glfwGetPrimaryMonitor()*/, NULL);
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

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //vertext buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //vertex shader
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
            oPos = aPos;
            ourColor = aColor;
            TexCoord = aTexCoord;
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

        in vec3 ourColor;
        in vec2 TexCoord;
        in vec3 oPos;
        uniform sampler2D ourTexture0;
        uniform sampler2D ourTexture1;

        void main()
        {
            if(abs(oPos.x) >= 0.45f
                || abs(oPos.y) >= 0.45f
            ){
                FragColor = vec4(ourColor, 1.0f);
                //FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
            }
            else{
                FragColor = texture(ourTexture0, TexCoord);

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



    unsigned int aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)0);
    glEnableVertexAttribArray(aPosLocation);

    unsigned int aColor = glGetAttribLocation(shaderProgram, "aColor");
    glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)(sizeof(Position)));
    glEnableVertexAttribArray(aColor);


    unsigned int aTexCoord = glGetAttribLocation(shaderProgram, "aTexCoord");
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAtt), (void*)(sizeof(Position)+sizeof(Color)));
    glEnableVertexAttribArray(aTexCoord);



//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, texture1);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, texture2);


    //begin loading image
    unsigned int texture0;
    if(1)

    {
        int width, height, nrChannels;


        glGenTextures(1, &texture0);
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        unsigned char *data = stbi_load(
            //"../qt-opengl-learn3-texture/wall.jpg",
            "../qt-opengl-learn3-texture/images.jpeg",

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
    }
    //end loading image

    unsigned int texture1;
    if(1)
    {
        int width, height, nrChannels;

        glGenTextures(1, &texture1);
        glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        unsigned char *data = stbi_load(
            "../qt-opengl-learn3-texture/wall.jpg",
            //"../qt-opengl-learn3-texture/images.jpeg",
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
    }

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");



    glfwSwapInterval(0);
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    glUseProgram(shaderProgram);
    glLineWidth(5);


    while (!glfwWindowShouldClose(window))
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

        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, texture1);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        //glDrawArrays(GL_LINE_LOOP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

int main_2texture_2shader_wrapper(){

    GLFWwindow* window = 0;
    if (GLFW_FALSE == glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int nWidth = 800, nHeight = 800;
    window = glfwCreateWindow(nWidth, nHeight, __FUNCTION__, NULL /* glfwGetPrimaryMonitor()*/, NULL);
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

    TextureBase re;
    TextureWall re_wall;

    int nInit = 0;
    nInit = re.initGL();
    if(nInit <0){
        return -1;
    }

    nInit = re_wall.initGL();
    if(nInit <0){
        return -1;
    }

    glfwSwapInterval(0);
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    glLineWidth(5);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    while (!glfwWindowShouldClose(window))
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

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        re.drawingGL();
        re_wall.drawingGL();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    return 0;
}
}//namespace l3

