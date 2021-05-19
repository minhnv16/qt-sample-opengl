#include "WindowCreator.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

using namespace std;

int main()
{
    WindowCreator a;

    cout << "Hello G1L!" << endl;





    GLFWwindow* window = 0;
    if (GLFW_FALSE == glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    cout << "glfwWindowHint" << endl;

    int nWidth = 800, nHeight = 600;
    //get_resolution(nWidth, nHeight);
    window = glfwCreateWindow(nWidth, nHeight, "Test OpenGL", NULL /* glfwGetPrimaryMonitor()*/, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }
    else{
        cout << "glfwCreateWindow OK" << endl;
    }

    GLenum err = glewInit();

    if (GLEW_OK != err) {
        glfwTerminate();
        cout << "GLEW_OK != err" <<err<< endl;
        return -1;
    }
    cout << "GLEW_OK" << endl;


    return 0;
}
