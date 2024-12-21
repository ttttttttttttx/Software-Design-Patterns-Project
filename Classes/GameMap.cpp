#include "GameMap.h"
CGameMap::CGameMap()
{
}

CGameMap::~CGameMap()
{
}


bool CGameMap::initWithImgPath(std::string strName)//����һ��strName������ͼ����Դ��·��
{
	if (!Node::init())
	{
		return false;
	}

	m_pTiledMap = TMXTiledMap::create(strName);//����
	//�����ṩ��ͼ����Դ·��������һ����Ƭ��ͼ���� m_pTiledMap

	m_pTiledMap->setAnchorPoint(Vec2::ZERO);//������Ƭ��ͼ��ê��Ϊ���½ǣ�0, 0��
	this->addChild(m_pTiledMap);//����Ƭ��ͼ������Ϊ��ǰ�ڵ���ӽڵ���ӵ�������


	m_tiledSize = m_pTiledMap->getTileSize();//��ȡ��Ƭ��ͼ�ĵ�����Ƭ��С�������䱣�浽 m_tiledSize ������
	m_mapSize = m_pTiledMap->getMapSize();//��ȡ������ͼ�Ĵ�С������ƬΪ��λ���������䱣�浽 m_mapSize ������

	return true;
}

CGameMap* CGameMap::createWithImgPath(std::string strName)
{
	CGameMap* pRef = new CGameMap();
	if (pRef&&pRef->initWithImgPath(strName))//����initWithImgPath���ص�ͼ��ͼ�񣬲����õ�ͼ������
	{
		return pRef;
	}
	//CC_SAFE_DELETE(pRef);
	return nullptr;
}

cocos2d::Vec2 CGameMap::getTiledByPos(Vec2 pixelPos)
{
	//������������ x ���� pixelPos.x ������Ƭ��� m_tiledSize.width���õ���Ƭ x ���� fX
	int fX = pixelPos.x / m_tiledSize.width;

	float fResult = pixelPos.y / m_tiledSize.height;
	fResult < 0 ?fResult=fResult-1 : fResult;
	int fY = m_mapSize.height-(int)fResult-1;

	return Vec2(fX,fY);
}

cocos2d::Vec2 CGameMap::getPixelByTiledPos(Vec2 tiledPos)
{
	//������Ƭ���������Ƭ��ȣ���������Ƭ��ȵ�һ�룬����������Ľ����������Ƭ�����ĵ�
	float fX = tiledPos.x*m_tiledSize.width+m_tiledSize.width/2;

	//�������Ƭ�ڵ�ͼ�е�λ�ã��ӵ�ͼ�ײ���ָ����Ƭ�Ĵ�ֱ���룩
	//Ȼ�������Ƭ�߶ȣ���������Ƭ�߶ȵ�һ�룬�Ի��������������
	float fY = (m_mapSize.height - tiledPos.y - 1)*m_tiledSize.height + m_tiledSize.height / 2;

	return Vec2(fX,fY);
}

//int CGameMap::clickEvent(Vec2 clickPos)
//{
//	return 0;
//}


//����Ϸ��ͼ���ض�����������ȡ�����λ����Ϣ��������Щλ����Ϣת��Ϊ��Ϸ����Ҫ���������꣬
//�Ա�����Ϸ��ʹ����ЩԤ�����λ����Ϣ
std::vector<Vec2> CGameMap::getInitPos()
{
	std::vector<Vec2> VecPos;//������һ�����ڴ洢���������
	TMXObjectGroup* pObject = m_pTiledMap->getObjectGroup("pathObject");//��ȡ������
	//�� TMXTiledMap �л�ȡ��Ϊ "pathObject" �Ķ����飬�����а���һЩԤ����Ķ���

	ValueVector vObject = pObject->getObjects();//���������е����ж���洢�� vObject ��

	for (int i = 0; i < vObject.size();i++)//�����������е����ж���
	{
		Value vData = vObject[i];//��ȡ�������еĵ� i ������
		ValueMap vMap = vData.asValueMap();//�����������ת��Ϊ ValueMap����� Map �����˸ö��������������Ϣ
		float fX = vMap["x"].asFloat();
		float fY = vMap["y"].asFloat();//�Ӹö���������л�ȡ x �� y ������Ϣ
		Vec2 tiledPos = this->getTiledByPos(Vec2(fX, fY));//����ȡ���� x �� y ����ת��Ϊ��ͼ��Ƭ����
		VecPos.push_back(this->getPixelByTiledPos(tiledPos));//���������������������ӵ� VecPos ������
	}
	return VecPos;
}


//��������ͼ����Ƿ�����ض�λ�õ���Ƭ�������Ƭ���е�ĳ��λ�ð�����Ƭ���򷵻� true�����򷵻� false
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

//�жϸ�������Ƭ�����Ƿ񳬳���ͼ�ı߽�
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
	TMXLayer* pLayer = m_pTiledMap->getLayer(strName);//��ȡstrName���ڵĲ�
	int nGID = pLayer->getTileGIDAt(tiledPos);//ʹ�ø�������Ƭ���� tiledPos ��ͼ���ϻ�ȡͼ���ȫ��ͼ��ID��GID��
	Value vData = m_pTiledMap->getPropertiesForGID(nGID);//ʹ�� GID ��ȡͼ���������Ϣ
	if (vData.isNull())
	{
		return ValueMap();
	}
	ValueMap vMap = vData.asValueMap();
	return vMap;

}


//��ȡ��ʼ��λ�����������һ��λ�ö�Ӧ����Ƭ�ڵ�ͼ�е���������λ��
cocos2d::Vec2 CGameMap::getLastTiledPos()
{
	return this->getPixelByTiledPos(this->getTiledByPos(this->getInitPos().back()));
}

//��ȡ��ʼ��λ�������е�һ��λ�ö�Ӧ����Ƭ�ڵ�ͼ�е���������λ��
cocos2d::Vec2 CGameMap::getFirstTiledPos()
{
	return this->getPixelByTiledPos(this->getTiledByPos(this->getInitPos()[0]));
}

