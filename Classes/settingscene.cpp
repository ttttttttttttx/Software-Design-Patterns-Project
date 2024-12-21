#include "GameMenu.h"
#include "settingscene.h"
#include "ui/CocosGUI.h"
#include "audio/include/AudioEngine.h"
static int bg_music_id;
static int sound_effect = 1;
using namespace cocos2d::experimental;
USING_NS_CC;
///*������*/
//static void problemLoading(const char* filename)
//{
//    printf("Error while loading: %s\n", filename);
//}
/**********************   OptionsScene��   ***********************************/
Scene* OptionsScene::createScene()
{
    return OptionsScene::create();
}
bool OptionsScene::init()
{
    /*��ʼ������*/
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();//��ȡ��������Ĵ�С
    Vec2 origin = Director::getInstance()->getVisibleOrigin();//��ȡ���������ԭ��

    /**********************  ѡ���  ***********************/
    auto set_layer = SetLayer::createLayer();//������Ϊ SetLayer ��ͼ�����
    set_layer->setName("SetLayer");
    this->addChild(set_layer);
    /*****************  ͳ�ƽ���  ******************/
    auto statistics_layer = StatisticsLayer::createLayer();
    statistics_layer->setName("StatisticsLayer");
    this->addChild(statistics_layer);
    /****************  ������Ա����  ***************/
    auto person_layer = PersonLayer::createLayer();
    person_layer->setName("PersonLayer");
    this->addChild(person_layer);
    /********************************  ���˵�  ************************************/
    auto menu_all = Menu::create();
    menu_all->setPosition(Vec2::ZERO);

    auto home = MenuItemImage::create("/OptionsScene/Btn_Return.PNG", "/OptionsScene/Btn_ReturnLight.PNG", CC_CALLBACK_1(OptionsScene::goto_menu, this));
    home->setPosition(Vec2(origin.x + visibleSize.width * 0.1,
        origin.y + visibleSize.height * 0.9));
    menu_all->addChild(home);

    this->addChild(menu_all);
    /*********************************  �л�ѡ�  ************************************/
    //setѡ�
    auto set_btn = ui::Button::create("/OptionsScene/setting02-hd_45_normal.PNG", "/OptionsScene/setting02-hd_45_normal.PNG", "/OptionsScene/setting02-hd_45.PNG");
    set_btn->setName("SetBtn");//���ð�ť������Ϊ "SetBtn"�����ڱ�ʶ�����ť
    set_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.3,
        origin.y + visibleSize.height * 0.925));
    set_btn->setContentSize(Size(set_btn->getContentSize().width * 2, set_btn->getContentSize().height));//�޸İ�ť�����ݴ�С�������ȱ�Ϊԭ��������
    //set_btn->addTouchEventListener(CC_CALLBACK_1(OptionsScene::goto_set, this));//����ť�����ʱ��ִ�� OptionsScene ���е� goto_set ����
    set_btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
            goto_set(sender); // �ڰ���ʱִ�к���
        }
    });

    set_btn->setEnabled(false);//�������õĿ�ʼ������ѡ�����
    this->addChild(set_btn);
   
    //statisticsѡ�
    auto statistics_btn = ui::Button::create("/OptionsScene/setting02-hd_43_normal.PNG", "/OptionsScene/setting02-hd_43_normal.PNG", "/OptionsScene/setting02-hd_43.PNG");
    statistics_btn->setName("StatisticsBtn");
    statistics_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height * 0.925));
    //statistics_btn->setScale(1.4);//�Ű�ť�Ĵ�СΪԭʼ��С��1.4���������˰�ť�ĳߴ�
    //statistics_btn->addTouchEventListener(CC_CALLBACK_1(OptionsScene::goto_statistics, this));//����ť�����ʱ��ִ�� OptionsScene ���е� goto_statistics ����
    statistics_btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
            goto_statistics(sender); // �ڰ���ʱִ�к���
        }
        });
    this->addChild(statistics_btn);
    
    //personѡ�
    auto person_btn = ui::Button::create("/OptionsScene/setting02-hd_48_normal.PNG", "/OptionsScene/setting02-hd_48_normal.PNG", "/OptionsScene/setting02-hd_48.PNG");
    person_btn->setName("PersonBtn");
    person_btn->setPosition(Vec2(origin.x + visibleSize.width * 0.7,
        origin.y + visibleSize.height * 0.925));
    //person_btn->setScale(1.4);//���Ű�ť�Ĵ�СΪԭʼ��С��1.4���������˰�ť�ĳߴ�
    //person_btn->addTouchEventListener(CC_CALLBACK_1(OptionsScene::goto_person, this));//����ť�����ʱ��ִ�� OptionsScene ���е� goto_person ����
    person_btn->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            AudioEngine::play2d("sound/Select.mp3", false, 1.2f);
            goto_person(sender); // �ڰ���ʱִ�к���
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
void OptionsScene::goto_set(Ref* psender)//���ý�ѡ���ɼ�
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    //�ڵ�ǰ�������ҵ��ض����ƵĽڵ㣨SetLayer��StatisticsLayer��PersonLayer��
    Node* set = this->getChildByName("SetLayer");
    Node* statistics = this->getChildByName("StatisticsLayer");
    Node* person = this->getChildByName("PersonLayer");

    set->setVisible(true);//���ò㣨SetLayer��������Ϊ�ɼ�
    statistics->setVisible(false);//ͳ�Ʋ㣨StatisticsLayer�����������㣨PersonLayer��������Ϊ���ɼ�
    person->setVisible(false);

    Node* set_btn = this->getChildByName("SetBtn");//ͨ�� getChildByName ������ȡ��Ϊ "SetBtn" �Ľڵ�
    static_cast<ui::Button*>(set_btn)->setEnabled(false);//����ȡ�Ľڵ� set_btn ת��Ϊ ui::Button ���ͣ�������ť״̬����Ϊ����
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
void OptionsScene::goto_statistics(Ref* psender)//���ý�ͳ�Ʋ�ɼ�
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
void OptionsScene::goto_person(Ref* psender)//���ý���Ա��ɼ�
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
/**************************   SetLayer��   ******************************/
cocos2d::Layer* SetLayer::createLayer()
{
    return SetLayer::create();//������ SetLayer::create() ����ͼ�㣬�����䷵��
}
bool SetLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //����������ý���ı���ͼƬ
    auto set = Sprite::create("/OptionsScene/SettingBG1.PNG");
        set->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));
        this->addChild(set);
    

    //��������
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
    
    /*********************************  ѡ���˵�  **************************************/
    auto set_menu = Menu::create();
    set_menu->setPosition(Vec2::ZERO);
    //��Чtoggle
    auto sound_on_sprite = Sprite::create("/OptionsScene/setting02-hd_6.PNG");
    auto sound_on = MenuItemSprite::create(sound_on_sprite, sound_on_sprite);//ʹ�� sound_on_sprite ��Ϊ����״̬��ͼ��ͱ�ѡ��״̬��ͼ��
    auto sound_off_sprite = Sprite::create("/OptionsScene/setting02-hd_11.PNG");
    auto sound_off = MenuItemSprite::create(sound_off_sprite, sound_off_sprite);
    MenuItemToggle* sound_toggle; //������һ�� MenuItemToggle �������ڴ�����Ч�Ŀ��ء�
    if (sound_effect==1) {
        sound_toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SetLayer::close_sound, this), sound_on, sound_off, NULL);
    }
    else {
        sound_toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SetLayer::close_sound, this), sound_off, sound_on, NULL);
    }
    sound_toggle->setPosition(Vec2(origin.x + visibleSize.width * 0.4,
        origin.y + visibleSize.height * 0.6));
    set_menu->addChild(sound_toggle);
    //��������toggle
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
    //������Ϸ
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
    //��ʵ��
}
/*************************   StatisticsLayer��  ****************************/
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

    //����ͼ
    auto statics = Sprite::create("/OptionsScene/SettingBG2.PNG");
        statics->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));

        this->addChild(statics);
    
    //��������
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
/*************************  PersonLayer��  ********************************/
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

    //����
    auto person = Sprite::create("/OptionsScene/SettingBG5.jpg");
        person->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));

        this->addChild(person);
    

    this->setVisible(false);
    return true;
}
