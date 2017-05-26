#ifndef __NPC_VENDOR_SPRITE_H__
#define __NPC_VENDOR_SPRITE_H__

#include "UISprite.h"

#define sVendorSprite NpcVendorSprite::GetInstance()
class NpcVendorSprite : public UISprite
{
public:
	//Return false if the creature don't have a vendor list;
	bool ShowVendorList(uint32 Creature_id);
	void SetCanRep(bool var)	{ RepairButton->setVisible(var); }
	static NpcVendorSprite* GetInstance();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
private:
	void InitFrame();
	void AddNewItemToList(uint32 ItemID, uint32 Exchange_Cost);
	NpcVendorSprite();
	~NpcVendorSprite();
	void HandleListScroll();

	Sprite* RepairButton;
	Sprite* CloseButton;
	std::list<Sprite*> ShowingList;
	Point MovePoint;
	Point LastPoint;

	Sprite* m_TouchedSprite;

	bool CanClickMenu;
};

#endif