#ifndef __LEARN_SPELL_SPRITE_H__
#define __LEARN_SPELL_SPRITE_H__

#include "UISprite.h"

#define sLearnSpellSprite LearnSpellSprite::GetInstance()

struct SpellInfo;
class LearnSpellSprite : public UISprite
{
public:
	static LearnSpellSprite* GetInstance();
	void InitWithCreature(uint32 creature_id);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
private:
	LearnSpellSprite();
	~LearnSpellSprite();
	void AddNewSpellToList(const SpellInfo& SpellID, uint32 Cost, uint32 Level);
	virtual bool OnUITouchBegin(Touch* pTouch);
	std::list<Sprite*> SingleFrameList;
	uint32 m_CreatureID;
	Sprite* CloseButton;
	Sprite* m_TouchedSprite;
};


#endif