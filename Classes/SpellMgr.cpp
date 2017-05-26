#include "SpellMgr.h"
#include "DataMgr.h"

static SpellMgr* _SpellMgr = nullptr;

SpellMgr::SpellMgr()
{

}

SpellMgr::~SpellMgr()
{
	_SpellMgr = nullptr;
}

void SpellMgr::Init()
{
	LoadSpellChain();
	LoadSpellTemplate();
}

SpellMgr* SpellMgr::GetInstance()
{
	if (!_SpellMgr)
		_SpellMgr = new SpellMgr();

	return _SpellMgr;
}

void SpellMgr::LoadSpellTemplate()
{
	m_SpellTemplate.clear();
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,"SELECT id,base_level_spell_id,require_class,require_level,spell_cast_range,spell_real_range,spell_type,spell_target_type,spell_target_count,effect_type,spell_value,spell_cast_time,spell_cold_down_time,arua_total_duration,arua_single_damage_duration,can_cast_while_moving,spell_linked_aura,spell_action_name,play_action_time,spell_icon_url,spell_name FROM spell_template"))
	{
		if (_Result.empty())
		{
			log("spell_chain Template Was Empty!");
			return;
		}
		else
		{
			for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
			{
				std::vector<RowInfo> row = ito->second;
				if (row.empty()) continue;
				SpellInfo SpellChainInfo;
				SpellChainInfo.ID						= row.at(0).GetInt();
				SpellChainInfo.BaseLevelSpellID			= row.at(1).GetInt();
				SpellChainInfo.RequireClass				= row.at(2).GetInt();
				SpellChainInfo.RequireLevel				= row.at(3).GetInt();
				SpellChainInfo.SpellCastRange			= row.at(4).GetFloat();
				SpellChainInfo.SpellRealRange			= row.at(5).GetFloat();
				SpellChainInfo.SpellType				= (Spell_Type)row.at(6).GetInt();
				SpellChainInfo.Spell_Target_Type		= (SpellTargetType)row.at(7).GetInt();
				SpellChainInfo.SpellTargetCount			= row.at(8).GetInt();
				SpellChainInfo.EffectType				= (SpellEffectType)row.at(9).GetInt();
				SpellChainInfo.SpellValue				= row.at(10).GetInt();
				SpellChainInfo.SpellCastTime			= row.at(11).GetFloat();
				SpellChainInfo.SpellColdDownTime		= row.at(12).GetFloat();
				SpellChainInfo.AruaTotalDuration		= row.at(13).GetFloat();
				SpellChainInfo.AruaSingleDamageDuration = row.at(14).GetFloat();
				SpellChainInfo.CanCastWhileMoving		= row.at(15).GetBool();
				SpellChainInfo.SpellLinkedAura			= row.at(16).GetInt();
				SpellChainInfo.SpellActionName			= row.at(17).GetString();
				SpellChainInfo.PlayActionTime			= row.at(18).GetFloat();
				SpellChainInfo.SpellIconUrl				= row.at(19).GetString();
				SpellChainInfo.SpellName				= row.at(20).GetString();
				m_SpellTemplate[row.at(0).GetInt()] = SpellChainInfo;
			}
		}
	}
}

SpellInfo SpellMgr::GetSpellInfo(uint32 spellid)
{
	SpellInfo _info;
	if (m_SpellTemplate.empty())
		return _info;

	if (m_SpellTemplate.find(spellid) != m_SpellTemplate.end())
		return m_SpellTemplate[spellid];

	return _info;
}

uint32 SpellMgr::GetSpellCurrentLevel(uint32 spellid, uint32 baseid)
{
	uint32 returnid = 0;
	if (m_SpellChainMap.find(baseid) == m_SpellChainMap.end())
		return returnid;

	std::vector<uint32> TempCheck = m_SpellChainMap[baseid];

	for (int i = 0; i != TempCheck.size(); i++)
	{
		if (TempCheck.at(i) == spellid)
		{
			return ++i;
		}
	}

	return returnid;
}
uint32 SpellMgr::GetSpellFrontLevelID(uint32 spellid, uint32 baseid)
{
	uint32 returnid = 0;
	if (m_SpellChainMap.find(baseid) == m_SpellChainMap.end())
		return returnid;

	std::vector<uint32> TempCheck = m_SpellChainMap[baseid];

	for (int i = 0; i != TempCheck.size(); i++)
	{
		if (TempCheck.at(i) == spellid)
		{
			returnid = TempCheck.at(--i);
			break;
		}
	}

	return returnid;
}

uint32 SpellMgr::GetSpellNextLevelID(uint32 spellid, uint32 baseid)
{
	uint32 returnid = 0;
	if (m_SpellChainMap.find(baseid) == m_SpellChainMap.end())
		return returnid;

	std::vector<uint32> TempCheck = m_SpellChainMap[baseid];

	for (int i = 0; i != TempCheck.size(); i++)
	{
		if (TempCheck.at(i) == spellid && i != TempCheck.size() - 1)
		{
			returnid = TempCheck.at(i + 1);
			break;
		}
	}

	return returnid;
}

void SpellMgr::LoadSpellChain()
{
	m_SpellChainMap.clear();
	std::string res = "SELECT level_1";
	char number[20];
	for (int i = 2; i != 11; i++)
	{
		snprintf(number, 20, ",level_%d", i);
		res += number;
	}
	res += " FROM spell_chain;";
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,res.c_str()))
	{
		if (_Result.empty())
		{
			log("spell_chain Template Was Empty!");
			return;
		}
		else
		{
			for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
			{
				std::vector<RowInfo> row = ito->second;
				if (row.empty()) continue;
				std::vector<uint32> SpellChainInfo;
				for (int i = 0; i != 10; i++)
				{
					if (!row.at(i).GetInt())
						break;
					SpellChainInfo.push_back(row.at(i).GetInt());
				}

				m_SpellChainMap[row.at(0).GetInt()] = SpellChainInfo;
			}
		}
	}
}