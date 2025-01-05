#ifndef __UILAYER_H__
#define __UILAYER_H__
#include "cocos2d.h"
USING_NS_CC;
class CUILayer : public Node
{
public:
	CUILayer();
	~CUILayer();
	virtual bool init();
	CREATE_FUNC(CUILayer);
	int getMoney()
	{
		return m_nMoney;
	}
	void createOver(int nCurWave,int nMaxWave,bool bWin);
	void addMoney(int nMoney);
	void setCurWave(int nCurWave);
private:
	int m_nMoney;
	LabelAtlas* m_pMoney;
	Node* m_pWave;
	Sprite* m_pPause;
	LabelAtlas* m_pMaxWave;
	LabelAtlas* m_pCurWaveLow;
	LabelAtlas* m_pCurWaveHigh;
	Node* m_pTime;
	Sprite* m_pTimeNumber;
	int m_nTime;
	Sprite* m_pCountDown;
	Sprite* m_pMenu;
	MenuItemToggle* m_pPauseResum;
};

#define WINSIZE Director::getInstance()->getVisibleSize()
#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif