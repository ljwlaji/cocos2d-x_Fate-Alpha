#ifndef __PLAYER_EQUIP_WINDOW_H__
#define __PLAYER_EQUIP_WINDOW_H__
#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

#define sPlayerEquip		PlayerEquipWindow::GetInstance()
#define sPlayerValueWindow	PlayerEuqipValueWindow::GetInstance()
class Slot;
class PlayerEquipWindow : public Sprite
{
public:
	static PlayerEquipWindow* GetInstance();
	void SwapVisiable();
	void onTouchBagBegan(Touch* touches);
	void onTouchBagMoved(Touch* touches);
	void onTouchBagEnded(Touch* touches);
private:
	enum PlayerEquipSlots
	{
		SLOT_WEAPON,
		SLOT_SECOND_WEAPON,
		SLOT_AMMOR,
		SLOT_SHOES,
		SLOT_RING_1,
		SLOT_RING_2,
		SLOT_RING_3,
		SLOT_RING_4,
		SLOT_END,
	};
	void InitWindow();
	PlayerEquipWindow();
	~PlayerEquipWindow();
	void OnClickedItemSlot(uint32 _tag);

private:
	Sprite* m_Character_ValueButton;
	Sprite* m_Character_Name_Frame;
#define ValueButtonTag 9
#define Name_Frame_Tag 10
	std::map<PlayerEquipSlots, Slot*> m_EquipSlots;
	Vec2 m_Start_Move_Position;
	Sprite* m_TouchedSprite;
};

class PlayerEuqipValueWindow : public Sprite
{
public:
	static PlayerEuqipValueWindow* GetInstance();
private:
	PlayerEuqipValueWindow();
	~PlayerEuqipValueWindow();
};

#endif