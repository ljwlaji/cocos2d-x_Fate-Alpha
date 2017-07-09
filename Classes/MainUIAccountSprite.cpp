#include "MainUIAccountSprite.h"
#include "Common.h"
#include "WorldPacket.h"
#include "SocketClient.h"
#include "AuctionUISprite.h"
static AccountSprite* _AccountSprite = nullptr;

AccountSprite::AccountSprite()
{
	LoadCacheFrame("Account_Frame.plist");
	initWithFile(Account_Sprite_Frame);
	autorelease();
	SetTouchType(Main_UISprite_Account_Sprite);
}

AccountSprite::~AccountSprite()
{
	removeFromParentAndCleanup(true);
	_AccountSprite = nullptr;
}

void AccountSprite::InitSprite()
{
	Title = FrameCreate(Account_Sprite_Title);
	Title->setPosition(getContentSize().width / 2, getContentSize().height - Title->getBoundingBox().size.height * 0.6f);
	addChild(Title);

	CancelButton = FrameCreate(Account_Sprite_Logout_Button);
	CancelButton->setPosition(getContentSize().width / 2, CancelButton->getBoundingBox().size.height);
	addChild(CancelButton);


	Buttons.clear();
	//InitMyAccount
	for (int i = MyAccount; i != End; i += 100)
	{
		//Sprite* Frame = FrameCreate(Account_Sprite_Main_ButtonFrame);
		Sprite* Frame = Sprite::create("testframe.png");
		Frame->setLocalZOrder(i);
		Frame->setTag(i);
		Frame->setAnchorPoint(Vec2(0, 0.5f));
		Frame->setPosition(0, Title->getBoundingBox().origin.y - ((i / 100) * Frame->getBoundingBox().size.height));
		addChild(Frame);

		Sprite* Text = nullptr;
		switch (i)
		{
		case AccountSprite::MyAccount:
			Text = FrameCreate(Account_Sprite_Text_MyAccount);
			break;
		case AccountSprite::Notify:
			Text = FrameCreate(Account_Sprite_Text_Notify);
			break;
		case AccountSprite::Guild:
			Text = FrameCreate(Account_Sprite_Text_Guild);
			break;
		case AccountSprite::Auction:
			Text = FrameCreate(Account_Sprite_Text_Auction);
			break;
		case AccountSprite::Setting:
			Text = FrameCreate(Account_Sprite_Text_AccountSettings);
			break;
		}
		Text->setAnchorPoint(Vec2(0, 0.5f));
		Text->setPosition(Frame->getContentSize().width * 0.05f, Frame->getContentSize().height / 2);
		Frame->addChild(Text);

		Sprite* Arrow = FrameCreate(Account_Sprite_Arrow);
		Arrow->setTag(ArrowTag);
		Arrow->setRotation(90.0f);
		Arrow->setPosition(Frame->getContentSize().width - Arrow->getBoundingBox().size.width, Frame->getContentSize().height / 2);
		Frame->addChild(Arrow);
		_InitSubButtons(Frame);
	}
}

AccountSprite* AccountSprite::GetInstance()
{
	if (!_AccountSprite)
		_AccountSprite = new AccountSprite();
	return _AccountSprite;
}

bool AccountSprite::OnUITouchBegin(Touch* pTouch)
{
	if (!CanTouch)
		return true;

	for (ButtonMap::iterator itr = Buttons.begin(); itr != Buttons.end(); itr++)
	{
		if (itr->first->IsContectPoint(pTouch->getLocation()))
		{
			OnTouchMainButton(itr->first);
			runAction(Sequence::create(DelayTime::create(0.7f), CallFunc::create(CC_CALLBACK_0(AccountSprite::ResetClick, this)), NULL));
			CanTouch = false;
			return true;
		}

		for (std::map<SubButtons, Sprite*>::iterator ito = itr->second.begin(); ito != itr->second.end(); ito++)
		{
			if (ito->second->isVisible() && ito->second->IsContectPoint(pTouch->getLocation()))
			{
				switch (ito->first)
				{
				case AccountSprite::MyAccount_Service:
					break;
				case AccountSprite::MyAccount_Infomation:
					break;
				case AccountSprite::Guild_Bank:
					break;
				case AccountSprite::Guild_Notify:
					break;
				case AccountSprite::Guild_Point:
					break;
				case AccountSprite::Guild_Settings:
					break;
				case AccountSprite::Guild_Member:
					break;
				case AccountSprite::Auction_PostItem:
					sAuction->SwapPage(AuctionPage_PostItem);
					break;
				case AccountSprite::Auction_Serch:
					sAuction->SwapPage(AuctionPage_ListItem);
					break;
				case AccountSprite::Auction_MyAuction:
					sAuction->SwapPage(AuctionPage_MyItems);
					break;
				case AccountSprite::Setting_SetLinked_Character:
					break;
				case AccountSprite::Setting_BattleNet_Name:
					break;
				}
			}
		}
	}
	return true;
}

void AccountSprite::OnUITouchMoved(Touch* pTouch)
{

}
void AccountSprite::OnUITouchEnded(Touch* pTouch)
{

}


void AccountSprite::OnTouchMainButton(Sprite* pSprite)
{
	bool IsActive = IsActiveMainButton(pSprite);
	float MovePosition = 0;
	for (std::map<SubButtons, Sprite*>::iterator itr = Buttons[pSprite].begin(); itr != Buttons[pSprite].end(); itr++)
	{
		MovePosition += itr->second->getBoundingBox().size.height;
		if (IsActive)
			MovePosition = 0 - (pSprite->getPositionY() - itr->second->getPositionY());

		itr->second->runAction(Sequence::create(DelayTime::create(IsActive ? 0.5f : 0), CallFunc::create(CC_CALLBACK_0(AccountSprite::_setvisable, this, itr->second)), NULL));

		itr->second->runAction(MoveTo::create(0.5f, Vec2(itr->second->getPositionX(), itr->second->getPositionY() - MovePosition)));
	}
	pSprite->getChildByTag(ArrowTag)->runAction(RotateTo::create(0.5f, IsActive ? 90.0f : 0));
	for (ButtonMap::iterator itr = Buttons.begin(); itr != Buttons.end(); itr++)
	{
		if (itr->first == pSprite || itr->first->getTag() < pSprite->getTag())
			continue;
		DoLinkMove(itr->first, MovePosition);
	}
}

void AccountSprite::DoLinkMove(Sprite* pSprite,float MovePos)
{
	pSprite->runAction(MoveTo::create(0.5f, Vec2(pSprite->getPositionX(), pSprite->getPositionY() - MovePos)));
	for (std::map<SubButtons, Sprite*>::iterator itr = Buttons[pSprite].begin(); itr != Buttons[pSprite].end(); itr++)
	{
		Sprite* TempSprite = itr->second;
		TempSprite->runAction(MoveTo::create(0.5f, Vec2(TempSprite->getPositionX(), TempSprite->getPositionY() - MovePos)));
	}
}

bool AccountSprite::IsActiveMainButton(Sprite* pSprite)
{
	if (pSprite->getChildByTag(ArrowTag)->getRotation() == 90.0f)
		return false;
	return true;
}

void AccountSprite::_InitSubButtons(Sprite* _button)
{
	uint16 StartTag = 0;
	uint16 EndTag = 0;
	switch (_button->getTag())
	{
	case AccountSprite::MyAccount:
		StartTag = MyAccount_Service;
		EndTag = MyAccount_End;
		break;
	case AccountSprite::Notify:
		Buttons[_button];
		return;
	case AccountSprite::Guild:
		StartTag = Guild_Bank;
		EndTag = Guild_End;
		break;
	case AccountSprite::Auction:
		StartTag = Auction_PostItem;
		EndTag = Auction_End;
		break;
	case AccountSprite::Setting:
		StartTag = Setting_SetLinked_Character;
		EndTag = Setting_End;
		break;
	}

	if (!StartTag || !EndTag)
		return;

	for (; StartTag != EndTag; StartTag++)
	{
		Sprite* Frame = FrameCreate(Account_Sprite_Sub_ButtonFrame);
		Frame->setAnchorPoint(Vec2(0, 0.5f));
		Frame->setPosition(_button->getPosition());
		Frame->setLocalZOrder(StartTag);
		Frame->setTag(StartTag);
		Frame->setVisible(false);
		addChild(Frame);
		char msg[255];
		snprintf(msg, 255, Account_Sprite_Text_Sub_Text, StartTag);
		Sprite* Text = FrameCreate(msg);
		Text->setAnchorPoint(Vec2(0, 0.5f));
		Text->setPosition(Frame->getContentSize().width * 0.08f, Frame->getContentSize().height / 2);
		Frame->addChild(Text);
		Buttons[_button][(SubButtons)StartTag] = Frame;
	}
}