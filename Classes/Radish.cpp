#include "Radish.h"
#include "GameScene.h"

// 萝卜类的构造函数
CRadish::CRadish()
{
}

// 萝卜类的析构函数
CRadish::~CRadish()
{
}

// 萝卜类的初始化方法
bool CRadish::init()
{
	// 判断基类Node的初始化是否成功
	if (!Node::init())
	{
		return false;
	}
	this->addTouch();
	// 初始生命值设置为10
	m_nHP = 10;

	// 创建生命值显示的精灵，使用格式化字符串获取对应生命值的图片帧名称
	m_pHp = Sprite::createWithSpriteFrameName(StringUtils::format("BossHP%02d.png", m_nHP));

	// 创建萝卜的模型精灵，使用格式化字符串获取对应生命值的图片帧名称
	m_pModel = Sprite::createWithSpriteFrameName(StringUtils::format("hlb%d.png", m_nHP));
	myup= Sprite::createWithSpriteFrameName(StringUtils::format("arrow.png"));
	myup->setPosition(25,-50);

	// 设置生命值显示的位置
	m_pHp->setPosition(80, 0);

	// 将生命值显示和萝卜模型添加到萝卜节点中
	this->addChild(m_pHp);
	this->addChild(m_pModel);
	this->addChild(myup);

	// 定时器，每3秒执行一次回调函数
	this->schedule([=](float) {
		// 如果生命值小于10，停止定时器
		if (m_nHP < 10)
		{
			this->unschedule("Move");
			return;
		}

		// 获取萝卜生长的动画
		Animate* pAnimate = CGameScene::getInstance()->getMyAnimate()->getAnimate(3023);
		pAnimate->getAnimation()->setRestoreOriginalFrame(true);

		// 停止所有动作，设置模型精灵为生长后的状态，并执行动画
		m_pModel->stopAllActions();
		m_pModel->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hlb10.png"));
		m_pModel->runAction(pAnimate);
		}, 3.0f, "Move");

	// 初始化成功
	return true;
}

// 萝卜受到伤害的方法，返回值表示是否萝卜存活
bool CRadish::Damage(int damage)
{
	m_nHP -= damage;

	// 如果生命值小于等于0，游戏结束
	if (m_nHP <= 0)
	{
		CGameScene::getInstance()->getMonsterLayer()->GameOver();
		return true;
	}

	// 停止模型精灵的所有动作
	m_pModel->stopAllActions();

	// 更新生命值显示的精灵
	std::string strName = StringUtils::format("BossHP%02d.png", m_nHP);
	SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	m_pHp->setSpriteFrame(pFrame);

	// 更新萝卜模型的精灵
	strName = StringUtils::format("hlb%d.png", m_nHP == 7 || m_nHP == 5 ? m_nHP + 1 : m_nHP);
	pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(strName);
	m_pModel->setSpriteFrame(pFrame);

	return false;
}

void CRadish::addTouch(){
	EventListenerTouchOneByOne* pListener = EventListenerTouchOneByOne::create();

	pListener->onTouchBegan = [](Touch* pTouch, Event*) {
		//处理萝卜对象的点击事件
		CGameScene::getInstance()->getRadish()->clickEvent(pTouch->getLocation());

		return true;
	};
	//将事件监听器添加到当前场景节点。
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}

// 处理点击事件的方法，clickPos表示点击的位置
void CRadish::clickEvent(Vec2 clickPos)
{
	// 如果生命值大于等于10，并且点击位置在模型精灵的边界框内
	if (m_nHP >= 10 && m_pModel->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		// 获取萝卜生长的动画
		Animate* pAnimate = CGameScene::getInstance()->getMyAnimate()->getAnimate(3022);
		pAnimate->getAnimation()->setRestoreOriginalFrame(true);

		// 停止所有动作，设置模型精灵为生长后的状态，并执行动画
		m_pModel->stopAllActions();
		m_pModel->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hlb10.png"));
		m_pModel->runAction(pAnimate);
	}
	if (m_nHP < 10 && myup->getBoundingBox().containsPoint(this->convertToNodeSpace(clickPos)))
	{
		// 增添特殊技能逻辑
		CGameScene::getInstance()->getMonsterLayer()->applySpecialSkill();
		
	}
}

