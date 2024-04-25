#ifndef __GamePlane__H__
#define __GamePlane__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class GamePlane : public Node
{
public:
    static GamePlane* createGamePlane();
    virtual bool init();

    CREATE_FUNC(GamePlane);
    
    
    int nWidth;
    int nHeight;
    Sprite* sPlane;
 

    Animation* createAnimation(std::string nameFrame, int numberFrame, float delay);

    int getMoveCheckRight();
    int getMoveCheckLeft();
    int getMoveCheckUp();
    int getMoveCheckDown();
    int getHealthPlane();
    int getWidth();
    int getHeight();
    

    void setHealthPlane();
    void setMoveRight();
    void setMoveLeft();
    void setMoveUp();
    void setMoveDown();
    void setMoveIdolLeft();
    void setMoveIdolRight();
    void setMoveIdolUp();
    void setMoveIdolDown();
  
    
    

private:
    int healthPlane;
    int mCheckLeft = 0;
    int mCheckRight = 0;
    int mCheckDown = 0;
    int mCheckUp = 0;
};
#endif; // __PlanGame__H__




