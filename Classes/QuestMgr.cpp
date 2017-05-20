#include "QuestMgr.h"
#include "DataMgr.h"
static QuestMgr* _QuestMgr = nullptr;


QuestMgr::QuestMgr()
{
	LoadQuestTemplateFromDB();
}

QuestMgr::~QuestMgr()
{
}

QuestMgr* QuestMgr::GetInstance()
{
	if (!_QuestMgr)
		_QuestMgr = new QuestMgr();

	return _QuestMgr;
}

std::vector<SingleQuestRequire>* QuestMgr::GetQuestRequire(uint32 QuestID)
{
	if (m_Quest_Templates.find(QuestID) != m_Quest_Templates.end())
		return &m_Quest_Templates[QuestID].QuestRequires;
	return nullptr;
}

const QuestTemplate* QuestMgr::GetQuestTemplate(uint32 QuestId)
{
	if (m_Quest_Templates.find(QuestId) != m_Quest_Templates.end())
		return &m_Quest_Templates[QuestId];
	return nullptr;
}

void QuestMgr::LoadQuestTemplateFromDB()
{
	Result _result;
	if (sDataMgr->selectUnitDataList(_result,"SELECT id,title,quest_text,require_level,requre_rep_id,require_rep_count,quest_require_type_1,quest_require_id_1,quest_require_count_1,quest_require_type_2,quest_require_id_2,quest_require_count_2,quest_require_type_3,quest_require_id_3,quest_require_count_3,quest_require_type_4,quest_require_id_4,quest_require_count_4,quest_reward_money,quest_reward_exp,quest_reward_cash,quest_reward_type_1,quest_reward_id_1,quest_reward_count_1,quest_reward_type_2,quest_reward_id_2,quest_reward_count_2,quest_reward_type_3,quest_reward_id_3,quest_reward_count_3,quest_reward_type_4,quest_reward_id_4,quest_reward_count_4 FROM quest_template"))
	{
		if (_result.empty())
		{

		}
		else
		{
			m_Quest_Templates.clear();
			std::vector<RowInfo> row;
			for (Result::iterator itr = _result.begin(); itr != _result.end(); itr++)
			{
				row = itr->second;
				QuestTemplate _template;
				_template.ID = row.at(0).GetInt();
				_template.Title = row.at(1).GetString();
				_template.QuestText = row.at(2).GetString();
				_template.AcceptNeedLevel = row.at(3).GetInt();
				_template.AcceptNeedRepID = row.at(4).GetInt();
				_template.AcceptNeedRepCount = row.at(5).GetInt();
				for (int i = 6; i != 18; i++)
				{
					SingleQuestRequire _SingleQuestRequire;
					_SingleQuestRequire.RequireType = (QuestRequireType)row.at(i).GetInt();
					_SingleQuestRequire.RequireID	= row.at(++i).GetInt();
					_SingleQuestRequire.Count		= row.at(++i).GetInt();
					_template.QuestRequires.push_back(_SingleQuestRequire);
				}
				_template.RewardMoney = row.at(18).GetInt();
				_template.RewardExp = row.at(19).GetInt();
				_template.RewardCash = row.at(20).GetInt();
				for (int i = 21; i != row.size(); i++)
				{
					SingleQuestReWard _SingleQuestReWard;
					_SingleQuestReWard.RewardType	= (QuestRewardType)row.at(i).GetInt();
					_SingleQuestReWard.RewardID		= row.at(++i).GetInt();
					_SingleQuestReWard.Count		= row.at(++i).GetInt();
					_template.QuestOtherRewards.push_back(_SingleQuestReWard);
				}

				m_Quest_Templates[_template.ID] = _template;
			}
		}
	}
	else
	{

	}
}