#ifndef __SPELL_BOOK_H__
#define __SPELL_BOOK_H__

#include "cocos2d.h"
#include "Types.h"

USING_NS_CC;

#define sPlayerSpellBook	SpellBook::GetInstance()

class SpellSlot : public Sprite
{
public:
	SpellSlot();
	~SpellSlot();

private:

};

class SpellBook : public Sprite
{
public:
	static SpellBook* GetInstance();
	void SwapVisiable()			{ isVisible() ? setVisible(false) : setVisible(true); }
	bool onTouchBagBegan(Touch* touches);
	void onTouchBagMoved(Touch* touches);
	void onTouchBagEnded(Touch* touches);
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
	};
	void InitFrame();
	void InitSlot();
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