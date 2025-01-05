#ifndef __BUFF_H__
#define __BUFF_H__

#include "cocos2d.h"
#include "DataClass.h"

using namespace cocos2d;

// Structure to store buff data
struct SData
{
	float fTime;     // Duration of the buff
	int nValue;      // Value of the buff effect
	Sprite* pSprite; // Sprite for buff visualization
};

// Base Buff class
class CBuffBase : public Node
{
public:
	// Constructor
	CBuffBase();

	// Destructor
	~CBuffBase();

	// Initialize function, override to implement specific initialization logic
	virtual bool init();

	// Add buff node to the map
	void addBuffNode(Node* pNode, SBuffDt* pBuff);

	// Remove buff node from the map
	void removeBuffNode(Node* pNode);

	// Update function called each frame, override to implement specific buff logic
	virtual void update(float delta);

protected:
	// Map storing buff nodes and their data
	std::map<Node*, SData*> m_mapBuff;
};

// Speed Buff class
class CBuffSpeed : public CBuffBase
{
public:
	// Initialize the speed buff
	virtual bool init();

	// Update function for speed buff logic
	virtual void update(float delta);

	// Static creation method
	CREATE_FUNC(CBuffSpeed);
};

// Damage Buff class
class CBuffHurt : public CBuffBase
{
public:
	// Initialize the damage buff
	virtual bool init();

	// Update function for damage buff logic
	virtual void update(float delta);

	// Static creation method
	CREATE_FUNC(CBuffHurt);
};

#endif