#include "shaderwrapper.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
namespace l2_shader_wrap{
using namespace std;

int main(){

    //ShaderWrapper shd("", "");
    ShaderWrapper ourShader("/home/minh/Desktop/vertex.sh", "/home/minh/Desktop/frag.sh");
    return 0;
}
}
