#ifndef __QUEST_BOOK_H__
#define __QUEST_BOOK_H__

#include "Types.h"
#include "UISprite.h"

#define sQuestBook QuestBook::GetInstance()

struct QuestTemplate;
struct PlayerQuestStatus;
class QuestBook : public UISprite
{
public:
	static QuestBook* GetInstance();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	void InitQuestFrame(const std::map<uint32, PlayerQuestStatus>& quests);
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