#include "GameMenu.h"
#include "settingscene.h"
#include "ui/CocosGUI.h"
#include "audio/include/AudioEngine.h"
static int bg_music_id;
static int sound_effect = 1;
using namespace cocos2d::experimental;
USING_NS_CC;
///*错误处理*/
//static void problemLoading(const char* filename)
//{
//    printf("Error while loading: %s\n", filename);
//}
/**********************   OptionsScene类   ***********************************/
Scene* OptionsScene::createScene()
{
    return OptionsScene::create();
}
bool OptionsScene::init()
{
    /*初始化场景*/
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();//获取可视区域的大小
    Vec2 origin = Director::getInstance()->getVisibleOrigin();//获取可视区域的原点

    /**********************  选项层  ***********************/
    auto set_layer = SetLayer::createLayer();//创建名为 SetLayer 的图层对象
    set_layer->setName("SetLayer");
    this->addChild(set_layer);
    /*****************  统计界面  ******************/
    auto statistics_layer = StatisticsLayer::createLayer();
    statistics_layer->setName("StatisticsLayer");
    this->addChild(statistics_layer);
    /****************  开发人员界面  ***************/
    auto person_layer = PersonLayer::createLayer();
    person_layer->setName("PersonLayer");
    this->addChild(person_layer);
    /********************************  主菜单  ************************************/
    auto menu_all = Menu::create();
    menu_all->setPosition(Vec2::ZERO);

    auto home = MenuItemImage::create("/OptionsScene/Btn_Return.PNG", "/OptionsScene/Btn_ReturnLight.PNG", CC_CALLBACK_1(OptionsScene::goto_menu, this));
    home->setPosition(Vec2(origin.x + visibleSize.width * 0.1,
        origin.y + visibleSize.height * 0.9));
    menu_all->addChild(home);

    this->addChild(menu_all);
    /*********************************  切换选项卡  ************************************/
    //set选项卡
    auto set_btn = ui::Button::create("/OptionsScene/setting02-hd_45_normal.PNG", "/OptionsScene/setting02-hd_45_normal.PNG", "/OptionsScene/setting02-hd_45.PNG");
    set_btn->setName("SetBtn");//设置按钮的名称为 "SetBtn"，用于标识这个按钮
    set_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.3,
        origin.y + visibleSize.height * 0.925));
    set_btn->setContentSize(Size(set_btn->getContentSize().width * 2, set_btn->getContentSize().height));//修改按钮的内容大小，将其宽度变为原来的两倍
    //set_btn->addTouchEventListener(CC_CALLBACK_1(OptionsScene::goto_set, this));//当按钮被点击时，执行 OptionsScene 类中的 goto_set 函数
    set_btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
            goto_set(sender); // 在按下时执行函数
        }
    });

    set_btn->setEnabled(false);//进入设置的开始界面在选项界面
    this->addChild(set_btn);
   
    //statistics选项卡
    auto statistics_btn = ui::Button::create("/OptionsScene/setting02-hd_43_normal.PNG", "/OptionsScene/setting02-hd_43_normal.PNG", "/OptionsScene/setting02-hd_43.PNG");
    statistics_btn->setName("StatisticsBtn");
    statistics_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.925));
    //statistics_btn->setScale(1.4);//放按钮的大小为原始大小的1.4倍，增大了按钮的尺寸
    //statistics_btn->addTouchEventListener(CC_CALLBACK_1(OptionsScene::goto_statistics, this));//当按钮被点击时，执行 OptionsScene 类中的 goto_statistics 函数
    statistics_btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
            goto_statistics(sender); // 在按下时执行函数
        }
        });
    this->addChild(statistics_btn);
    
    //person选项卡
    auto person_btn = ui::Button::create("/OptionsScene/setting02-hd_48_normal.PNG", "/OptionsScene/setting02-hd_48_normal.PNG", "/OptionsScene/setting02-hd_48.PNG");
    person_btn->setName("PersonBtn");
    person_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.7,
        origin.y + visibleSize.height * 0.925));
    //person_btn->setScale(1.4);//缩放按钮的大小为原始大小的1.4倍，增大了按钮的尺寸
    //person_btn->addTouchEventListener(CC_CALLBACK_1(OptionsScene::goto_person, this));//当按钮被点击时，执行 OptionsScene 类中的 goto_person 函数
    person_btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
            goto_person(sender); // 在按下时执行函数
        }
    });
    this->addChild(person_btn);

    return true;
}
void OptionsScene::goto_menu(Ref* psender)
{
    AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
    auto menu_scene = CGameMenu::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, menu_scene, Color3B::BLACK));
}
void OptionsScene::goto_set(Ref* psender)//设置仅选项层可见
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    //在当前场景中找到特定名称的节点（SetLayer、StatisticsLayer、PersonLayer）
    Node* set = this->getChildByName("SetLayer");
    Node* statistics = this->getChildByName("StatisticsLayer");
    Node* person = this->getChildByName("PersonLayer");

    set->setVisible(true);//设置层（SetLayer）被设置为可见
    statistics->setVisible(false);//统计层（StatisticsLayer）和制作方层（PersonLayer）被设置为不可见
    person->setVisible(false);

    Node* set_btn = this->getChildByName("SetBtn");//通过 getChildByName 方法获取名为 "SetBtn" 的节点
    static_cast<ui::Button*>(set_btn)->setEnabled(false);//将获取的节点 set_btn 转换为 ui::Button 类型，并将按钮状态设置为禁用
    set_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.3,
        origin.y + visibleSize.height * 0.925));

    Node* statistics_btn =this->getChildByName("StatisticsBtn");
    static_cast<ui::Button*>(statistics_btn)->setEnabled(true);
    statistics_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.925));


    Node* person_btn = this->getChildByName("PersonBtn");
    static_cast<ui::Button*>(person_btn)->setEnabled(true);
    person_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.7,
        origin.y + visibleSize.height * 0.925));

}
void OptionsScene::goto_statistics(Ref* psender)//设置仅统计层可见
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Node* set = this->getChildByName("SetLayer");
    Node* statistics = this->getChildByName("StatisticsLayer");
    Node* person = this->getChildByName("PersonLayer");

    set->setVisible(false);
    statistics->setVisible(true);
    person->setVisible(false);

    Node* set_btn = this->getChildByName("SetBtn");
    static_cast<ui::Button*>(set_btn)->setEnabled(true);
    set_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.3,
        origin.y + visibleSize.height * 0.93));


    Node* statistics_btn = this->getChildByName("StatisticsBtn");
    static_cast<ui::Button*>(statistics_btn)->setEnabled(false);
    statistics_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.925));


    Node* person_btn = this->getChildByName("PersonBtn");
    static_cast<ui::Button*>(person_btn)->setEnabled(true);
    person_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.7,
        origin.y + visibleSize.height * 0.925));

}
void OptionsScene::goto_person(Ref* psender)//设置仅人员层可见
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Node* set = this->getChildByName("SetLayer");
    Node* statistics = this->getChildByName("StatisticsLayer");
    Node* person = this->getChildByName("PersonLayer");

    set->setVisible(false);
    statistics->setVisible(false);
    person->setVisible(true);

    Node* set_btn = this->getChildByName("SetBtn");
    static_cast<ui::Button*>(set_btn)->setEnabled(true);
    set_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.3,
        origin.y + visibleSize.height * 0.93));

    Node* statistics_btn = this->getChildByName("StatisticsBtn");
    static_cast<ui::Button*>(statistics_btn)->setEnabled(true);
    statistics_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.925));

    Node* person_btn = this->getChildByName("PersonBtn");
    static_cast<ui::Button*>(person_btn)->setEnabled(false);
    person_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.7,
        origin.y + visibleSize.height * 0.925));
}
/**************************   SetLayer类   ******************************/
cocos2d::Layer* SetLayer::createLayer()
{
    return SetLayer::create();//调用了 SetLayer::create() 创建图层，并将其返回
}
bool SetLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //添加整个设置界面的背景图片
    auto set = Sprite::create("/OptionsScene/SettingBG1.PNG");
        set->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));
        this->addChild(set);
    

    //其他背景
    auto sound_txt = Sprite::create("/OptionsScene/setting02-hd_0.PNG");
        sound_txt->setPosition(Vec2(origin.x + visibleSize.width * 0.4,
            origin.y + visibleSize.height * 0.7));
        this->addChild(sound_txt);
    

    auto bgmusic_txt = Sprite::create("/OptionsScene/setting02-hd_2.PNG");
        bgmusic_txt->setPosition(Vec2(origin.x + visibleSize.width * 0.6,
            origin.y + visibleSize.height * 0.7));
        this->addChild(bgmusic_txt);
    

    auto appeal_txt = Sprite::create("/OptionsScene/setting02-hd_7.PNG");
        appeal_txt->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));
        this->addChild(appeal_txt);
    

    auto weibo_image = Sprite::create("/OptionsScene/setting02-hd_12.PNG");

        weibo_image->setPosition(Vec2(origin.x + visibleSize.width * 0.4,
            origin.y + visibleSize.height * 0.35));
        weibo_image->setScale(2);
        this->addChild(weibo_image);
    

    auto wechat_image = Sprite::create("/OptionsScene/setting02-hd_16.PNG");

        wechat_image->setPosition(Vec2(origin.x + visibleSize.width * 0.6,
            origin.y + visibleSize.height * 0.35));
        wechat_image->setScale(2);
        this->addChild(wechat_image);
    
    /*********************************  选项层菜单  **************************************/
    auto set_menu = Menu::create();
    set_menu->setPosition(Vec2::ZERO);
    //音效toggle
    auto sound_on_sprite = Sprite::create("/OptionsScene/setting02-hd_6.PNG");
    auto sound_on = MenuItemSprite::create(sound_on_sprite, sound_on_sprite);//使用 sound_on_sprite 作为正常状态的图标和被选中状态的图标
    auto sound_off_sprite = Sprite::create("/OptionsScene/setting02-hd_11.PNG");
    auto sound_off = MenuItemSprite::create(sound_off_sprite, sound_off_sprite);
    MenuItemToggle* sound_toggle; //创建了一个 MenuItemToggle 对象用于处理音效的开关。
    if (sound_effect==1) {
        sound_toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SetLayer::close_sound, this), sound_on, sound_off, NULL);
    }
    else {
        sound_toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SetLayer::close_sound, this), sound_off, sound_on, NULL);
    }
    sound_toggle->setPosition(Vec2(origin.x + visibleSize.width * 0.4,
        origin.y + visibleSize.height * 0.6));
    set_menu->addChild(sound_toggle);
    //背景音乐toggle
    auto bgmusic_on_sprite = Sprite::create("/OptionsScene/setting02-hd_15.PNG");
    auto bgmusic_on = MenuItemSprite::create(bgmusic_on_sprite, bgmusic_on_sprite);
    auto bgmusic_off_sprite = Sprite::create("/OptionsScene/setting02-hd_21.PNG");
    auto bgmusic_off = MenuItemSprite::create(bgmusic_off_sprite, bgmusic_off_sprite);
    MenuItemToggle* bgmusic_toggle;
    if (AudioEngine::getState(bg_music_id) == AudioEngine::AudioState(1)) {
        bgmusic_toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SetLayer::close_bgmusic, this), bgmusic_on, bgmusic_off, NULL);
    }
    else {
        bgmusic_toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SetLayer::close_bgmusic, this), bgmusic_off, bgmusic_on, NULL);
    }
    bgmusic_toggle->setPosition(Vec2(origin.x + visibleSize.width * 0.6,
        origin.y + visibleSize.height * 0.6));
    set_menu->addChild(bgmusic_toggle);
    //重置游戏
    auto reset_game = MenuItemImage::create("/OptionsScene/setting02-hd_55.PNG", "/OptionsScene/setting02-hd_54.PNG", CC_CALLBACK_1(SetLayer::reset_game, this));
    reset_game->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.15));
    set_menu->addChild(reset_game);

    this->addChild(set_menu);
    this->setVisible(true);


    return true;
}
void SetLayer::close_sound(Ref* psender)
{
    if (sound_effect == 1) {
        sound_effect = 0;
    }
    else {
        sound_effect = 1;
    }
}
void SetLayer::close_bgmusic(Ref* psender)
{
    auto state = AudioEngine::getState(bg_music_id);
    if (AudioEngine::getState(bg_music_id) == AudioEngine::AudioState(1)) {
        AudioEngine::pause(bg_music_id);
    }
    else if (AudioEngine::getState(bg_music_id) == AudioEngine::AudioState(2)) {
        AudioEngine::resume(bg_music_id);
    }
}
void SetLayer::reset_game(Ref* psender)
{
    //待实现
}
/*************************   StatisticsLayer类  ****************************/
cocos2d::Layer* StatisticsLayer::createLayer()
{
    return StatisticsLayer::create();
}
bool StatisticsLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //背景图
    auto statics = Sprite::create("/OptionsScene/SettingBG2.PNG");
        statics->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));

        this->addChild(statics);
    
    //其他背景
    auto bg1 = Sprite::create("/OptionsScene/setting02-hd_22.PNG");
        bg1->setPosition(Vec2(origin.x + visibleSize.width * 0.5,
            origin.y + visibleSize.height * 0.68));
        this->addChild(bg1);
    
    auto bg2 = Sprite::create("/OptionsScene/setting02-hd_27.PNG");
        bg2->setPosition(Vec2(origin.x + visibleSize.width * 0.5,
            origin.y + visibleSize.height * 0.58));
        this->addChild(bg2);
    
    auto bg3 = Sprite::create("/OptionsScene/setting02-hd_31.PNG");
        bg3->setPosition(Vec2(origin.x + visibleSize.width * 0.51,
            origin.y + visibleSize.height * 0.49));
        this->addChild(bg3);
    
    auto bg4 = Sprite::create("/OptionsScene/setting02-hd_35.PNG");
        bg4->setPosition(Vec2(origin.x + visibleSize.width * 0.485,
            origin.y + visibleSize.height * 0.41));
        this->addChild(bg4);
    
    auto bg5 = Sprite::create("/OptionsScene/setting02-hd_38.PNG");
        bg5->setPosition(Vec2(origin.x + visibleSize.width * 0.523,
            origin.y + visibleSize.height * 0.32));
        this->addChild(bg5);
    
    auto bg6 = Sprite::create("/OptionsScene/setting02-hd_42.PNG");
        bg6->setPosition(Vec2(origin.x + visibleSize.width * 0.534,
            origin.y + visibleSize.height * 0.23));
        this->addChild(bg6);
    
    auto bg7 = Sprite::create("/OptionsScene/setting02-hd_44.PNG");
        bg7->setPosition(Vec2(origin.x + visibleSize.width * 0.523,
            origin.y + visibleSize.height * 0.14));
        this->addChild(bg7);
    

    this->setVisible(false);


    return true;
}
/*************************  PersonLayer类  ********************************/
cocos2d::Layer* PersonLayer::createLayer()
{
    return PersonLayer::create();
}
bool PersonLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //背景
    auto person = Sprite::create("/OptionsScene/SettingBG5.jpg");
        person->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));

        this->addChild(person);
    

    this->setVisible(false);
    return true;
}
