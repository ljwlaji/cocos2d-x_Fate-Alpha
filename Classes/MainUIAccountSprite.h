#ifndef __ACCOUNT_SPRITE_H__
#define __ACCOUNT_SPRITE_H__

#include "UISprite.h"

#define sAccount AccountSprite::GetInstance()

class AccountSprite : public UISprite
{
public:
	static AccountSprite* GetInstance();

	void InitSprite();
private:
	enum MainButtons
	{
		MyAccount = 100,
		Notify = 200,
		Guild = 300,
		Auction = 400,
		Setting = 500,
		End = 600,
	};
	
	enum SubButtons
	{
		MyAccount_Service = 1,
		MyAccount_Infomation = 2,
		MyAccount_End = 3,


		//AutoNotify
		NotifyStart = 101,
		NotifyEnd = 199,

		Guild_Bank = 201,
		Guild_Notify = 202,
		Guild_Point = 203,
		Guild_Settings = 204,
		Guild_Member = 205,
		Guild_End,

		Auction_PostItem = 301,
		Auction_Serch = 302,
		Auction_MyAuction = 303,
		Auction_End,

		Setting_SetLinked_Character = 401,
		Setting_BattleNet_Name = 402,
		Setting_End,
	};
	typedef std::map<Sprite*, std::map<SubButtons, Sprite*>> ButtonMap;
#define ArrowTag 9999
	AccountSprite();
	~AccountSprite();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	void _setvisable(Sprite*pSprite)	{ pSprite->setVisible(pSprite->isVisible() ? false : true); }
	void ResetClick()					{ CanTouch = true; }
	void _InitSubButtons(Sprite* _button);
	void OnTouchMainButton(Sprite* pSprite);
	void DoLinkMove(Sprite* pSprite,float MovePos);
	bool IsActiveMainButton(Sprite* pSprite);


	bool CanTouch;
	Sprite* Title;
	Sprite* CancelButton;
	Sprite* NotifyFrame;
	Sprite* NotifyText;

	ButtonMap Buttons;
};


#endif