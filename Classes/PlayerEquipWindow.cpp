#include "PlayerEquipWindow.h"
#include "PlayerUILayer.h"

static PlayerEquipWindow* _PlayerEquipWindow = nullptr;
static PlayerEuqipValueWindow* _PlayerEuqipValueWindow = nullptr;

PlayerEquipWindow::PlayerEquipWindow()
{
	m_TouchedSprite = nullptr;
	m_Character_ValueButton = nullptr;
	initWithFile(PlayerUIEquipFrame);
	autorelease();
	InitWindow();
	setVisible(false);
}

void PlayerEquipWindow::InitWindow()
{
	float TempXPoint = 0;
	float TempYPoint = 0;
	for (int i = SLOT_WEAPON; i != SLOT_END; i++)
	{
		Slot* TempSlot = new Slot("Player_UI_Equip_Slot_Frame.png");
		i % 2 ? TempXPoint = getContentSize().width * 0.17f : TempXPoint = getContentSize().width - getContentSize().width * 0.12f;
		uint8 fieldcount = i / 2;
		TempYPoint = getBoundingBox().size.height * 0.15f + (fieldcount * (TempSlot->getBoundingBox().size.height));
		TempSlot->setPosition(TempXPoint, TempYPoint);
		TempSlot->setTag(i);
		addChild(TempSlot);
	}
	m_Character_ValueButton = Sprite::create("Player_UI_Equip_Character_Value_Title.png");
	m_Character_ValueButton->setPosition(getContentSize().width * 0.5225f, m_Character_ValueButton->getBoundingBox().size.height * 0.75f);
	m_Character_ValueButton->setTag(ValueButtonTag);
	addChild(m_Character_ValueButton);

	m_Character_Name_Frame = Sprite::create("Player_UI_Equip_Character_Name.png");
	m_Character_Name_Frame->setPosition(m_Character_ValueButton->getPositionX(),getContentSize().height - m_Character_Name_Frame->getBoundingBox().size.height * 1.2f);
	m_Character_Name_Frame->setTag(Name_Frame_Tag);
	addChild(m_Character_Name_Frame);
}


void PlayerEquipWindow::onTouchBagBegan(Touch* touches)
{
	m_TouchedSprite = nullptr;

	for (Vector<Node*>::iterator itr = getChildren().begin(); itr != getChildren().end(); itr++)
	{
		if ((*itr)->IsContectPoint(touches->getLocation()))
		{
			m_TouchedSprite = (Sprite*)(*itr);
			return;
		}
	}

	m_TouchedSprite = this;
	m_Start_Move_Position = touches->getLocation();
}

void PlayerEquipWindow::onTouchBagMoved(Touch* touches)
{
	if (m_TouchedSprite == this)
	{
		float X_Modify = touches->getLocation().x - m_Start_Move_Position.x;
		float Y_Modify = touches->getLocation().y - m_Start_Move_Position.y;
		setPosition(getPositionX() + X_Modify, getPositionY() + Y_Modify);
		sPlayerValueWindow->setPosition(sPlayerValueWindow->getPositionX() + X_Modify, sPlayerValueWindow->getPositionY() + Y_Modify);
		m_Start_Move_Position = touches->getLocation();
	}
}

void PlayerEquipWindow::onTouchBagEnded(Touch* touches)
{
	if (!m_TouchedSprite || !m_TouchedSprite->IsContectPoint(touches->getLocation()))
		return;
	switch (m_TouchedSprite->getTag())
	{
	case PlayerEquipWindow::SLOT_WEAPON:
	case PlayerEquipWindow::SLOT_SECOND_WEAPON:
	case PlayerEquipWindow::SLOT_AMMOR:
	case PlayerEquipWindow::SLOT_SHOES:
	case PlayerEquipWindow::SLOT_RING_1:
	case PlayerEquipWindow::SLOT_RING_2:
	case PlayerEquipWindow::SLOT_RING_3:
	case PlayerEquipWindow::SLOT_RING_4:
	case PlayerEquipWindow::SLOT_END:
		OnClickedItemSlot(m_TouchedSprite->getTag());
		break;
	case ValueButtonTag:
		sPlayerValueWindow->isVisible() ? sPlayerValueWindow->setVisible(false) : sPlayerValueWindow->setVisible(true);
		break;
	case Name_Frame_Tag:
		break;
	}
}

void PlayerEquipWindow::OnClickedItemSlot(uint32 _tag)
{

}

void PlayerEquipWindow::SwapVisiable()			
{ 
	if (isVisible())
		setVisible(false);
	else
	{
		setVisible(true);
		if (!sPlayerValueWindow->isVisible())
			sPlayerValueWindow->setVisible(true);
	}
}

PlayerEquipWindow::~PlayerEquipWindow()
{
}

PlayerEquipWindow* PlayerEquipWindow::GetInstance()
{
	if (!_PlayerEquipWindow)
		_PlayerEquipWindow = new PlayerEquipWindow();
	return _PlayerEquipWindow;
}

PlayerEuqipValueWindow::PlayerEuqipValueWindow()
{
	_PlayerEuqipValueWindow = nullptr;
	initWithFile(PlayerUIEquipValueFrame);
	autorelease();
	setVisible(false);
}

PlayerEuqipValueWindow::~PlayerEuqipValueWindow()
{
}


PlayerEuqipValueWindow* PlayerEuqipValueWindow::GetInstance()
{
	if (!_PlayerEuqipValueWindow)
		_PlayerEuqipValueWindow = new PlayerEuqipValueWindow();
	return _PlayerEuqipValueWindow;
}
