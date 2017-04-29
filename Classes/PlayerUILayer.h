#ifndef __PLAYER_UI_LAYER_H__
#define __PLAYER_UI_LAYER_H__

#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

#define sPlayerBag	PlayerBag::GetInstance()
#define sPlayerUi	PlayerUILayer::GetInstance()

class Item;
class PlayerBag;

class PlayerUILayer : public Layer
{
public:
	static PlayerUILayer* GetInstance();
private:
	CREATE_FUNC(PlayerUILayer);
	PlayerUILayer();
	~PlayerUILayer();
	virtual void update(float diff);
	virtual bool init();
	void CreateVirtualRoker();
	void InitUI();
	float GetVirtualRokerOrgin(Vec2 CenterPoint,Vec2 RokerPoint);
	virtual bool onTouchRockerBegan(Touch* touches, Event *_event);
	virtual void onTouchRockerMoved(Touch* touches, Event *_event);
	virtual void onTouchRockerEnded(Touch* touches, Event *_event);
	void ResetVirtualRokerOrgin(float Orgin);
	Layer* m_VirtualRokerLayer;
	Sprite* m_VirtualRoker_BackGround;
	Sprite* m_VirtualRoker_Roker;
	Vec2 visiablesize;
	Vec2 RockerLastPostion;
	Sprite* m_Player_Info_UI;
	ProgressTimer* m_Player_Info_UI_Hp;
	Sprite* m_Player_Info_UI_Hp_Back;
	Sprite* m_Player_Info_UI_Level;
};

class Slot : public Sprite
{
public:
	Slot(const std::string& Url);
	~Slot();
	Item* GetItem()				{ return m_Item; }
	bool SetItem(Item* pItem)	{ m_Item = pItem; }
private:
	Item* m_Item;
};
class PlayerBag : public Sprite
{
public:
	static PlayerBag* GetInstance();
	bool SwapItem(Slot* slot_one, Slot* slot_two);
private:
	Slot* GetSlot(uint8 _Page, uint8 _Slot);
	PlayerBag();
	~PlayerBag();
	void InitPage();
	void InitEmptySlots(Sprite* SinglePageSprite);
	std::string url;
	std::map<PlayerBagPage, SlotMap> m_PlayerBagTemplate;
	std::vector<Sprite*> m_PlayerBagPageSprites;
};

#endif