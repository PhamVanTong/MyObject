#include "GameMenu.h"
#include "GamePlay.h"
#include "GamePause.h"

USING_NS_CC;


Layer* GamePause::createScene()
{
    return GamePause::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GamePause::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    GamePause::addSprite();

    return true;
}
void GamePause::addSprite()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    spriteBackGround = Sprite::create(Game_Sprite_BackGround);
    spriteBackGround->setScale(2);
    this->setContentSize(spriteBackGround->getContentSize());
    this->addChild(spriteBackGround, -1);
    
    auto buttonContinue{ ui::Button::create(Game_Sprite_Button_Continue)};
    buttonContinue->setPosition(Vec2(0, visibleSize.height * 0.2));
    buttonContinue->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
            {
                GamePause::moveUp();
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                break;
            default:
                break;
            }
            
        });
    this->addChild(buttonContinue, 5);

    auto buttonReplay{ cocos2d::ui::Button::create(Game_Sprite_Button_Replay) };
    if (buttonReplay == nullptr)
    {
        problemLoading(Game_Sprite_Button_Replay);
    }
    else
    {
        buttonReplay->setPosition(Vec2(visibleSize.width * 0 + origin.y, visibleSize.height * 0 + origin.y));
        buttonReplay->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
            {
                switch (type)
                {
                case cocos2d::ui::Widget::TouchEventType::ENDED:
                {
                    auto mScene{ GamePlay::createScene(mSkin) };
                    Director::getInstance()->replaceScene(mScene);
                    break;
                }
                default:
                    break;
                }
            });
        this->addChild(buttonReplay, 1);
    }

    auto buttonMenu{ ui::Button::create(Game_Sprite_Button_Menu) };
    buttonMenu->setPosition(Vec2(0, -visibleSize.height * 0.2));
    buttonMenu->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
            {
                auto directorMenu = GameMenu::create();
                Director::getInstance()->replaceScene(directorMenu);
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                break;
            default:
                break;
            }
           
        });
    this->addChild(buttonMenu, 5);
}
void GamePause::getSkin(int m)
{
    mSkin = m;
}
void GamePause::moveDown()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    checkPause = true;
    //auto move{ MoveBy::create(0.5,Vec2(0, -visibleSize.height)) };
    auto move{ MoveTo::create(0.5,Vec2(visibleSize.width * 0.5,visibleSize.height * 0.5)) };
    this->runAction(move);
}
void GamePause::moveUp()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    checkPause = false;
    //auto move{ MoveBy::create(0.5,Vec2(0, visibleSize.height)) };
    auto move{ MoveTo::create(0.5,Vec2(visibleSize.width * 0.5, visibleSize.height * 1.5)) };
    this->runAction(move);
}
bool GamePause::getCheckPause()
{
    return checkPause;
}


