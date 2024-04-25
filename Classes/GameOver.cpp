#include "GameMenu.h"
#include "GamePlay.h"
#include "GameOver.h"

USING_NS_CC;

int mScore;
int mHighScore;

Scene* GameOver::createScene(int score, int highscore)
{
    mScore = score;
    mHighScore = highscore;
    return GameOver::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    

    GameOver::addButton();
    GameOver::addLabel();
  
    return true;
}

void GameOver::addButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

   
    auto buttonReplay{ cocos2d::ui::Button::create(Game_Sprite_Button_Replay) };
    if (buttonReplay == nullptr)
    {
        problemLoading(Game_Sprite_Button_Replay);
    }
    else
    {
        buttonReplay->setPosition(Vec2(visibleSize.width / 2 + origin.y, visibleSize.height * 0.7 + origin.y));
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
    buttonMenu->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
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
                auto moveGameMenu = GameMenu::create();
                Director::getInstance()->replaceScene(moveGameMenu);
                break;
            }
            case cocos2d::ui::Widget::TouchEventType::CANCELED:
                break;
            default:
                break;
            }
        });
    this->addChild(buttonMenu,1);
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
}

void GameOver::addLabel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    std::string strScore{ StringUtils::format("%d",mScore) };
    auto labelScore{ Label::createWithSystemFont(strScore, "Arial", 40)};
    labelScore->setPosition(Vec2(visibleSize.width * 0.3, visibleSize.height * 0.5));
    labelScore->setColor(Color3B::WHITE);
    this->addChild(labelScore);

    std::string strHighScore{ StringUtils::format("%d", mHighScore) };
    auto labelHighScore{ Label::createWithSystemFont(strHighScore, "Arial", 40) };
    labelHighScore->setPosition(Vec2(visibleSize.width * 0.7, visibleSize.height * 0.5));
    labelHighScore->setColor(Color3B::WHITE);
    this->addChild(labelHighScore);
}
