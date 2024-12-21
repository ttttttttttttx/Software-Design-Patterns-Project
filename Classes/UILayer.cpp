#include "UILayer.h"
#include "DataClass.h"
#include "DataMgr.h"
#include "GameScene.h"
#include "GameLevel.h"
CUILayer::CUILayer()
{

}

CUILayer::~CUILayer()
{

}

bool CUILayer::init()
{
	if (!Node::init())
	{
		return false;
	}

	//��ȫ�ֵ����ݹ������л�ȡ�ؿ����ݹ�������ʵ����������������ڴ���ؿ����ݵĴ�ȡ��
	CLevelDtMgr* pLevelDtMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
	
	//��ȡ��ǰ�ؿ������ݣ����Ը��ݵ�ǰ��Ϸ���Ȼ������õĲ�������ȡ�ؿ������ݣ���ؿ��ĳ�ʼ�����
	SLevelDt* pLevelDt = pLevelDtMgr->getCurData();

	//�ӹؿ������л�ȡ��ʼ����������洢�� m_nMoney �����С����ֵ�������ڽ�������ʾ��ǰӵ�еĽ������
	m_nMoney = pLevelDt->nStartMoney;

	Sprite* pBg = Sprite::createWithSpriteFrameName("MenuBG.png"); //�˵�����
	pBg->setPosition(WINSIZE.width / 2, WINSIZE.height - pBg->getContentSize().height / 2);
	this->addChild(pBg);

	m_pMoney = LabelAtlas::create(StringUtils::format("%d", m_nMoney), "font/numwhite-hd.png", 20, 32, '.');//���
	m_pMoney->setPosition(100, WINSIZE.height - pBg->getContentSize().height / 2);
	this->addChild(m_pMoney);

	//��ͣ��ť
	MenuItemImage* pPause = MenuItemImage::create();
	SpriteFrame* pPauseNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause01.png");
	pPause->setNormalSpriteFrame(pPauseNormalFrame);
	SpriteFrame* pPauseSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause02.png");
	pPause->setSelectedSpriteFrame(pPauseSelectFrame);

	//�ָ���ť
	MenuItemImage* pResum = MenuItemImage::create();
	SpriteFrame* pResumNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause11.png");
	pResum->setNormalSpriteFrame(pResumNormalFrame);
	SpriteFrame* pResumSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause12.png");
	pResum->setSelectedSpriteFrame(pResumSelectFrame);

	m_pPauseResum = MenuItemToggle::createWithCallback([=](Ref* pRef){
		if (0 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			m_pPause->setVisible(false);//����ͣ��ť����
			m_pWave->setVisible(true);//��ʾ�����������
			Director::getInstance()->resume();//�ָ���Ϸ����������
		}
		else if (1 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			m_pPause->setVisible(true);//��ʾ��ͣ��ť
			m_pWave->setVisible(false);//���ز����������
			Director::getInstance()->pause();//��ͣ��Ϸ����������
		}
	}, pPause, pResum, NULL);
	m_pPauseResum->setPosition(800, WINSIZE.height - pBg->getContentSize().height / 2);

	//�����ٶȰ�ť
	MenuItemImage* pSpeed = MenuItemImage::create();//�����յĲ˵���pSpeed
	SpriteFrame* pSpeedNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed11.png");//����˵��������״̬���
	pSpeed->setNormalSpriteFrame(pSpeedNormalFrame);//����Ϊ�˵��� pSpeed ������״̬�����
	SpriteFrame* pSpeedSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed12.png");//����˵��ѡ��ʱ�����
	pSpeed->setSelectedSpriteFrame(pSpeedSelectFrame);//����Ϊ�˵��� pSpeed ��ѡ��ʱ�����

	//���ٰ�ť
	MenuItemImage* pTwoSpeed = MenuItemImage::create();//�����˿յĲ˵��� pTwoSpeed
	SpriteFrame* pTwoSpeedNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed21.png");//����˵��������״̬���
	pTwoSpeed->setNormalSpriteFrame(pTwoSpeedNormalFrame);//����Ϊ�˵��� pTwoSpeed ������״̬�����
	SpriteFrame* pTwoSpeedSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed22.png");//����˵��ѡ��ʱ�����
	pTwoSpeed->setSelectedSpriteFrame(pTwoSpeedSelectFrame);//����Ϊ�˵��� pTwoSpeed ��ѡ��ʱ�����

	MenuItemToggle* pAddSpeed = MenuItemToggle::createWithCallback([=](Ref* pRef){
		//������һ���л���ť��
		if (0 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			//�������Ϊ 0����ʾ��һ����ť pSpeed ��ѡ�У���ʱ����������Ϊ 1.0f���������ٶ�
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		}
		else if (1 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			//�������Ϊ 1����ʾ�ڶ�����ť pTwoSpeed ��ѡ�У���ʱ����������Ϊ 2.0f���������ٶȼӿ�
			Director::getInstance()->getScheduler()->setTimeScale(2.0f);
		}
	}, pSpeed, pTwoSpeed, NULL);
	pAddSpeed->setPosition(700, WINSIZE.height - pBg->getContentSize().height / 2);// ���˰�ť�� pAddSpeed ��λ��


	//�˵���ť
	MenuItemImage* pMenuButton = MenuItemImage::create("", "", [=](Ref*){//����һ��ͼ��˵���
		m_pPauseResum->setSelectedIndex(1);//��ʾ��ͣ��
		m_pPauseResum->setEnabled(false);
		m_pMenu->setVisible(true);
		Director::getInstance()->pause();//��ͣ����Ϸ���������
	});
	SpriteFrame* pMenuButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu01.png");
	pMenuButton->setNormalSpriteFrame(pMenuButtonNormalFrame);//���ò˵��ť������״̬����ʾ�ľ���֡
	SpriteFrame* pMenuButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu02.png");
	pMenuButton->setSelectedSpriteFrame(pMenuButtonSelectFrame);//���ò˵��ť��ѡ��״̬����ʾ�ľ���֡
	pMenuButton->setPosition(900, WINSIZE.height - pBg->getContentSize().height / 2);

	//ʹ�� Menu::create() ����һ���˵�������֮ǰ�����������˵���Ͳ˵��ť
	Menu* pMenu = Menu::create(m_pPauseResum, pAddSpeed,pMenuButton, NULL);
	
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);

	//�˵�
	m_pMenu = Sprite::createWithSpriteFrameName("menu_bg.png");
	m_pMenu->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	m_pMenu->setVisible(false);
	this->addChild(m_pMenu,1);

	//������Ϸ��ť
	MenuItemImage* pResumButton = MenuItemImage::create("", "", [=](Ref*){
		m_pPauseResum->setSelectedIndex(0);//��ʾ������
		m_pPauseResum->setEnabled(true);//������ͣ/�ָ���ť
		Director::getInstance()->resume();//�ָ���Ϸ���������
		m_pMenu->setVisible(false);//�˵����ɼ�
	});
	SpriteFrame* pResumButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_normal_CN.png");
	pResumButton->setNormalSpriteFrame(pResumButtonNormalFrame);// ���ò˵��ť������״̬����ʾ�ľ���֡
	SpriteFrame* pResumButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_pressed_CN.png");
	pResumButton->setSelectedSpriteFrame(pResumButtonSelectFrame);//���ò˵��ť��ѡ��״̬����ʾ�ľ���֡
	pResumButton->setPosition(240,275);

	//���¿�ʼ��Ϸ��ť
	MenuItemImage* pRestartButton = MenuItemImage::create("", "", [=](Ref*){//������һ���˵���ť�� pRestartButton
		Director::getInstance()->resume();//�ָ���Ϸ��������С�
		m_pMenu->setVisible(false);//�� m_pMenu �˵�����Ϊ���ɼ�״̬
		CGameScene::deletInstance();//ɾ����Ϸ�����ĵ���ʵ�����ͷ��Ѿ����ص���Ϸ�������Ա����¿�ʼ��Ϸ��
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		Director::getInstance()->replaceScene(CGameScene::getInstance());
	});
	SpriteFrame* pRestartButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_normal_CN.png");
	pRestartButton->setNormalSpriteFrame(pRestartButtonNormalFrame);
	SpriteFrame* pRestartButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_pressed_CN.png");
	pRestartButton->setSelectedSpriteFrame(pRestartButtonSelectFrame);
	pRestartButton->setPosition(240, 182);

	//ѡ��ؿ���ť
	MenuItemImage* pQuitButton = MenuItemImage::create("", "", [=](Ref*){
		Director::getInstance()->resume();//�ָ���Ϸ���������
		m_pMenu->setVisible(false);// ��֮ǰ�����Ĳ˵���Ϊ���ɼ�״̬
		CGameScene::deletInstance();//ɾ����Ϸ����
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);//������Ϸʱ������Ϊ�����ٶ�
		Director::getInstance()->replaceScene(CGameLevel::create());//�滻��ǰ����Ϊ�´�������Ϸ�ؿ�����
	});
	SpriteFrame* pQuitButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_quit_normal_CN.png");
	pQuitButton->setNormalSpriteFrame(pQuitButtonNormalFrame);//���ò˵��ť������״̬����ʾ�ľ���֡
	SpriteFrame* pQuitButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_quit_pressed_CN.png");
	pQuitButton->setSelectedSpriteFrame(pQuitButtonSelectFrame);//���ò˵��ť��ѡ��״̬����ʾ�ľ���֡
	pQuitButton->setPosition(240, 88);

	//����һ���µĲ˵���pMenu1����������������Ϸ����ť�������¿�ʼ��Ϸ����ť�Լ���ѡ��ؿ�����ť��ӵ�����
	Menu* pMenu1 = Menu::create(pResumButton, pRestartButton, pQuitButton, NULL);
	pMenu1->setPosition(Vec2::ZERO);
	//������´����Ĳ˵���Ϊ�Ӳ˵���ӵ�֮ǰ���������˵� m_pMenu ��
	m_pMenu->addChild(pMenu1);

	//��ʾ��Ϸ��������
	m_pWave = Node::create();
	Sprite* pWave = Sprite::createWithSpriteFrameName("MenuCenter_01_CN.png");
	m_pWave->setPosition(400, WINSIZE.height - pBg->getContentSize().height / 2);
	m_pWave->addChild(pWave);

	//m_pMaxWave ��ʾ�ܲ�����ʹ�� LabelAtlas ����ʾ����
	m_pMaxWave = LabelAtlas::create(StringUtils::format("%d", pLevelDt->vecWave.size()), "font/numwhite-hd.png", 20, 32, '.');//�ܲ���
	m_pWave->addChild(m_pMaxWave);


	/*
	1" ��Ҫ��ʾ�ĳ�ʼ�ַ���
	"font/numwhite-hd.png" ��λͼ�����������ļ�·��
	20 ��ÿ���ַ��Ŀ�ȣ�������Ϊ��λ��
	32 ��ÿ���ַ��ĸ߶ȣ�������Ϊ��λ��
	'.' �����ڼ���ַ��Ŀհ��ַ�
	*/
	//m_pCurWaveLow ��ʾ��ǰ������λ���֣������ڸ�λ���ֵ�λ��
	m_pCurWaveLow = LabelAtlas::create("1", "font/numwhite-hd.png", 20, 32, '.');//��ǰ������λ
	m_pCurWaveLow->setPosition(-65, 0);
	m_pWave->addChild(m_pCurWaveLow);

	m_pCurWaveHigh = LabelAtlas::create("0", "font/numwhite-hd.png", 20, 32, '.');//��ǰ����ʮλ
	m_pCurWaveHigh->setPosition(-113, 0);
	m_pWave->addChild(m_pCurWaveHigh);
	this->addChild(m_pWave);

	//��ͣ��״̬
	m_pPause = Sprite::createWithSpriteFrameName("MenuCenter_02_CN.png");
	m_pPause->setPosition(400, WINSIZE.height - pBg->getContentSize().height / 2);
	m_pPause->setVisible(false);
	this->addChild(m_pPause);

	//����ʱ
	m_nTime = 1;//��ʼ������ʱ����
	m_pTime = Node::create();//������һ���ڵ� m_pTime�����ڴ�ŵ���ʱ����ľ���Ԫ��
	m_pTime->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	Sprite* pCountDownBg = Sprite::createWithSpriteFrameName("countdown_11.png");//��������ͼ
	m_pCountDown = Sprite::createWithSpriteFrameName("countdown_12.png");//������תͼ
	m_pTimeNumber = Sprite::createWithSpriteFrameName("countdown_01.png");//��������
	m_pTime->addChild(pCountDownBg);
	m_pTime->addChild(m_pCountDown);
	m_pTime->addChild(m_pTimeNumber);

	
	CallFunc* pCall = CallFunc::create([&]() {//����һ���ص�����
		if (m_nTime < 3) {
			m_nTime++;
			std::string frameName = StringUtils::format("countdown_%02d.png", m_nTime);
			//�����µĵ���ʱʱ������һ���µ�ͼƬ֡���ơ�
			m_pTimeNumber->setSpriteFrame(frameName);
		}
	});

	RotateBy* pRotateBy = RotateBy::create(1.0f, -360);//��ת����
	//������һ����ת���� pRotateBy����1���ڽ���ת���� -360 ��
	Sequence* pSeq = Sequence::createWithTwoActions(pRotateBy, pCall);

	// go�������
	CallFunc* pCallGo = CallFunc::create([&]() {
		m_pCountDown->stopAllActions(); //ֹͣ�˵���ʱ��תͼ m_pCountDown ������ִ�еĶ���
		m_pCountDown->removeFromParent();//������ʱ��תͼ���丸�ڵ����Ƴ�

		m_pTimeNumber->setScale(1.0f); // �ָ�ԭʼ��С
		m_pTimeNumber->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("countdown_13.png"));

		// ���ź͵�������
		ScaleTo* pScaleTo = ScaleTo::create(1.0f, 0.2f);
		FadeOut* fadeOut = FadeOut::create(1.0f);

		// �Ƴ��ڵ㲢��ʼ�����Ĵ���
		CallFunc* pCallEnd = CallFunc::create([&]() {
			m_pTimeNumber->stopAllActions();
			m_pTime->removeFromParent();
			CGameScene::getInstance()->getMonsterLayer()->startCreatMonster();
			});

		// ���ж���������С�ٵ�������ɺ��Ƴ��ڵ�
		Sequence* pSeq = Sequence::create(pScaleTo, fadeOut, pCallEnd, nullptr);
		m_pTimeNumber->runAction(pSeq);
		});

	// ִ�ж�������
	m_pCountDown->runAction(Sequence::createWithTwoActions(Repeat::create(pSeq, 3), pCallGo));
	//�������������������ɣ�ʹ�� Repeat �������ظ�ִ������ pSeq ���Σ�Ȼ��ִ�лص����� pCallGo

	this->addChild(m_pTime);

	return true;
}

