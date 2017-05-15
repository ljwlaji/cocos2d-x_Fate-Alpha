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
class UISprite;
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
	void ResetAllUIValuesNumber();
	void ShowDeadSign()	{ DeadSign->setVisible(true); }
	void UnShowDeadSign(){ DeadSign->setVisible(false); }
	void PushSprite(UISprite* pUIsprite);
private:
	CREATE_FUNC(PlayerUILayer);
	PlayerUILayer();
	~PlayerUILayer();
	uint32 GetTypeByUISprite(UISprite* pUISprite);
	void InitButtomMenu();
	virtual void update(float diff);
	virtual bool init();
	void AutoUpdateHeadBar();
	void AutoUpdateListZorder();
	void AutoUpdateCastingBar();
	void AutoUpdateExpBar();
	void SwapCastingBarVisable();
	void InitSpellDefaultFrame();
	void InitUI();
	//重置头像的等级
	void ResetHeadLevel();
	void InitButtomSpellBar();
	void InitExpBar();
	// Return A SpellSlot If Touched Button
	SpellSlot* CheckTouchSpellButton(const Vec2& Loc);
	//顶层Button
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
	Sprite* DefaultFrame;
	Sprite* m_ButtonSpellBar;
	Sprite* m_ButtonSpellItem[8];
	Sprite* DeadSign;

	ProgressTimer* m_Player_Info_Casting_Bar;
	ProgressTimer* m_Player_Info_UI_Hp;
	ProgressTimer* m_Player_Info_UI_Mp;
	ProgressTimer* m_Player_Exp_Bar_Scroll;

	LabelTTF* SpellText;

	SpellSlot* TouchedSpellSlot;

	std::vector<Sprite*> m_Player_Info_UI_Level_Sprite;
	EventListenerTouchAllAtOnce* RokerListener;
	bool m_TopMenuIsVisable;
	bool m_NeedUpdateListZorder;
	bool CanTouchButton;
	PlayerUITouchType m_touchtype;

	std::list<UISprite*> UISpriteList;
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
	void CreateItemVisualIfNeed();
private:
	Item* m_Item;
	Sprite* m_DisPlaySprite;
};

#endif