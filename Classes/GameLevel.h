#ifndef __GAMELEVEL_H__
#define __GAMELEVEL_H__
#include "cocos2d.h"
#include "ui/UIPageView.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

using namespace ui;
class CGameLevel : public Scene
{
public:
	CGameLevel();
	~CGameLevel();
	virtual bool init();
	void addPageView();
	void createButton();
	CREATE_FUNC(CGameLevel);
	PageView* getPageView() const {
		return m_pPageView;
	}
	void setPageViewCurrentPage(int index) {
		if (m_pPageView) {
			m_pPageView->scrollToItem(index);
		}
	}
private:
	MenuItemImage* m_pLeftButton;
	MenuItemImage* m_pRightButton;
	PageView* m_pPageView;
	Sprite* m_pSprite;
	ui::Button* m_pStartButton;
};

#define WINSIZE Director::getInstance()->getVisibleSize()
//����һ�� Size ���͵Ķ��󣬱�ʾ��ǰ��Ϸ���ڿɼ�����ĳߴ�

#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif