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

/*�˵�����*/
Scene* CGameMenu::createScene()
{
	return CGameMenu::create();
}


///*�����������ڴ�����Դ���س�������������ͼƬ���ļ��޷�����*/
//static void problemLoading(const char* filename)
//{
//	printf("Error while loading: %s\n", filename);
//}



bool CGameMenu::init()
{
    //������ʽ���ó����� init() ����
    if (!Scene::init())
    {
        return false;
    }

    //��ȡ��ǰ��Ļ�ɼ�����Ĵ�С visibleSize ��ԭ������ origin
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    ;
    /*****************************************����ͼ**************************************************/
    auto background_image = Sprite::create("/MenuScene/MainBG.PNG");
    //������һ������ͼƬ�� Sprite ���� background_image����������Ϊ "MainBG.PNG" ��ͼƬ��Դ

        background_image->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));//������ͼ��λ������Ϊ��Ļ����

        this->addChild(background_image);//������ӵ���ǰ������
    /******************************************��*******************************************************/
    auto cloud = Sprite::create();//����һ����Ϊ cloud �Ŀվ���ڵ�
    cloud->setName("Cloud");//Ϊ�Ʋ�ڵ���������Ϊ "Cloud"

    //���ƣ�cloud1��
    auto cloud1 = Sprite::create("/MenuScene/Cloud.PNG");//���� cloud1 ���飬������Ϊ "Cloud.PNG" ���ƶ�ͼƬ��Դ

        cloud1->setPosition(Vec2(origin.x - cloud1->getContentSize().width,/*cloud1->getContentSize().width �� cloud1 ����Ŀ��*/
            origin.y + visibleSize.height * 0.85));
        cloud1->setOpacity(85);/*���� cloud1 �Ĳ�͸����Ϊ 85��ʹ����΢��͸��*/
        //��Ӷ���
        auto cloud1_moveto_1 = MoveTo::create(15, Vec2(origin.x + visibleSize.width + cloud1->getContentSize().width, origin.y + visibleSize.height * 0.85));
        auto cloud1_moveto_2 = MoveTo::create(15, Vec2(origin.x - cloud1->getContentSize().width, origin.y + visibleSize.height * 0.85));
        auto cloud1_sequence = Sequence::create(cloud1_moveto_1, DelayTime::create(5), cloud1_moveto_2, DelayTime::create(5), nullptr);
        cloud1->runAction(RepeatForever::create(cloud1_sequence));
        cloud->addChild(cloud1);
    
    //С��
    auto cloud2 = Sprite::create("/MenuScene/Cloud2.PNG");
        cloud2->setPosition(Vec2(origin.x + visibleSize.width + cloud2->getContentSize().width,
            origin.y + visibleSize.height * 0.8));
        cloud2->setScale(2);
        cloud2->setOpacity(95);
        //��Ӷ���
        auto cloud2_moveto_1 = MoveTo::create(15, Vec2(origin.x - cloud2->getContentSize().width, origin.y + visibleSize.height * 0.85));
        auto cloud2_moveto_2 = MoveTo::create(15, Vec2(origin.x + visibleSize.width + cloud2->getContentSize().width, origin.y + visibleSize.height * 0.85));
        auto cloud2_sequence = Sequence::create(cloud2_moveto_1, DelayTime::create(5), cloud2_moveto_2, DelayTime::create(5), nullptr);
        cloud2->runAction(RepeatForever::create(cloud2_sequence));
        cloud->addChild(cloud2);

    this->addChild(cloud);
    /*****************************************************��ӹ���ͼ��****************************************************/
    auto monster = Sprite::create("/MenuScene/FlyMonster.PNG");

        monster->setPosition(Vec2(origin.x + visibleSize.width / 4,
            origin.y + visibleSize.height * 0.8));
        //��Ӷ���
        auto monster_moveby_up = MoveBy::create(1.0f, Vec2(0, monster->getContentSize().height / 4));//������һ�������ƶ��Ķ���
        auto monster_moveby_down = MoveBy::create(1.0f, Vec2(0, -monster->getContentSize().height / 4));//������һ�������ƶ��Ķ���
        /*1.0f��ʾ����������ʱ�䣬����ָ��������1s
        * Vec2(0, -monster->getContentSize().height / 4)��λ������
        * 0��ʾx���ϲ��ƶ�����ˮƽ�����ƶ�
        * monster->getContentSize().height�ǹ���߶ȣ������ķ�֮һ������/���ƶ�
        */
        auto monster_sequence = Sequence::createWithTwoActions(monster_moveby_down, monster_moveby_up);//�������ƶ�������Ϊһ���������� 
        monster->runAction(RepeatForever::create(monster_sequence));//��������������Ϊ����Ķ�����ʹ��RepeatForever::createʹ�������ظ�ִ��
        this->addChild(monster);
    
    /****************************************************����ܲ�ͼ��*****************************************************/
    auto carrot = Sprite::create();//������һ���յ� Sprite����һ���ܲ�Ԫ��
    carrot->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 4));
    //1,����ܲ�Ҷ��
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
    

    //2,����ܲ�����
    auto carrot_body = Sprite::create("/MenuScene/CarrotBody.PNG");

        carrot_body->setPosition(Vec2(origin.x,
            origin.y + visibleSize.height / 4 + carrot_body->getContentSize().height / 4));

        carrot->addChild(carrot_body);
    

    this->addChild(carrot);
   
    /*************************************************************��ӱ����ܲ�����******************************************************************/
    auto title = Sprite::create("/MenuScene/MainTitle.PNG");
        title->setPosition(Vec2(origin.x + visibleSize.width / 2 + title->getContentSize().width / 20,
            origin.y + visibleSize.height / 2 - title->getContentSize().height / 7));
        this->addChild(title);
   
    /*****************************************************************�˵�����************************************************************************/
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    //�˳���Ϸ��ť
    auto close_item = MenuItemFont::create("EXIT", CC_CALLBACK_1(CGameMenu::game_close, this));
        close_item->setPosition(Vec2(origin.x + visibleSize.width - close_item->getContentSize().width * 0.6,
            origin.y + visibleSize.height - close_item->getContentSize().height / 2));
        menu->addChild(close_item);
    
    //���ð�ť
    auto options_item = MenuItemImage::create("/MenuScene/Btn_Set.PNG", "/MenuScene/Btn_SetLight.PNG", CC_CALLBACK_1(CGameMenu::goto_sets, this));
    //����һ���˵����������ͼƬ·���������ֱ��ǰ�ť����״̬�Ͱ�ť��ѡ��״̬��ͼƬ·��
    //����������CC_CALLBACK_1(CGameMenu::goto_options, this)��ʾ����˵���ʱ����� CGameMenu ���е� goto_options ������
        options_item->setPosition(Vec2(origin.x + visibleSize.width * 0.2,
            origin.y + visibleSize.height / 3));//�����˲˵��� options_item ��λ��
        menu->addChild(options_item);//�������Ĳ˵��� options_item ��ӵ� menu ��
    
    //������ť
    auto helper_item = MenuItemImage::create("/MenuScene/Btn_Help.PNG", "/MenuScene/Btn_HelpLight.PNG", CC_CALLBACK_1(CGameMenu::goto_helper, this));
        helper_item->setPosition(Vec2(origin.x + visibleSize.width * 0.8,
            origin.y + visibleSize.height / 3));
        menu->addChild(helper_item);
    
    //ð��ģʽ
    auto adventure_item = MenuItemImage::create("/MenuScene/Btn_NormalModle.PNG", "/MenuScene/Btn_NormalModleLight.PNG", CC_CALLBACK_1(CGameMenu::goto_adventure, this));
        adventure_item->setScale(0.9f);
        adventure_item->setPosition(Vec2(origin.x + visibleSize.width / 2 - adventure_item->getContentSize().width,
            origin.y + visibleSize.height / 8));


        menu->addChild(adventure_item);
    
    //BOSSģʽ
    auto boss_item = MenuItemImage::create("/MenuScene/Btn_Boss.PNG", "/MenuScene/Btn_BossLight.PNG", CC_CALLBACK_1(CGameMenu::goto_bossmode, this));

        boss_item->setScale(0.9f);
        boss_item->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 8));
        menu->addChild(boss_item);

    ////��
    //    Sprite* Locked1 = nullptr;
    //    InitSprite(Locked1, boss_item, "MenuScene/front_btn_floor_locked.png", boss_item->getContentSize().width-12, boss_item->getContentSize().height / 2, 1);
    //    if (nullptr == Locked1)
    //        return false;

    //���ﳲ
    auto nest_item = MenuItemImage::create("/MenuScene/Btn_MonsterNest.PNG", "/MenuScene/Btn_MonsterNestLight.PNG", CC_CALLBACK_1(CGameMenu::goto_monsternest, this));
        nest_item->setScale(0.9f);
        nest_item->setPosition(Vec2(origin.x + visibleSize.width / 2 + nest_item->getContentSize().width,
            origin.y + visibleSize.height / 8));
        menu->addChild(nest_item);


    //��
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
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, options_scene, Color3B::BLACK)); // ʹ�õ��뵭��Ч����ʱ��Ϊ0.5��
}


void CGameMenu::goto_helper(Ref* pSender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    auto helper_scene = HelperScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, helper_scene, Color3B::BLACK)); // ʹ�õ��뵭��Ч����ʱ��Ϊ0.5��
}


void  CGameMenu::goto_adventure(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
    pLevelMgr->setCurIndex(3);
    CGameScene::deletInstance();
    Director::getInstance()->replaceScene(CGameLevel::create());
    //������һ�� CGameLevel ��ʵ����Ϊ�µĳ�������CGameLevel.cpp
}

void  CGameMenu::goto_bossmode(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
    pLevelMgr->setCurIndex(3); // ���õ�ǰ�ؿ�����Ϊ���ĸ��ؿ���������0��ʼ��
    CGameScene::deletInstance();
    Director::getInstance()->replaceScene(CGameScene::getInstance());// ���� CGameLevel ��ʵ����Ϊ�µĳ���
}

void  CGameMenu::goto_monsternest(Ref* psender) {
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    //������
}