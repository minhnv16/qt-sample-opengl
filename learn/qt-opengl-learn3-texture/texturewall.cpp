#include "texturewall.h"
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/type_ptr.hpp"
namespace l3_textures{

TextureWall::TextureWall()
{
}
const char *TextureWall::GetTexturePath()
{
    return "../qt-opengl-learn3-texture/images.jpeg";

}
const char *TextureWall::GetFragmentShaderSource()
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
            FragColor = texture(ourTexture, TexCoord);
        }
    )GLSL";
    return FragmentShaderSource;
}

int TextureWall::initVertex()
{
    // positions                           // colors                // texture coords
    vertices.push_back(VertexAtt(Position(+1.0f, +1.0f, 0.0f), Color(1.0f, 0.0f, 0.0f), TextureCoord(1.0f, 1.0f)));   // top right
    vertices.push_back(VertexAtt(Position(+1.0f, +0.5f, 0.0f), Color(0.0f, 1.0f, 0.0f), TextureCoord(1.0f, 0.0f)));   // bottom right
    vertices.push_back(VertexAtt(Position(+0.5f, +0.5f, 0.0f), Color(0.0f, 0.0f, 1.0f), TextureCoord(0.0f, 0.0f)));   // bottom left
    vertices.push_back(VertexAtt(Position(+0.5f, +1.0f, 0.0f), Color(1.0f, 1.0f, 0.0f), TextureCoord(0.0f, 1.0f)));   // top left
    return 0;
}

void TextureWall::drawingGL()
{
    unsigned int transformLoc = 0;
    glm::mat4 transform = glm::mat4(1.0f);

    glUseProgram(m_shaderProgram);

    transformLoc = glGetUniformLocation(m_shaderProgram, "transform");
    transform = glm::rotate(transform, -(float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}



}
