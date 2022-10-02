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
#include "texturebase.h"
#include "texturewall.h"

namespace l3_textures_1renderer_optimize_performance{
using namespace std;




unsigned int VAO, VBO, EBO;
std::vector<Object> g_vecObject;
std::vector<VertexAtt> g_vecVertices;
std::vector<int> g_indices;

void initVecObject(int nNumberObject){

    int numOfTriangleBottom = nNumberObject;
    float width = 2.0f/numOfTriangleBottom;
    float height = width;




    for(int i=0;i<numOfTriangleBottom;i++){
        int numOfTriangThisLine = numOfTriangleBottom;// - i;

        float fBottom = -1.0f+i*height;
        float fTop = fBottom + height;

        for(int j=0;j<numOfTriangThisLine;j++){

            float fLeft = -1.0f + j*width;
            float fRight = fLeft + width;

            Object obj;

            VertexAtt pt1(Position(fLeft, fBottom, 0.0f), Color(0.0f, 0.0f, g_vecObject.size()), TextureCoord(1.0f, 1.0f));
            VertexAtt pt2(Position(fRight, fBottom, 0.0f), Color(0.0f, 0.0f, g_vecObject.size()), TextureCoord(1.0f, 0.0f));
            VertexAtt pt3(Position(fRight - 0.2*width, fTop - 0.2*width, 0.0f), Color(0.0f, 0.0f, g_vecObject.size()), TextureCoord(0.0f, 0.0f));
            VertexAtt pt4(Position(fLeft + 0.2*width, fTop - 0.2*width, 0.0f), Color(0.0f, 0.0f, g_vecObject.size()), TextureCoord(0.0f, 1.0f));

            obj.vecVertex.push_back(pt1);
            obj.vecVertex.push_back(pt2);
            obj.vecVertex.push_back(pt3);
            obj.vecVertex.push_back(pt4);
            float a =0.0f, x=0.0f, y=0.0f;
            get_area_centroid(obj, a, x, y);
            for(int ptIndex = 0;ptIndex<obj.vecVertex.size();ptIndex++){
                obj.vecVertex[ptIndex].color.r = x;
                obj.vecVertex[ptIndex].color.g = y;
            }

            g_vecObject.push_back(obj);

        }
    }


    VertexAtt pt0(Position(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), TextureCoord(0.0f, 0.0f));
    g_vecVertices.push_back(pt0);

    for(int i=0;i<g_vecObject.size();i++){
        Object obj = g_vecObject[i];
        g_indices.push_back(0);
        int nIndexLastArrayVertex = g_vecVertices.size();

        for(int j=0;j<obj.vecVertex.size();j++){
            g_indices.push_back(g_vecVertices.size());
            g_vecVertices.push_back(obj.vecVertex[j]);
        }

        g_vecObject[i].nIndexInVertices = nIndexLastArrayVertex;

    }
}


void update_object_to_vertex(const Object & obj){
    int nStartIndex = obj.nIndexInVertices;
    for(int i=0; i < obj.vecVertex.size();i++){
        g_vecVertices[nStartIndex] = obj.vecVertex[i];
        nStartIndex++;
    }
}

void transform_all_object(){
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    double time  = glfwGetTime();
    for( int i=0;i < g_vecObject.size();i++){
        Object obj = g_vecObject[i];
        float a, x, y;
        get_area_centroid(obj, a, x, y);

        float angle = time * i / 3;
        rotate_my_obj(obj, x, y, angle);
        update_object_to_vertex(obj);
    }    

    glBufferData(GL_ARRAY_BUFFER, g_vecVertices.size()*sizeof(VertexAtt), &g_vecVertices[0], GL_STATIC_DRAW);

}
void key_callback_learn(GLFWwindow* window, int key, int scancode, int action, int mode) {

    transform_all_object();

}
int main_1renderer_2texture_multi_object_optimize()
{
    //init data

    initVecObject(50);

    //end init data
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
    glfwSetKeyCallback(window, key_callback_learn);

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

    //unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //vertext buffer object
    //unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, g_vecVertices.size()*sizeof(VertexAtt), &g_vecVertices[0], GL_STATIC_DRAW);

    //unsigned int EBO;

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_indices.size()* sizeof(int), &g_indices[0], GL_STATIC_DRAW);


    //vertex shader
    const char * VertexShaderSource = R"GLSL(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;
        uniform float aTime;
        uniform mat4 transform;
        out vec3 ourColor;
        out vec2 TexCoord;


        void main()
        {
            vec3 tmpPos = aPos;
            float angle = 0.0f;
            //if(aColor.x == 3)
            {
                angle = aTime * aColor.z / 3;
            }

            float s = sin(angle);
            float c = cos(angle);

            //center point for rotate
            float cx = aColor.x;
            float cy = aColor.y;

            tmpPos.x -= cx;
            tmpPos.y -= cy;

            // rotate point
            float xnew = tmpPos.x * c - tmpPos.y * s;
            float ynew = tmpPos.x * s + tmpPos.y * c;

            // translate point back:
            tmpPos.x = xnew + cx;
            tmpPos.y = ynew + cy;

            gl_Position = transform * vec4(tmpPos, 1.0);


            //oPos = aPos;
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

        in vec2 TexCoord;
//        in vec3 oPos;
        uniform sampler2D texture0;
        uniform sampler2D texture1;

        void main()
        {
            FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.6);
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
    unsigned int texture0;
    if(1)

    {
        int width, height, nrChannels;


        glGenTextures(1, &texture0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

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
    }
    //end loading image

    unsigned int texture1;
    if(1)
    {
        int width, height, nrChannels;

        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(
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
    glUseProgram(shaderProgram);

    unsigned int aTimeLoc = glGetUniformLocation(shaderProgram, "aTime");
    glUniform1f(aTimeLoc, glfwGetTime());

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));



    glfwSwapInterval(0);
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    glUniform1i(glGetUniformLocation(shaderProgram, "texture0"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0);

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

        unsigned int aTimeLoc = glGetUniformLocation(shaderProgram, "aTime");
        glUniform1f(aTimeLoc, currentTime);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDrawElements(GL_TRIANGLE_FAN, g_indices.size(), GL_UNSIGNED_INT, 0);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

}//namespace l3

