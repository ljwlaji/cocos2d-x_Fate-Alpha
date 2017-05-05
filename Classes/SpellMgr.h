#ifndef __SPELL_MGR_H__
#define __SPELL_MGR_H__


#include "Types.h"

#define sSpellMgr SpellMgr::GetInstance()

enum Spell_Type
{
	SpellType_Melee,
	SpellType_Magic,
	SpellType_Arua,
};

enum SpellTargetType
{
	SpellTargetType_Empty,
	SpellTargetType_Self,
	SpellTargetType_Enemy_Single,
	SpellTargetType_Enemy_Multi,
	SpellTargetType_Friend_Single,
	SpellTargetType_Friend_Multi,
};

enum SpellEffectType
{
	//生命
	Heath,
	//移动速度
	Speed,
	//昏迷
	Stun,
	//攻击属性
	Attack,
	//防御属性
	Defance,
};

struct SpellInfo
{
	//技能ID
	uint32 ID = 0;
	//一级技能ID--用于定位技能级别
	uint32 BaseLevelSpellID = 0;
	//需求职业
	uint8 RequireClass = 0;
	//需求等级
	uint8 RequireLevel = 0;
	//技能范围
	float SpellCastRange = 0;
	//技能作用范围
	float SpellRealRange = 0;
	//技能定义类型
	Spell_Type SpellType = SpellType_Melee;
	//技能目标类型
	SpellTargetType Spell_Target_Type = SpellTargetType_Self;
	//技能作用目标数量
	uint8 SpellTargetCount = 0;
	//技能作用类型
	SpellEffectType EffectType = Heath;
	//数值
	int32 SpellValue = 0;
	//施法时间
	float SpellCastTime = 0;
	//冷却时间
	float SpellColdDownTime = 0;
	//技能持续时间
	float AruaTotalDuration = 0;
	//单次作用间隔
	float AruaSingleDamageDuration = 0;
	//允许移动释放
	bool CanCastWhileMoving = false;
	//技能链接效果
	uint32 SpellLinkedAura = 0;
	//技能释放动画名称
	std::string SpellActionName = "";
	//技能释放动作时长
	float PlayActionTime = 0;
	//技能图标路径
	std::string SpellIconUrl = "";
	//技能名称
	std::string SpellName = "";
};

class SpellMgr
{
public:
	static SpellMgr* GetInstance();
	void Init();
	SpellInfo GetSpellInfo(uint32 spellid);
	uint32 GetSpellNextLevelID(uint32 spellid, uint32 baseid);
	uint32 GetSpellCurrentLevel(uint32 spellid, uint32 baseid);
private:
	SpellMgr();
	~SpellMgr();
	void LoadSpellTemplate();
	void LoadSpellChain();
	SpellChainMap m_SpellChainMap;
	std::map<uint32, SpellInfo> m_SpellTemplate;
};


#endif