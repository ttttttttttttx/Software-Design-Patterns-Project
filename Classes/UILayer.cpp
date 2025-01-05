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

	// Get level data manager instance from global data manager for handling level data
	CLevelDtMgr* pLevelDtMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
	
	// Get current level data based on game progress and parameters
	SLevelDt* pLevelDt = pLevelDtMgr->getCurData();

	// Get initial money from level data and store in m_nMoney for displaying current money
	m_nMoney = pLevelDt->nStartMoney;

	Sprite* pBg = Sprite::createWithSpriteFrameName("MenuBG.png");  // Menu background
	pBg->setPosition(WINSIZE.width / 2, WINSIZE.height - pBg->getContentSize().height / 2);
	this->addChild(pBg);

	m_pMoney = LabelAtlas::create(StringUtils::format("%d", m_nMoney), "font/numwhite-hd.png", 20, 32, '.');  // Money display
	m_pMoney->setPosition(100, WINSIZE.height - pBg->getContentSize().height / 2);
	this->addChild(m_pMoney);

	// Pause button
	MenuItemImage* pPause = MenuItemImage::create();
	SpriteFrame* pPauseNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause01.png");
	pPause->setNormalSpriteFrame(pPauseNormalFrame);
	SpriteFrame* pPauseSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause02.png");
	pPause->setSelectedSpriteFrame(pPauseSelectFrame);

	// Resume button
	MenuItemImage* pResum = MenuItemImage::create();
	SpriteFrame* pResumNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause11.png");
	pResum->setNormalSpriteFrame(pResumNormalFrame);
	SpriteFrame* pResumSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pause12.png");
	pResum->setSelectedSpriteFrame(pResumSelectFrame);

	m_pPauseResum = MenuItemToggle::createWithCallback([=](Ref* pRef){
		if (0 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			m_pPause->setVisible(false);// Hide pause button
			m_pWave->setVisible(true);// Show wave
			Director::getInstance()->resume();// Resume game
		}
		else if (1 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			m_pPause->setVisible(true);// Show pause button
			m_pWave->setVisible(false);// Hide wave
			Director::getInstance()->pause();// Pause game
		}
	}, pPause, pResum, NULL);
	m_pPauseResum->setPosition(800, WINSIZE.height - pBg->getContentSize().height / 2);

	// Speed button
	MenuItemImage* pSpeed = MenuItemImage::create();// pSpeed button
	SpriteFrame* pSpeedNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed11.png");// pSpeed button normal state
	pSpeed->setNormalSpriteFrame(pSpeedNormalFrame);// Set pSpeed button to normal state
	SpriteFrame* pSpeedSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed12.png");// pSpeed button selected state
	pSpeed->setSelectedSpriteFrame(pSpeedSelectFrame);// Set pSpeed button to selected state

	// Double speed button
	MenuItemImage* pTwoSpeed = MenuItemImage::create();// pTwoSpeed button
	SpriteFrame* pTwoSpeedNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed21.png");// pTwoSpeed button normal state
	pTwoSpeed->setNormalSpriteFrame(pTwoSpeedNormalFrame);// Set pTwoSpeed button to normal state
	SpriteFrame* pTwoSpeedSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("speed22.png");// pTwoSpeed button selected state
	pTwoSpeed->setSelectedSpriteFrame(pTwoSpeedSelectFrame);// Set pTwoSpeed button to selected state

	MenuItemToggle* pAddSpeed = MenuItemToggle::createWithCallback([=](Ref* pRef){
		// Click on the first button
		if (0 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			// If the first button is selected, set time scale to 1.0f for normal speed
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		}
		else if (1 == static_cast<MenuItemToggle*>(pRef)->getSelectedIndex())
		{
			// If the second button is selected, set time scale to 2.0f for double speed
			Director::getInstance()->getScheduler()->setTimeScale(2.0f);
		}
	}, pSpeed, pTwoSpeed, NULL);
	pAddSpeed->setPosition(700, WINSIZE.height - pBg->getContentSize().height / 2);// Position pAddSpeed button


	// Menu button
	MenuItemImage* pMenuButton = MenuItemImage::create("", "", [=](Ref*){// Menu button
		m_pPauseResum->setSelectedIndex(1);// Show pause
		m_pPauseResum->setEnabled(false);
		m_pMenu->setVisible(true);
		Director::getInstance()->pause();// Pause game
	});
	SpriteFrame* pMenuButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu01.png");
	pMenuButton->setNormalSpriteFrame(pMenuButtonNormalFrame);// Set pMenu button normal state
	SpriteFrame* pMenuButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu02.png");
	pMenuButton->setSelectedSpriteFrame(pMenuButtonSelectFrame);// Set pMenu button selected state
	pMenuButton->setPosition(900, WINSIZE.height - pBg->getContentSize().height / 2);

	// Create menu with pPauseResum, pAddSpeed, pMenuButton, and NULL
	Menu* pMenu = Menu::create(m_pPauseResum, pAddSpeed,pMenuButton, NULL);
	
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);

	// Menu background
	m_pMenu = Sprite::createWithSpriteFrameName("menu_bg.png");
	m_pMenu->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	m_pMenu->setVisible(false);
	this->addChild(m_pMenu,1);

	// Resume button
	MenuItemImage* pResumButton = MenuItemImage::create("", "", [=](Ref*){
		m_pPauseResum->setSelectedIndex(0);// Show resume
		m_pPauseResum->setEnabled(true);// Enable pause/resume button
		Director::getInstance()->resume();// Resume game
		m_pMenu->setVisible(false);// Hide menu
	});
	SpriteFrame* pResumButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_normal_CN.png");
	pResumButton->setNormalSpriteFrame(pResumButtonNormalFrame);// Set pResum button normal state
	SpriteFrame* pResumButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_resume_pressed_CN.png");
	pResumButton->setSelectedSpriteFrame(pResumButtonSelectFrame);// Set pResum button selected state
	pResumButton->setPosition(240,275);

	// Restart button
	MenuItemImage* pRestartButton = MenuItemImage::create("", "", [=](Ref*){// Restart button
		Director::getInstance()->resume();// Resume game
		m_pMenu->setVisible(false);// Hide menu
		CGameScene::deletInstance();// Delete game scene instance
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		Director::getInstance()->replaceScene(CGameScene::getInstance());
	});
	SpriteFrame* pRestartButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_normal_CN.png");
	pRestartButton->setNormalSpriteFrame(pRestartButtonNormalFrame);
	SpriteFrame* pRestartButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_restart_pressed_CN.png");
	pRestartButton->setSelectedSpriteFrame(pRestartButtonSelectFrame);
	pRestartButton->setPosition(240, 182);

	// Quit button
	MenuItemImage* pQuitButton = MenuItemImage::create("", "", [=](Ref*){
		Director::getInstance()->resume();// Resume game
		m_pMenu->setVisible(false);// Hide menu
		CGameScene::deletInstance();// Delete game scene instance
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);// Set game speed to normal
		Director::getInstance()->replaceScene(CGameLevel::create());// Replace current scene with GameLevel
	});
	SpriteFrame* pQuitButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_quit_normal_CN.png");
	pQuitButton->setNormalSpriteFrame(pQuitButtonNormalFrame);// Set pQuit button normal state
	SpriteFrame* pQuitButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("menu_quit_pressed_CN.png");
	pQuitButton->setSelectedSpriteFrame(pQuitButtonSelectFrame);// Set pQuit button selected state
	pQuitButton->setPosition(240, 88);

	// Create menu with pResumButton, pRestartButton, pQuitButton, and NULL
	Menu* pMenu1 = Menu::create(pResumButton, pRestartButton, pQuitButton, NULL);
	pMenu1->setPosition(Vec2::ZERO);
	// Add pMenu1 to m_pMenu
	m_pMenu->addChild(pMenu1);

	// Wave background
	m_pWave = Node::create();
	Sprite* pWave = Sprite::createWithSpriteFrameName("MenuCenter_01_CN.png");
	m_pWave->setPosition(400, WINSIZE.height - pBg->getContentSize().height / 2);
	m_pWave->addChild(pWave);

	// m_pMaxWave shows total waves using LabelAtlas
	m_pMaxWave = LabelAtlas::create(StringUtils::format("%d", pLevelDt->vecWave.size()), "font/numwhite-hd.png", 20, 32, '.');// Total waves
	m_pWave->addChild(m_pMaxWave);


	/*
	1"  Need to display initial string
	"font/numwhite-hd.png" is the path to the font image
	20 is the width of each character
	32 is the height of each character
	'.' is the space character
	*/
	// m_pCurWaveLow shows current wave in the format of 1.0f
	m_pCurWaveLow = LabelAtlas::create("1", "font/numwhite-hd.png", 20, 32, '.');// Current wave
	m_pCurWaveLow->setPosition(-65, 0);
	m_pWave->addChild(m_pCurWaveLow);

	m_pCurWaveHigh = LabelAtlas::create("0", "font/numwhite-hd.png", 20, 32, '.');// Current wave
	m_pCurWaveHigh->setPosition(-113, 0);
	m_pWave->addChild(m_pCurWaveHigh);
	this->addChild(m_pWave);

	// Pause button
	m_pPause = Sprite::createWithSpriteFrameName("MenuCenter_02_CN.png");
	m_pPause->setPosition(400, WINSIZE.height - pBg->getContentSize().height / 2);
	m_pPause->setVisible(false);
	this->addChild(m_pPause);

	// Time
	m_nTime = 1;// Start time
	m_pTime = Node::create();// Create a new node m_pTime
	m_pTime->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	Sprite* pCountDownBg = Sprite::createWithSpriteFrameName("countdown_11.png");// Countdown background
	m_pCountDown = Sprite::createWithSpriteFrameName("countdown_12.png");// Countdown image
	m_pTimeNumber = Sprite::createWithSpriteFrameName("countdown_01.png");// Time number
	m_pTime->addChild(pCountDownBg);
	m_pTime->addChild(m_pCountDown);
	m_pTime->addChild(m_pTimeNumber);

	
	CallFunc* pCall = CallFunc::create([&]() {// Create a new function
		if (m_nTime < 3) {
			m_nTime++;
			std::string frameName = StringUtils::format("countdown_%02d.png", m_nTime);
			// Create a new sprite frame for the current time
			m_pTimeNumber->setSpriteFrame(frameName);
		}
	});

	RotateBy* pRotateBy = RotateBy::create(1.0f, -360);// Rotate by 360 degrees
	// Create a sequence with pRotateBy and pCall
	Sequence* pSeq = Sequence::createWithTwoActions(pRotateBy, pCall);

	// go
	CallFunc* pCallGo = CallFunc::create([&]() {
		m_pCountDown->stopAllActions(); // Stop all actions on m_pCountDown
		m_pCountDown->removeFromParent();// Remove m_pCountDown from parent

		m_pTimeNumber->setScale(1.0f); // Reset original scale
		m_pTimeNumber->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("countdown_13.png"));

		// ScaleTo and FadeOut actions
		ScaleTo* pScaleTo = ScaleTo::create(1.0f, 0.2f);
		FadeOut* fadeOut = FadeOut::create(1.0f);

		// CallFunc to stop all actions on m_pTimeNumber and remove m_pTime
		CallFunc* pCallEnd = CallFunc::create([&]() {
			m_pTimeNumber->stopAllActions();
			m_pTime->removeFromParent();
			CGameScene::getInstance()->getMonsterLayer()->startCreatMonster();
			});

		// Create a sequence with pScaleTo, fadeOut, pCallEnd, and NULL
		Sequence* pSeq = Sequence::create(pScaleTo, fadeOut, pCallEnd, nullptr);
		m_pTimeNumber->runAction(pSeq);
		});

	// Run the sequence
	m_pCountDown->runAction(Sequence::createWithTwoActions(Repeat::create(pSeq, 3), pCallGo));
	// Repeat the sequence 3 times, then call pCallGo

	this->addChild(m_pTime);

	return true;
}

