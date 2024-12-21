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

/*菜单场景*/
Scene* CGameMenu::createScene()
{
	return CGameMenu::create();
}


///*错误处理，它用于处理资源加载出错的情况，比如图片或文件无法加载*/
//static void problemLoading(const char* filename)
//{
//	printf("Error while loading: %s\n", filename);
//}



bool CGameMenu::init()
{
    //引擎隐式调用场景的 init() 方法
    if (!Scene::init())
    {
        return false;
    }

    //获取当前屏幕可见区域的大小 visibleSize 和原点坐标 origin
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    ;
    /*****************************************背景图**************************************************/
    auto background_image = Sprite::create("/MenuScene/MainBG.PNG");
    //创建了一个背景图片的 Sprite 对象 background_image，加载了名为 "MainBG.PNG" 的图片资源

        background_image->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));//将背景图的位置设置为屏幕中心

        this->addChild(background_image);//将其添加到当前场景中
    /******************************************云*******************************************************/
    auto cloud = Sprite::create();//创建一个名为 cloud 的空精灵节点
    cloud->setName("Cloud");//为云层节点设置名称为 "Cloud"

    //大云（cloud1）
    auto cloud1 = Sprite::create("/MenuScene/Cloud.PNG");//创建 cloud1 精灵，加载名为 "Cloud.PNG" 的云朵图片资源

        cloud1->setPosition(Vec2(origin.x - cloud1->getContentSize().width,/*cloud1->getContentSize().width 是 cloud1 精灵的宽度*/
            origin.y + visibleSize.height * 0.85));
        cloud1->setOpacity(85);/*设置 cloud1 的不透明度为 85，使其稍微半透明*/
        //添加动画
        auto cloud1_moveto_1 = MoveTo::create(15, Vec2(origin.x + visibleSize.width + cloud1->getContentSize().width, origin.y + visibleSize.height * 0.85));
        auto cloud1_moveto_2 = MoveTo::create(15, Vec2(origin.x - cloud1->getContentSize().width, origin.y + visibleSize.height * 0.85));
        auto cloud1_sequence = Sequence::create(cloud1_moveto_1, DelayTime::create(5), cloud1_moveto_2, DelayTime::create(5), nullptr);
        cloud1->runAction(RepeatForever::create(cloud1_sequence));
        cloud->addChild(cloud1);
    
    //小云
    auto cloud2 = Sprite::create("/MenuScene/Cloud2.PNG");
        cloud2->setPosition(Vec2(origin.x + visibleSize.width + cloud2->getContentSize().width,
            origin.y + visibleSize.height * 0.8));
        cloud2->setScale(2);
        cloud2->setOpacity(95);
        //添加动画
        auto cloud2_moveto_1 = MoveTo::create(15, Vec2(origin.x - cloud2->getContentSize().width, origin.y + visibleSize.height * 0.85));
        auto cloud2_moveto_2 = MoveTo::create(15, Vec2(origin.x + visibleSize.width + cloud2->getContentSize().width, origin.y + visibleSize.height * 0.85));
        auto cloud2_sequence = Sequence::create(cloud2_moveto_1, DelayTime::create(5), cloud2_moveto_2, DelayTime::create(5), nullptr);
        cloud2->runAction(RepeatForever::create(cloud2_sequence));
        cloud->addChild(cloud2);

    this->addChild(cloud);
    /*****************************************************添加怪物图形****************************************************/
    auto monster = Sprite::create("/MenuScene/FlyMonster.PNG");

        monster->setPosition(Vec2(origin.x + visibleSize.width / 4,
            origin.y + visibleSize.height * 0.8));
        //添加动作
        auto monster_moveby_up = MoveBy::create(1.0f, Vec2(0, monster->getContentSize().height / 4));//创建了一个向上移动的动作
        auto monster_moveby_down = MoveBy::create(1.0f, Vec2(0, -monster->getContentSize().height / 4));//创建了一个向下移动的动作
        /*1.0f表示动作持续的时间，这里指动作持续1s
        * Vec2(0, -monster->getContentSize().height / 4)是位移向量
        * 0表示x轴上不移动，即水平方向不移动
        * monster->getContentSize().height是怪物高度，乘以四分之一并向上/下移动
        */
        auto monster_sequence = Sequence::createWithTwoActions(monster_moveby_down, monster_moveby_up);//将两个移动动作化为一个动作序列 
        monster->runAction(RepeatForever::create(monster_sequence));//将动作序列设置为怪物的动作，使用RepeatForever::create使其永久重复执行
        this->addChild(monster);
    
    /****************************************************添加萝卜图形*****************************************************/
    auto carrot = Sprite::create();//创建了一个空的 Sprite，是一个萝卜元素
    carrot->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 4));
    //1,添加萝卜叶子
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
    

    //2,添加萝卜主体
    auto carrot_body = Sprite::create("/MenuScene/CarrotBody.PNG");

        carrot_body->setPosition(Vec2(origin.x,
            origin.y + visibleSize.height / 4 + carrot_body->getContentSize().height / 4));

        carrot->addChild(carrot_body);
    

    this->addChild(carrot);
   
    /*************************************************************添加保卫萝卜标题******************************************************************/
    auto title = Sprite::create("/MenuScene/MainTitle.PNG");
        title->setPosition(Vec2(origin.x + visibleSize.width / 2 + title->getContentSize().width / 20,
            origin.y + visibleSize.height / 2 - title->getContentSize().height / 7));
        this->addChild(title);
   
    /*****************************************************************菜单界面************************************************************************/
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    //退出游戏按钮
    auto close_item = MenuItemFont::create("EXIT", CC_CALLBACK_1(CGameMenu::game_close, this));
        close_item->setPosition(Vec2(origin.x + visibleSize.width - close_item->getContentSize().width * 0.6,
            origin.y + visibleSize.height - close_item->getContentSize().height / 2));
        menu->addChild(close_item);
    
    //设置按钮
    auto options_item = MenuItemImage::create("/MenuScene/Btn_Set.PNG", "/MenuScene/Btn_SetLight.PNG", CC_CALLBACK_1(CGameMenu::goto_sets, this));
    //创建一个菜单项，接受两个图片路径参数，分别是按钮正常状态和按钮被选中状态的图片路径
    //第三个参数CC_CALLBACK_1(CGameMenu::goto_options, this)表示点击菜单项时会调用 CGameMenu 类中的 goto_options 函数。
        options_item->setPosition(Vec2(origin.x + visibleSize.width * 0.2,
            origin.y + visibleSize.height / 3));//设置了菜单项 options_item 的位置
        menu->addChild(options_item);//将创建的菜单项 options_item 添加到 menu 中
    
    //帮助按钮
    auto helper_item = MenuItemImage::create("/MenuScene/Btn_Help.PNG", "/MenuScene/Btn_HelpLight.PNG", CC_CALLBACK_1(CGameMenu::goto_helper, this));
        helper_item->setPosition(Vec2(origin.x + visibleSize.width * 0.8,
            origin.y + visibleSize.height / 3));
        menu->addChild(helper_item);
    
    //冒险模式
    auto adventure_item = MenuItemImage::create("/MenuScene/Btn_NormalModle.PNG", "/MenuScene/Btn_NormalModleLight.PNG", CC_CALLBACK_1(CGameMenu::goto_adventure, this));
        adventure_item->setScale(0.9f);
        adventure_item->setPosition(Vec2(origin.x + visibleSize.width / 2 - adventure_item->getContentSize().width,
            origin.y + visibleSize.height / 8));


        menu->addChild(adventure_item);
    
    //BOSS模式
    auto boss_item = MenuItemImage::create("/MenuScene/Btn_Boss.PNG", "/MenuScene/Btn_BossLight.PNG", CC_CALLBACK_1(CGameMenu::goto_bossmode, this));

        boss_item->setScale(0.9f);
        boss_item->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 8));
        menu->addChild(boss_item);

    ////锁
    //    Sprite* Locked1 = nullptr;
    //    InitSprite(Locked1, boss_item, "MenuScene/front_btn_floor_locked.png", boss_item->getContentSize().width-12, boss_item->getContentSize().height / 2, 1);
    //    if (nullptr == Locked1)
    //        return false;

    //怪物巢
    auto nest_item = MenuItemImage::create("/MenuScene/Btn_MonsterNest.PNG", "/MenuScene/Btn_MonsterNestLight.PNG", CC_CALLBACK_1(CGameMenu::goto_monsternest, this));
        nest_item->setScale(0.9f);
        nest_item->setPosition(Vec2(origin.x + visibleSize.width / 2 + nest_item->getContentSize().width,
            origin.y + visibleSize.height / 8));
        menu->addChild(nest_item);


    //锁
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
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, options_scene, Color3B::BLACK)); // 使用淡入淡出效果，时间为0.5秒
}


void CGameMenu::goto_helper(Ref* pSender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    auto helper_scene = HelperScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, helper_scene, Color3B::BLACK)); // 使用淡入淡出效果，时间为0.5秒
}


void  CGameMenu::goto_adventure(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
    pLevelMgr->setCurIndex(3);
    CGameScene::deletInstance();
    Director::getInstance()->replaceScene(CGameLevel::create());
    //创建了一个 CGameLevel 的实例作为新的场景，见CGameLevel.cpp
}

void  CGameMenu::goto_bossmode(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
    pLevelMgr->setCurIndex(3); // 设置当前关卡索引为第四个关卡（索引从0开始）
    CGameScene::deletInstance();
    Director::getInstance()->replaceScene(CGameScene::getInstance());// 创建 CGameLevel 的实例作为新的场景
}

void  CGameMenu::goto_monsternest(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    //待开发
}