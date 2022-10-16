#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "common.h"

#include <vector>



//handle data model - map, object, charactor
class GameModel: public IControlEventListener
{
public:
    std::vector<Object> m_vecObject;
    IModelChangeListener *m_pModelChangeListener = nullptr;
public:

    GameModel();
    void initVecObject(int nNumberObject);
    int InitModel();
    virtual void OnControllerEvent(ControllerEventData event);


};

#endif // GAMEMODEL_H
