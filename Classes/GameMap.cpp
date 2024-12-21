#include "GameMap.h"
CGameMap::CGameMap()
{
}

CGameMap::~CGameMap()
{
}


bool CGameMap::initWithImgPath(std::string strName)//接收一个strName，用作图像资源的路径
{
	if (!Node::init())
	{
		return false;
	}

	m_pTiledMap = TMXTiledMap::create(strName);//背景
	//根据提供的图像资源路径创建了一个瓦片地图对象 m_pTiledMap

	m_pTiledMap->setAnchorPoint(Vec2::ZERO);//设置瓦片地图的锚点为左下角（0, 0）
	this->addChild(m_pTiledMap);//将瓦片地图对象作为当前节点的子节点添加到场景中


	m_tiledSize = m_pTiledMap->getTileSize();//获取瓦片地图的单个瓦片大小，并将其保存到 m_tiledSize 变量中
	m_mapSize = m_pTiledMap->getMapSize();//获取整个地图的大小（以瓦片为单位），并将其保存到 m_mapSize 变量中

	return true;
}

CGameMap* CGameMap::createWithImgPath(std::string strName)
{
	CGameMap* pRef = new CGameMap();
	if (pRef&&pRef->initWithImgPath(strName))//调用initWithImgPath加载地图的图像，并设置地图的属性
	{
		return pRef;
	}
	//CC_SAFE_DELETE(pRef);
	return nullptr;
}

cocos2d::Vec2 CGameMap::getTiledByPos(Vec2 pixelPos)
{
	//将给定的像素 x 坐标 pixelPos.x 除以瓦片宽度 m_tiledSize.width，得到瓦片 x 坐标 fX
	int fX = pixelPos.x / m_tiledSize.width;

	float fResult = pixelPos.y / m_tiledSize.height;
	fResult < 0 ?fResult=fResult-1 : fResult;
	int fY = m_mapSize.height-(int)fResult-1;

	return Vec2(fX,fY);
}

cocos2d::Vec2 CGameMap::getPixelByTiledPos(Vec2 tiledPos)
{
	//横向瓦片坐标乘以瓦片宽度，并加上瓦片宽度的一半，这样计算出的结果会落在瓦片的中心点
	float fX = tiledPos.x*m_tiledSize.width+m_tiledSize.width/2;

	//计算出瓦片在地图中的位置（从地图底部到指定瓦片的垂直距离）
	//然后乘以瓦片高度，并加上瓦片高度的一半，以获得纵向像素坐标
	float fY = (m_mapSize.height - tiledPos.y - 1)*m_tiledSize.height + m_tiledSize.height / 2;

	return Vec2(fX,fY);
}

//int CGameMap::clickEvent(Vec2 clickPos)
//{
//	return 0;
//}


//从游戏地图的特定对象组中提取对象的位置信息，并将这些位置信息转换为游戏中需要的像素坐标，
//以便在游戏中使用这些预定义的位置信息
std::vector<Vec2> CGameMap::getInitPos()
{
	std::vector<Vec2> VecPos;//创建了一个用于存储坐标的向量
	TMXObjectGroup* pObject = m_pTiledMap->getObjectGroup("pathObject");//获取对象组
	//从 TMXTiledMap 中获取名为 "pathObject" 的对象组，该组中包含一些预定义的对象

	ValueVector vObject = pObject->getObjects();//将对象组中的所有对象存储在 vObject 中

	for (int i = 0; i < vObject.size();i++)//遍历对象组中的所有对象
	{
		Value vData = vObject[i];//获取对象组中的第 i 个对象
		ValueMap vMap = vData.asValueMap();//将对象的数据转换为 ValueMap，这个 Map 包含了该对象的所有属性信息
		float fX = vMap["x"].asFloat();
		float fY = vMap["y"].asFloat();//从该对象的属性中获取 x 和 y 坐标信息
		Vec2 tiledPos = this->getTiledByPos(Vec2(fX, fY));//将获取到的 x 和 y 坐标转换为地图瓦片坐标
		VecPos.push_back(this->getPixelByTiledPos(tiledPos));//将经过处理的像素坐标添加到 VecPos 向量中
	}
	return VecPos;
}


//检查给定的图块层是否包含特定位置的瓦片。如果瓦片层中的某个位置包含瓦片，则返回 true，否则返回 false
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

//判断给定的瓦片坐标是否超出地图的边界
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
	TMXLayer* pLayer = m_pTiledMap->getLayer(strName);//获取strName所在的层
	int nGID = pLayer->getTileGIDAt(tiledPos);//使用给定的瓦片坐标 tiledPos 在图层上获取图块的全局图块ID（GID）
	Value vData = m_pTiledMap->getPropertiesForGID(nGID);//使用 GID 获取图块的属性信息
	if (vData.isNull())
	{
		return ValueMap();
	}
	ValueMap vMap = vData.asValueMap();
	return vMap;

}


//获取初始化位置向量中最后一个位置对应的瓦片在地图中的像素坐标位置
cocos2d::Vec2 CGameMap::getLastTiledPos()
{
	return this->getPixelByTiledPos(this->getTiledByPos(this->getInitPos().back()));
}

//获取初始化位置向量中第一个位置对应的瓦片在地图中的像素坐标位置
cocos2d::Vec2 CGameMap::getFirstTiledPos()
{
	return this->getPixelByTiledPos(this->getTiledByPos(this->getInitPos()[0]));
}

