

#ifndef __GameMenu_SCENE_H__
#define __GameMenu_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Definitions.h"


class GameMenu : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameMenu);

    int mSkin{ 0 };
    void addButton();
    void addSprite();
    void checkGold(int a);
   
};

#endif // __HELLOWORLD_SCENE_H__
