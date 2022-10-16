#include "gamemanager.h"

GameManager::GameManager()
{
    m_pGameRender = new GameRender();
    m_pGameController = new GameController();
    m_pGameModel = new GameModel();
    m_pGameModel->m_pModelChangeListener = m_pGameRender;
    m_pGameController->m_pControlEventListener = m_pGameModel;
}

void GameManager::key_callback_learn(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    GameController *pGameController = GameManager::GetInstance()->GetGameController();
    if(pGameController){
        pGameController->On_key_callback_learn(window, key, scancode, action, mode);
    }
}
GameManager* GameManager::m_pGameManager = NULL;
GameManager *GameManager::GetInstance()
{
    if(m_pGameManager == NULL){
        m_pGameManager = new GameManager();
    }
    return m_pGameManager;
}

void GameManager::DeleteInstance()
{
    if(m_pGameManager){
        delete m_pGameManager;
        m_pGameManager = NULL;
    }
}

GameManager::~GameManager()
{
    delete m_pGameRender;
    delete m_pGameController;
    delete m_pGameModel;
}

int GameManager::InitWindow()
{
    if (GLFW_FALSE == glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int nWidth = 800, nHeight = 800;
    m_pWindow = glfwCreateWindow(nWidth, nHeight, __FUNCTION__, NULL /* glfwGetPrimaryMonitor()*/, NULL);
    if (!m_pWindow) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(m_pWindow);
    glfwSetKeyCallback(m_pWindow, key_callback_learn);


    //glewExperimental = GL_TRUE;


    return 0;
}

int GameManager::InitModel()
{
    m_pGameModel->InitModel();
    return 0;
}

int GameManager::InitRenderer()
{
    m_pGameRender->InitRenderer();

    return 0;
}

int GameManager::DrawWindow()
{
    while (!glfwWindowShouldClose(m_pWindow))
    {
        m_pGameRender->DrawWindow();
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
