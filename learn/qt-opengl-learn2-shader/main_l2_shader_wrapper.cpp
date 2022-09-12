#include "shaderwrapper.h"
#include "testclass.h"

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <sys/stat.h>
namespace l2_shader_wrap{

int main(){

    struct stat stVertex;
    stat("/home/minh/Desktop/vertex.sh", &stVertex);
    size_t sizeVertex = stVertex.st_size;
    printf("%ld", sizeVertex);
    ShaderWrapper ourShader("/home/minh/Desktop/vertex.sh", "/home/minh/Desktop/frag.sh");
    return 0;
}
}
