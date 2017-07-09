#ifndef __AUCTION_SPRITE_H__
#define __AUCTION_SPRITE_H__


#include "UISprite.h"
#include "Common.h"

struct SingleAuctionItem
{
public:
	SingleAuctionItem(){};
	SingleAuctionItem(uint32& id, std::string& Owner, uint32& AuctionID, std::string& ItemName, uint32& BuyOutPrice, uint32& Time, uint32& LastBid, uint32& StartBid) :
	ID(id), OwnerName(Owner), AuctionID(AuctionID), ItemName(ItemName), BuyOutPrice(BuyOutPrice), Time(Time), LastBid(LastBid), StartBid(StartBid)
	{}
	uint32 ID;
	std::string OwnerName;
	uint32 AuctionID;
	std::string ItemName;
	uint32 BuyOutPrice;
	uint32 Time;
	uint32 LastBid;
	uint32 StartBid;
};

enum AuctionPage
{
	AuctionPage_ListItem,
	AuctionPage_PostItem,
	AuctionPage_MyItems,
	AuctionPage_End,
};

enum Buttons
{
	Auction_Button_ListItem = 1,
	Auction_Button_PostItem,
	Auction_Button_MyAuction,

	Auction_Button_Serch,
	Auction_Button_Input,
	Auction_Button_Page_Back,
	Auction_Button_Page_Next,
};

#define sAuction AcutionSprite::GetInstance()

class AcutionSprite : public UISprite
{
public:
	static AcutionSprite* GetInstance();
	void AddNewItem(SingleAuctionItem _SingleAuctionItem);
	void InitSprite();
	void SwapPage(AuctionPage _AuctionPage);
	bool IsInShow()	{ return m_IsInShow; }
	virtual bool OnUITouchBegin(Touch* pTouch);
private:
	AcutionSprite();
	~AcutionSprite();
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	void _DoButtonAction(uint32 Tag);
	void Dissappear();
	void InitPage(AuctionPage _AuctionPage);
	std::map<uint32,SingleAuctionItem> AuctionMap;

	Sprite* Title;
	Sprite* AuctionTitleFrame;
	Sprite* Back_Arrow;
	Sprite* SerchFrame;
	Sprite* SerchButton;
	Sprite* SerchInput;
	Sprite* Serch_Button;
	Sprite* PageBack;
	Sprite* PageNext;
	AuctionPage m_CurrentPage;
	bool m_IsInShow;

	std::list<Sprite*> ButtonList;
};
#endif