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
	SpellTargetType_Self,
	SpellTargetType_Enemy_Single,
	SpellTargetType_Enemy_Multi,
	SpellTargetType_Friend_Single,
	SpellTargetType_Friend_Multi,
};

enum SpellEffectType
{
	//����
	Heath,
	//�ƶ��ٶ�
	Speed,
	//����
	Stun,
	//��������
	Attack,
	//��������
	Defance,
};

struct SpellInfo
{
	//����ID
	uint32 ID = 0;
	//һ������ID--���ڶ�λ���ܼ���
	uint32 BaseLevelSpellID = 0;
	//����ְҵ
	uint8 RequireClass = 0;
	//����ȼ�
	uint8 RequireLevel = 0;
	//���ܷ�Χ
	float SpellCastRange = 0;
	//�������÷�Χ
	float SpellRealRange = 0;
	//���ܶ�������
	Spell_Type SpellType = SpellType_Melee;
	//����Ŀ������
	SpellTargetType SpellTargetType = SpellTargetType_Self;
	//��������Ŀ������
	uint8 SpellTargetCount = 0;
	//������������
	SpellEffectType EffectType = Heath;
	//��ֵ
	int32 SpellValue = 0;
	//ʩ��ʱ��
	float SpellCastTime = 0;
	//��ȴʱ��
	float SpellColdDownTime = 0;
	//���ܳ���ʱ��
	float AruaTotalDuration = 0;
	//�������ü��
	float AruaSingleDamageDuration = 0;
	//�����ƶ��ͷ�
	bool CanCastWhileMoving = false;
	//��������Ч��
	uint32 SpellLinkedAura = 0;
	//�����ͷŶ�������
	std::string SpellActionName = "";
	//�����ͷŶ���ʱ��
	float PlayActionTime = 0;
	//����ͼ��·��
	std::string SpellIconUrl = "";
};

class SpellMgr
{
public:
	static SpellMgr* GetInstance();
	void Init();
	SpellInfo GetSpellInfo(uint32 spellid);
private:
	SpellMgr();
	~SpellMgr();
	void LoadSpellTemplate();
	void LoadSpellChain();
	SpellChainMap m_SpellChainMap;
	std::map<uint32, SpellInfo> m_SpellTemplate;
};


#endif