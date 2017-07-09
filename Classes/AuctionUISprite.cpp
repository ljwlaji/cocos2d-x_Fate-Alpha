#include "AuctionUISprite.h"
#include "MainUIAccountSprite.h"
#pragma once
#pragma execution_character_set("utf-8")
static AcutionSprite* _AcutionSprite = nullptr;

AcutionSprite::AcutionSprite() : m_CurrentPage(AuctionPage_ListItem), m_IsInShow(false)
{
	LoadCacheFrame("Auction_House.plist");
	SetTouchType(Main_UISprite_AuctionUI_Sprite);
	initWithFile("mainuilayer_background.png");
	autorelease();
}

AcutionSprite::~AcutionSprite()
{
	_AcutionSprite = nullptr;
}

AcutionSprite* AcutionSprite::GetInstance()
{
	if (!_AcutionSprite)
		_AcutionSprite = new AcutionSprite();
	return _AcutionSprite;
}

void AcutionSprite::InitSprite()
{
	AuctionTitleFrame = FrameCreate(Auction_Title_FrameImage);
	AuctionTitleFrame->setPosition(getContentSize().width / 2, getContentSize().height - AuctionTitleFrame->getBoundingBox().size.height / 2);
	addChild(AuctionTitleFrame);


	Title = FrameCreate(Auction_Title);
	Title->setPosition(AuctionTitleFrame->getContentSize() / 2);
	AuctionTitleFrame->addChild(Title);

	Back_Arrow = FrameCreate(World_Chat_Back_Arrow);
	Back_Arrow->setPosition(AuctionTitleFrame->getContentSize().width * 0.1f, AuctionTitleFrame->getContentSize().height / 2);
	AuctionTitleFrame->addChild(Back_Arrow);


	for (int i = 0; i != 3; i++)
	{
		Sprite* TempFrame = FrameCreate(Auction_Button_Frame);
		TempFrame->setPosition(TempFrame->getBoundingBox().size.width / 2 + (i * TempFrame->getBoundingBox().size.width), AuctionTitleFrame->getBoundingBox().origin.y - TempFrame->getBoundingBox().size.height / 2);
		addChild(TempFrame);

		char msg[100];
		snprintf(msg, 100, Auction_Button_Text, i);
		Sprite* TempText = FrameCreate(msg);
		TempText->setPosition(TempFrame->getContentSize() / 2);
		TempFrame->addChild(TempText);
	}
	for (int i = AuctionPage_ListItem; i != AuctionPage_End; i++)
		InitPage((AuctionPage)i);
	//SerchButton = FrameCreate();
}

void AcutionSprite::SwapPage(AuctionPage _AuctionPage)
{
	if (m_IsInShow)
		return;
	runAction(MoveTo::create(0.5f, Vec2(getPositionX(), sAccount->getPositionY())));
	m_IsInShow = true;
}

void AcutionSprite::Dissappear()
{
	if (!m_IsInShow)
		return;
	runAction(MoveTo::create(0.5f, Vec2(getPositionX(), getContentSize().height * 1.5f)));
	m_IsInShow = false;
}

bool AcutionSprite::OnUITouchBegin(Touch* pTouch)
{
	if (!m_IsInShow)
		return false;

	if (Back_Arrow->IsContectPoint(pTouch->getLocation()))
	{
		Dissappear();
		return true;
	}

	for (std::list<Sprite*>::iterator itr = ButtonList.begin(); itr != ButtonList.end(); itr++)
	{
		if ((*itr)->isVisible() && (*itr)->IsContectPoint(pTouch->getLocation()))
		{
			_DoButtonAction((*itr)->getTag());
			return true;
		}
	}
	return true;
}

void AcutionSprite::_DoButtonAction(uint32 Tag)
{
	switch (Tag)
	{
	case Auction_Button_ListItem:
		break;
	case Auction_Button_PostItem:
		break;
	case Auction_Button_MyAuction:
		break;
	case Auction_Button_Serch:
		break;
	case Auction_Button_Input:
		break;
	case Auction_Button_Page_Back:
		break;
	case Auction_Button_Page_Next:
		break;
	default:
		break;
	}
}

void AcutionSprite::OnUITouchMoved(Touch* pTouch)
{
	if (!m_IsInShow)
		return;
}

void AcutionSprite::OnUITouchEnded(Touch* pTouch)
{

}

void AcutionSprite::AddNewItem(SingleAuctionItem _SingleAuctionItem)
{
	AuctionMap[_SingleAuctionItem.ID] = _SingleAuctionItem;
}

void AcutionSprite::InitPage(AuctionPage _AuctionPage)
{
	switch (_AuctionPage)
	{
	case AuctionPage_ListItem:
	{
		SerchFrame = FrameCreate(Auction_Serch_Frame);
		SerchFrame->setPosition(getContentSize().width / 2, AuctionTitleFrame->getBoundingBox().origin.y - FrameCreate(Auction_Button_Frame)->getBoundingBox().size.height - SerchFrame->getBoundingBox().size.height / 2);
		addChild(SerchFrame);

		SerchInput = FrameCreate(Auction_Serch_Input_Frame);
		SerchInput->setTag(Auction_Button_Input);
		SerchInput->setPosition(SerchFrame->getContentSize() / 2);
		SerchFrame->addChild(SerchInput);
		ButtonList.push_back(SerchInput);


		Serch_Button = FrameCreate(Auction_Serch_Button);
		Serch_Button->setPosition(SerchInput->getContentSize().width - Serch_Button->getBoundingBox().size.width * 0.65f, SerchInput->getContentSize().height / 2);
		SerchInput->addChild(Serch_Button);
		Serch_Button->setTag(Auction_Button_Serch);
		ButtonList.push_back(Serch_Button);

		PageBack = FrameCreate(Auction_Serch_Back_Button);
		PageNext = FrameCreate(Auction_Serch_Next_Button);
		PageBack->setPosition(getContentSize().width / 2 - PageBack->getBoundingBox().size.width, PageBack->getBoundingBox().size.height * 1.5f);
		PageNext->setPosition(getContentSize().width / 2 + PageBack->getBoundingBox().size.width, PageBack->getBoundingBox().size.height * 1.5f);
		addChild(PageBack);
		addChild(PageNext);
		ButtonList.push_back(PageBack);
		ButtonList.push_back(PageNext);
		PageNext->setTag(Auction_Button_Page_Next);
		PageBack->setTag(Auction_Button_Page_Back);


		Sprite* SerchIcon = FrameCreate(Auction_Serch_Icon);
		SerchIcon->setPosition(Serch_Button->getBoundingBox().size.width / 2, SerchInput->getContentSize().height / 2);
		SerchInput->addChild(SerchIcon);


		TextFieldTTF* TTF = TextFieldTTF::textFieldWithPlaceHolder("物品名称", "Airal", 32);
		TTF->setAnchorPoint(Vec2(0, 0.5f));
		TTF->setPosition(SerchIcon->getBoundingBox().size.width * 1.5f, SerchInput->getContentSize().height / 2);
		SerchInput->addChild(TTF);
		break;
	}
	case AuctionPage_PostItem:
		break;
	case AuctionPage_MyItems:
		break;
	}
}