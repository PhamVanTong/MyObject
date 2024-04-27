#include "GameEnemy.h"
#include "math.h"

USING_NS_CC;

int checkRound;
float checkNumberHeight;
float checkNumberWidth;
#define ROTATION_SPEED 180.f


GameEnemyOne* GameEnemyOne::createGameEnemyOne(int round, float numberWidth, float numberHeight)
{
    checkNumberHeight = numberHeight;
    checkNumberWidth = numberWidth;
    checkRound = round;
    return GameEnemyOne::create();
}

bool GameEnemyOne::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    numberHeight = checkNumberHeight;
    numberWidth = checkNumberWidth;

    if (checkRound == 1)
    {
        mHealthEnemy = 1;
        spriteEnemyOne = Sprite::create("enemyOne1.png");
    }
    else if (checkRound == 2)
    {
        mHealthEnemy = 5;
        spriteEnemyOne = Sprite::create("enemyOne2.png");
    }
    else if (checkRound == 3)
    {
        mHealthEnemy = 1;
        spriteEnemyOne = Sprite::create("enemyOne2.png");
    }
    else if (checkRound == 4)
    {
        mHealthEnemy = 100;
        spriteEnemyOne = Sprite::createWithSpriteFrameName("bossOne1.png");
        
    }
    spriteEnemyOne->setScale(0.5);
    this->addChild(spriteEnemyOne);

    auto physicsEnemyBody = PhysicsBody::createBox(spriteEnemyOne->getContentSize() * 0.5);
    physicsEnemyBody->setDynamic(false);
    physicsEnemyBody->setContactTestBitmask(true);
    physicsEnemyBody->setCollisionBitmask(3);
    this->addComponent(physicsEnemyBody);

    if (checkRound == 1)
    {
        auto moveDown{ MoveBy::create(2 + (2 * numberHeight / 10 + 2 * numberWidth * 0.9),Vec2(0,  -visibleSize.height * (1 + numberHeight / 10 + numberWidth * 0.9)))};
        auto sequenceCall = CallFunc::create([=]()
            {
                checkTime = 0;
                this->schedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle180, 10), 0.001);
            });
        auto sequenceMain = Sequence::create(moveDown, sequenceCall, nullptr);
        this->runAction(sequenceMain);
    }
    else
    {
        if (checkRound == 2)
        {
            Animate* aPlan = Animate::create(GameEnemyOne::createAnimation("bossOne", 6, 0.1));
            spriteEnemyOne->runAction(RepeatForever::create(aPlan));
        }
        auto moveDown{ MoveBy::create(2 + (2 * numberHeight / 10 + 2 * numberWidth * 0.9),Vec2(0,  -visibleSize.height * (1 + numberHeight / 10 + numberWidth * 0.9))) };
        auto sequenceCall = CallFunc::create([=]()
            {
                checkTime = 0;
                this->schedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle180, 10), 0.001);
            });
        auto sequenceMain = Sequence::create(moveDown, sequenceCall, nullptr);
        this->runAction(sequenceMain);
    }
    return true;
}
int GameEnemyOne::getHealthEnemy()
{
    return this->mHealthEnemy;
}
void GameEnemyOne::getCheckPause()
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
void GameEnemyOne::setHealthEnemy(int dame)
{
    mHealthEnemy -= dame;
}
void GameEnemyOne::removeEnemy()
{
    this->removeAllComponents();
    this->removeAllChildrenWithCleanup(true);
}
Animation* GameEnemyOne::createAnimation(std::string nameFrame, int numberFrame, float delay)
{
    Vector<SpriteFrame*> frameAni;
    for (int i = 1; i <= numberFrame; i++)
    {
        char buf[20] = { 0 };
        sprintf(buf, "%d.png", i);
        std::string sFrame = nameFrame + buf;
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sFrame);
        frameAni.pushBack(frame);
    }
    Animation* aAnimation = Animation::createWithSpriteFrames(frameAni, delay);
    return aAnimation;
}
void GameEnemyOne::moveCircle180(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if (checkTime > checkTimeEnd)
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle180));
        auto moveRight{ MoveBy::create(2, Vec2(visibleSize.width * 0.7,0)) };
        auto sequenceCall = CallFunc::create([=]()
            {
                checkStart = 270;
                xRotation = 0.85;
                checkTime = 0;
                this->schedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle270), 0.01);
            });
        auto sequenceMain = Sequence::create(moveRight, sequenceCall, nullptr);
        this->runAction(sequenceMain);

    }
    checkTime += dt;
    checkRotation = checkStart * Pi / 180 + checkTime * ROTATION_SPEED * Pi / 180;
    
    sinEnemy = std::sin(checkRotation);
    cosEnemy = std::cos(checkRotation);
    this->setPosition(Vec2(visibleSize.width * xRotation + cosEnemy * visibleSize.width * 0.1,
        visibleSize.height * yRotation + sinEnemy * visibleSize.height * 0.3));
}
void GameEnemyOne::moveCircle270(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if (checkTime > checkTimeEnd)
    {
        auto delay = DelayTime::create((3 - numberWidth) * (2 * 0.9));

        this->unschedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle270));
        auto moveUp{ MoveBy::create(1 ,Vec2(0,visibleSize.height * (0.4 - numberWidth * 0.1)))};
        auto moveLeft{ MoveBy::create(2.5 - (2 * numberHeight / 10),Vec2(-visibleSize.width * (0.85 - numberHeight / 10) , 0)) };
        auto sequenceCall = CallFunc::create([=]()
            {
                auto moveRightOne{ MoveBy::create(1,Vec2(visibleSize.width * 0.05, 0)) };
                auto moveLeftOne{ MoveBy::create(2, Vec2(-visibleSize.width * 0.1, 0)) };
                auto sequenceOne{ Sequence::create(moveRightOne, moveLeftOne, moveRightOne,nullptr) };
                auto sequenceRepeat = RepeatForever::create(sequenceOne);
                this->runAction(sequenceRepeat);
            });
        auto seq{ Sequence::create(moveUp, moveLeft, delay ,sequenceCall, nullptr) };
        this->runAction(seq);
    }
    checkTime += dt;
    checkRotation = checkStart * Pi / 180 + checkTime * ROTATION_SPEED * Pi / 180;

    sinEnemy = std::sin(checkRotation);
    cosEnemy = std::cos(checkRotation);
    this->setPosition(Vec2(visibleSize.width * xRotation + cosEnemy * visibleSize.width * 0.1,
        visibleSize.height * yRotation + sinEnemy * visibleSize.height * 0.3));
}



