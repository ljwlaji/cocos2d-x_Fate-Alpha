#ifndef __SPELL_BOOK_H__
#define __SPELL_BOOK_H__

#include "Types.h"
#include "UISprite.h"
#include "SpellMgr.h"

USING_NS_CC;

#define sPlayerSpellBook	SpellBook::GetInstance()

class SpellSlot : public Sprite
{
public:
	SpellSlot(const char* url = nullptr);
	~SpellSlot();
	uint32 GetSpellId()	{ return m_spellid; }
	Sprite* GetSpellFrame()	{ return m_SpellFrame; }
	void SetSpellFrame(Sprite* pFrame)	{ m_SpellFrame = pFrame; }
	void SetSlotSpell(const SpellInfo& spellid);
private:
	Sprite* m_SpellFrame;
	uint32 m_spellid;
};

class SpellBook : public UISprite
{
public:
	static SpellBook* GetInstance();
	void SwapVisiable();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
private:
	enum SpellBookButtonTag
	{
		SpellButtonTag_Close,
		SpellButtonTag_Tag_1,
		SpellButtonTag_Tag_2,
		SpellButtonTag_Left_Arrow,
		SpellButtonTag_Right_Arrow,
		SpellButtonTag_End,
	};
	enum SpellBookTouchType
	{
		SpellBookTouchType_None,
		SpellBookTouchType_Button,
		SpellBookTouchType_Slot,
		SpellBookTouchType_MoveSlot,
	};
	void InitFrame();
	void InitSlot();
	void LoadPlayerSpells();
	void HandleTouchButton(SpellBookButtonTag _tag);
	SpellBook();
	~SpellBook();

	Sprite* m_LeftArrow;
	Sprite* m_CloseButton;
	Sprite* m_SpellPage;
	Sprite* m_RightArrow;
	Sprite* m_Tag_1;
	Sprite* m_Tag_2;
	Sprite* TouchedSprite;

	Vec2 m_Start_Move_Position;
	SpellBookTouchType m_TouchType;
	std::vector<SpellSlot*> m_SpellSlotTemplate;
};



#endif