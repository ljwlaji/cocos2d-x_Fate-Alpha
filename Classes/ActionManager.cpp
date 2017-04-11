#include "ActionManager.h"
#include "Player.h"

ActionMgr::ActionMgr(Player* _player)
{
	DoingAction = 0;
	TempPlayerKeys.clear();
	DeterMineVector.clear();
	_holder = _player;
	LoadActionFromDB();
}

ActionMgr::~ActionMgr()
{

}

void ActionMgr::OnPlayerPressKey(cocos2d::EventKeyboard::KeyCode keyCode)
{
	log("press Key: %d", keyCode);
	uint16 keycode = (uint16)keyCode;
	TempPlayerKeys.push_back((uint16)keyCode);
	bool Keep;
	if (ActionType MatchId = (ActionType)CheckActionEcho(Keep))
	{
		_holder->DoAction(MatchId);
		log("play Action: %d", MatchId);
		TempPlayerKeys.clear();
	}
	if (!Keep)
		TempPlayerKeys.clear();
	else _holder->ResetKeyTimer();
}

void ActionMgr::LoadActionFromDB()
{
	Actions.clear();
	char msg[255];//			0			1			2			3	4		5	6		7		8			9
	snprintf(msg, 255, "SELECT character,actionid,require_action,key_0,key_1,key_2,key_3,key_4,action_name,require_move_type FROM Action");
	Result _result;
	if (sDataMgr->selectUnitDataList(msg, _result))
	{
		if (_result.empty()) return;

		for (Result::iterator itr = _result.begin(); itr != _result.end(); itr++)
		{
			std::vector<RowInfo> row = itr->second;
			if (row.empty()) continue;
			SingleAction _SingleAction;
			_SingleAction.Character				= row.at(0).GetInt();
			_SingleAction.ActionID				= row.at(1).GetInt();
			_SingleAction.RequireAction			= row.at(2).GetInt();
			_SingleAction.ActionName			= row.at(8).GetString();
			_SingleAction.Require_MoveType		= (MoveType)row.at(9).GetInt();
			for (int i = 3; i != 8; i++)
			{
				if (int tempkey = row.at(i).GetInt())
					_SingleAction.KeyVector.push_back(tempkey);
				else break;
			}
			Actions.push_back(_SingleAction);
		}
	}
}

ActionType ActionMgr::CheckActionEcho(bool& keepretain)
{
	if (Actions.empty())
	{
		log("empty");
		return Action_None;
	}

	//for (int i = 0; i != Actions.size(); i++)
	//{
	//	SingleAction _SingleAction = Actions.at(i);
	//	if (_SingleAction.KeyVector.empty())
	//		continue;
	//	if (_SingleAction.RequireAction && _SingleAction.RequireAction != DoingAction)
	//		continue;
	//	if (_SingleAction.Character != _holder->GetClass())
	//		continue;
	//	if (_SingleAction.Require_MoveType && _SingleAction.Require_MoveType != _holder->GetMoveType())
	//		continue;
	//
	//	bool SingleMatch = true;
	//
	//	for (int k = 0; k != TempPlayerKeys.size(); k++)
	//	{
	//		if (k - 1 > _SingleAction.KeyVector.size() || TempPlayerKeys.at(k) != _SingleAction.KeyVector.at(k))
	//		{
	//			SingleMatch = false;
	//			break;
	//		}
	//	}
	//}


	for (int i = 0; i != Actions.size(); i++)
	{
		SingleAction _SingleAction = Actions.at(i);
		if (_SingleAction.KeyVector.empty())
			continue;
		if (_SingleAction.RequireAction && _SingleAction.RequireAction != DoingAction)
			continue;
		if (_SingleAction.Character != _holder->GetClass())
			continue;
		if (_SingleAction.Require_MoveType && _SingleAction.Require_MoveType != _holder->GetMoveType())
			continue;

		bool IsMatch = true;

		//for (int j = 0; j != _SingleAction.KeyVector.size(); j++)
		//{
		//	if (TempPlayerKeys.size() != j)
		//	{
		//		break;
		//	}
		//	else
		//	{
		//		if (TempPlayerKeys.at(j) != _SingleAction.KeyVector.at(j))
		//			IsMatch = false;
		//	}
		//}
		for (int k = 0; k != TempPlayerKeys.size(); k++)
		{
			if (TempPlayerKeys.at(k) != _SingleAction.KeyVector.at(k))
			{
				IsMatch = false;
				break;
			}
		}
		IsMatch ? log("match") : log("nomatch");
		if (IsMatch)
		{
			if (TempPlayerKeys.size() < _SingleAction.KeyVector.size())
			{
				log("hold");
				keepretain = true;
				return Action_None;
			}
			TempPlayerKeys.clear();
			return (ActionType)_SingleAction.ActionID;
		}
	}
	TempPlayerKeys.clear();
	return Action_None;
}