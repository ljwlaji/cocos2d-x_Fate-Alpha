#ifndef __PLAYER_TALK_LAYER_H__
#define __PLAYER_TALK_LAYER_H__

#include "Types.h"

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
	static Player_Talk_Layer* GetInstance();
	void SendMenuToPlayer(const std::string& MainString);
	void OnPlayerClickOnTalkClass();
	void InitTalkClass();
	void SendQuestMenuToPlayer(uint32 CreatureID);
	void OnPlayerTouchedFrame( uint32 sender, uint32 action);
	void CloseMenuWithCleanUp(bool cleanup = true);
	void SetQuestTalking(bool _var)			{ IsQuestTalking = _var; }
private:
	Player_Talk_Layer();
	~Player_Talk_Layer();
	virtual bool init();
	void _Send();

	CREATE_FUNC(Player_Talk_Layer);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	std::vector<ShowingTalkClass> m_Showing_Message;
	EventListenerTouchOneByOne* listener;
	Vec2 VisiableSize;
	uint32 m_QuestID;

	LabelTTF* TalkClassString;
	Sprite* TalkClassFrame;
	Sprite* CancelButton;
	Sprite* ComfirmButton;
	Sprite* m_Touched_Frame;
	Sprite* SpriteFrame[4];
	Sprite* ButtonFrame;

	bool IsQuestTalking;
};

#endif