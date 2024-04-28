#include "GameEnemy.h"
#include "math.h"

USING_NS_CC;

int m_Round;
float m_Width;
float m_Height;

GameEnemyOne* GameEnemyOne::createGameEnemyOne(int round, float numWidth, float numHeight)
{
    m_Round = round;
    m_Width = numWidth;
    m_Height = numHeight;

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

    checkRound = m_Round;
    numberHeight = m_Height;
    numberWidth = m_Width;
    checkRotationSpeed = 180;

    if (checkRound == 1)
    {
        checkStart = 180;
        xRotation = 0.15;
        mHealthEnemy = 1;
        spriteEnemyOne = Sprite::create("enemyOne1.png");
        auto moveDown{ MoveBy::create(2 + (2 * numberHeight / 10 + 2 * numberWidth * 0.9),Vec2(0,  -visibleSize.height * (1 + numberHeight / 10 + numberWidth * 0.9))) };
        auto sequenceCall = CallFunc::create([=]()
            {
                checkTime = 0;
                this->schedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle180), 0.001);
            });
        auto sequenceMain = Sequence::create(moveDown, sequenceCall, nullptr);
        this->runAction(sequenceMain);
    }
    else if (checkRound == 2)
    {
        checkStart = 0;
        xRotation = 0.85;
        mHealthEnemy = 1;
        spriteEnemyOne = Sprite::create("enemyOne1.png");
        auto moveDown{ MoveBy::create(2 + (2 * numberHeight / 10 + 2 * numberWidth * 0.9),Vec2(0,  -visibleSize.height * (1 + numberHeight / 10 + numberWidth * 0.9))) };
        auto sequenceCall = CallFunc::create([=]()
            {
                checkTime = 0;
                this->schedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle180, 10), 0.001);
            });
        auto sequenceMain = Sequence::create(moveDown, sequenceCall, nullptr);
        this->runAction(sequenceMain);
    }
    else if (checkRound == 3)
    {
        mHealthEnemy = 2;
        spriteEnemyOne = Sprite::create("enemyOne1.png");

        auto moveDown{ MoveBy::create(2 + (2 * numberHeight / 10 ),Vec2(0, -visibleSize.height * (1 + numberHeight / 10))) };
        auto sequenceCall = CallFunc::create([=]()
            {
                checkTime = 0;
                checkTimeEnd = 10000;
                checkRotationSpeed = 180;
                if ((int(numberWidth) % 2) == 0)
                {
                    checkStart = 180;
                    xRotation = 0.25;
                    this->schedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle180, 10), 0.001);
                }
                else
                {
                    checkStart = 180;
                    xRotation = 0.75;
                    this->schedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle270, 10), 0.001);
                }
                
            });
        auto sequenceMain = Sequence::create(moveDown, sequenceCall, nullptr);
        this->runAction(sequenceMain);
    }
    else if (checkRound == 4)
    {
        mHealthEnemy = 100;
        spriteEnemyOne = Sprite::createWithSpriteFrameName("bossOne1.png");
        Animate* aPlan = Animate::create(GameEnemyOne::createAnimation("bossOne", 6, 0.1));
        spriteEnemyOne->runAction(RepeatForever::create(aPlan));
        auto moveDown{ MoveBy::create(2, Vec2(0, -visibleSize.height)) };
        auto sequenceCall = CallFunc::create([=]() {
            auto moveLeftOne{ MoveBy::create(1, Vec2(-visibleSize.width * 0.25, 0)) };
            auto moveLeftTwo{ MoveBy::create(2,Vec2(-visibleSize.width * 0.5, 0)) };
            auto moveRightOne{ MoveBy::create(1,Vec2(visibleSize.width * 0.25, 0)) };
            auto sequenceRepeat{ Sequence::create(moveLeftOne, moveRightOne, moveRightOne, moveLeftOne, nullptr) };
            this->runAction(RepeatForever::create(sequenceRepeat));
            });
        auto sequenceFunc{ Sequence::create(moveDown, sequenceCall, nullptr) };
        this->runAction(sequenceFunc);
    }
    spriteEnemyOne->setScale(0.5);
    this->addChild(spriteEnemyOne);

    auto physicsEnemyBody = PhysicsBody::createBox(spriteEnemyOne->getContentSize() * 0.5);
    physicsEnemyBody->setDynamic(false);
    physicsEnemyBody->setContactTestBitmask(true);
    physicsEnemyBody->setCollisionBitmask(3);
    this->addComponent(physicsEnemyBody);


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
        if (checkRound == 1)
        {
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
        else if (checkRound == 2)
        {
            auto moveLeft{ MoveBy::create(2, Vec2(-visibleSize.width * 0.7,0)) };
            auto sequenceCall = CallFunc::create([=]()
                {
                    checkStart = 270;
                    xRotation = 0.15;
                    checkTime = 0;
                    this->schedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle270), 0.01);
                });
            auto sequenceMain = Sequence::create(moveLeft, sequenceCall, nullptr);
            this->runAction(sequenceMain);
        }
    }
    if (checkRound == 1 || checkRound == 3)
    {
        checkTime += dt;
        checkRotation = checkStart * Pi / 180 + checkTime * checkRotationSpeed * Pi / 180;
        sinEnemy = std::sin(checkRotation);
        cosEnemy = std::cos(checkRotation);
        this->setPosition(Vec2(visibleSize.width * xRotation + cosEnemy * visibleSize.width * 0.1,
            visibleSize.height * yRotation + sinEnemy * visibleSize.height * 0.3));
    }
    else if (checkRound == 2)
    {
        checkTime += dt;
        checkRotation = checkStart * Pi / 180 + checkTime * checkRotationSpeed * Pi / 180;

        sinEnemy = std::sin(checkRotation);
        cosEnemy = std::cos(checkRotation);
        this->setPosition(Vec2(visibleSize.width * xRotation + (cosEnemy * visibleSize.width * 0.1),
            visibleSize.height * yRotation - (sinEnemy * visibleSize.height * 0.3)));
    }
    
}
void GameEnemyOne::moveCircle270(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if (checkTime > checkTimeEnd)
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(GameEnemyOne::moveCircle270));
        auto delay = DelayTime::create(((4 - numberWidth) * 1.8) + numberWidth * 0.1);
        if (checkRound == 1)
        {
            float xxx = 0.5 - numberWidth * 0.1 ;
            auto moveUp{ MoveBy::create(xxx ,Vec2(0,visibleSize.height * (0.4 - numberWidth * 0.1))) };
            auto moveLeft{ MoveBy::create(2 - (2 * numberHeight / 10),Vec2(-visibleSize.width * (0.85 - numberHeight * 0.1) , 0)) };
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
        else if (checkRound == 2)
        {
            auto moveUp{ MoveBy::create(0.5 - numberWidth * 0.1 ,Vec2(0,visibleSize.height * (0.4 - numberWidth * 0.1))) };
            auto moveRight{ MoveBy::create(2.5 - (2 * numberHeight / 10),Vec2(visibleSize.width * (0.85 - float(numberHeight) / 10) , 0)) };
            auto sequenceCall = CallFunc::create([=]()
                {
                    auto moveRightOne{ MoveBy::create(1,Vec2(visibleSize.width * 0.05, 0)) };
                    auto moveLeftOne{ MoveBy::create(2, Vec2(-visibleSize.width * 0.1, 0)) };
                    auto sequenceOne{ Sequence::create(moveRightOne, moveLeftOne, moveRightOne,nullptr) };
                    auto sequenceRepeat = RepeatForever::create(sequenceOne);
                    this->runAction(sequenceRepeat);
                });
            auto seq{ Sequence::create(moveUp, moveRight, delay ,sequenceCall, nullptr) };
            this->runAction(seq);
        }
    }
    checkTime += dt;
    checkRotation = checkStart * Pi / 180 + checkTime * checkRotationSpeed * Pi / 180;

    if (checkRound == 1)
    {
        sinEnemy = std::sin(checkRotation);
        cosEnemy = std::cos(checkRotation);
        this->setPosition(Vec2(visibleSize.width * xRotation + cosEnemy * visibleSize.width * 0.1,
            visibleSize.height * yRotation + sinEnemy * visibleSize.height * 0.3));
    }
    else if(checkRound == 2 || checkRound == 3)
    {
        sinEnemy = std::sin(checkRotation);
        cosEnemy = std::cos(checkRotation);
        this->setPosition(Vec2(visibleSize.width * xRotation - cosEnemy * visibleSize.width * 0.1,
            visibleSize.height * yRotation + sinEnemy * visibleSize.height * 0.3));
    }
}



