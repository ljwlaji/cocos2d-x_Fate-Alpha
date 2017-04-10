#ifndef __ACTION_MANAGER_H__
#define __ACTION_MANAGER_H__

#include "Types.h"


enum KeyValue
{
	//�ƶ���
	KEY_A,
	//�ƶ���
	KEY_S,
	//�ƶ���
	KEY_D,
	//�ƶ���
	KEY_W,
	//����
	KEY_J,
	//��Ծ
	KEY_K,
	//����
	KEY_L,
	//�ͷ�
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