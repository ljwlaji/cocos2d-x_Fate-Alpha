#ifndef __ITEM_H__
#define __ITEM_H__

#include "cocos2d.h"
#include "Types.h"
#include "HelloWorldScene.h"
USING_NS_CC;

class Slot;
class Item
{
public:
	//Create
	//Return Nullptr If Failed To Find Template
	static Item* CreateItem(const uint32& ItemEntry);
	~Item();
public:
	//Function
	bool CanEquipToSlot(const Slot* pSlot);
	const ItemTemplate* GetTemplate()						{ return m_Template; }
	std::string GetIconUrl()								{ return m_Template->Url; }
	void SetCount(const uint8& Count)						{ m_ItemCount = Count; }
private:
	Item(const ItemTemplate* pTemplate);
	std::string m_Icon_Url;
	uint8 m_ItemCount;
	const ItemTemplate* m_Template;
};


#endif