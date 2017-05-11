#ifndef __QUEST_BOOK_H__
#define __QUEST_BOOK_H__

#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

#define sQuestBook QuestBook::GetInstance()

struct QuestTemplate;
struct PlayerQuestStatus;
class QuestBook : public Sprite
{
public:
	static QuestBook* GetInstance();
	void InitQuestFrame(const std::map<uint32, PlayerQuestStatus>& quests);
	void OnTouchBegin(Touch* Loc);
	void OnTouchEnded(Touch* Loc);
	void SwapVisable();
	void AddNewQuestToBook(const QuestTemplate* pTemplate);
private:
	void SwapQuestDetail(uint32 QuestID);
	void InitFrame();
	void SwapPage(bool pageup);
	Sprite* CreateNewQuestFrame(const QuestTemplate* pTemplate);
	QuestBook();
	~QuestBook();
	enum QuestBookTag
	{
		Scroll_Bar_Uper_Arrow,
		Scroll_Bar_Button_Arrow,
		Close_Button,
		Button_End,
	};
	std::map<QuestBookTag, Sprite*> QuestBookButtons;
	std::map<uint32/*page*/, std::vector<Sprite*>> m_SingleQuestFrames;
	Sprite* ScrollBarHandler;

	Sprite* m_TouchedSprite;
	Sprite* ScrollBar_Frame;
	LabelTTF* QuestTitleBig;
	LabelTTF* QuestString;
	LabelTTF* QuestRewardText[3];
	uint8 CurrentPage;
};



#endif