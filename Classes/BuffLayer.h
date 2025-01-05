#ifndef __BUFFLAYER_H__
#define __BUFFLAYER_H__

#include "cocos2d.h"
#include "DataClass.h"
#include "DataMgr.h"
#include "Buff.h"

using namespace cocos2d;

// Buff Layer class
class CBuffLayer : public Node
{
public:
	// Constructor
	CBuffLayer();

	// Destructor
	~CBuffLayer();

	// Initialize the buff layer
	virtual bool init();

	// Initialize callback functions
	void inintCallBack();

	// Initialize callback for specific buff type
	void initCallbackForBuff(int buffId, CBuffBase* buffInstance);

	// Update function called each frame
	virtual void update(float delta);

	// Add buff to specified node
	void addBuff(Node* pNode, int nBuffID);

	// Remove buff from specified node
	void removeBuff(Node* pNode);

	// Static creation method
	CREATE_FUNC(CBuffLayer);

private:
	// Map storing buff callbacks, key is buff ID, value is buff instance
	std::map<int, CBuffBase*> m_mapCallBack;
};

// Get screen size macro
#define WINSIZE Director::getInstance()->getVisibleSize()

// Get screen origin macro
#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif