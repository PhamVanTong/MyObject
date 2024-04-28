

#ifndef __GamePlay_SCENE_H__
#define __GamePlay_SCENE_H__

#include "cocos2d.h"
#include "GamePlane.h"
#include "GameLazer.h"
#include "GameEnemy.h"
#include "GamePause.h"


class GamePlay : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene(int m_skin);
    virtual bool init();
    CREATE_FUNC(GamePlay);

    void addButton();
    void addSprite();
    void checkGoldShop(int a);
    
    GamePlane* pPlaneOne;
    GamePause* classPause;
    PhysicsBody* physicEnemy;
    Label* lHealth;
    Label* labelDame;
    Label* labelScore;
    Label* labelGold;
    Label* labelHighScore;
    Label* labelContinue;
    Sprite* sHealth;
    Sprite* sPowUp;
    Sprite* sCoin;
    Sprite* spriteScore;
    Sprite* spriteGold;
    
    Animation* createAnimation(std::string nameFrame, int numberFrame, float delay);
    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    virtual void onKeyRealesed(EventKeyboard::KeyCode keyCode, Event* event);
    void updateMove(float);
    void setLabel(Label* labelCheck, int a);
    void addItem();
    bool onPhysicsContact(PhysicsContact& contact);
    void addEnemyLazer(float delay);
    void setPhysics(Sprite* spri, int tag, int second);
    void setRoundOne();
    void setRoundTwo();
    void setRoundThree();
    void setRoundFour();
    //void setRoundFour();
private:
    int mHealth{ 3 };
    int mDame{ 1 };
    int mScore{ 0 };
    int mGold{ 0 };
    int iHighScore{ 0 };
    bool checkPause{ false };
    int checkWin{ 0 };
    bool checkItem{ false };
    int xItem;
    int yItem;
    int checkRound{ 1 };
    int checkRound3{ 0 };
    PhysicsWorld* world;
    
    void setPhysicWorld(PhysicsWorld* _world)
    {
        world = _world;
    }
};

#endif // __HELLOWORLD_SCENE_H__
