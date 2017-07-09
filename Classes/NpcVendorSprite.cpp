#include "NpcVendorSprite.h"
#include "HelloWorldScene.h"
#include "NotifyMgr.h"

static NpcVendorSprite* _NpcVendorSprite = nullptr;

NpcVendorSprite::NpcVendorSprite()
{
	/*PlayerUIVendorSingleIconFrame	
	  PlayerUIVendorSingleFrame		
	  PlayerUIVendorFrame				
	  PlayerUIVendorCloseButton		
	*/
	initWithFile(PlayerUIVendorFrame);
	autorelease();
	InitFrame();
	setVisible(false);
	SetTouchType(PlayerUITouch_NpcVendorSprite);
}

NpcVendorSprite::~NpcVendorSprite()
{
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
	_NpcVendorSprite = nullptr;
}

void NpcVendorSprite::InitFrame()
{
	CloseButton = Sprite::create(PlayerUIVendorCloseButton);
	CloseButton->setPosition(getContentSize().width - CloseButton->getBoundingBox().size.width, getContentSize().height - CloseButton->getBoundingBox().size.height);
	addChild(CloseButton);

	RepairButton = Sprite::create(PlayerUIVendorRepairButton);
	RepairButton->setPosition(getContentSize().width - RepairButton->getBoundingBox().size.width * 1.3f, CloseButton->getPositionY());
	addChild(RepairButton);
}

NpcVendorSprite* NpcVendorSprite::GetInstance()
{
	if (!_NpcVendorSprite)
		_NpcVendorSprite = new NpcVendorSprite();
	return _NpcVendorSprite;
}

void NpcVendorSprite::HandleListScroll()
{
	if (ShowingList.empty())
		return;
	if (MovePoint.y < 0)
	{
		if (Sprite* psp = *ShowingList.begin())
		{
			if (psp->getBoundingBox().origin.y + psp->getBoundingBox().size.height < getContentSize().height * 0.9f)
				return;
		}
	}
	else
	{
		std::list<Sprite*>::iterator itr = ShowingList.end();
		--itr;
		if ((*itr)->getBoundingBox().origin.y > 0)
			return;
	}
	for (std::list<Sprite*>::iterator itr = ShowingList.begin(); itr != ShowingList.end(); itr++)
	{
		Sprite* TempSp = (*itr);
		TempSp->setPositionY(TempSp->getPositionY() + MovePoint.y);
		if (IsSpriteOutOfRange(TempSp) && TempSp->isVisible())
			TempSp->setVisible(false);
		else if (!IsSpriteOutOfRange(TempSp) && !TempSp->isVisible())
			TempSp->setVisible(true);
	}
}

bool NpcVendorSprite::OnUITouchBegin(Touch* pTouch)
{
	m_TouchedSprite = nullptr;
	CanClickMenu = true;

	for (std::list<Sprite*>::iterator itr = ShowingList.begin(); itr != ShowingList.end(); itr++)
	{
		if ((*itr)->IsContectPoint(pTouch->getLocation()) && (*itr)->isVisible())
		{
			m_TouchedSprite = (*itr);
			LastPoint = pTouch->getLocation();
			return true;
		}
	}

	if (CloseButton->IsContectPoint(pTouch->getLocation()))
	{
		m_TouchedSprite = CloseButton;
		return true;
	}

	if (RepairButton->isVisible() && RepairButton->IsContectPoint(pTouch->getLocation()))
	{
		m_TouchedSprite = RepairButton;
		return true;
	}
	LastPoint = pTouch->getLocation();
	m_TouchedSprite = this;
	return true;
}

void NpcVendorSprite::OnUITouchMoved(Touch* pTouch)
{
	if (m_TouchedSprite == CloseButton)
		return;

	if (m_TouchedSprite == this)
	{
		MovePoint = ccpSub(pTouch->getLocation(), LastPoint);
		setPosition(getPositionX() + MovePoint.x, getPositionY() + MovePoint.y);
		LastPoint = pTouch->getLocation();
	}
	else
	{
		MovePoint.y = pTouch->getLocation().y - LastPoint.y;
		HandleListScroll();
		LastPoint.y = pTouch->getLocation().y;
		CanClickMenu = false;
	}
}

