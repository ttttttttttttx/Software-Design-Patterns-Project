#include "GameLevel.h"
#include "DataMgr.h"
#include "DataClass.h"
#include "GameScene.h"
CGameLevel::CGameLevel()
{

}

CGameLevel::~CGameLevel()
{

}

bool CGameLevel::init()
{
	if (!Scene::init())
	{
		return false;
	}//����Scene�ĳ�ʼ��������ʼ��ʧ���򷵻�false��


	Node* pMenuNode = CSLoader::createNode("GameLevel.csb");//����һ�������ڵ㣬������Ϸ�ؿ���UI���档
	this->addChild(pMenuNode);

	m_pStartButton = static_cast<ui::Button*>(pMenuNode->getChildByName("LevelStartButton"));
	//��UI�����л�ȡ��Ϊ "LevelStartButton" �İ�ť�������丳ֵ�� m_pStartButton ������

	//Ϊ m_pStartButton ��ť�����һ�������¼��ļ�����������ť�����ʱ�������¼���������
	m_pStartButton->addTouchEventListener([=](Ref*, Widget::TouchEventType){
		CLevelDtMgr* pLevelMgr=static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
		pLevelMgr->setCurIndex(m_pPageView->getCurPageIndex());
		CGameScene::deletInstance();
		Director::getInstance()->replaceScene(CGameScene::getInstance());//�л�����Ϸ������
	//������Ϸ������
	});

	this->addPageView();
	this->createButton();
	return true;
}


//������һ�����Ի�����ҳ�� PageView��ÿһҳ��һ�����֣������˹ؿ�ͼƬ�͹ؿ���Ƭ���������ѡ��ؿ�
void CGameLevel::addPageView()
{
	CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));
	//�����ݹ������л�ȡ�ؿ����ݹ�������ʵ�� pLevelMgr�����ڻ�ȡ�ؿ���Ϣ��
	
	m_pPageView = PageView::create();//������һ�� PageView ���� m_pPageView������չʾ����ؿ���
	
	Sprite* pSpriteTemp = Sprite::createWithSpriteFrameName(static_cast<SLevelDt*>(pLevelMgr->getDataByID(1001))->strViewImg);
	//����һ����ʱ���� pSpriteTemp�����ڻ�ȡһ���ؿ�����ʽͼ��
	
	m_pPageView->setContentSize(WINSIZE);//���� PageView �Ĵ�СΪ��Ļ�ߴ� WINSIZE��ȷ��������Ļ
	for (int i = 0; i < pLevelMgr->getsize();i++)//���Layout
	{
		Layout* pLayout = Layout::create();//����һ�� Layout ���� pLayout ��Ϊ�ؿ���������

		Sprite* pSprite=Sprite::createWithSpriteFrameName(static_cast<SLevelDt*>(pLevelMgr->getDataByID(1001 + i))->strViewImg);
		Sprite* pSpriteCard = Sprite::createWithSpriteFrameName(static_cast<SLevelDt*>(pLevelMgr->getDataByID(1001 + i))->strCardView);
        //������������ pSprite �� pSpriteCard���ֱ�����չʾ�ؿ�ͼƬ�͹ؿ���Ƭ��

		pLayout->addChild(pSprite);
		pLayout->addChild(pSpriteCard);
		pSprite->setPosition(WINSIZE.width/2,WINSIZE.height/2+50);
		pSpriteCard->setPosition(WINSIZE.width / 2, WINSIZE.height / 2-pSprite->getContentSize().height/2);
		//��������������ӵ� pLayout �����У����������ǵ�λ��
		
		m_pPageView->addPage(pLayout);
		//�� pLayout ���Ϊһ��ҳ�棬������ҳ����ӵ� PageView ��
	}

	m_pPageView->setTouchEnabled(false);
	//���� PageView �Ĵ����¼��������������ֻ����ͨ����ť����ҳ���л�

	this->addChild(m_pPageView);
	//�������õ� PageView ��ӵ���ǰ��������ʾ��
}