//������ҵĽ������
void CUILayer::addMoney(int nMoney)
{
	m_nMoney += nMoney;

	//�����µĽ���������ַ�����ʽ��ʾ�ڽ�����
	m_pMoney->setString(StringUtils::format("%d", m_nMoney));
}

//���õ�ǰ��������ʾ
void CUILayer::setCurWave(int nCurWave)//nCurWave �Ǵ���ĵ�ǰ����ֵ
{
	m_pCurWaveLow->setString(StringUtils::format("%d", nCurWave % 10));//�����˸�λ������ʾ
	m_pCurWaveHigh->setString(StringUtils::format("%d", nCurWave / 10));//������ʮλ������ʾ
}

void CUILayer::createOver(int nCurWave, int nMaxWave,bool bWin)
{
	Director::getInstance()->pause();
	Sprite* pOver;

	//ѡ��ؿ���ť
	MenuItemImage* pQuitButton = MenuItemImage::create("", "", [=](Ref*){//�����˵���ť pQuitButton
		Director::getInstance()->resume();//�ָ�����Ϸ���������״̬
		CGameScene::deletInstance();//ɾ���˵�ǰ��Ϸ������ʵ��
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);//����Ϸ�����ʱ�����ű�������Ϊ 1.0
		Director::getInstance()->replaceScene(CGameLevel::create());//�滻��ǰ����Ϊ�ؿ�ѡ��ĳ���
	});

	//���ð�ť����ͨ״̬��ѡ��״̬�µ�ͼƬ֡��
	SpriteFrame* pQuitButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("select_normal_CN.png");
	pQuitButton->setNormalSpriteFrame(pQuitButtonNormalFrame);
	SpriteFrame* pQuitButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("select_pressed_CN.png");
	pQuitButton->setSelectedSpriteFrame(pQuitButtonSelectFrame);
	pQuitButton->setPosition(500, 140);

	if (bWin)
	{
		//�޸�Ϊ��һ��
		CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
		pLevelMgr->addLockLevel();//������һ�ؿ�
		//ʤ��
		pOver = Sprite::createWithSpriteFrameName("win_bg.png");//ʹ��ʤ��ʱ�ı���ͼƬ
		//������Ϸ��ť
		MenuItemImage* pRestartButton = MenuItemImage::create("", "", [=](Ref*){
			Director::getInstance()->resume();//�ָ���Ϸ���������״̬
			CGameScene::deletInstance();//ɾ����ǰ��Ϸ������ʵ��
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);//����Ϸ�����ʱ�����ű�������Ϊ 1.0
			pLevelMgr->addCurIndex();//���ӹؿ�����
			TransitionScene* pTran = TransitionFade::create(1.0f, CGameScene::getInstance(), Color3B::WHITE);//ʹ��TransitionFade�������
			Director::getInstance()->replaceScene(pTran);

		});
		SpriteFrame* pRestartButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("continue_normal_CN.png");
		pRestartButton->setNormalSpriteFrame(pRestartButtonNormalFrame);
		SpriteFrame* pRestartButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("continue_pressed_CN.png");
		pRestartButton->setSelectedSpriteFrame(pRestartButtonSelectFrame);
		pRestartButton->setPosition(240, 140);

		Menu* pMenu = Menu::create(pRestartButton, pQuitButton, NULL);
		pMenu->setPosition(Vec2::ZERO);
		pOver->addChild(pMenu);


	}
	else
	{
		//ʧ��
		pOver = Sprite::createWithSpriteFrameName("lose_bg.png");

		//���¿�ʼ��ť
		MenuItemImage* pRestartButton = MenuItemImage::create("", "", [=](Ref*){
			Director::getInstance()->resume();//�ָ���Ϸ���������״̬��
			CGameScene::deletInstance();// ɾ����ǰ��Ϸ������ʵ����
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);//����Ϸ�����ʱ�����ű�������Ϊ 1.0
			Director::getInstance()->replaceScene(CGameScene::getInstance());//�л�����Ϸ��������ʵ��
		});
		SpriteFrame* pRestartButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("retry_normal_CN.png");
		pRestartButton->setNormalSpriteFrame(pRestartButtonNormalFrame);
		SpriteFrame* pRestartButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("retry_pressed_CN.png");
		pRestartButton->setSelectedSpriteFrame(pRestartButtonSelectFrame);
		pRestartButton->setPosition(240, 140);
		Menu* pMenu = Menu::create(pRestartButton, pQuitButton, NULL);
		pMenu->setPosition(Vec2::ZERO);
		pOver->addChild(pMenu);
	}

	//������ʾ
	//������������ͼ
	Sprite* pWaveImg = Sprite::createWithSpriteFrameName("lose_bg_CN.png");
	pWaveImg->setPosition(385, 240);

	//��ʾ��ǰ������λ����ʮλ��
	LabelAtlas* pCurWaveLow = LabelAtlas::create(StringUtils::format("%d", nCurWave % 10), "font/numwhite-hd.png", 20, 32, '.');//��ǰ������λ
	pCurWaveLow->setPosition(390, 255);
	pOver->addChild(pCurWaveLow);

	LabelAtlas* pCurWaveHigh = LabelAtlas::create(StringUtils::format("%d", nCurWave/10), "font/numwhite-hd.png", 20, 32, '.');//��ǰ����ʮλ
	pCurWaveHigh->setPosition(345, 255);
	pOver->addChild(pCurWaveHigh);

	//��ʾ������͵�ǰ�ؿ�
	LabelAtlas* pMaxWave = LabelAtlas::create(StringUtils::format("%d", nMaxWave), "font/numwhite-hd.png", 20, 32, '.');//�����
	pMaxWave->setPosition(460, 255);
	pOver->addChild(pMaxWave);

	int nCurLevel = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"))->getCurIndex() + 1;
	//ͨ�����ݹ����� CDataMgr ��ȡ��Ϊ "LevelMgr" �����ݣ�������ת��Ϊ CLevelDtMgr ���͡�Ȼ����� getCurIndex() ������ȡ��ǰ�ؿ���������������ֵ��1��ͨ����Ϸ��������0��ʼ���õ���ǰ�ؿ�����ֵ
	LabelAtlas* pCurLevel = LabelAtlas::create(StringUtils::format("%d", nCurLevel), "font/numwhite-hd.png", 20, 32, '.');//��ǰ�ؿ�
	pCurLevel->setPosition(320, 195);
	pOver->addChild(pCurLevel);

	pOver->addChild(pWaveImg);

	//�˵�
	pOver->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	EventListenerTouchOneByOne* pLister = EventListenerTouchOneByOne::create();
	//������һ�����㴥�������� pLister�������� setSwallowTouches(true)����ʾ��û�����¼�����ֹ�����¼����´��ݵ�����������������
	pLister->setSwallowTouches(true);
	pLister->onTouchBegan = [=](Touch*, Event*){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pLister, pOver);
	this->addChild(pOver);


}

