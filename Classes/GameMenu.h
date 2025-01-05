#ifndef __GAMEMENU_H__
#define __GAMEMENU_H__
#include "cocos2d.h"
USING_NS_CC;
class CGameMenu : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();=

    virtual bool init();

    void game_close(Ref* pSender);

    void goto_sets(Ref* pSender);

    void goto_helper(Ref* pSender);

    void goto_adventure(Ref* psender);

    void goto_bossmode(Ref* psender);

    void goto_monsternest(Ref* psender);

    void InitSprite(cocos2d::Sprite*& outSprite, cocos2d::Node* parentNode, const std::string& fileName, float x, float y, int localZOrder);
    CREATE_FUNC(CGameMenu);
};

//#define WINSIZE Director::getInstance()->getVisibleSize()
//#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif