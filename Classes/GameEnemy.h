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

    float numberWidth{ 0 };
    float numberHeight{ 0 };
    int checkRound{ 0 };

    float checkTime{ 0 };
    float checkTimeEnd{ 0.5 };
    float checkRotation{ 0.f };
    int checkStart{ 0 };
    int checkRotationSpeed{ 0 };
    
    Animation* createAnimation(std::string nameFrame, int number, float delay);

    void moveCircle180(float dt);
    void moveCircle270(float dt);
    void setHealthEnemy(int dame);
    void removeEnemy();

    float sinEnemy{ 0 };
    float cosEnemy{ 0 };
    
    float Pi{ 3.14159265359 };
    float xRotation{ 0 };
    float yRotation{ 0.5 };
    
private:
    bool checkPause{ false };
};
#endif; // __GameEnemyOne__H__




