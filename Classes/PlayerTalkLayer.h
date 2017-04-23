#ifndef __PLAYER_TALK_LAYER_H__
#define __PLAYER_TALK_LAYER_H__

#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

struct ShowingTalkClass
{
	uint32 ImageID = 0;
	uint32 sender = 0;
	uint32 action = 0;
	std::string talkstring;
	Sprite* SingleTalkClass = nullptr;
};
#define sPlayerTalkLayer Player_Talk_Layer::GetInstance()
class Player_Talk_Layer : public Layer
{
public:
	Player_Talk_Layer();
	~Player_Talk_Layer();
	static Player_Talk_Layer* GetInstance();
	CREATE_FUNC(Player_Talk_Layer);
	void SendMenuToPlayer(std::string MainString);
	void OnPlayerClickOnTalkClass();
	void InitTalkClass();
	void OnPlayerTouchedFrame( uint32 sender, uint32 action);
	void CloseMenuWithCleanUp(bool cleanup = true);
private:
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	Sprite* TalkClassFrame;
	std::vector<ShowingTalkClass> m_Showing_Message;
	virtual bool init();
	void _Send();
	Vec2 VisiableSize;
	int8 m_Touched_Frame;
};

#endif