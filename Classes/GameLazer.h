#ifndef __GameLazer__H__
#define __GameLazer__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class GameLazer : public Node
{
public:
    static GameLazer* createGameLazer(int dame);
    virtual bool init();

    CREATE_FUNC(GameLazer);

    Sprite* spriteLazer;
    Animation* createAnimation(std::string nameFrame, int numberFrame, float delay);

    int getLazerDame();
    void setDameLazerI();
    void setDameLazerD();
    void removeLazer();
    void removeLazerAnimation();
    void getCheckPause();

private:
    bool checkPause{ false };
    int dameLazer;
};
#endif; // __PlanGame__H__