void NpcVendorSprite::OnUITouchEnded(Touch* pTouch)
{
	if (m_TouchedSprite == CloseButton && CloseButton->IsContectPoint(pTouch->getLocation()))
	{
		setVisible(false);
		return;
	}
	if (m_TouchedSprite == RepairButton && RepairButton->IsContectPoint(pTouch->getLocation()))
	{
		sPlayer->RepairItems();
		return;
	}
	if (m_TouchedSprite != this && CanClickMenu)
	{
		if (m_TouchedSprite->IsContectPoint(pTouch->getLocation()))
		{
			uint32 Cost = sGame->GetItemTemplate(m_TouchedSprite->getTag())->BuyPrice;
			if (sPlayer->GetMoney() >= Cost)
			{
				sPlayer->SetMoney(sPlayer->GetMoney() - Cost);
				sPlayer->SendUpdateValueRequire();
				sPlayer->AddItem(m_TouchedSprite->getTag(), 1);
			}
			else
				sNotifyMgr->ShowNotify("Not Enough Money");
		}
	}
}

std::string itoc(int i)
{
	char msg[255];
	snprintf(msg, 255, "%d", i);
	return msg;
}

void NpcVendorSprite::AddNewItemToList(uint32 ItemID, uint32 Exchange_Cost)
{

	if (const ItemTemplate* TempTemplate = sGame->GetItemTemplate(ItemID))
	{
		Sprite* TempFrame = Sprite::create(PlayerUIVendorSingleFrame);
		TempFrame->setAnchorPoint(Vec2(0, 0.5f));
		TempFrame->setPosition(getContentSize().width * 0.06f, getContentSize().height * 0.8f - (ShowingList.size() * TempFrame->getBoundingBox().size.height));
		TempFrame->setTag(TempTemplate->Entry);
		addChild(TempFrame);

		Sprite* TempIconFrame = Sprite::create(PlayerUIVendorSingleIconFrame);
		TempIconFrame->setPosition(TempFrame->getContentSize().width * 0.15f, TempFrame->getContentSize().height / 2);
		TempFrame->addChild(TempIconFrame);

		Sprite* ItemIcon = Sprite::create(TempTemplate->Url.c_str());
		ItemIcon->setPosition(TempIconFrame->getContentSize().width / 2, TempIconFrame->getContentSize().height / 2);
		TempIconFrame->addChild(ItemIcon);

		LabelTTF* NameTTF = LabelTTF::create(TempTemplate->Name.c_str(), "Arial", 28, Size::ZERO, TextHAlignment::LEFT);
		NameTTF->setAnchorPoint(Vec2(0, 0.5f));
		NameTTF->setPosition(TempFrame->getContentSize().width * 0.28f, TempFrame->getContentSize().height * 0.5f + NameTTF->getBoundingBox().size.height * 0.6f);
		TempFrame->addChild(NameTTF);

		uint32 Cost = TempTemplate->BuyPrice;
		uint32 Gold = Cost / 10000;
		uint32 Sliv = Cost % 10000 / 100;
		uint32 Coin = Cost % 100;
		for (int i = 0; i != 3; i++)
		{
			LabelTTF* Cost;
			switch (i)
			{
			case 0:
				Cost = LabelTTF::create(itoc(Gold).c_str(), "Arial", 28, Size::ZERO, TextHAlignment::RIGHT);
				Cost->setPosition(TempFrame->getContentSize().width * 0.3f, TempFrame->getContentSize().height / 2 - Cost->getBoundingBox().size.height * 0.6f);
				break;
			case 1:
				Cost = LabelTTF::create(itoc(Sliv).c_str(), "Arial", 28, Size::ZERO, TextHAlignment::RIGHT);
				Cost->setPosition(TempFrame->getContentSize().width * 0.45f, TempFrame->getContentSize().height / 2 - Cost->getBoundingBox().size.height * 0.6f);
				break;
			case 2:
				Cost = LabelTTF::create(itoc(Coin).c_str(), "Arial", 28, Size::ZERO, TextHAlignment::RIGHT);
				Cost->setPosition(TempFrame->getContentSize().width * 0.6f, TempFrame->getContentSize().height / 2 - Cost->getBoundingBox().size.height * 0.6f);
				break;
			}
			TempFrame->addChild(Cost);
		}
		if (ShowingList.size() >= 5)
			TempFrame->setVisible(false);

		ShowingList.push_back(TempFrame);
	}
}


bool NpcVendorSprite::ShowVendorList(uint32 Creature_id)
{
	for (std::list<Sprite*>::iterator itr = ShowingList.begin(); itr != ShowingList.end(); itr++)
		(*itr)->removeFromParentAndCleanup(true);
	ShowingList.clear();

	if (const VendorList* TempList = sGame->GetCreatureVendorInfo(Creature_id))
	{
		for (VendorList::const_iterator itr = TempList->begin(); itr != TempList->end(); itr++)
			AddNewItemToList(itr->Item_Id, itr->Exchange_Cost);

		setVisible(true);
		return true;
	}
	return false;
}