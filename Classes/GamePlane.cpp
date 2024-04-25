#include "GamePlane.h"

USING_NS_CC;

GamePlane* GamePlane::createGamePlane()
{
    return GamePlane::create();
}

bool GamePlane::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    healthPlane = 3;

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plane.plist","plane.png");

    sPlane = Sprite::createWithSpriteFrameName("planeOne1.png");
    nWidth = sPlane->getContentSize().width;
    nHeight = sPlane->getContentSize().height;
    this->addChild(sPlane);

    auto sPlaneBody = PhysicsBody::createBox(sPlane->getContentSize());
    sPlaneBody->setDynamic(false);
    sPlaneBody->setContactTestBitmask(true);
    sPlaneBody->setCollisionBitmask(1);
   
    this->addComponent(sPlaneBody);


    Animate* aPlan = Animate::create(GamePlane::createAnimation("planeOne", 5, 0.1));
    sPlane->runAction(RepeatForever::create(aPlan));

    return true;
}
Animation* GamePlane::createAnimation(std::string nameFrame, int numberFrame, float delay)
{
    Vector<SpriteFrame*> frameAni;
    for (int i = 1; i <= numberFrame; i++)
    {
        char buf[20] = { 0 };
        sprintf(buf,"%d.png",i);
        std::string sFrame = nameFrame + buf;
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sFrame);
        frameAni.pushBack(frame);
    }
    Animation* aAnimation = Animation::createWithSpriteFrames(frameAni, delay);
    return aAnimation;
}

int GamePlane::getMoveCheckRight()
{
    return this->mCheckRight;
}
int GamePlane::getMoveCheckLeft()
{
    return this->mCheckLeft;
}
int GamePlane::getMoveCheckUp()
{
    return this->mCheckUp;
}
int GamePlane::getMoveCheckDown()
{
    return this->mCheckDown;
}

void GamePlane::setMoveRight()
{
    mCheckRight = 1;
}
void GamePlane::setMoveLeft()
{
    mCheckLeft = 1;
}
void GamePlane::setMoveUp()
{
    mCheckUp = 1;
}
void GamePlane::setMoveDown()
{
    mCheckDown = 1;
}

void GamePlane::setMoveIdolRight()
{
    mCheckRight = 0;
}
void GamePlane::setMoveIdolLeft()
{
    mCheckLeft = 0;
}
void GamePlane::setMoveIdolUp()
{
    mCheckUp = 0;
}
void GamePlane::setMoveIdolDown()
{
    mCheckDown = 0;
}


int GamePlane::getWidth()
{
    return nWidth;
}

int GamePlane::getHeight()
{
    return nHeight;
}

int GamePlane::getHealthPlane()
{
    return this->healthPlane;
}
void GamePlane::setHealthPlane()
{
    this->healthPlane -= 1;
}


