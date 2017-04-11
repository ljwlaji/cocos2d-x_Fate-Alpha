#ifndef __ACTION_MANAGER_H__
#define __ACTION_MANAGER_H__

#include "Types.h"
struct SingleAction
{
	uint8 Character;
	uint16 RequireAction;
	std::vector<uint16> KeyVector;
	uint16 ActionID;
	std::string ActionName;
	MoveType Require_MoveType;
};

#define MatchNull 0

class Player;
class ActionMgr
{
public:
	ActionMgr(Player* _player);
	~ActionMgr();
	void ClearKey()				{ TempPlayerKeys.clear(); }
	void OnPlayerPressKey(cocos2d::EventKeyboard::KeyCode keyCode);
	void LoadActionFromDB();
	ActionType CheckActionEcho(bool& keepretain);
private:

	std::vector<uint16> TempPlayerKeys;
	std::vector<SingleAction> Actions;
	std::vector<SingleAction> DeterMineVector;
	uint16 DoingAction;

	Player* _holder;
};
#endif