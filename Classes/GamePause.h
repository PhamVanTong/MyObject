

#ifndef __GamePause_Layer_H__
#define __GamePause_Layer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"


class GamePause : public cocos2d::Layer
{
public:
    static cocos2d::Layer* createScene();
    virtual bool init();

    CREATE_FUNC(GamePause);

    Sprite* spriteBackGround;

    bool getCheckPause();

    void addSprite();
    void moveDown();
    void moveUp();
    void getSkin(int m);

private:
    int mSkin{ 1 };
    bool checkPause{ false };
};

#endif // __GamePause_Layer_H__
