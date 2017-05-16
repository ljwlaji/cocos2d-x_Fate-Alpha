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
	//Set Guid To 0 If Is New Loot*/
	static Item* CreateItem(const uint32& ItemEntry, const uint32& Page, const uint32& Slot,const uint32& ItemGuid = 0);
	static uint32 CreateNewItemGuid();
	void SaveToDB();
	void DeleteFromDB();
	void SetSlot(const uint32& Page, const uint32& Slot);
	~Item();
public:
	//Function
	bool CanEquipToSlot(const Slot* pSlot);
	const ItemTemplate* GetTemplate()						{ return m_Template; }
	std::string GetIconUrl()								{ return m_Template->Url; }
	void SetCount(const uint8& Count)						{ m_ItemCount = Count; }
private:
	void InsertToDB();
	void SetGuid(const uint32& guid)						{ m_Guid = guid; }
	Item(const ItemTemplate* pTemplate);
	std::string m_Icon_Url;
	uint8 m_ItemCount;
	const ItemTemplate* m_Template;
	uint32 m_Slot = 0;
	uint32 m_Page = 0;
	uint32 m_Guid = 0;
};


#endif