// Add money
void CUILayer::addMoney(int nMoney)
{
	m_nMoney += nMoney;

	// Update money display
	m_pMoney->setString(StringUtils::format("%d", m_nMoney));
}

// Set current wave
void CUILayer::setCurWave(int nCurWave)// nCurWave is the current wave value
{
	m_pCurWaveLow->setString(StringUtils::format("%d", nCurWave % 10));// Display current wave
	m_pCurWaveHigh->setString(StringUtils::format("%d", nCurWave / 10));// Display current wave
}

void CUILayer::createOver(int nCurWave, int nMaxWave,bool bWin)
{
	Director::getInstance()->pause();
	Sprite* pOver;

	// Quit button
	MenuItemImage* pQuitButton = MenuItemImage::create("", "", [=](Ref*){// Quit button
		Director::getInstance()->resume();// Resume game
		CGameScene::deletInstance();// Delete current game scene instance
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);// Set game speed to normal
		Director::getInstance()->replaceScene(CGameLevel::create());// Replace current scene with GameLevel
	});

	// Quit button
	SpriteFrame* pQuitButtonNormalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("select_normal_CN.png");
	pQuitButton->setNormalSpriteFrame(pQuitButtonNormalFrame);
	SpriteFrame* pQuitButtonSelectFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("select_pressed_CN.png");
	pQuitButton->setSelectedSpriteFrame(pQuitButtonSelectFrame);
	pQuitButton->setPosition(500, 140);

	if (bWin)
	{
		// Add a new level
		CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
		pLevelMgr->addLockLevel();// Add a new level
		// Win
		pOver = Sprite::createWithSpriteFrameName("win_bg.png");// Use win image
		// Restart button
		MenuItemImage* pRestartButton = MenuItemImage::create("", "", [=](Ref*){
			Director::getInstance()->resume();// Resume game
			CGameScene::deletInstance();// Delete current game scene instance
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);// Set game speed to normal
			pLevelMgr->addCurIndex();// Add current level
			TransitionScene* pTran = TransitionFade::create(1.0f, CGameScene::getInstance(), Color3B::WHITE);// Use TransitionFade
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
		// Lose
		pOver = Sprite::createWithSpriteFrameName("lose_bg.png");

		// Restart button
		MenuItemImage* pRestartButton = MenuItemImage::create("", "", [=](Ref*){
			Director::getInstance()->resume();// Resume game
			CGameScene::deletInstance();// Delete current game scene instance
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);// Set game speed to normal
			Director::getInstance()->replaceScene(CGameScene::getInstance());// Delete current game scene instance
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

	// Display wave image
	Sprite* pWaveImg = Sprite::createWithSpriteFrameName("lose_bg_CN.png");
	pWaveImg->setPosition(385, 240);

	// Display current wave
	LabelAtlas* pCurWaveLow = LabelAtlas::create(StringUtils::format("%d", nCurWave % 10), "font/numwhite-hd.png", 20, 32, '.');// Current wave
	pCurWaveLow->setPosition(390, 255);
	pOver->addChild(pCurWaveLow);

	LabelAtlas* pCurWaveHigh = LabelAtlas::create(StringUtils::format("%d", nCurWave/10), "font/numwhite-hd.png", 20, 32, '.');// Current wave
	pCurWaveHigh->setPosition(345, 255);
	pOver->addChild(pCurWaveHigh);

	// Display total waves
	LabelAtlas* pMaxWave = LabelAtlas::create(StringUtils::format("%d", nMaxWave), "font/numwhite-hd.png", 20, 32, '.');// Total waves
	pMaxWave->setPosition(460, 255);
	pOver->addChild(pMaxWave);

	int nCurLevel = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"))->getCurIndex() + 1;
	// Get current level from CDataMgr
	LabelAtlas* pCurLevel = LabelAtlas::create(StringUtils::format("%d", nCurLevel), "font/numwhite-hd.png", 20, 32, '.');// Current level
	pCurLevel->setPosition(320, 195);
	pOver->addChild(pCurLevel);

	pOver->addChild(pWaveImg);

	// Menu background
	pOver->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	EventListenerTouchOneByOne* pLister = EventListenerTouchOneByOne::create();
	// Set pLister to swallow touches
	pLister->setSwallowTouches(true);
	pLister->onTouchBegan = [=](Touch*, Event*){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pLister, pOver);
	this->addChild(pOver);


}

