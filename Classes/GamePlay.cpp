#include "GameMenu.h"
#include "GamePlay.h"
#include "GameOver.h"
#include "AudioEngine.h"


USING_NS_CC;

int mSkin;
using namespace cocos2d;


const char* GOLD = "keygold";
const char* High_Score = "key";

Scene* GamePlay::createScene(int m_skin)
{
    auto scene = GamePlay::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    mSkin = m_skin;
    auto layer = GamePlay::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GamePlay::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    // add file .plist
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("health.plist", "health.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("frame.plist", "frame.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("coin.plist", "coin.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("BossOne.plist", "BossOne.png");

    // physics
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize,PHYSICSBODY_MATERIAL_DEFAULT, 0);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    edgeNode->setPhysicsBody(edgeBody);
    this->addChild(edgeNode);

    GamePlay::addButton();
    GamePlay::addSprite();
    GamePlay::setRoundOne();
    //GamePlay::setRoundTwo();
    //GamePlay::setRoundThree();

    /*checkRound = 4;
    GamePlay::setRoundFour();*/

    // keyboard
    this->schedule(CC_SCHEDULE_SELECTOR(GamePlay::updateMove));
    

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GamePlay::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GamePlay::onKeyRealesed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // event listener physics contact
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GamePlay::onPhysicsContact, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    classPause = GamePause::create();
    classPause->getSkin(mSkin);
    classPause->setPosition(Vec2(visibleSize.width/2,visibleSize.height * 1.5));
    this->addChild(classPause, 99);

    return true;
}
void GamePlay::addButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bSetting = cocos2d::ui::Button::create("iSetting.png");
    bSetting->setScale(0.5);
    bSetting->setPosition(Vec2(visibleSize.width - bSetting->getContentSize().width / 4 + origin.x,
        visibleSize.height - bSetting->getContentSize().height / 4 + origin.y));
    
    bSetting->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
        {
            if (classPause->getCheckPause() == false)
            {
                if (type == ui::Widget::TouchEventType::ENDED)
                {
                    this->unschedule(CC_SCHEDULE_SELECTOR(GamePlay::addEnemyLazer));
                    checkPause = true;
                    Vector<PhysicsBody*> bodies = world->getAllBodies();
                    
                    for (auto body : bodies)
                    {
                        if (body->getNode()->getTag() == 3)
                        {
                            auto enemyPause = dynamic_cast<GameEnemyOne*>(body->getNode());
                            enemyPause->getCheckPause();
                        }
                        else if (body->getNode()->getTag() == 2)
                        {
                            auto enemyPause1 = dynamic_cast<GameLazer*>(body->getNode());
                            enemyPause1->getCheckPause();
                        }
                        else if (body->getNode()->getTag() == 10 || body->getNode()->getTag() == 9
                            || body->getNode()->getTag() == 11)
                        {
                            body->getNode()->pause();
                        }
                    }
                    classPause->moveDown();
                }
            }
        });
    this->addChild(bSetting, 10);
}
void GamePlay::addSprite()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    pPlaneOne = GamePlane::create(); // add plane of player
    pPlaneOne->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.2));
    pPlaneOne->setTag(1);
    this->addChild(pPlaneOne, 2);

    sHealth = Sprite::createWithSpriteFrameName("health1.png");
    sHealth->setScale(0.1);
    sHealth->setPosition(Vec2(visibleSize.width * 0.05, visibleSize.height * 0.075));
    this->addChild(sHealth, 10);
    Animate* animateHealth = Animate::create(GamePlay::createAnimation("health", 8, 0.5));
    sHealth->runAction(RepeatForever::create(animateHealth));
    
    std::string strHealth{ StringUtils::format("%d",mHealth) };
    lHealth = Label::createWithSystemFont(strHealth, "Arial", 30);
    lHealth->setPosition(Vec2(visibleSize.width * 0.1, visibleSize.height * 0.075));
    lHealth->setColor(Color3B::WHITE);
    this->addChild(lHealth,1);

    sPowUp = Sprite::createWithSpriteFrameName("frame-1.png");
    sPowUp->setScale(0.1);
    sPowUp->setPosition(Vec2(visibleSize.width * 0.2, visibleSize.height * 0.075));
    this->addChild(sPowUp);
    Animate* animatePowUp = Animate::create(GamePlay::createAnimation("frame-", 10 , 0.1));
    sPowUp->runAction(RepeatForever::create(animatePowUp));


    std::string strPow{ StringUtils::format("%d", mDame) };
    labelDame = Label::createWithSystemFont(strPow, "Arial", 30);
    labelDame->setPosition(Vec2(visibleSize.width * 0.25, visibleSize.height * 0.075));
    labelDame->setColor(Color3B::WHITE);
    this->addChild(labelDame, 1);

    
    spriteGold = Sprite::createWithSpriteFrameName("coin1.png");  // add sprite gold
    spriteGold->setScale(0.1);
    spriteGold->setPosition(Vec2(visibleSize.width * 0.05, visibleSize.height * 0.9));
    this->addChild(spriteGold, 1);
    Animate* animateScore{ Animate::create(GamePlay::createAnimation("coin",10,0.05)) };
    spriteGold->runAction(RepeatForever::create(animateScore));

    std::string strGold{ StringUtils::format("%d", mGold) }; // add label gold
    labelGold = Label::createWithSystemFont(strGold, "Arial", 30);
    labelGold->setPosition(Vec2(visibleSize.width * 0.1, visibleSize.height * 0.9));
    labelGold->setColor(Color3B::WHITE);
    this->addChild(labelGold, 1);

    std::string strScore{ StringUtils::format("%d", mScore) };
    labelScore = Label::createWithSystemFont(strScore, "Arial", 30);
    labelScore->setPosition(Vec2(visibleSize.width * 0.2, visibleSize.height * 0.9));
    labelScore->setColor(Color3B::WHITE);
    this->addChild(labelScore, 1);


    auto defScore = UserDefault::getInstance();
    iHighScore = defScore->getIntegerForKey(High_Score); 
    std::string strHighScore = StringUtils::format("%d", iHighScore);
    labelHighScore = Label::createWithSystemFont(strHighScore, "Arial", 30);
    labelHighScore->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.9));
    labelHighScore->setColor(Color3B::WHITE);
    this->addChild(labelHighScore, 1);
}
void GamePlay::setLabel(Label* check,int a)
{
    std::string strCheck{ StringUtils::format("%d", a) };
    check->setString(strCheck);
}
void GamePlay::checkGoldShop(int a)
{
    auto defGold = UserDefault::getInstance();
    defGold->getIntegerForKey(GOLD, a);
    defGold->flush();
}
Animation* GamePlay::createAnimation(std::string nameFrame, int numberFrame, float delay)
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
void GamePlay::setPhysics(Sprite* spri, int tag, int second)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bodySprite = PhysicsBody::createBox(spri->getContentSize());
    bodySprite->setDynamic(false);
    bodySprite->setContactTestBitmask(true);
    bodySprite->setCollisionBitmask(tag);
    spri->setPhysicsBody(bodySprite);
    spri->setTag(tag);
    if (checkRound !=4)
    {
        auto moveEnemyLazer{ MoveBy::create(second, Vec2(0, -visibleSize.height * 2)) };
        auto removeEnemyLazer = CallFunc::create([=]() {
            this->removeChild(spri);
            });
        auto seqLazerEnemy{ Sequence::create(moveEnemyLazer, removeEnemyLazer,nullptr) };
        spri->runAction(seqLazerEnemy);
    }
}
void GamePlay::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (classPause->getCheckPause() == false && checkPause == false)
    {
        if (keyCode == EventKeyboard::KeyCode::KEY_A)
        {
            pPlaneOne->setMoveLeft();
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_D)
        {
            pPlaneOne->setMoveRight();
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_W)
        {
            pPlaneOne->setMoveUp();
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_S)
        {
            pPlaneOne->setMoveDown();
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
        {
            auto cLazerOne = GameLazer::createGameLazer(mDame);
            cLazerOne->setPosition(pPlaneOne->getPosition());
            cLazerOne->setTag(2);
            this->addChild(cLazerOne, 1);
            AudioEngine::play2d(Game_Music_Attack, false, 1.0f);
        }
    }
}
void GamePlay::onKeyRealesed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_A )
    {
        pPlaneOne->setMoveIdolLeft();
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_D)
    {
        pPlaneOne->setMoveIdolRight();
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_W)
    {
        pPlaneOne->setMoveIdolUp();
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_S)
    {
        pPlaneOne->setMoveIdolDown();
    }
}
void GamePlay::updateMove(float delay)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // update continue
    if (classPause->getCheckPause() == false && checkPause == true)
    {
        checkPause = false;
        this->schedule(CC_SCHEDULE_SELECTOR(GamePlay::addEnemyLazer), 1.0);

        Vector<PhysicsBody*> bodies = world->getAllBodies();
        for (auto body : bodies)
        {
            if (body->getNode()->getTag() == 3)
            {
                auto enemyPause = dynamic_cast<GameEnemyOne*>(body->getNode());
                enemyPause->getCheckPause();
            }
            else if (body->getNode()->getTag() == 2)
            {
                auto enemyPause = dynamic_cast<GameLazer*>(body->getNode());
                enemyPause->getCheckPause();
            }
            else if (body->getNode()->getTag() == 10 || body->getNode()->getTag() == 9
                || body->getNode()->getTag() == 11)
            {
                body->getNode()->resume();
            }
        }
    }
    if (checkWin == 0)
    {
        if (checkRound == 1)
        {
            checkRound++;
            GamePlay::setRoundTwo();
            this->unschedule(CC_SCHEDULE_SELECTOR(GamePlay::addEnemyLazer));
        }
        else if (checkRound == 2 )
        {
            checkRound++;
            GamePlay::setRoundThree();
        }
        else if (checkRound == 3)
        {
            if (checkRound3 < 4)
            {
                GamePlay::setRoundThree();
            }
            else
            {
                checkRound++;
                GamePlay::setRoundFour();
            }
        }
        else if (checkRound == 4)
        {
            checkRound++;
        }
        else
        {
            auto moveWin = GameOver::createScene(mScore, iHighScore);
            Director::getInstance()->replaceScene(moveWin);
        }
    }
    
    if (checkItem)
    {
        checkItem = false;
        GamePlay::addItem();
    }

    auto xPlane{ pPlaneOne->getPositionX() };
    auto yPlane{ pPlaneOne->getPositionY() };
    auto xMax{ visibleSize.width - pPlaneOne->getWidth() / 2 };
    auto yMax{ visibleSize.height - pPlaneOne->getHeight() / 2 };
    auto xMin{ pPlaneOne->getWidth() / 2 };
    auto yMin{ pPlaneOne->getHeight() / 2 };

    auto mUp{ pPlaneOne->getMoveCheckUp() };
    auto mDown{ pPlaneOne->getMoveCheckDown() };
    auto mRight{ pPlaneOne->getMoveCheckRight() };
    auto mLeft{ pPlaneOne->getMoveCheckLeft() };
    auto mSpeed = 10;
   
    if (mRight == 1 || mLeft == 1 || mUp == 1 || mDown == 1)
    {
        if (mUp == 1 && mDown == 0 && mRight == 1 && mLeft == 0)
        {
            if (xPlane < xMax && yPlane < yMax)
            {
                pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() + mSpeed, pPlaneOne->getPositionY() + mSpeed));
            }
            else
            {
                if (xPlane < xMax)
                {
                    pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() + mSpeed, pPlaneOne->getPositionY()));
                }
                else if (yPlane < yMax)
                {
                    pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX(), pPlaneOne->getPositionY() + mSpeed));
                }
            }
        }
        else if (mUp == 1 && mDown == 0 && mRight == 0 && mLeft == 1)
        {
            if (xPlane > xMin && yPlane < yMax)
            {
                pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() - mSpeed, pPlaneOne->getPositionY() + mSpeed));
            }
            else
            {
                if (xPlane > xMin)
                {
                    pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() - mSpeed, pPlaneOne->getPositionY()));
                }
                else if (yPlane < yMax)
                {
                    pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX(), pPlaneOne->getPositionY() + mSpeed));
                }
            }
        }
        else if (mUp == 0 && mDown == 1 && mRight == 1 && mLeft == 0)
        {
            if (xPlane < xMax && yPlane > yMin)
            {
                pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() + mSpeed, pPlaneOne->getPositionY() - mSpeed));
            }
            else
            {
                if (xPlane < xMax)
                {
                    pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() + mSpeed, pPlaneOne->getPositionY()));
                }
                else if (yPlane > yMin)
                {
                    pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX(), pPlaneOne->getPositionY() - mSpeed));
                }
            }
            
        }
        else if (mUp == 0 && mDown == 1 && mRight == 0 && mLeft == 1)
        {
            if (xPlane > xMin && yPlane > yMin)
            {
                pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() - mSpeed, pPlaneOne->getPositionY() - mSpeed));
            }
            else
            {
                if (xPlane > xMin)
                {
                    pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() - mSpeed, pPlaneOne->getPositionY()));
                }
                else if (yPlane > yMin)
                {
                    pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX(), pPlaneOne->getPositionY() - mSpeed));
                }
            }
        }
        else if (mUp == 1 && mDown == 0 && ((mRight == 0  && mLeft == 0) || (mRight == 1 && mLeft == 1)) && yPlane < yMax)
        {
            pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX(), pPlaneOne->getPositionY() + mSpeed));
        }
        else if (mUp == 0 && mDown == 1 && ((mRight == 0 && mLeft == 0) || (mRight == 1 && mLeft == 1)) && yPlane > yMin)
        {
            pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX(), pPlaneOne->getPositionY() - mSpeed));
        }
        else if (((mUp == 0 && mDown == 0) || (mUp == 1 && mDown == 1)) && mRight == 1 && mLeft == 0 && xPlane < xMax)
        {
            pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() + mSpeed, pPlaneOne->getPositionY()));
        }
        else if (((mUp == 0 && mDown == 0) || (mUp == 1 && mDown == 1)) && mRight == 0 && mLeft == 1 && xPlane > xMin)
        {
            pPlaneOne->setPosition(Vec2(pPlaneOne->getPositionX() - mSpeed, pPlaneOne->getPositionY()));
        }
    }
    else
    {
        pPlaneOne->stopAllActions();
    }
}
bool GamePlay::onPhysicsContact(PhysicsContact& contact) // physicscontact
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto shapeA = contact.getShapeA()->getBody();
    auto shapeB = contact.getShapeB()->getBody();
    if ((shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 3)
        || (shapeA->getCollisionBitmask() == 3 && shapeB->getCollisionBitmask() == 2))
    {
        if (shapeA->getCollisionBitmask() == 3)
        {
            auto enemyO = dynamic_cast<GameEnemyOne*>(shapeA->getNode());
            enemyO->setHealthEnemy(mDame);
            auto enemy1 = dynamic_cast<GameLazer*>(shapeB->getNode());
            enemy1->removeLazerAnimation();

            if (enemyO->getHealthEnemy() <= 0)
            {
                checkItem = true;
              
                mScore++;
                checkWin--;
                xItem = shapeA->getPosition().x;
                yItem = shapeA->getPosition().y;

                GamePlay::setLabel(labelScore, mScore);
                enemyO->removeEnemy();
            }
            auto defScore = UserDefault::getInstance();
            iHighScore = defScore->getIntegerForKey(High_Score);
            if (mScore > iHighScore)
            {
                defScore->setIntegerForKey(High_Score, mScore);
                defScore->flush();
                GamePlay::setLabel(labelHighScore, mScore);
                iHighScore = mScore;
            } 
        }
        else if (shapeB->getCollisionBitmask() == 3)
        {
            auto enemyO = dynamic_cast<GameEnemyOne*>(shapeB->getNode());
            enemyO->setHealthEnemy(mDame);
            auto enemy1 = dynamic_cast<GameLazer*>(shapeA->getNode());
            enemy1->removeLazerAnimation();

            if (enemyO->getHealthEnemy() <= 0)
            {
                checkItem = true;

                checkWin--;
                xItem = shapeB->getPosition().x;
                yItem = shapeB->getPosition().y;
                
                mScore++;
                GamePlay::setLabel(labelScore, mScore);
                enemyO->removeEnemy();
            }
            auto defScore = UserDefault::getInstance();
            iHighScore = defScore->getIntegerForKey(High_Score);
            if (mScore > iHighScore)
            {
                defScore->setIntegerForKey(High_Score, mScore);
                defScore->flush();
                GamePlay::setLabel(labelHighScore, mScore);
                iHighScore = mScore;
            }
        }
    }
    else if ((shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 3)
        || (shapeA->getCollisionBitmask() == 3 && shapeB->getCollisionBitmask() == 1))
    {
        if (mDame > 1)
        {
            if (mDame > 5)
            {
                mDame -= 2;
            }
            else
            {
                mDame--;
            }
            GamePlay::setLabel(labelDame,mDame);
        }
        AudioEngine::play2d(Game_Music_Die,false,1.0f);
        if (mHealth == 1)
        {
            auto dGameOver{ GameOver::createScene(mScore, iHighScore) };
            Director::getInstance()->replaceScene(dGameOver);
        }
        else
        {
            mHealth--;
            if (shapeA->getCollisionBitmask() == 1)
            {
                auto diePlane = dynamic_cast<GamePlane*>(shapeA->getNode());
                pPlaneOne->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.2));
            }
            else if (shapeB->getCollisionBitmask() == 1)
            {
                auto diePlane = dynamic_cast<GamePlane*>(shapeB->getNode());
                pPlaneOne->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.2));
            }
        }
    }
    else if ((shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 9) 
        || (shapeA->getCollisionBitmask() == 9 && shapeB->getCollisionBitmask() == 1))
    {
        mDame++;
        if (shapeA->getCollisionBitmask() == 9)
        {
            GamePlay::setLabel(labelDame,mDame);
            this->removeChild(shapeA->getNode());
        }
        else if (shapeB->getCollisionBitmask() == 9)
        {
            GamePlay::setLabel(labelDame, mDame);
            this->removeChild(shapeB->getNode());
        }
    }
    else if ((shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 10)
        || (shapeA->getCollisionBitmask() == 10 && shapeB->getCollisionBitmask() == 1))
        {
            if (mDame > 1)
            {
                if (mDame > 5)
                {
                    mDame -= 2;
                }
                else
                {
                    mDame--;
                }
                GamePlay::setLabel(labelDame, mDame);
            }
            AudioEngine::play2d(Game_Music_Die, false, 1.0f);
            if (mHealth == 1)
            {
                auto dGameOver{ GameOver::createScene(mScore, iHighScore) };
                Director::getInstance()->replaceScene(dGameOver);
            }
            else
            {
                mHealth--;
                if (shapeA->getCollisionBitmask() == 1)
                {
                    auto diePlane = dynamic_cast<GamePlane*>(shapeA->getNode());
                    pPlaneOne->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.2));
                    this->removeChild(shapeB->getNode());
                }
                else if (shapeB->getCollisionBitmask() == 1)
                {
                    auto diePlane = dynamic_cast<GamePlane*>(shapeB->getNode());
                    pPlaneOne->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.2));
                    this->removeChild(shapeA->getNode());
                }
            }
    }
    return true;
}
void GamePlay::addItem()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto checkRandom = random(1, 10);

    if (checkRandom == 1)
    {
        auto spriteItemDame = Sprite::create(Game_Sprite_Item_Dame);
        spriteItemDame->setPosition(Vec2(xItem, yItem));
        this->addChild(spriteItemDame, 5);
        GamePlay::setPhysics(spriteItemDame, 9, 5);
    }
}
void GamePlay::addEnemyLazer(float delay)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Vector<PhysicsBody*> bodies = world->getAllBodies();
    if (checkRound == 1 || checkRound == 2)
    {
        for (auto body : bodies)
        {
            if (body->getNode()->getTag() == 3)
            {
                float xEnemy = body->getNode()->getPosition().x;
                float yEnemy{ body->getNode()->getPosition().y };
                auto randomLazerEnemy = random(1, 20);
                if (randomLazerEnemy == 1)
                {
                    auto enemyLazer{ Sprite::create(Game_Sprite_Lazer) };
                    enemyLazer->setPosition(Vec2(xEnemy, yEnemy));
                    this->addChild(enemyLazer, 5);
                    GamePlay::setPhysics(enemyLazer, 3, 2);
                }
            }
        }
    }
    else if (checkRound == 4)
    {
        for (auto body : bodies)
        {
            if (body->getNode()->getTag() == 20)
            {
                float xEnemy = body->getNode()->getPosition().x;
                float yEnemy{ body->getNode()->getPosition().y };
                auto randomLazerEnemy = random(1, 2);
                if (randomLazerEnemy == 1)
                {
                    for (int i = -1; i < 2; i++)
                    {
                        auto enemyLazer1{ Sprite::create(Game_Sprite_Lazer) };
                        enemyLazer1->setPosition(Vec2(xEnemy, yEnemy));
                        this->addChild(enemyLazer1, 5);
                        GamePlay::setPhysics(enemyLazer1, 3, 2);

                        auto moveDown1 {MoveBy::create(3, Vec2(visibleSize.width * 0.5 * i, -visibleSize.height * 2)) };
                        auto removeEnemyLazer = CallFunc::create([=]() {
                            this->removeChild(enemyLazer1);
                            });
                        auto seqe{ Sequence::create(moveDown1, removeEnemyLazer,nullptr) };
                        enemyLazer1->runAction(seqe);
                    }
                }
            }
        }
    }
}
void GamePlay::setRoundOne()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    for (float j = 0; j < 4; j++)
    {
        for (float i = 0; i < 9; i++)
        {
            auto sEnemyTwo = GameEnemyOne::createGameEnemyOne(checkRound, j, i);
            sEnemyTwo->setPosition(Vec2(visibleSize.width * 0.05, visibleSize.height * (1.5 + i / 10 + j * 0.9)));
            sEnemyTwo->setTag(3);
            this->addChild(sEnemyTwo, 2);
            checkWin++;
        }
    }
    auto delayGame = DelayTime::create(15);
    auto callfuncUpdate = CallFunc::create([=]() {
        this->schedule(CC_SCHEDULE_SELECTOR(GamePlay::addEnemyLazer), 2.0);
        });
    auto sequenceDelay{ Sequence::create(delayGame, callfuncUpdate, nullptr) };
    this->runAction(sequenceDelay);
}
void GamePlay::setRoundTwo()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    for (float j = 0; j < 4; j++)
    {
        for (float i = 0; i < 9; i++)
        {
            auto sEnemyTwo = GameEnemyOne::createGameEnemyOne(checkRound, j, i);
            sEnemyTwo->setPosition(Vec2(visibleSize.width * 0.95, visibleSize.height * (1.5 + i / 10 + j * 0.9)));
            sEnemyTwo->setTag(3);
            this->addChild(sEnemyTwo, 2);
            checkWin++;
        }
    }
    auto delayGame = DelayTime::create(15);
    auto callfuncUpdate = CallFunc::create([=]() {
        this->schedule(CC_SCHEDULE_SELECTOR(GamePlay::addEnemyLazer), 2.0);
        });
    auto sequenceDelay{ Sequence::create(delayGame, callfuncUpdate, nullptr) };
    this->runAction(sequenceDelay);
}
void GamePlay::setRoundThree()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float x;

    if (checkRound3 % 2  == 0)
    {
        x = 0.15;
    }
    else
    {
        x = 0.85;
    }
    for (float i = 0; i < 10; i++)
    {
        auto spriteEnemy = GameEnemyOne::createGameEnemyOne(checkRound, checkRound3, i);
        spriteEnemy->setPosition(Vec2(visibleSize.width * x, visibleSize.height * (1.5 + i / 10)));
        spriteEnemy->setTag(3);
        this->addChild(spriteEnemy, 2);
        checkWin++;
    }
    checkRound3++;
}
void GamePlay::setRoundFour()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto spriteBoss{ GameEnemyOne::createGameEnemyOne(checkRound, 0, 0) };
    spriteBoss->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 1.8));
    spriteBoss->setTag(20);
    this->addChild(spriteBoss);
    checkWin++;

    auto delayGame = DelayTime::create(2);
    auto callfuncUpdate = CallFunc::create([=]() {
        this->schedule(CC_SCHEDULE_SELECTOR(GamePlay::addEnemyLazer), 1.0);
        });
    auto sequenceDelay{ Sequence::create(delayGame, callfuncUpdate, nullptr) };
    this->runAction(sequenceDelay);
}

