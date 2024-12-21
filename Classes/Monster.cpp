#include "Monster.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;

// 初始化怪物路径
vector<Vec2> CMonster::m_vecPath;

// 怪物类的构造函数
CMonster::CMonster()
{
}

// 怪物类的析构函数
CMonster::~CMonster()
{
}

// 使用怪物数据初始化怪物实例的方法
bool CMonster::initWithData(SMonsterDt* pMonster)
{
	if (!CRoleBase::init()) return false;

	// 初始化怪物属性
	initMonsterProperties(pMonster);

	// 创建并设置怪物的模型
	createAndSetMonsterModel(pMonster);

	// 设置怪物血条位置
	setHpNodePosition();

	// 设置怪物初始位置和方向
	setInitialPositionAndDirection();

	// 开启每帧更新
	scheduleUpdate();

	return true;
}

void CMonster::initMonsterProperties(SMonsterDt* pMonster)
{
	// 初始化怪物属性
	m_nIndex = 0;
	m_TiledSize = CGameScene::getInstance()->getGameMap()->getTiledSize();
	m_fSaveSpeed = m_fSpeed = pMonster->fSpeed;
	m_bChangeDir = false;
	if (pMonster->fSpeed == 10)
	{
		m_nHp = m_nMaxHp = 600000;
	}
	else
	{
		m_nHp = m_nMaxHp = 10000;
	}
	m_nMoney = pMonster->nMoney;
}

void CMonster::createAndSetMonsterModel(SMonsterDt* pMonster)
{
	// 创建怪物的模型
	m_pModel = Sprite::createWithSpriteFrameName(pMonster->strImg);

	// 切换怪物模型的动画，参数包括精灵、动画ID、是否循环、动画帧间隔时间
	CGameScene::getInstance()->getMyAnimate()->changeAction(m_pModel, pMonster->AnimateID, true, 0.2f);

	// 将怪物模型添加到怪物对象中
	this->addChild(m_pModel);
}

void CMonster::setHpNodePosition()
{
	// 设置怪物血条位置
	m_pHpNode->setPosition(0, m_pModel->getContentSize().height / 2);
}

void CMonster::setInitialPositionAndDirection()
{
	// 设置怪物初始位置
	this->setPosition(m_vecPath[m_nIndex]);

	// 计算怪物初始方向
	calculateDir();
}

// 根据怪物数据创建怪物实例的静态方法
CMonster* CMonster::createWithData(SMonsterDt* pMonster)
{
	CMonster* pRef = new (std::nothrow) CMonster();

	// 判断实例是否创建成功，并进行初始化
	if (pRef && pRef->initWithData(pMonster))
	{
		return pRef;  // 返回创建的怪物实例
	}

	delete pRef;  // 使用 delete 释放内存

	return nullptr;  // 返回空指针表示创建失败
}

// 怪物类的每帧更新方法
void CMonster::update(float delta)
{
	// 如果怪物改变了方向，检查是否需要移除怪物
	if (this->changeUpdateDir())
	{
		if (m_nMoney == 999)
		{
			CGameScene::getInstance()->getRadish()->Damage(10);
		}
		else
		{
			CGameScene::getInstance()->getRadish()->Damage(1);
		}
		
		// 添加怪物死亡动画
		CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), 3024);

		// 移除怪物
		this->removeMonster();

		return;
	}

	// 移动怪物
	Vec2 pos = this->getPosition();
	pos.x += delta * m_vDir.x * m_fSpeed;
	pos.y += delta * m_vDir.y * m_fSpeed;
	this->setPosition(pos);
}

// 辅助方法：检查指定位置是否具有路径属性
bool CMonster::hasPathProperty(const Vec2& pos)
{
	// 获取指定位置的路径属性
	ValueMap vmap = CGameScene::getInstance()->getGameMap()->getProperty("path", pos);
	return vmap.size() != 0 && vmap["point"].asBool();
}

// 辅助方法：更新怪物的方向和索引
bool CMonster::updateDirectionAndIndex()
{
	// 标记需要改变方向
	m_bChangeDir = true;
	// 增加路径点索引
	m_nIndex++;
	// 重新计算方向
	this->calculateDir();

	// 返回是否需要移除怪物
	return (m_nIndex >= m_vecPath.size() - 1);
}

// 改变怪物更新方向的方法
bool CMonster::changeUpdateDir()
{
	// 获取怪物当前坐标
	Vec2 monsterPos = this->getPosition();
	// 计算怪物下一个格子坐标
	Vec2 nextPos = Vec2(monsterPos.x + m_vDir.x * (m_TiledSize.width / 2 + 3), monsterPos.y + m_vDir.y * (m_TiledSize.height / 2 + 3));
	// 获取怪物的下一个格子坐标
	Vec2 nextTiled = CGameScene::getInstance()->getGameMap()->getTiledByPos(nextPos);
	// 获取怪物的当前格子坐标
	Vec2 tiled = CGameScene::getInstance()->getGameMap()->getTiledByPos(monsterPos);

	// 如果下一个格子和当前格子相同，表示怪物未改变方向
	if (nextTiled == tiled)
	{
		m_bChangeDir = false;
	}

	// 如果下一个格子出界
	if (CGameScene::getInstance()->getGameMap()->isOutOfMap(nextTiled))
	{
		return updateDirectionAndIndex();
	}
	// 如果下一个格子是路径
	else if (CGameScene::getInstance()->getGameMap()->isInLayer("path", nextTiled))
	{
		// 如果未改变方向且当前格子有路径属性
		if (!m_bChangeDir && hasPathProperty(tiled) && nextTiled != tiled)
		{
			return updateDirectionAndIndex();
		}
	}
	// 如果下一个格子不是路径
	else
	{
		return updateDirectionAndIndex();
	}

	// 返回是否需要移除怪物
	return false;
}

// 计算怪物方向的方法
void CMonster::calculateDir()
{
	// 如果当前路径索引小于路径总数
	if (m_nIndex + 1 < m_vecPath.size())
	{
		// 计算方向向量
		Vec2 pos = m_vecPath[m_nIndex + 1] - m_vecPath[m_nIndex];
		m_vDir = pos.getNormalized();
	}
	else
	{
		// 处理越界的情况，例如输出错误日志或进行其他处理
	}
}

// 设置怪物路径的静态方法
void CMonster::setPath(vector<Vec2> vecPath)
{
	m_vecPath = vecPath;
}

// 移除怪物的方法
void CMonster::removeMonster()
{
	// 移除怪物身上的Buff
	CGameScene::getInstance()->getBuffLayer()->removeBuff(this);

	// 如果怪物走到了路径的终点，固定金币奖励为14
	if (m_nIndex >= m_vecPath.size() - 1)
	{
		m_nMoney = 14;
	}

	// 增加玩家金币数量
	CGameScene::getInstance()->getUILayer()->addMoney(m_nMoney);

	// 创建金币精灵
	Sprite* pSprite = Sprite::createWithSpriteFrameName(StringUtils::format("money%d.png", m_nMoney));
	pSprite->setPosition(this->getPosition());
	MoveBy* pMoveBy = MoveBy::create(0.5, Vec2(0, 50));
	pSprite->runAction(Sequence::createWithTwoActions(pMoveBy, RemoveSelf::create()));
	CGameScene::getInstance()->getUILayer()->addChild(pSprite);

	// 添加怪物死亡动画
	CGameScene::getInstance()->getMyAnimate()->createAnimate(this->getPosition(), 3024);

	//怪物死亡音效
	AudioEngine::play2d("sound/monsterDie.mp3", false, 1.1f);

	// 从父节点移除怪物
	this->removeFromParent();
}

