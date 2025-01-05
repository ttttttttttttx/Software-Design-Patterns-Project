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

class CRadishState
{
public:
	virtual ~CRadishState() {}
	virtual void damage(CRadish* radish, int damage) = 0;
	virtual void clickEvent(CRadish* radish, Vec2 clickPos) = 0;
	virtual void update(CRadish* radish) = 0;
};

class HealthyState : public CRadishState
{
public:
	void damage(CRadish* radish, int damage) override {
		radish->setHp(radish->getHp() - damage);
		if (radish->getHp() <= 0) {
			radish->changeState(new DeadState());
		}
		else {
			radish->changeState(new InjuredState());
		}
	}

	void clickEvent(CRadish* radish, Vec2 clickPos) override {
		// 健康状态下的点击事件处理
	}

	void update(CRadish* radish) override {
		// 健康状态下的更新逻辑
	}
};

class InjuredState : public CRadishState
{
public:
	void damage(CRadish* radish, int damage) override {
		radish->setHp(radish->getHp() - damage);
		if (radish->getHp() <= 0) {
			radish->changeState(new DeadState());
		}
	}

	void clickEvent(CRadish* radish, Vec2 clickPos) override {
		// 受伤状态下的点击事件处理
	}

	void update(CRadish* radish) override {
		// 受伤状态下的更新逻辑
	}
};

class DeadState : public CRadishState
{
public:
	void damage(CRadish* radish, int damage) override {
		// 死亡状态下，不会再受到伤害
	}

	void clickEvent(CRadish* radish, Vec2 clickPos) override {
		// 死亡状态下，点击事件无效
	}

	void update(CRadish* radish) override {
		// 死亡状态下的更新逻辑
	}
};

class CRadish : public Node
{
public:
	CRadish();
	~CRadish();
	bool init();
	void changeState(CRadishState* newState);
	void addTouch();
	void clickEvent(Vec2 clickPos);
	bool Damage(int damage);
	CREATE_FUNC(CRadish);
	CC_SYNTHESIZE(int, m_nHP, Hp);

private:
	CRadishState* m_state;  // 当前状态
	Sprite* m_pHp;
	Sprite* m_pModel;
	Sprite* myup;
};

CRadish::CRadish()
{
	m_state = new HealthyState();  // 初始化为健康状态
}

void CRadish::changeState(CRadishState* newState)
{
	if (m_state != nullptr) {
		delete m_state;
	}
	m_state = newState;
}

bool CRadish::Damage(int damage)
{
	m_state->damage(this, damage);
	return false;
}

void CRadish::clickEvent(Vec2 clickPos)
{
	m_state->clickEvent(this, clickPos);
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

