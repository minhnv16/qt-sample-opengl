#ifndef GAMERENDER_H
#define GAMERENDER_H


#include "GLFW/glfw3.h"

#include "common.h"

#include <vector>

//handle the renderer for drawing screen - shader program, vertex shader, fragment shader
class GameRender: public IModelChangeListener
{
private:

    unsigned int VAO, VBO, EBO;
    std::vector<VertexAtt> m_vecVertices;
    std::vector<int> m_indices;
    unsigned int m_shaderProgram;
    std::vector<unsigned int> m_vecTexture;


public:
    virtual void OnModelChange(void *pModel);
    void initVecTexture(int nNumberTexture);
    void OnSelectedId(int selectedId);
    void OnUpdateObject(Object obj);
    GameRender();
    int InitRenderer();
    int DrawWindow();

};

#endif // GAMERENDER_H
