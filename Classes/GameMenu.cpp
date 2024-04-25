#include "GameMenu.h"
#include "GamePlay.h"
#include "AudioEngine.h"

USING_NS_CC;


const char* SKIN_STATE = "keyskin";
const char* GOLD_STATE = "keygold";

Scene* GameMenu::createScene()
{
    return GameMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    GameMenu::addButton();
    GameMenu::addSprite();

    return true;
}


void GameMenu::addButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto audio = AudioEngine::play2d(Game_Music_Background, true, 1.0f);
    AudioEngine::setVolume(audio, 1.0f);

    // add button play game
    auto bPlay{ cocos2d::ui::Button::create(Game_Sprite_Button_Play) };
    if (bPlay == nullptr)
    {
        problemLoading(Game_Sprite_Button_Play);
    }
    else
    {
        bPlay->setPosition(Vec2(visibleSize.width / 2 + origin.y, visibleSize.height * 0.7 + origin.y));
        bPlay->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
            {
                switch (type)
                {
                case cocos2d::ui::Widget::TouchEventType::ENDED:
                {
                    //AudioEngine::stop(audio);
                    AudioEngine::stopAll();
                    auto mScene{ GamePlay::createScene(mSkin) };
                    Director::getInstance()->replaceScene(mScene);
                    break;
                }
                default:
                    break;
                }
            });
        this->addChild(bPlay, 1);
    }
    
    auto buttonExit{ cocos2d::ui::Button::create(Game_Sprite_Button_Exit) };
    buttonExit->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.3));
    buttonExit->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case cocos2d::ui::Widget::TouchEventType::ENDED:
            {
                Director::getInstance()->end();
                break;
            }
            default:
                break;
            }
        });
    this->addChild(buttonExit, 1);
    
    auto buttonOptions{ ui::Button::create(Game_Sprite_Button_Options) };
    buttonOptions->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
    buttonOptions->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
            case cocos2d::ui::Widget::TouchEventType::MOVED:
                break;
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                break;
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                break;
            default:
                break;
            }
        });
    this->addChild(buttonOptions,1);
}

void GameMenu::addSprite()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add background
    auto spriteBackground{ Sprite::create(Game_Sprite_BackGround) };
    spriteBackground->setScale(2);
    spriteBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    spriteBackground->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(spriteBackground, -1);
}
void GameMenu::checkGold(int a)
{
    auto defGold = UserDefault::getInstance();
    defGold->setIntegerForKey(GOLD_STATE, a);
    defGold->flush();
}
