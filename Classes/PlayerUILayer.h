#ifndef __PLAYER_UI_LAYER_H__
#define __PLAYER_UI_LAYER_H__

#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

#define sPlayerBag			PlayerBag::GetInstance()
#define sPlayerUi			PlayerUILayer::GetInstance()
#define sPlayerEquip		PlayerEquipWindow::GetInstance()
#define sPlayerSpellBook	SpellBook::GetInstance()
class Item;
class PlayerBag;
struct ButtonMenuInfo
{
	Sprite* _sprite;
	Vec2 Loc;
};
class PlayerUILayer : public Layer
{
public:
	static PlayerUILayer* GetInstance();
private:
	enum PlayerUITouchType
	{
		PlayerUITouch_None,
		PlayerUITouch_Bag,
		PlayerUITouch_SpellBook,
		PlayerUITouch_Roker,
		PlayerUITouch_Buttom_Menu,
	};
	CREATE_FUNC(PlayerUILayer);
	PlayerUILayer();
	~PlayerUILayer();
	virtual void update(float diff);
	virtual bool init();
	void CreateVirtualRoker();
	void InitUI();
	void InitButtomMenu();
	float GetVirtualRokerOrgin(Vec2 CenterPoint,Vec2 RokerPoint);
	virtual bool onTouchBegan(Touch* touches, Event *_event);
	virtual void onTouchMoved(Touch* touches, Event *_event);
	virtual void onTouchEnded(Touch* touches, Event *_event);
	void ResetVirtualRokerOrgin(float Orgin);
	void SwapButtomMenuType();
	void ButtonMenuCallBack();
	Layer* m_VirtualRokerLayer;
	Sprite* m_VirtualRoker_BackGround;
	Sprite* m_VirtualRoker_Roker;
	Vec2 visiablesize;
	Vec2 RockerLastPostion;
	Sprite* m_Player_Info_UI;
	ProgressTimer* m_Player_Info_UI_Hp;
	Sprite* m_Player_Info_UI_Hp_Back;
	Sprite* m_Player_Info_UI_Level;
	std::vector<ButtonMenuInfo> m_Buttom_Menus;
	PlayerUITouchType m_touchtype;
	Sprite* m_ButtomMenu;
	bool CanTouchButton;
};


class Slot : public Sprite
{
public:
	Slot();
	~Slot();
	Item* GetItem()				{ return m_Item; }
	bool SetItem(Item* pItem)	{ m_Item = pItem; }
private:
	Item* m_Item;
};


class PlayerEquipWindow : public Sprite
{
public:
	static PlayerEquipWindow* GetInstance();
	void SwapVisiable()			{ isVisible() ? setVisible(false) : setVisible(true); }
private:
	enum PlayerEquipSlots
	{
		SLOT_WEAPON,
		SLOT_SECOND_WEAPON,
		SLOT_AMMOR,
		SLOT_SHOES,
		SLOT_RING_1,
		SLOT_RING_2,
		SLOT_END,
	};
	void InitWindow();
	PlayerEquipWindow();
	~PlayerEquipWindow();
	std::map<PlayerEquipSlots, Slot*> m_EquipSlots;
};

class PlayerBag : public Sprite
{
public:
	static PlayerBag* GetInstance();
	bool SwapItem(Slot* slot_one, Slot* slot_two);
	void SwapPage(PlayerBagPage PageEnable, PlayerBagPage PageDisable = Page_None);
	bool onTouchBagBegan(Touch* touches);
	void onTouchBagMoved(Touch* touches);
	void onTouchBagEnded(Touch* touches);
	void SwapVisiable()			{ isVisible() ? setVisible(false) : setVisible(true); }
private:
	enum BagTouchType
	{
		Bag_Type_None,
		Bag_Type_SwapPage,
		Bag_Type_SeleItem,
	};
	PlayerBag();
	~PlayerBag();

	Slot* GetSlot(uint8 _Page, uint8 _Slot);
	void InitPage();
	void InitEmptySlots(Sprite* SinglePageSprite);
	std::string url;
	std::map<PlayerBagPage, SlotMap> m_PlayerBagTemplate;
	std::vector<Sprite*> m_PlayerBagPageSprites;

	Sprite* TouchedSprite;
	Vec2 m_Start_Move_Position;
	BagTouchType m_BagTouchType;

	PlayerBagPage m_CurrentPage;
};

#endif