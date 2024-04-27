#ifndef __GameEnemyOne__H__
#define __GameEnemyOne__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameEnemyClass.h"
#include "GamePlay.h"

using namespace cocos2d;

class GameEnemyOne : public GameEnemyClass
{
public:
    static GameEnemyOne* createGameEnemyOne(int round, float numWidth, float numHeight);
    virtual bool init();
    

    CREATE_FUNC(GameEnemyOne);
    
    Sprite* spriteEnemyOne;

    int getHealthEnemy();
    void getCheckPause();

    void setHealthEnemy(int dame);
    void removeEnemy();

    
    Animation* createAnimation(std::string nameFrame, int number, float delay);

    void moveCircle180(float dt);
    void moveCircle270(float dt);
    float checkTime{ 0 };
    float checkRotation{ 0 };
    float sinEnemy{ 0 };
    float cosEnemy{ 0 };
    int checkStart{ 0 };
    float Pi{ 3.14159265359 };
    float xRotation{ 0 };
    float yRotation{ 0.5 };
    float checkTimeEnd{ 0.5 };
    float numberWidth{ 0 };
    float numberHeight{ 0 };
    int checkRound{ 0 };
    
private:
    bool checkPause{ false };
};
#endif; // __GameEnemyOne__H__




