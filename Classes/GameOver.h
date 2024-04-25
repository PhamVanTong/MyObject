

#ifndef __GameOver_SCENE_H__
#define __GameOver_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"


class GameOver : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene(int score, int highscore);
    virtual bool init();
    CREATE_FUNC(GameOver);

    void addButton();
    void addLabel();
    int mSkin=1;
};

#endif // __HELLOWORLD_SCENE_H__
