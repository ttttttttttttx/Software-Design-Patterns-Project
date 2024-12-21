#ifndef __CARD_H__
#define __CARD_H__
#include "cocos2d.h"
#include "DataMgr.h"
#include "DataClass.h"
#include "Arms.h"
USING_NS_CC;

#define WINSIZE Director::getInstance()->getVisibleSize()
#define ORIGIN Director::getInstance()->getVisibleOrigin()

class MyCard : public Sprite
{
public:
	static MyCard* createMoneyCard(SCardDt* pCard); //创建一个实例
	void ifMoney(int nMoney);
	CC_SYNTHESIZE(int, myArmsID, ArmsID);
	CC_SYNTHESIZE(int, myArmsMoney, Money);
private:
	string CardName;  //卡片的名称
};

class ArmsCard : public Node
{
public:
	virtual bool init();
	void createAllArmsCard();                  //创建卡片
	void clickEventFirst(Vec2 clickPos);     //第一次点击事件
	void clickEventSecond(Vec2 clickPos);     //第二次点击事件
	void addTouch();
	CREATE_FUNC(ArmsCard);
private:
	Sprite* cardContainers;
	std::vector<MyCard*> allCard;      //存储卡片对象的向量
}; 

class UpCard : public Node
{
public:
	virtual bool init();
	void clickEventFirst(Vec2 clickPos, MyArms* pArms);
	void clickEventSecond(Vec2 clickPos, MyArms* pArms);
	void upCard(MyArms* pArms);
	CREATE_FUNC(UpCard);
private:
	Sprite* myUpGrade;
	Sprite* mySell;
	Sprite* myRander;//背景圈圈
	MyArms* nowArms;//武器
};

#endif