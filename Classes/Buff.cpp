#include "Buff.h"
#include "GameScene.h"
#include "Monster.h"
// CBuffBase 类的构造函数
CBuffBase::CBuffBase()
{

}

// CBuffBase 类的析构函数
CBuffBase::~CBuffBase()
{

}

// CBuffBase 类的初始化方法
bool CBuffBase::init()
{
	return Node::init();
}

// CBuffBase 类的每帧更新方法
void CBuffBase::update(float delta)
{
	// 这里暂时不做任何操作，由子类具体实现
}

// CBuffBase 类的添加Buff节点方法
void CBuffBase::addBuffNode(Node* pNode, SBuffDt* pBuff)
{
	// 在m_mapBuff中查找与给定节点pNode关联的Buff
	auto iter = m_mapBuff.find(pNode);

	// 如果找到了对应的Buff
	if (iter != m_mapBuff.end())
	{
		// 更新Buff的剩余时间
		m_mapBuff[pNode]->fTime = pBuff->fTime;
	}
	else
	{
		// 如果没有找到对应的Buff，则创建一个新的SData结构体用于存储Buff相关数据
		SData* pData = new SData();
		pData->fTime = pBuff->fTime;
		pData->nValue = pBuff->nValue;

		// 创建Buff的图像
		pData->pSprite = Sprite::create();

		// 在游戏场景的动画层中切换Buff的动作
		auto animateLayer = CGameScene::getInstance()->getMyAnimate();
		animateLayer->changeAction(pData->pSprite, pBuff->nAnimateID, true, 0.1f);

		// 设置Buff图像的位置
		pData->pSprite->setPositionY(static_cast<CMonster*>(pNode)->getContentSize().height / 2);

		// 将Buff图像添加到节点上
		pNode->addChild(pData->pSprite);

		// 将新创建的SData结构体添加到m_mapBuff中，与给定节点pNode关联
		m_mapBuff[pNode] = pData;
	}
	
}

// CBuffBase 类的移除Buff节点方法
void CBuffBase::removeBuffNode(Node* pNode)
{
	auto iter = m_mapBuff.find(pNode);

	if (iter != m_mapBuff.end())
	{
		// 停止Buff图像上的所有动作
		iter->second->pSprite->stopAllActions();

		// 从父节点移除Buff图像
		iter->second->pSprite->removeFromParent();

		// 从m_mapBuff中移除与给定节点pNode关联的Buff
		m_mapBuff.erase(iter);
	}
}

// CBuffSpeed 类的初始化方法
bool CBuffSpeed::init()
{
    return CBuffBase::init();
}

// CBuffSpeed 类的每帧更新方法
void CBuffSpeed::update(float delta)
{
    // 检查Buff是否存在
    if (m_mapBuff.empty())
        return;

    // 使用范围基于范围的 for 循环
    for (auto iter = m_mapBuff.begin(); iter != m_mapBuff.end();)
    {
        // 减少Buff的剩余时间
        iter->second->fTime -= delta;

        // 获取当前Buff关联的怪物，并将其速度设置为Buff的数值
        auto monster = static_cast<CMonster*>(iter->first);
        if (monster->getSpeed() >= iter->second->nValue)
        {
            monster->setSpeed(iter->second->nValue);
        }
        else
        {
            monster->setSpeed(8);
        }

        // 检查Buff是否已经超时
        if (iter->second->fTime <= 0)
        {
            // 恢复怪物原始速度
            monster->setSpeed(monster->getSaveSpeed());

            // 停止所有动作并从父节点移除Buff的图像
            auto buffSprite = iter->second->pSprite;
            buffSprite->stopAllActions();
            buffSprite->removeFromParent();

            // 从Buff Map 中移除当前Buff，并获取下一个迭代器
            iter = m_mapBuff.erase(iter);
        }
        else
        {
            // 移动到下一个Buff
            ++iter;
        }
    }
}

// CBuffHurt 类的初始化方法
bool CBuffHurt::init()
{
    return CBuffBase::init();
}

// CBuffHurt 类的每帧更新方法
void CBuffHurt::update(float delta)
{
    // 检查Buff是否存在
    if (m_mapBuff.empty())
        return;

    // 使用范围基于范围的 for 循环
    for (auto iter = m_mapBuff.begin(); iter != m_mapBuff.end();)
    {
        // 减少Buff的剩余时间
        iter->second->fTime -= delta;

        // 获取当前Buff关联的怪物，并对其造成伤害
        CMonster* monster = static_cast<CMonster*>(iter->first);
        if (monster->Damage(iter->second->nValue))
        {
            // 移除怪物并终止循环
            monster->removeMonster();
            return;
        }

        // 检查Buff是否已经超时
        if (iter->second->fTime <= 0)
        {
            // 停止所有动作并从父节点移除Buff的图像
            auto buffSprite = iter->second->pSprite;
            buffSprite->stopAllActions();
            buffSprite->removeFromParent();

            // 从Buff Map 中移除当前Buff，并获取下一个迭代器
            iter = m_mapBuff.erase(iter);
        }
        else
        {
            // 移动到下一个Buff
            ++iter;
        }
    }
}

