#ifndef __ACTION_MANAGER_H__
#define __ACTION_MANAGER_H__

#include "Types.h"


enum KeyValue
{
	//移动左
	KEY_A,
	//移动右
	KEY_S,
	//移动上
	KEY_D,
	//移动下
	KEY_W,
	//攻击
	KEY_J,
	//跳跃
	KEY_K,
	//道具
	KEY_L,
	//释放
	KEY_I,
};
struct SingleAction
{
	uint16 UperActionID;
};

class Player;
class ActionManager
{
public:
	ActionManager();
	~ActionManager();
	static ActionManager* GetInstance();
	void OnPlayerPressKey(cocos2d::EventKeyboard::KeyCode keyCode);
	bool CanDoAction(Player* _player);
private:

	std::vector<uint16> TempPlayerKeys;
};
#endif