#ifndef __GameEnemyClass__H__
#define __GameEnemyClass__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class GameEnemyClass : public Node
{
public:
    

    virtual void setHealthEnemy(int dame) = 0;
    virtual int getHealthEnemy() = 0;
    
protected:
    float mHealthEnemy;

};
#endif; // __PlanGame__H__




