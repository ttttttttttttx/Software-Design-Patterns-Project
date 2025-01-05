#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__
#include "cocos2d.h"
USING_NS_CC;
class CGameMap : public Node
{
public:
	CGameMap();
	~CGameMap();
	virtual bool initWithImgPath(std::string strName);
	static CGameMap* createWithImgPath(std::string strName);

	// Convert pixel position to tile position
	Vec2 getTiledByPos(Vec2 pixelPos);
	// Convert tile position to pixel position
	Vec2 getPixelByTiledPos(Vec2 tiledPos);
	//int clickEvent(Vec2 clickPos);
	
	// Get initial object positions from map (start point, end point)
	std::vector<Vec2> getInitPos();
	// Check if a position is in a specific layer
	bool isInLayer(std::string strName, Vec2 TiledPos);
	// Check if a position is outside the map
	bool isOutOfMap(Vec2 tiledPos);
	// Get tile properties from a specific layer at position
	ValueMap getProperty(std::string strName, Vec2 tiledPos);
	// Get the last path position in pixel coordinates
	Vec2 getLastTiledPos();
	// Get the first path position in pixel coordinates
	Vec2 getFirstTiledPos();
	// Tile size property
	CC_SYNTHESIZE(Size, m_tiledSize, TiledSize);

	CC_SYNTHESIZE(TMXTiledMap*, m_pTiledMap, TiledMap);
private:
	Size m_mapSize;
};

#define WINSIZE Director::getInstance()->getVisibleSize()
#define ORIGIN Director::getInstance()->getVisibleOrigin()

#endif