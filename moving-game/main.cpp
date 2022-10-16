#include "gamemanager.h"
#include "gamerender.h"

#include <iostream>
#include <GLFW/glfw3.h>

using namespace std;

int main()
{


    cout << "Hello World!" << endl;
    GameManager* pMgr = GameManager::GetInstance();
    if(pMgr == NULL){
        return -1;
    }


    int nRet = 0;
    nRet = pMgr->InitWindow();
    if(nRet <0 ) return -1;
    nRet = pMgr->InitRenderer();
    if(nRet <0 ) return -1;
    nRet = pMgr->InitModel();
    if(nRet <0 ) return -1;
    nRet = pMgr->DrawWindow();
    if(nRet <0 ) return -1;
    GameManager::DeleteInstance();

    return 0;
}
