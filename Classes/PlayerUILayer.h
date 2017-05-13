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
	void ResetAllUIValuesNumber();
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
		PlayerUITouch_SwapTopButton,
		PlayerUITouch_QuestBook,
		PlayerUITouch_DeathTalkClass,
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
	void AutoUpdateHeadBar();
	void AutoUpdateCastingBar();
	void AutoUpdateExpBar();
	void SwapCastingBarVisable();
	void InitSpellDefaultFrame();
	void InitUI();
	//����ͷ��ĵȼ�
	void ResetHeadLevel();
	void InitButtomSpellBar();
	void InitExpBar();
	// Return A SpellSlot If Touched Button
	SpellSlot* CheckTouchSpellButton(const Vec2& Loc);
	//����Button
	void InitButtomMenu();
	virtual void onTouchBegan(const std::vector<Touch*>& touches, Event *_event);
	virtual void onTouchMoved(const std::vector<Touch*>& touches, Event *_event);
	virtual void onTouchEnded(const std::vector<Touch*>& touches, Event *_event);
	bool IsSingleTouch(const std::vector<Touch*>& touches, PlayerUITouchType _type);
	void SwapButtomMenuType();
	void ButtonMenuCallBack();

	Vec2 visiablesize;

	Sprite* m_Player_Info_UI;
	Sprite* m_Player_Info_UI_Level_Frame;
	Sprite* m_Player_UI_TopButton_Swap_Button;
	Sprite* m_Player_Exp_Bar_Frame;
	Sprite* m_Player_Info_UI_Level;
	Sprite* m_Player_Info_Casting_Bar_Frame;
	Sprite* m_Player_Info_Casting_Bar_Icon;
	Sprite* m_ButtomMenu;
	Sprite* DefaultFrame;
	Sprite* m_ButtonSpellBar;
	Sprite* m_ButtonSpellItem[8];

	ProgressTimer* m_Player_Info_Casting_Bar;
	ProgressTimer* m_Player_Info_UI_Hp;
	ProgressTimer* m_Player_Info_UI_Mp;
	ProgressTimer* m_Player_Exp_Bar_Scroll;

	LabelTTF* SpellText;

	SpellSlot* TouchedSpellSlot;

	std::map<TopButtonLabelTTF, LabelTTF*> TopMenuLabel;
	std::vector<Sprite*> m_Player_Info_UI_Level_Sprite;
	std::vector<Sprite*> m_Buttom_Menus;
	EventListenerTouchAllAtOnce* RokerListener;
	bool m_TopMenuIsVisable;
	bool CanTouchButton;
	PlayerUITouchType m_touchtype;
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