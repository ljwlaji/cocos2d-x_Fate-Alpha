#ifndef __PLAYER_EQUIP_WINDOW_H__
#define __PLAYER_EQUIP_WINDOW_H__
#include "cocos2d.h"
#include "Types.h"
#include "UISprite.h"
USING_NS_CC;

#define sPlayerEquip		PlayerEquipWindow::GetInstance()
#define sPlayerValueWindow	PlayerEuqipValueWindow::GetInstance()
class Slot;
class PlayerEquipWindow : public UISprite
{
public:
	Slot* GetSlotByTouch(Touch* toouch);
	static PlayerEquipWindow* GetInstance();
	void SwapVisiable();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
private:
	bool IsTouchedDisPlaySprite;
	void InitWindow();
	PlayerEquipWindow();
	~PlayerEquipWindow();
	void OnClickedItemSlot(const uint32& _tag);

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
	void SwapVisable()	{ isVisible() ? setVisible(false) : setVisible(true); }
	void Init();
	void ResetValueDefault();
private:
	enum ValueWindowTTF
	{
		Attack,
		Def,
		Str,
		Dex,
		Int,
		TTF_END,
	};
	PlayerEuqipValueWindow();
	~PlayerEuqipValueWindow();

	std::map<ValueWindowTTF, LabelTTF*> m_TTFs;
	std::map<ValueWindowTTF, LabelTTF*> PlusTTF;
};

#endif