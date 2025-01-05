#include "GameMenu.h"
#include "GameLevel.h"
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "settingscene.h"
#include "HelperScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;
/*CGameMenu::CGameMenu()
{

}

CGameMenu::~CGameMenu()
{

}*/

Scene* CGameMenu::createScene()
{
	return CGameMenu::create();
}

//static void problemLoading(const char* filename)
//{
//	printf("Error while loading: %s\n", filename);
//}

bool CGameMenu::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // Get visible size and origin of current screen
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /************************************ Background ************************************/
    auto background_image = Sprite::create("/MenuScene/MainBG.PNG");
    // Create a background sprite using "MainBG.PNG" image resource

    background_image->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));  // Set background position to screen center

    this->addChild(background_image);  // Add background to current scene

    /*********************************** Clouds ***************************************/
    auto cloud = Sprite::create();  // Create an empty cloud sprite node
    cloud->setName("Cloud");  // Set cloud node name to "Cloud"

    // Cloud 1
    auto cloud1 = Sprite::create("/MenuScene/Cloud.PNG");  // Create cloud1 using "Cloud.PNG" image

    cloud1->setPosition(Vec2(origin.x - cloud1->getContentSize().width,
        origin.y + visibleSize.height * 0.85));
    cloud1->setOpacity(85);  // Set cloud1 opacity to 85 for slight transparency
    // Add animation
    auto cloud1_moveto_1 = MoveTo::create(15, Vec2(origin.x + visibleSize.width + cloud1->getContentSize().width, origin.y + visibleSize.height * 0.85));
    auto cloud1_moveto_2 = MoveTo::create(15, Vec2(origin.x - cloud1->getContentSize().width, origin.y + visibleSize.height * 0.85));
    auto cloud1_sequence = Sequence::create(cloud1_moveto_1, DelayTime::create(5), cloud1_moveto_2, DelayTime::create(5), nullptr);
    cloud1->runAction(RepeatForever::create(cloud1_sequence));
    cloud->addChild(cloud1);

    // Cloud 2
    auto cloud2 = Sprite::create("/MenuScene/Cloud2.PNG");
    cloud2->setPosition(Vec2(origin.x + visibleSize.width + cloud2->getContentSize().width,
        origin.y + visibleSize.height * 0.8));
    cloud2->setScale(2);
    cloud2->setOpacity(95);
    // Add animation
    auto cloud2_moveto_1 = MoveTo::create(15, Vec2(origin.x - cloud2->getContentSize().width, origin.y + visibleSize.height * 0.85));
    auto cloud2_moveto_2 = MoveTo::create(15, Vec2(origin.x + visibleSize.width + cloud2->getContentSize().width, origin.y + visibleSize.height * 0.85));
    auto cloud2_sequence = Sequence::create(cloud2_moveto_1, DelayTime::create(5), cloud2_moveto_2, DelayTime::create(5), nullptr);
    cloud2->runAction(RepeatForever::create(cloud2_sequence));
    cloud->addChild(cloud2);

    this->addChild(cloud);

    /************************************ Monsters ************************************/
    auto monster = Sprite::create("/MenuScene/FlyMonster.PNG");

    monster->setPosition(Vec2(origin.x + visibleSize.width / 4,
        origin.y + visibleSize.height * 0.8));
    // Add animation
    auto monster_moveby_up = MoveBy::create(1.0f, Vec2(0, monster->getContentSize().height / 4));  // Move by 1 second
    auto monster_moveby_down = MoveBy::create(1.0f, Vec2(0, -monster->getContentSize().height / 4));  // Move by 1 second
    auto monster_sequence = Sequence::createWithTwoActions(monster_moveby_down, monster_moveby_up);  // Create a sequence of actions
    monster->runAction(RepeatForever::create(monster_sequence));  // Repeat the sequence indefinitely
    this->addChild(monster);

    /************************************ Carrots ************************************/
    auto carrot = Sprite::create();  // Create an empty carrot sprite node
    carrot->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 4));
    // Carrot leaf 1
    auto carrot_leaf1 = Sprite::create("/MenuScene/Leaf1.PNG");

    carrot_leaf1->setPosition(Vec2(origin.x - carrot_leaf1->getContentSize().width / 2,
        origin.y + visibleSize.height / 4 + carrot_leaf1->getContentSize().height * 1.2));
    carrot->addChild(carrot_leaf1);

    auto carrot_leaf2 = Sprite::create("/MenuScene/Leaf2.PNG");
    carrot_leaf2->setPosition(Vec2(origin.x + carrot_leaf2->getContentSize().width * 0.6,
        origin.y + visibleSize.height / 4 + carrot_leaf2->getContentSize().height * 1.3));
    carrot->addChild(carrot_leaf2);

    auto carrot_leaf3 = Sprite::create("/MenuScene/Leaf3.PNG");

    carrot_leaf3->setPosition(Vec2(origin.x,
        origin.y + visibleSize.height / 4 + carrot_leaf3->getContentSize().height * 1.1));
    carrot->addChild(carrot_leaf3);

    // Carrot body
    auto carrot_body = Sprite::create("/MenuScene/CarrotBody.PNG");

    carrot_body->setPosition(Vec2(origin.x,
        origin.y + visibleSize.height / 4 + carrot_body->getContentSize().height / 4));

    carrot->addChild(carrot_body);

    this->addChild(carrot);

    /************************************ Title ************************************/
    auto title = Sprite::create("/MenuScene/MainTitle.PNG");
    title->setPosition(Vec2(origin.x + visibleSize.width / 2 + title->getContentSize().width / 20,
        origin.y + visibleSize.height / 2 - title->getContentSize().height / 7));
    this->addChild(title);

    /************************************ Menu ************************************/
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    // Exit button
    auto close_item = MenuItemFont::create("EXIT", CC_CALLBACK_1(CGameMenu::game_close, this));
    close_item->setPosition(Vec2(origin.x + visibleSize.width - close_item->getContentSize().width * 0.6,
        origin.y + visibleSize.height - close_item->getContentSize().height / 2));
    menu->addChild(close_item);

    // Options button
    auto options_item = MenuItemImage::create("/MenuScene/Btn_Set.PNG", "/MenuScene/Btn_SetLight.PNG", CC_CALLBACK_1(CGameMenu::goto_sets, this));
    // Create a menu item with a label and image, and set the image path for both normal and selected states
    // Using CC_CALLBACK_1(CGameMenu::goto_options, this) indicates that the callback function is in the CGameMenu class
    options_item->setPosition(Vec2(origin.x + visibleSize.width * 0.2,
        origin.y + visibleSize.height / 3));  // Set the position of the options_item
    menu->addChild(options_item);  // Add the options_item to the menu

    // Helper button
    auto helper_item = MenuItemImage::create("/MenuScene/Btn_Help.PNG", "/MenuScene/Btn_HelpLight.PNG", CC_CALLBACK_1(CGameMenu::goto_helper, this));
    helper_item->setPosition(Vec2(origin.x + visibleSize.width * 0.8,
        origin.y + visibleSize.height / 3));
    menu->addChild(helper_item);

    // Adventure mode
    auto adventure_item = MenuItemImage::create("/MenuScene/Btn_NormalModle.PNG", "/MenuScene/Btn_NormalModleLight.PNG", CC_CALLBACK_1(CGameMenu::goto_adventure, this));
    adventure_item->setScale(0.9f);
    adventure_item->setPosition(Vec2(origin.x + visibleSize.width / 2 - adventure_item->getContentSize().width,
        origin.y + visibleSize.height / 8));

    menu->addChild(adventure_item);

    // Boss mode
    auto boss_item = MenuItemImage::create("/MenuScene/Btn_Boss.PNG", "/MenuScene/Btn_BossLight.PNG", CC_CALLBACK_1(CGameMenu::goto_bossmode, this));

    boss_item->setScale(0.9f);
    boss_item->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 8));
    menu->addChild(boss_item);

    // Monster nest
    auto nest_item = MenuItemImage::create("/MenuScene/Btn_MonsterNest.PNG", "/MenuScene/Btn_MonsterNestLight.PNG", CC_CALLBACK_1(CGameMenu::goto_monsternest, this));
    nest_item->setScale(0.9f);
    nest_item->setPosition(Vec2(origin.x + visibleSize.width / 2 + nest_item->getContentSize().width,
        origin.y + visibleSize.height / 8));
    menu->addChild(nest_item);

    // Lock
    Sprite* Locked2 = nullptr;
    InitSprite(Locked2, nest_item, "MenuScene/front_btn_floor_locked.png", boss_item->getContentSize().width - 12, boss_item->getContentSize().height / 2, 1);
    if (nullptr == Locked2)
        return false;

    this->addChild(menu);

    return true;
}