//���������ҷ�ҳ��ť�������û��ڹؿ�ѡ������з�ҳ����Ϊ�����ư�ť����ʾ������
//���ݵ�ǰҳ�������͹ؿ������ȼ������ƿ�ʼ��ť�����������
void CGameLevel::createButton()
{
	//������ҳ��ť
	m_pLeftButton = MenuItemImage::create("Level/LeftNormal.png"/*����״̬*/, "Level/LeftSelect.png"/*ѡ��״̬*/, [=](Ref*) {
		m_pRightButton->setVisible(true);//�����ҷ�ҳ��ť�ɼ���

		// �� PageView �ĵ�ǰҳ��������ȥ1��ʵ������ҳЧ��
		m_pPageView->setCurrentPageIndex(m_pPageView->getCurPageIndex() - 1);
		
		//�����ǰҳ������С�ڵ���0����������ҳ��ť����һҳ����ť��
		if (m_pPageView->getCurPageIndex()<=0)
		{
			m_pLeftButton->setVisible(false);
		}

		//��ȡ�ؿ����ݹ�������ʵ�� pLevelMgr�������ݵ�ǰҳ��������ؿ������ȼ����ÿ�ʼ��ť��״̬
		CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));//�õ��ؿ����ݹ�����
		
		//m_pPageView->getCurPageIndex() ��ȡ��ǰ��ѡҳ�������
		//pLevelMgr->getLockLevel() ��ȡ�ؿ��������еĹؿ������ȼ�
		if (m_pPageView->getCurPageIndex()<pLevelMgr->getLockLevel()) {
			//����û���ѡ��ҳ������С�ڹؿ������ȼ������û�ѡ��Ĺؿ�δ�ﵽ�������ĵȼ�
			m_pStartButton->setEnabled(true);//���ÿ�ʼ��ť
		}
	});
	m_pLeftButton->setVisible(false);//������ҳ��ť��ʼʱ���ɼ�


	//�����ҷ�ҳ��ť
	m_pRightButton = MenuItemImage::create("Level/RightNormal.png", "Level/RightSelect.png", [=](Ref*){
		m_pLeftButton->setVisible(true);//������ҳ��ť�ɼ���

		//�� PageView �ĵ�ǰҳ����������1��ʵ�����ҷ�ҳЧ����
		m_pPageView->setCurrentPageIndex(m_pPageView->getCurPageIndex() + 1);

		int nCount = m_pPageView->getChildrenCount();//��ȡ PageView �е�����������
		
		// �����ǰҳ���������ڵ�������������1���������ҷ�ҳ��ť�����һҳ���Ұ�ť��
		if (m_pPageView->getCurPageIndex() >= nCount-1)
		{
			m_pRightButton->setVisible(false);
		}

		//��ȡ�ؿ����ݹ�������ʵ�� pLevelMgr�������ݵ�ǰҳ��������ؿ������ȼ����ÿ�ʼ��ť��״̬
		CLevelDtMgr* pLevelMgr = static_cast<CLevelDtMgr*>(CDataMgr::getInstance()->getData("LevelMgr"));//�õ��ؿ����ݹ�����
		
		if (m_pPageView->getCurPageIndex() >= pLevelMgr->getLockLevel()){
			//����û���ѡ��ҳ���������ڻ���ڹؿ������ȼ������û�ѡ��Ĺؿ��ﵽ�򳬹��˱������ĵȼ�
			m_pStartButton->setEnabled(false);//���ÿ�ʼ��ť
		}/*111*/
	});

	
	m_pLeftButton->setPosition(80, WINSIZE.height / 2);//��ҳ��ť������Ļ��߽� 80 ����λ����ֱ��������Ļ�߶�
	m_pRightButton->setPosition(WINSIZE.width - 80, WINSIZE.height / 2);//��ʾ�ҷ�ҳ��ť������Ļ�ұ߽�80����λ����ֱ��������Ļ�߶�
	Menu* pMenu = Menu::create(m_pLeftButton, m_pRightButton, NULL);//������һ���˵�����pMenu���������ҷ�ҳ��ť��ӽ�ȥ
	pMenu->setPosition(Vec2::ZERO);//Vec2::ZERO ��ʾ��Ļ�����½ǣ�0, 0����Ϊ�˵�����ʼλ��
	this->addChild(pMenu);//���˵���ӵ���ǰ�ĳ����У��Ա���ʾ
}
