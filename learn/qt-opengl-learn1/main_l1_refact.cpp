
#include "glrender.h"
#include "glrender_indices.h"
#include <unistd.h>
namespace l1_refact{

using namespace std;
int main(){
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

    GlRender re(window);
    re.numOfTriangleBottom = 3000;
    re.initGL();

    double lastTime = glfwGetTime();
    int nbFrames = 0;
    glfwSwapInterval(0);


    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame\n", 1000.0f / double(nbFrames));
            printf("fps=%d\n", nbFrames);

            printf("number of vertex=%d\n", re.vertices.size());
            nbFrames = 0;
            lastTime = currentTime;
        }

        re.drawingGL();
    }
    return 0;
}



GlRender_indices *g_pRender = NULL;

void key_callback_learn(GLFWwindow* window, int key, int scancode, int action, int mode) {
    std::cout << "key :" << key << std::endl;
    if (action == GLFW_PRESS) {
        if (key >= 320 && key <= 324) {
            if(g_pRender){
                g_pRender->MoveVertexData();
            }
        }
    }
}
int main_indices(){
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
    glfwSetKeyCallback(window, key_callback_learn);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    //init glew
    GLenum err = glewInit();

    if (GLEW_OK != err) {
        glfwTerminate();
        return -1;
    }

    GlRender_indices re(window);
    g_pRender = &re;
    re.numOfTriangleBottom = 3000;
    re.initGL();


    double lastTime = glfwGetTime();
    int nbFrames = 0;
    glfwSwapInterval(0);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0);

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame\n", 1000.0f / double(nbFrames));
            printf("fps=%d\n", nbFrames);

            printf("number of vertex=%d\n", re.vertices.size());
            printf("number of indices=%d\n", re.indices.size());

            nbFrames = 0;
            lastTime = currentTime;
        }

        re.drawingGL();
    }

    return 0;
}


}
