#include "GameLevel.h"
#include "DataMgr.h"
#include "DataClass.h"
#include "GameScene.h"
CGameLevel::CGameLevel()
{

}

CGameLevel::~CGameLevel()
{

}

bool CGameLevel::init()
{
	if (!Scene::init())
	{
		return false;
	}//基类Scene的初始化，若初始化失败则返回false。


	Node* pMenuNode = CSLoader::createNode("GameLevel.csb");//创建一个场景节点，加载游戏关卡的UI界面。
	this->addChild(pMenuNode);

	m_pStartButton = static_cast<ui::Button*>(pMenuNode->getChildByName("LevelStartButton"));
	//从UI界面中获取名为 "LevelStartButton" 的按钮，并将其赋值给 m_pStartButton 变量。

	//为 m_pStartButton 按钮添加了一个触摸事件的监听器。当按钮被点击时触发该事件处理函数。
	m_pStartButton->addTouchEventListener([=](Ref*, Widget::TouchEventType){
		CLevelDtMgr* pLevelMgr=static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
		pLevelMgr->setCurIndex(m_pPageView->getCurPageIndex());
		CGameScene::deletInstance();
		Director::getInstance()->replaceScene(CGameScene::getInstance());//切换到游戏主场景
	//进入游戏主场景
	});

	this->addPageView();
	this->createButton();
	return true;
}


//创建了一个可以滑动翻页的 PageView，每一页是一个布局，包含了关卡图片和关卡卡片，方便玩家选择关卡
void CGameLevel::addPageView()
{
	CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
	//从数据管理器中获取关卡数据管理器的实例 pLevelMgr，用于获取关卡信息。
	
	m_pPageView = PageView::create();//创建了一个 PageView 对象 m_pPageView，用于展示多个关卡。
	
	Sprite* pSpriteTemp = Sprite::createWithSpriteFrameName(static_cast<SLevelDt*>(pLevelMgr->getDataByID(1001))->strViewImg);
	//创建一个临时精灵 pSpriteTemp，用于获取一个关卡的样式图。
	
	m_pPageView->setContentSize(WINSIZE);//设置 PageView 的大小为屏幕尺寸 WINSIZE，确保适配屏幕
	for (int i = 0; i < pLevelMgr->getsize();i++)//添加Layout
	{
		Layout* pLayout = Layout::create();//创建一个 Layout 对象 pLayout 作为关卡的容器。

		Sprite* pSprite=Sprite::createWithSpriteFrameName(static_cast<SLevelDt*>(pLevelMgr->getDataByID(1001 + i))->strViewImg);
		Sprite* pSpriteCard = Sprite::createWithSpriteFrameName(static_cast<SLevelDt*>(pLevelMgr->getDataByID(1001 + i))->strCardView);
        //创建两个精灵 pSprite 和 pSpriteCard，分别用于展示关卡图片和关卡卡片。

		pLayout->addChild(pSprite);
		pLayout->addChild(pSpriteCard);
		pSprite->setPosition(WINSIZE.width/2,WINSIZE.height/2+50);
		pSpriteCard->setPosition(WINSIZE.width / 2, WINSIZE.height / 2-pSprite->getContentSize().height/2);
		//将这两个精灵添加到 pLayout 布局中，并设置它们的位置
		
		m_pPageView->addPage(pLayout);
		//将 pLayout 添加为一个页面，并将该页面添加到 PageView 中
	}

	m_pPageView->setTouchEnabled(false);
	//禁用 PageView 的触摸事件，在这个场景下只允许通过按钮进行页面切换

	this->addChild(m_pPageView);
	//将创建好的 PageView 添加到当前场景中显示。
}


//创建了左右翻页按钮，根据用户在关卡选择界面中翻页的行为，控制按钮的显示与隐藏
//根据当前页面索引和关卡锁定等级来控制开始按钮的启用与禁用
void CGameLevel::createButton()
{
	//创建左翻页按钮
	m_pLeftButton = MenuItemImage::create("Level/LeftNormal.png"/*正常状态*/, "Level/LeftSelect.png"/*选中状态*/, [=](Ref*) {
		m_pRightButton->setVisible(true);//设置右翻页按钮可见。

		// 将 PageView 的当前页面索引减去1，实现向左翻页效果
		m_pPageView->setCurrentPageIndex(m_pPageView->getCurPageIndex() - 1);
		
		//如果当前页面索引小于等于0，则隐藏左翻页按钮（第一页无左按钮）
		if (m_pPageView->getCurPageIndex()<=0)
		{
			m_pLeftButton->setVisible(false);
		}

		//获取关卡数据管理器的实例 pLevelMgr，并根据当前页面索引与关卡锁定等级设置开始按钮的状态
		CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));//拿到关卡数据管理者
		
		//m_pPageView->getCurPageIndex() 获取当前所选页面的索引
		//pLevelMgr->getLockLevel() 获取关卡管理器中的关卡锁定等级
		if (m_pPageView->getCurPageIndex()<pLevelMgr->getLockLevel()) {
			//如果用户所选的页面索引小于关卡锁定等级，即用户选择的关卡未达到被锁定的等级
			m_pStartButton->setEnabled(true);//启用开始按钮
		}
	});
	m_pLeftButton->setVisible(false);//设置左翻页按钮初始时不可见


	//创建右翻页按钮
	m_pRightButton = MenuItemImage::create("Level/RightNormal.png", "Level/RightSelect.png", [=](Ref*){
		m_pLeftButton->setVisible(true);//设置左翻页按钮可见。

		//将 PageView 的当前页面索引加上1，实现向右翻页效果。
		m_pPageView->setCurrentPageIndex(m_pPageView->getCurPageIndex() + 1);

		int nCount = m_pPageView->getChildrenCount();//获取 PageView 中的子项数量。
		
		// 如果当前页面索引大于等于子项数量减1，则隐藏右翻页按钮（最后一页无右按钮）
		if (m_pPageView->getCurPageIndex() >= nCount-1)
		{
			m_pRightButton->setVisible(false);
		}

		//获取关卡数据管理器的实例 pLevelMgr，并根据当前页面索引与关卡锁定等级设置开始按钮的状态
		CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));//拿到关卡数据管理者
		
		if (m_pPageView->getCurPageIndex() >= pLevelMgr->getLockLevel()){
			//如果用户所选的页面索引大于或等于关卡锁定等级，即用户选择的关卡达到或超过了被锁定的等级
			m_pStartButton->setEnabled(false);//禁用开始按钮
		}/*111*/
	});

	
	m_pLeftButton->setPosition(80, WINSIZE.height / 2);//左翻页按钮距离屏幕左边界 80 个单位，垂直居中于屏幕高度
	m_pRightButton->setPosition(WINSIZE.width - 80, WINSIZE.height / 2);//表示右翻页按钮距离屏幕右边界80个单位，垂直居中于屏幕高度
	Menu* pMenu = Menu::create(m_pLeftButton, m_pRightButton, NULL);//创建了一个菜单对象pMenu，并将左右翻页按钮添加进去
	pMenu->setPosition(Vec2::ZERO);//Vec2::ZERO 表示屏幕的左下角（0, 0）作为菜单的起始位置
	this->addChild(pMenu);//将菜单添加到当前的场景中，以便显示
}
