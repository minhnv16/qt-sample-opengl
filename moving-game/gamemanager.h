#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H


#include "GLFW/glfw3.h"
#include "gamecontroller.h"
#include "gamemodel.h"
#include "gamerender.h"

//manager all for game (window, controller, data model, renderer)
class GameManager
{
private:
    static GameManager*         m_pGameManager;
    GLFWwindow*                 m_pWindow {NULL};
    GameController*             m_pGameController {NULL};
    GameRender*                 m_pGameRender {NULL};
    GameModel*                  m_pGameModel {NULL};

    GameManager();

    static void key_callback_learn(GLFWwindow* window, int key, int scancode, int action, int mode);

public:
    static GameManager* GetInstance();
    static void DeleteInstance();
    ~GameManager();

    GameController* GetGameController(){return m_pGameController;};
    GameRender* GetGameRender(){return m_pGameRender;};
    GameModel* GetGameModel(){return m_pGameModel;};

    int InitWindow();
    int InitModel();
    int InitRenderer();
    int DrawWindow();

};

#endif // GAMEMANAGER_H
