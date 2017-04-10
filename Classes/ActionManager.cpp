#include "ActionManager.h"

static ActionManager* _ActionManager = nullptr;

ActionManager::ActionManager()
{
	TempPlayerKeys.clear();
}

ActionManager::~ActionManager()
{

}

ActionManager* ActionManager::GetInstance()
{
	if (!_ActionManager)
		_ActionManager = new ActionManager();
	return _ActionManager;
}

void ActionManager::OnPlayerPressKey(cocos2d::EventKeyboard::KeyCode keyCode)
{

}