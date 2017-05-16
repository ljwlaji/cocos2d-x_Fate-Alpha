#ifndef __ITEM_DETAIL_SPRITE_H__
#define __ITEM_DETAIL_SPRITE_H__

#include "UISprite.h"

#define sItemDetailSprite ItemDetailSprite::GetInstance()

class Item;
struct ItemTemplate;
class ItemDetailSprite : public UISprite
{
public:
	static ItemDetailSprite* GetInstance();
	void ShowWithItem(Item* pItem);
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
private:
	enum ItemDetailValueTag
	{
		Name,
		MaxDamage,
		Item_Slot,
		Require_Level,
		Require_Class,
		BuyPrice,
		SellPrice,
	};
	ItemDetailSprite();
	~ItemDetailSprite();
	virtual void update(float diff);
	void InitDefault();
	void ResetMiddleFrame(const ItemTemplate* ValueCount);
	void ResetButtomFramePosition();
	void ResetDefaultString(const ItemTemplate* Value);
	std::string GetSlotStringBuyType(PlayerEquipSlots _slot);
	float MiddlePosY;
	std::list<Sprite*> MiddleSprites;

	LabelTTF* ItemNameTTF;
	LabelTTF* ItemSlotTTF;
	LabelTTF* ItemDamageTTF;
	LabelTTF* ItemRequireClassTTF;
	LabelTTF* ItemRequireLevelTTF;
	std::vector<LabelTTF*> ItemSellPriceTTF;
	Sprite* ButtomFrame;

	float m_DisappearTime;

	Point LastPoint;
	Point MovePoint;
};

#endif