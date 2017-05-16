#ifndef __PLAYER_BAG_H__
#define __PLAYER_BAG_H__

#include "Types.h"
#include "UISprite.h"
class Slot;
class PlayerBag : public UISprite
{
public:
	static PlayerBag* GetInstance();
	bool SwapItem(Slot* slot_one, Slot* slot_two);
	void SwapPage(PlayerBagPage PageEnable, PlayerBagPage PageDisable = Page_None);
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	uint16 GetEmptySlot();
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