void CGameMenu::InitSprite(cocos2d::Sprite*& outSprite, cocos2d::Node* parentNode, const std::string& fileName, float x, float y, int localZOrder)
{
    auto pinfo = AutoPolygon::generatePolygon(fileName);
    outSprite = Sprite::create(pinfo);

    outSprite->setPosition(Vec2(x, y));
    parentNode->addChild(outSprite, localZOrder);
}

void CGameMenu::game_close(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    Director::getInstance()->end();

    exit(0);
}

void CGameMenu::goto_sets(Ref* pSender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    auto options_scene = OptionsScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, options_scene, Color3B::BLACK)); // Use fade transition effect with 0.5s duration
}

void CGameMenu::goto_helper(Ref* pSender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    auto helper_scene = HelperScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, helper_scene, Color3B::BLACK)); // Use fade transition effect with 0.5s duration
}

void  CGameMenu::goto_adventure(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
    pLevelMgr->setCurIndex(3);
    CGameScene::deletInstance();
    Director::getInstance()->replaceScene(CGameLevel::create());
    // Create a new CGameLevel instance as the new scene
}

void  CGameMenu::goto_bossmode(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
    pLevelMgr->setCurIndex(3); // Set current level index to 4 (starting from 0)
    CGameScene::deletInstance();
    Director::getInstance()->replaceScene(CGameScene::getInstance());  // Create CGameLevel instance as new scene
}

void  CGameMenu::goto_monsternest(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    // Empty function
}