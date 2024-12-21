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

	//从全局的数据管理器中获取关卡数据管理器的实例。这个管理器用于处理关卡数据的存取。
	CLevelDtMgr* pLevelDtMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
	
	//获取当前关卡的数据，可以根据当前游戏进度或者设置的参数来获取关卡的数据，如关卡的初始金币数
	SLevelDt* pLevelDt = pLevelDtMgr->getCurData();

	//从关卡数据中获取初始金币数，并存储在 m_nMoney 变量中。这个值会用于在界面中显示当前拥有的金币数量
	m_nMoney = pLevelDt->nStartMoney;

	Sprite* pBg = Sprite::createWithSpriteFrameName("MenuBG.png"); //菜单背景
	pBg->setPosition(WINSIZE.width / 2, WINSIZE.height - pBg->getContentSize().height / 2);
	this->addChild(pBg);

	m_pMoney = LabelAtlas::create(StringUtils::format("%d", m_nMoney), "font/numwhite-hd.png", 20, 32, '.');//金币
	m_pMoney->setPosition(100, WINSIZE.height - pBg->getContentSize().height / 2);
	this->addChild(m_pMoney);

	//暂停按钮
	MenuItemImage* pPause = MenuItemImage::create();
	SpriteFrame* pPauseNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause01.png");
	pPause->setNormalSpriteFrame(pPauseNormalFrame);
	SpriteFrame* pPauseSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause02.png");
	pPause->setSelectedSpriteFrame(pPauseSelectFrame);

	//恢复按钮
	MenuItemImage* pResum = MenuItemImage::create();
	SpriteFrame* pResumNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause11.png");
	pResum->setNormalSpriteFrame(pResumNormalFrame);
	SpriteFrame* pResumSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause12.png");
	pResum->setSelectedSpriteFrame(pResumSelectFrame);

	m_pPauseResum = MenuItemToggle::createWithCallback([=](Ref* pRef){
		if (0 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			m_pPause->setVisible(false);//将暂停按钮隐藏
			m_pWave->setVisible(true);//显示波数相关内容
			Director::getInstance()->resume();//恢复游戏场景的运行
		}
		else if (1 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			m_pPause->setVisible(true);//显示暂停按钮
			m_pWave->setVisible(false);//隐藏波数相关内容
			Director::getInstance()->pause();//暂停游戏场景的运行
		}
	}, pPause, pResum, NULL);
	m_pPauseResum->setPosition(800, WINSIZE.height - pBg->getContentSize().height / 2);

	//正常速度按钮
	MenuItemImage* pSpeed = MenuItemImage::create();//创建空的菜单项pSpeed
	SpriteFrame* pSpeedNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed11.png");//代表菜单项的正常状态外观
	pSpeed->setNormalSpriteFrame(pSpeedNormalFrame);//设置为菜单项 pSpeed 的正常状态的外观
	SpriteFrame* pSpeedSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed12.png");//代表菜单项被选中时的外观
	pSpeed->setSelectedSpriteFrame(pSpeedSelectFrame);//设置为菜单项 pSpeed 被选中时的外观

	//加速按钮
	MenuItemImage* pTwoSpeed = MenuItemImage::create();//创建了空的菜单项 pTwoSpeed
	SpriteFrame* pTwoSpeedNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed21.png");//代表菜单项的正常状态外观
	pTwoSpeed->setNormalSpriteFrame(pTwoSpeedNormalFrame);//设置为菜单项 pTwoSpeed 的正常状态的外观
	SpriteFrame* pTwoSpeedSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed22.png");//代表菜单项被选中时的外观
	pTwoSpeed->setSelectedSpriteFrame(pTwoSpeedSelectFrame);//设置为菜单项 pTwoSpeed 被选中时的外观

	MenuItemToggle* pAddSpeed = MenuItemToggle::createWithCallback([=](Ref* pRef){
		//创建了一个切换按钮组
		if (0 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			//如果索引为 0，表示第一个按钮 pSpeed 被选中，将时间缩放设置为 1.0f，即正常速度
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		}
		else if (1 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			//如果索引为 1，表示第二个按钮 pTwoSpeed 被选中，将时间缩放设置为 2.0f，即两倍速度加快
			Director::getInstance()->getScheduler()->setTimeScale(2.0f);
		}
	}, pSpeed, pTwoSpeed, NULL);
	pAddSpeed->setPosition(700, WINSIZE.height - pBg->getContentSize().height / 2);// 置了按钮组 pAddSpeed 的位置


	//菜单按钮
	MenuItemImage* pMenuButton = MenuItemImage::create("", "", [=](Ref*){//创建一个图像菜单项
		m_pPauseResum->setSelectedIndex(1);//显示暂停键
		m_pPauseResum->setEnabled(false);
		m_pMenu->setVisible(true);
		Director::getInstance()->pause();//暂停了游戏引擎的运行
	});
	SpriteFrame* pMenuButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu01.png");
	pMenuButton->setNormalSpriteFrame(pMenuButtonNormalFrame);//设置菜单项按钮的正常状态下显示的精灵帧
	SpriteFrame* pMenuButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu02.png");
	pMenuButton->setSelectedSpriteFrame(pMenuButtonSelectFrame);//设置菜单项按钮的选中状态下显示的精灵帧
	pMenuButton->setPosition(900, WINSIZE.height - pBg->getContentSize().height / 2);

	//使用 Menu::create() 创建一个菜单，包括之前创建的其他菜单项和菜单项按钮
	Menu* pMenu = Menu::create(m_pPauseResum, pAddSpeed,pMenuButton, NULL);
	
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);

	//菜单
	m_pMenu = Sprite::createWithSpriteFrameName("menu_bg.png");
	m_pMenu->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	m_pMenu->setVisible(false);
	this->addChild(m_pMenu,1);

	//继续游戏按钮
	MenuItemImage* pResumButton = MenuItemImage::create("", "", [=](Ref*){
		m_pPauseResum->setSelectedIndex(0);//显示继续键
		m_pPauseResum->setEnabled(true);//启用暂停/恢复按钮
		Director::getInstance()->resume();//恢复游戏引擎的运行
		m_pMenu->setVisible(false);//菜单不可见
	});
	SpriteFrame* pResumButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_normal_CN.png");
	pResumButton->setNormalSpriteFrame(pResumButtonNormalFrame);// 设置菜单项按钮的正常状态下显示的精灵帧
	SpriteFrame* pResumButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_pressed_CN.png");
	pResumButton->setSelectedSpriteFrame(pResumButtonSelectFrame);//设置菜单项按钮的选中状态下显示的精灵帧
	pResumButton->setPosition(240,275);

	//重新开始游戏按钮
	MenuItemImage* pRestartButton = MenuItemImage::create("", "", [=](Ref*){//创建了一个菜单按钮项 pRestartButton
		Director::getInstance()->resume();//恢复游戏引擎的运行。
		m_pMenu->setVisible(false);//将 m_pMenu 菜单设置为不可见状态
		CGameScene::deletInstance();//删除游戏场景的单例实例，释放已经加载的游戏场景，以便重新开始游戏。
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		Director::getInstance()->replaceScene(CGameScene::getInstance());
	});
	SpriteFrame* pRestartButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_normal_CN.png");
	pRestartButton->setNormalSpriteFrame(pRestartButtonNormalFrame);
	SpriteFrame* pRestartButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_pressed_CN.png");
	pRestartButton->setSelectedSpriteFrame(pRestartButtonSelectFrame);
	pRestartButton->setPosition(240, 182);

	//选择关卡按钮
	MenuItemImage* pQuitButton = MenuItemImage::create("", "", [=](Ref*){
		Director::getInstance()->resume();//恢复游戏引擎的运行
		m_pMenu->setVisible(false);// 将之前创建的菜单设为不可见状态
		CGameScene::deletInstance();//删除游戏场景
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);//设置游戏时间缩放为正常速度
		Director::getInstance()->replaceScene(CGameLevel::create());//替换当前场景为新创建的游戏关卡场景
	});
	SpriteFrame* pQuitButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_quit_normal_CN.png");
	pQuitButton->setNormalSpriteFrame(pQuitButtonNormalFrame);//设置菜单项按钮的正常状态下显示的精灵帧
	SpriteFrame* pQuitButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_quit_pressed_CN.png");
	pQuitButton->setSelectedSpriteFrame(pQuitButtonSelectFrame);//设置菜单项按钮的选中状态下显示的精灵帧
	pQuitButton->setPosition(240, 88);

	//创建一个新的菜单（pMenu1），并将“继续游戏”按钮、“重新开始游戏”按钮以及“选择关卡”按钮添加到其中
	Menu* pMenu1 = Menu::create(pResumButton, pRestartButton, pQuitButton, NULL);
	pMenu1->setPosition(Vec2::ZERO);
	//将这个新创建的菜单作为子菜单添加到之前创建的主菜单 m_pMenu 中
	m_pMenu->addChild(pMenu1);

	//显示游戏波数界面
	m_pWave = Node::create();
	Sprite* pWave = Sprite::createWithSpriteFrameName("MenuCenter_01_CN.png");
	m_pWave->setPosition(400, WINSIZE.height - pBg->getContentSize().height / 2);
	m_pWave->addChild(pWave);

	//m_pMaxWave 显示总波数，使用 LabelAtlas 来显示数字
	m_pMaxWave = LabelAtlas::create(StringUtils::format("%d", pLevelDt->vecWave.size()), "font/numwhite-hd.png", 20, 32, '.');//总波数
	m_pWave->addChild(m_pMaxWave);


	/*
	1" 是要显示的初始字符。
	"font/numwhite-hd.png" 是位图字体纹理集的文件路径
	20 是每个字符的宽度（以像素为单位）
	32 是每个字符的高度（以像素为单位）
	'.' 是用于间隔字符的空白字符
	*/
	//m_pCurWaveLow 显示当前波数个位数字，放置在个位数字的位置
	m_pCurWaveLow = LabelAtlas::create("1", "font/numwhite-hd.png", 20, 32, '.');//当前波数个位
	m_pCurWaveLow->setPosition(-65, 0);
	m_pWave->addChild(m_pCurWaveLow);

	m_pCurWaveHigh = LabelAtlas::create("0", "font/numwhite-hd.png", 20, 32, '.');//当前波数十位
	m_pCurWaveHigh->setPosition(-113, 0);
	m_pWave->addChild(m_pCurWaveHigh);
	this->addChild(m_pWave);

	//暂停中状态
	m_pPause = Sprite::createWithSpriteFrameName("MenuCenter_02_CN.png");
	m_pPause->setPosition(400, WINSIZE.height - pBg->getContentSize().height / 2);
	m_pPause->setVisible(false);
	this->addChild(m_pPause);

	//倒计时
	m_nTime = 1;//初始化倒计时变量
	m_pTime = Node::create();//创建了一个节点 m_pTime，用于存放倒计时所需的精灵元素
	m_pTime->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	Sprite* pCountDownBg = Sprite::createWithSpriteFrameName("countdown_11.png");//创建背景图
	m_pCountDown = Sprite::createWithSpriteFrameName("countdown_12.png");//创建旋转图
	m_pTimeNumber = Sprite::createWithSpriteFrameName("countdown_01.png");//创建数字
	m_pTime->addChild(pCountDownBg);
	m_pTime->addChild(m_pCountDown);
	m_pTime->addChild(m_pTimeNumber);

	
	CallFunc* pCall = CallFunc::create([&]() {//创建一个回调动作
		if (m_nTime < 3) {
			m_nTime++;
			std::string frameName = StringUtils::format("countdown_%02d.png", m_nTime);
			//根据新的倒计时时间生成一个新的图片帧名称。
			m_pTimeNumber->setSpriteFrame(frameName);
		}
	});

	RotateBy* pRotateBy = RotateBy::create(1.0f, -360);//旋转动作
	//创建了一个旋转动作 pRotateBy，在1秒内将旋转精灵 -360 度
	Sequence* pSeq = Sequence::createWithTwoActions(pRotateBy, pCall);

	// go动作设计
	CallFunc* pCallGo = CallFunc::create([&]() {
		m_pCountDown->stopAllActions(); //停止了倒计时旋转图 m_pCountDown 上正在执行的动作
		m_pCountDown->removeFromParent();//将倒计时旋转图从其父节点中移除

		m_pTimeNumber->setScale(1.0f); // 恢复原始大小
		m_pTimeNumber->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("countdown_13.png"));

		// 缩放和淡出动作
		ScaleTo* pScaleTo = ScaleTo::create(1.0f, 0.2f);
		FadeOut* fadeOut = FadeOut::create(1.0f);

		// 移除节点并开始怪物层的创建
		CallFunc* pCallEnd = CallFunc::create([&]() {
			m_pTimeNumber->stopAllActions();
			m_pTime->removeFromParent();
			CGameScene::getInstance()->getMonsterLayer()->startCreatMonster();
			});

		// 序列动作：先缩小再淡出，完成后移除节点
		Sequence* pSeq = Sequence::create(pScaleTo, fadeOut, pCallEnd, nullptr);
		m_pTimeNumber->runAction(pSeq);
		});

	// 执行动作序列
	m_pCountDown->runAction(Sequence::createWithTwoActions(Repeat::create(pSeq, 3), pCallGo));
	//这个序列由两个动作组成：使用 Repeat 动作来重复执行序列 pSeq 三次，然后执行回调动作 pCallGo

	this->addChild(m_pTime);

	return true;
}

