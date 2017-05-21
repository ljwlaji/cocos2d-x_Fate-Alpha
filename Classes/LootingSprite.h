#ifndef __LOOTING_SPRITE_H__
#define __LOOTING_SPRITE_H__

#include "Types.h"
#include "UISprite.h"

#define sLootingSprite LootingSprite::GetInstance()


class Loot;
class LootingSprite : public UISprite
{
public:
	static LootingSprite* GetInstance();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	void ResetWithLoot(Loot* pLoot);
private:
	typedef uint32 Page;
	void ScorllUpdate();
	void Init();
	void AddNewLootVisual(uint32 Item_ID, uint32 Count);
	LootingSprite();
	~LootingSprite();

	Sprite* CloseButton;
	Sprite* ScorllFrame;
	Sprite* ScorllController;
	Sprite* ScorllUp;
	Sprite* ScorllDown;
	
	std::list<Sprite*> LootList;

	Sprite* m_TouchedSprite;
	Point LastPoint;
	Point MovePoint;
	Loot* m_Linked_Loot;
	bool CanLootItem;
};

#endif