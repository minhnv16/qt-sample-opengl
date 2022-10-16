#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include "GLFW/glfw3.h"
#include "common.h"

//handle key controller, game logic
class GameController
{
public:
    int m_nSelectedId = -1;
    Object* m_objSelectedObj = nullptr;
public:
    IControlEventListener* m_pControlEventListener = nullptr;
    GameController();
    void On_key_callback_learn(GLFWwindow* window, int key, int scancode, int action, int mode);

};

#endif // GAMECONTROLLER_H
