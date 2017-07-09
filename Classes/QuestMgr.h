#ifndef __QUEST_MGR_H__
#define __QUEST_MGR_H__

#include "Types.h"


struct QuestTemplate
{
	uint32 ID;
	std::string Title;
	std::string QuestText;
	uint32 AcceptNeedLevel;
	uint32 AcceptNeedRepID;
	uint32 AcceptNeedRepCount;
	//需要移动到单独的List内
	//接受任务NPC
	uint32 AcceptNpcEntry;
	//完成任务NPC
	uint32 InvolvedrelationNpcEntry;
	std::vector<SingleQuestRequire> QuestRequires;
	uint32 RewardMoney;
	uint32 RewardExp;
	uint32 RewardCash;
	std::vector<SingleQuestReWard> QuestOtherRewards;
};

#define sQuestMgr	QuestMgr::GetInstance()

class QuestMgr
{
public:
	static QuestMgr* GetInstance();
	std::vector<SingleQuestRequire>* GetQuestRequire(uint32 QuestID);
	const QuestTemplate* GetQuestTemplate(uint32 QuestId);
private:
	QuestMgr();
	~QuestMgr();
	void LoadQuestTemplateFromDB();
	std::map<uint32, QuestTemplate> m_Quest_Templates;
};

#endif