#include "gamecontroller.h"
#include "gamemanager.h"


GameController::GameController()
{

}

void GameController::On_key_callback_learn(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    IControlEventListener::eControllerEvent event;

    if (action == GLFW_PRESS) {
        if(key==262){
            //right
            event = IControlEventListener::eControllerEvent::eMoveRight;
        }
        else if(key==263){
            //left
            event = IControlEventListener::eControllerEvent::eMoveLeft;

        }
        else if(key==264){
            //down
            event = IControlEventListener::eControllerEvent::eMoveDown;
        }
        else if(key==265){
            //up
            event = IControlEventListener::eControllerEvent::eMoveUp;

        }
        else{
            event = IControlEventListener::eControllerEvent::eSelectedId;
            m_nSelectedId = key;
        }
    }
    else{
        return;
    }

    GameManager* pMgr = GameManager::GetInstance();
    GameRender* pGameRender = pMgr->GetGameRender();
    GameModel* pGameModel = pMgr->GetGameModel();

    if(event == IControlEventListener::eControllerEvent::eSelectedId){
        m_objSelectedObj = & pGameModel->m_vecObject[m_nSelectedId];
        pGameRender->OnSelectedId(m_nSelectedId);
    }
    else{
        IControlEventListener::ControllerEventData data;
        data.event = event;
        data.nData = m_nSelectedId;
        if(m_pControlEventListener)
            m_pControlEventListener->OnControllerEvent(data);
    }




}
