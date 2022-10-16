#include "gamemanager.h"
#include "gamemodel.h"

GameModel::GameModel()
{

}

int GameModel::InitModel()
{
    initVecObject(9);
    return 0;
}

void GameModel::initVecObject(int nNumberObject)
{
    int numOfTriangleBottom = nNumberObject;
    float width = 2.0f/numOfTriangleBottom;
    float height = width;




    for(int i=0;i<numOfTriangleBottom;i++){
        int numOfTriangThisLine = numOfTriangleBottom;// - i;

        float fBottom = -1.0f+i*height;
        float fTop = fBottom + height;

        for(int j=0;j<numOfTriangThisLine;j++){

            float fLeft = -1.0f + j*width;
            float fRight = fLeft + width;

            Object obj;

            VertexAtt pt1(Position(fLeft, fBottom, 0.0f), Color(0.0f, 0.0f, m_vecObject.size()), TextureCoord(1.0f, 1.0f));
            VertexAtt pt2(Position(fRight, fBottom, 0.0f), Color(0.0f, 0.0f, m_vecObject.size()), TextureCoord(1.0f, 0.0f));
            VertexAtt pt3(Position(fRight - 0.2*width, fTop - 0.2*width, 0.0f), Color(0.0f, 0.0f, m_vecObject.size()), TextureCoord(0.0f, 0.0f));
            VertexAtt pt4(Position(fLeft + 0.2*width, fTop - 0.2*width, 0.0f), Color(0.0f, 0.0f, m_vecObject.size()), TextureCoord(0.0f, 1.0f));

            obj.vecVertex.push_back(pt1);
            obj.vecVertex.push_back(pt2);
            obj.vecVertex.push_back(pt3);
            obj.vecVertex.push_back(pt4);
            float a =0.0f, x=0.0f, y=0.0f;
            get_area_centroid(obj, a, x, y);
            for(int ptIndex = 0;ptIndex<obj.vecVertex.size();ptIndex++){
                obj.vecVertex[ptIndex].color.r = x;
                obj.vecVertex[ptIndex].color.g = y;
            }

            m_vecObject.push_back(obj);

        }
    }
    m_pModelChangeListener->OnModelChange(this);



}



void GameModel::OnControllerEvent(ControllerEventData event)
{
    GameManager* pMgr = GameManager::GetInstance();
    GameRender* pGameRender = pMgr->GetGameRender();

    if(event.nData>=0){
        Object &obj = m_vecObject[event.nData];
        float fOffset = 0.03;
        switch (event.event) {
        case IControlEventListener::eMoveRight:
            transition_my_obj(obj, fOffset, 0.0f);
            break;
        case IControlEventListener::eMoveLeft:
            transition_my_obj(obj, -fOffset, 0.0f);

            break;
        case IControlEventListener::eMoveDown:
            transition_my_obj(obj, 0.0f, -fOffset);

            break;
        case IControlEventListener::eMoveUp:
            transition_my_obj(obj, 0.0f, +fOffset);

            break;
        default:
            break;
        }
        pGameRender->OnUpdateObject(obj);
    }
}
