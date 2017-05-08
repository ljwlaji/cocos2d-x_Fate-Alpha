#ifndef __PLAYER_UI_LAYER_H__
#define __PLAYER_UI_LAYER_H__

#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

#define sPlayerBag			PlayerBag::GetInstance()
#define sPlayerUi			PlayerUILayer::GetInstance()
class Item;
class PlayerBag;
class SpellSlot;
struct ButtonMenuInfo
{
	Sprite* _sprite;
	Vec2 Loc;
};
class PlayerUILayer : public Layer
{
public:
	static PlayerUILayer* GetInstance();
	void ReSetSpellFramePosition();
	void SwapSpellFrameVisable();
	void ReSetSpellFrameText(uint32 SpellID);
	SpellSlot* GetContactButtonSlot(const Vec2& Loc);
	void ResetUpButtonString();
private:
	enum PlayerUITouchType
	{
		PlayerUITouch_None,
		PlayerUITouch_UperButton,
		PlayerUITouch_Bag,
		PlayerUITouch_SpellBook,
		PlayerUITouch_Roker,
		PlayerUITouch_Buttom_Menu,
		PlayerUITouch_Button_SpellSlot,
		PlayerUITouch_Equip_Window,
		PlayerUITouch_SettingMenu,
	};
	enum TopButtonLabelTTF
	{
		MoneyTTF,
		AttackTTF,
		CashTTF,
		NameTTF,
		EndOfTopTTF,
	};
	CREATE_FUNC(PlayerUILayer);
	PlayerUILayer();
	~PlayerUILayer();
	virtual void update(float diff);
	virtual bool init();
	void AutoUpdateCastingBar();
	void SwapCastingBarVisable();
	void InitSpellDefaultFrame();
	void CreateVirtualRoker();
	void InitUI();
	void InitButtomSpellBar();
	// Return A SpellSlot If Touched Button
	SpellSlot* CheckTouchSpellButton(const Vec2& Loc);
	//∂•≤„Button
	void InitButtomMenu();
	float GetVirtualRokerOrgin(Vec2 CenterPoint,Vec2 RokerPoint);
	virtual void onTouchBegan(const std::vector<Touch*>& touches, Event *_event);
	virtual void onTouchMoved(const std::vector<Touch*>& touches, Event *_event);
	virtual void onTouchEnded(const std::vector<Touch*>& touches, Event *_event);
	bool IsSingleTouch(const std::vector<Touch*>& touches, PlayerUITouchType _type);
	void ResetVirtualRokerOrgin(float Orgin);
	void SwapButtomMenuType();
	void ButtonMenuCallBack();
	Layer* m_VirtualRokerLayer;
	Sprite* m_VirtualRoker_BackGround;
	Sprite* m_VirtualRoker_Roker;
	Vec2 visiablesize;
	Vec2 RockerLastPostion;
	Sprite* m_Player_Info_UI;
	Sprite* m_Player_Info_UI_Hp_Back;
	ProgressTimer* m_Player_Info_UI_Hp;
	Sprite* m_Player_Info_UI_MP_Back;
	ProgressTimer* m_Player_Info_UI_Mp;

	Sprite* m_Player_Info_Casting_Bar_Frame;
	Sprite* m_Player_Info_Casting_Bar_Icon;
	ProgressTimer* m_Player_Info_Casting_Bar;

	Sprite* m_Player_Info_UI_Level;

	std::vector<Sprite*> m_Buttom_Menus;
	PlayerUITouchType m_touchtype;
	SpellSlot* TouchedSpellSlot;
	Sprite* m_ButtomMenu;
	bool CanTouchButton;
	Sprite* DefaultFrame;
	LabelTTF* SpellText;
	Sprite* m_ButtonSpellBar;
	Sprite* m_ButtonSpellItem[8];

	std::map<TopButtonLabelTTF, LabelTTF*> TopMenuLabel;
};


class Slot : public Sprite
{
public:
	Slot(const std::string& url = "");
	~Slot();
	Item* GetItem()				{ return m_Item; }
	Sprite* GetDisPlaySprite()	{ return m_DisPlaySprite; }
	void SetItem(Item* pItem = nullptr);
	void SwapItem(Slot* Instead);
private:
	Item* m_Item;
	Sprite* m_DisPlaySprite;
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
	Slot* GetSlotByTouch(Touch* touches);
	Slot* GetSlotByPageTag(const uint8& Page, const uint8& SlotTag);
private:
	enum BagTouchType
	{
		Bag_Type_None,
		Bag_Type_SwapPage,
		Bag_Type_SeleItem,
	};
	PlayerBag();
	~PlayerBag();
	void LoadInventory();
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