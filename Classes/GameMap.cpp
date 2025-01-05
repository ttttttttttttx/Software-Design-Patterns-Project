#include "GameMap.h"
CGameMap::CGameMap()
{
}

CGameMap::~CGameMap()
{
}


bool CGameMap::initWithImgPath(std::string strName)
{
	if (!Node::init())
	{
		return false;
	}

	m_pTiledMap = TMXTiledMap::create(strName);

	m_pTiledMap->setAnchorPoint(Vec2::ZERO);
	this->addChild(m_pTiledMap);


	m_tiledSize = m_pTiledMap->getTileSize();
	m_mapSize = m_pTiledMap->getMapSize();

	return true;
}

CGameMap* CGameMap::createWithImgPath(std::string strName)
{
	CGameMap* pRef = new CGameMap();
	if (pRef&&pRef->initWithImgPath(strName))  // Initialize map with image and set map properties
	{
		return pRef;
	}
	//CC_SAFE_DELETE(pRef);
	return nullptr;
}

cocos2d::Vec2 CGameMap::getTiledByPos(Vec2 pixelPos)
{
	// Calculate tile X coordinate by dividing pixel X by tile width
	int fX = pixelPos.x / m_tiledSize.width;

	float fResult = pixelPos.y / m_tiledSize.height;
	fResult < 0 ?fResult=fResult-1 : fResult;
	int fY = m_mapSize.height-(int)fResult-1;

	return Vec2(fX,fY);
}

cocos2d::Vec2 CGameMap::getPixelByTiledPos(Vec2 tiledPos)
{
	// Calculate pixel X coordinate by multiplying tile X by tile width and adding half tile width
	float fX = tiledPos.x*m_tiledSize.width+m_tiledSize.width/2;

	// Calculate pixel Y coordinate based on tile position from bottom of map
	// Add half tile height to center the position
	float fY = (m_mapSize.height - tiledPos.y - 1)*m_tiledSize.height + m_tiledSize.height / 2;

	return Vec2(fX,fY);
}

//int CGameMap::clickEvent(Vec2 clickPos)
//{
//	return 0;
//}


// Get initial positions from map objects and convert them to game coordinates
// These positions are used as predefined path points in the game
std::vector<Vec2> CGameMap::getInitPos()
{
	std::vector<Vec2> VecPos;  // Vector to store positions
	TMXObjectGroup* pObject = m_pTiledMap->getObjectGroup("pathObject");  // Get object group
	// Get "pathObject" group from TMXTiledMap which contains predefined objects

	ValueVector vObject = pObject->getObjects();  // Store all objects in vObject

	for (int i = 0; i < vObject.size();i++)  // Iterate through all objects
	{
		Value vData = vObject[i];  // Get object at index i
		ValueMap vMap = vData.asValueMap();  // Convert to ValueMap containing object properties
		float fX = vMap["x"].asFloat();
		float fY = vMap["y"].asFloat();  // Get x and y coordinates from object
		Vec2 tiledPos = this->getTiledByPos(Vec2(fX, fY));  // Convert coordinates to tile position
		VecPos.push_back(this->getPixelByTiledPos(tiledPos));  // Add converted coordinates to vector
	}
	return VecPos;
}


// Check if a tile exists at specific position in layer, return true if tile exists, false otherwise
bool CGameMap::isInLayer(std::string strName, Vec2 TiledPos)
{
	TMXLayer* pLayer = m_pTiledMap->getLayer(strName);
	int nGID = pLayer->getTileGIDAt(TiledPos);
	if (0==nGID)
	{
		return false;
	}
	return true;
}

// Check if tile position is outside map boundaries
bool CGameMap::isOutOfMap(Vec2 tiledPos)
{
	if (tiledPos.x<0
		||tiledPos.y<0
		||tiledPos.x>m_mapSize.width-1
		||tiledPos.y>m_mapSize.height-1)
	{
		return true;
	}
	else
		return false;

}

cocos2d::ValueMap CGameMap::getProperty(std::string strName, Vec2 tiledPos)
{
	TMXLayer* pLayer = m_pTiledMap->getLayer(strName);  // Get layer by name
	int nGID = pLayer->getTileGIDAt(tiledPos);  // Get tile GID at position
	Value vData = m_pTiledMap->getPropertiesForGID(nGID);  // Get tile properties using GID
	if (vData.isNull())
	{
		return ValueMap();
	}
	ValueMap vMap = vData.asValueMap();
	return vMap;

}


// Get last position from initial positions and convert to pixel coordinates
cocos2d::Vec2 CGameMap::getLastTiledPos()
{
	return this->getPixelByTiledPos(this->getTiledByPos(this->getInitPos().back()));
}

// Get first position from initial positions and convert to pixel coordinates
cocos2d::Vec2 CGameMap::getFirstTiledPos()
{
	return this->getPixelByTiledPos(this->getTiledByPos(this->getInitPos()[0]));
}

