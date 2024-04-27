#include "GameLazer.h"
USING_NS_CC;
int mDame;
GameLazer* GameLazer::createGameLazer(int dame)
{
    mDame = dame;
    return GameLazer::create();
}

bool GameLazer::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("lazer.plist", "lazer.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("lazerAttack.plist", "lazerAttack.png");
    dameLazer = mDame;

    spriteLazer = Sprite::createWithSpriteFrameName("lazerTwo1.png");
    this->addChild(spriteLazer);
    
    auto phyLazer = PhysicsBody::createBox(spriteLazer->getContentSize());
    phyLazer->setDynamic(false);
    phyLazer->setContactTestBitmask(true);
    phyLazer->setCollisionBitmask(2);
    this->addComponent(phyLazer);

    Animate* animateLazer = Animate::create(GameLazer::createAnimation("lazerTwo", 10, 0.05));
    spriteLazer->runAction(RepeatForever::create(animateLazer));
    

    auto moveLazer = MoveBy::create(1, Vec2(0, visibleSize.height * 2));
    auto removeLazer = CallFunc::create([=]()
        {
            GameLazer::removeLazer();
        });
    auto seqLazer = Sequence::create(moveLazer, removeLazer, nullptr);
    this->runAction(seqLazer);

    

    return true;
}
Animation* GameLazer::createAnimation(std::string nameFrame, int numberFrame, float delay)
{
    Vector<SpriteFrame*> frameLazer;
    for (int i = 1; i <= numberFrame; i++)
    {
        char buf[20] = { 0 };
        sprintf(buf,"%d.png",i);
        std::string sFrame = nameFrame + buf;
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sFrame);
        frameLazer.pushBack(frame);
    }
    Animation* aAnimation = Animation::createWithSpriteFrames(frameLazer, delay);
    return aAnimation;
}
int GameLazer::getLazerDame()
{
    return this->dameLazer;
}
void GameLazer::setDameLazerI()
{
    dameLazer++;
}
void GameLazer::setDameLazerD()
{
    dameLazer--;
}
void GameLazer::removeLazer()
{
    this->removeAllComponents();
    this->removeAllChildrenWithCleanup(true);
}
void GameLazer::getCheckPause()
{
    if (checkPause == false)
    {
        checkPause = true;
        this->pause();
    }
    else
    {
        checkPause = false;
        this->resume();
    }
}
void GameLazer::removeLazerAnimation()
{
    this->stopAllActions();
    Animate* animateRemove{ Animate::create(GameLazer::createAnimation("lazerAttack", 9, 0.005)) };
    auto A = cocos2d::CallFunc::create([=]()
        {
            GameLazer::removeLazer();
        });
    auto sequenceRemove = Sequence::create(animateRemove, A, nullptr);
    spriteLazer->runAction(sequenceRemove);
}