//增加玩家的金币数量
void CUILayer::addMoney(int nMoney)
{
	m_nMoney += nMoney;

	//将最新的金币数量以字符串格式显示在界面上
	m_pMoney->setString(StringUtils::format("%d", m_nMoney));
}

//设置当前波数的显示
void CUILayer::setCurWave(int nCurWave)//nCurWave 是传入的当前波数值
{
	m_pCurWaveLow->setString(StringUtils::format("%d", nCurWave % 10));//更新了个位数的显示
	m_pCurWaveHigh->setString(StringUtils::format("%d", nCurWave / 10));//更新了十位数的显示
}

void CUILayer::createOver(int nCurWave, int nMaxWave,bool bWin)
{
	Director::getInstance()->pause();
	Sprite* pOver;

	//选择关卡按钮
	MenuItemImage* pQuitButton = MenuItemImage::create("", "", [=](Ref*){//创建菜单按钮 pQuitButton
		Director::getInstance()->resume();//恢复了游戏引擎的运行状态
		CGameScene::deletInstance();//删除了当前游戏场景的实例
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);//将游戏引擎的时间缩放比例设置为 1.0
		Director::getInstance()->replaceScene(CGameLevel::create());//替换当前场景为关卡选择的场景
	});

	//设置按钮的普通状态和选中状态下的图片帧。
	SpriteFrame* pQuitButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("select_normal_CN.png");
	pQuitButton->setNormalSpriteFrame(pQuitButtonNormalFrame);
	SpriteFrame* pQuitButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("select_pressed_CN.png");
	pQuitButton->setSelectedSpriteFrame(pQuitButtonSelectFrame);
	pQuitButton->setPosition(500, 140);

	if (bWin)
	{
		//修改为下一关
		CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
		pLevelMgr->addLockLevel();//解锁下一关卡
		//胜利
		pOver = Sprite::createWithSpriteFrameName("win_bg.png");//使用胜利时的背景图片
		//继续游戏按钮
		MenuItemImage* pRestartButton = MenuItemImage::create("", "", [=](Ref*){
			Director::getInstance()->resume();//恢复游戏引擎的运行状态
			CGameScene::deletInstance();//删除当前游戏场景的实例
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);//将游戏引擎的时间缩放比例设置为 1.0
			pLevelMgr->addCurIndex();//增加关卡索引
			TransitionScene* pTran = TransitionFade::create(1.0f, CGameScene::getInstance(), Color3B::WHITE);//使用TransitionFade渐变过渡
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
		//失败
		pOver = Sprite::createWithSpriteFrameName("lose_bg.png");

		//重新开始按钮
		MenuItemImage* pRestartButton = MenuItemImage::create("", "", [=](Ref*){
			Director::getInstance()->resume();//恢复游戏引擎的运行状态。
			CGameScene::deletInstance();// 删除当前游戏场景的实例。
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);//将游戏引擎的时间缩放比例设置为 1.0
			Director::getInstance()->replaceScene(CGameScene::getInstance());//切换到游戏场景的新实例
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

	//波数显示
	//创建波数背景图
	Sprite* pWaveImg = Sprite::createWithSpriteFrameName("lose_bg_CN.png");
	pWaveImg->setPosition(385, 240);

	//显示当前波数个位数和十位数
	LabelAtlas* pCurWaveLow = LabelAtlas::create(StringUtils::format("%d", nCurWave % 10), "font/numwhite-hd.png", 20, 32, '.');//当前波数个位
	pCurWaveLow->setPosition(390, 255);
	pOver->addChild(pCurWaveLow);

	LabelAtlas* pCurWaveHigh = LabelAtlas::create(StringUtils::format("%d", nCurWave/10), "font/numwhite-hd.png", 20, 32, '.');//当前波数十位
	pCurWaveHigh->setPosition(345, 255);
	pOver->addChild(pCurWaveHigh);

	//显示最大波数和当前关卡
	LabelAtlas* pMaxWave = LabelAtlas::create(StringUtils::format("%d", nMaxWave), "font/numwhite-hd.png", 20, 32, '.');//最大波数
	pMaxWave->setPosition(460, 255);
	pOver->addChild(pMaxWave);

	int nCurLevel = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"))->getCurIndex() + 1;
	//通过数据管理器 CDataMgr 获取名为 "LevelMgr" 的数据，并将其转换为 CLevelDtMgr 类型。然后调用 getCurIndex() 方法获取当前关卡索引，并将索引值加1（通常游戏中索引从0开始）得到当前关卡的数值
	LabelAtlas* pCurLevel = LabelAtlas::create(StringUtils::format("%d", nCurLevel), "font/numwhite-hd.png", 20, 32, '.');//当前关卡
	pCurLevel->setPosition(320, 195);
	pOver->addChild(pCurLevel);

	pOver->addChild(pWaveImg);

	//菜单
	pOver->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	EventListenerTouchOneByOne* pLister = EventListenerTouchOneByOne::create();
	//创建了一个单点触摸监听器 pLister。设置了 setSwallowTouches(true)，表示吞没触摸事件，防止触摸事件向下传递到其他触摸监听器。
	pLister->setSwallowTouches(true);
	pLister->onTouchBegan = [=](Touch*, Event*){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pLister, pOver);
	this->addChild(pOver);


}

