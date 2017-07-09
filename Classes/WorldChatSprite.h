#ifndef __WORLD_CHAT_H__
#define __WORLD_CHAT_H__

#include "UISprite.h"
#include "Common.h"


struct ChatInfo
{
	uint32 ChatTime;
	bool IsInputMessage;
	std::string SendBy;
	BattleNet_Message_Channel Channel;
	std::string Message;
	Sprite* LinkedSprite;
	LabelTTF* LinkedLabel;
};

#define sWorldChat WorldChatSprite::GetInstance()

class WorldChatSprite : public UISprite
{
public:
	static WorldChatSprite* GetInstance();
	void InitSprite();
	void PushNewMessage(uint8 Channel, const std::string& Message, std::string SendBy = "", bool Input = false);
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
private:
	WorldChatSprite();
	~WorldChatSprite();
	void _fadein(Sprite* pSprte);
	void _fadeout(Sprite* pSprite);
	void AddNewMessageToList(ChatInfo& info);
	void SetSelectedChannel(BattleNet_Message_Channel _channel)	{ SelectedChannel = _channel; }
	Sprite* Title;
	Sprite* _TitleFrame;
	Sprite* Back_Arrow;
	Sprite* Buttom_Frame;
	Sprite* Select_Channel;
	Sprite* Channel_World;
	Sprite* Channel_Guild;
	Sprite* InputFrame;
	Sprite* SendButton;
	TextFieldTTF* TextTyping;

	std::list<ChatInfo> MessageList;

	BattleNet_Message_Channel SelectedChannel;
	virtual void update(float diff);

	float diffa;
};



#endif