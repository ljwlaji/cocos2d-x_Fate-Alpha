#include "Player.h"
#include "ActionManager.h"
#include "MovementMgr.h"
#include "HelloWorldScene.h"
#include "PlayerTalkLayer.h"
#include "SpellMgr.h"
#include "Spell.h"
#include "DataMgr.h"
#include "QuestMgr.h"
#include "QuestBook.h"
#include "PlayerUILayer.h"
#include "PlayerEquipWindow.h"
#include "Item.h"
#include "NotifyMgr.h"
#include "DeadTalkClass.h"
#include "Creature.h"
#include "PlayerBag.h"
#include "MiniMap.h"

static Player* _player = nullptr;

Player::Player(SkeletonAnimation* _SkeletonAnimation, CharacterEnumInfo& _info) : Unit(_SkeletonAnimation)
{
	_player = this;
	m_Player_Target = nullptr;
	SetTypeId(TYPEID_PLAYER);
	SetGuid(_info.guid);
	SetName(_info.name.c_str());
	SetClass(_info.Class);
	SetMoney(_info.Money);
	SetExp(_info.Exp);
	SetLevel(_info.Level);
	NextLevelRequireExp = sGame->GetCurrentExpPerLevel(GetLevel() + 1);
	SetMapid(_info.Mapid);
	SetFaction(_info.Faction);
	SetSpeed(100);
	SetRealPosition(_info.Pos_X, _info.Pos_Y);
	setLocalZOrder(999999);
	ActionMgr* _mgr = new ActionMgr(this);
	_ActionMgr = _mgr;
	for (int i = MoveKey_Left; i != MoveKey_Endl; i++)
		MoveKeyStatus[(MoveKeyValue)i] = false;
	KeyVectorClearTimer = Base_Clear_Key_Time;
	PlayerTalkClass = new TalkClass();
	PlayerTalkClass->ClearMenu();
	LoadPlayerQuests();
	CombatList.clear();
	scheduleUpdate();
}

Player::~Player()
{
	SaveToDB();
	if (_ActionMgr)
		delete _ActionMgr;

	if (PlayerTalkClass)
		delete PlayerTalkClass;
	DestoryVision();
	removeAllChildrenWithCleanup(true);
	_player = nullptr;
}

Player* Player::GetInstance()
{
	if (!_player)
		return nullptr;

	return _player;
}

void Player::AddUnitToPlayerCombatList(Unit* pUnit)
{
	CombatList.push_back(pUnit);
}

bool Player::IsUnitInCombatList(Unit* pUnit)
{
	for (std::list<Unit*>::iterator itr = CombatList.begin(); itr != CombatList.end(); itr++)
		if (*itr == pUnit)
			return true;
	return false;
}

void Player::ReMoveUnitFromPlayerCombatList(Unit* pUnit)
{
	for (std::list<Unit*>::iterator itr = CombatList.begin(); itr != CombatList.end(); itr++)
	{
		if (*itr == pUnit)
			CombatList.erase(itr);
	}
}

void Player::AutoUpdateCombatList()
{
	if (!IsInCombat())
		return;

	std::list<Unit*> RemoveList;
	for (std::list<Unit*>::iterator itr = CombatList.begin(); itr != CombatList.end(); ++itr)
	{
		if (Creature* TempChecking = (*itr)->ToCreature())
		{
			if (!TempChecking->IsInCombatWith(this) && !TempChecking->IsAlive())
				RemoveList.push_back(*itr);
		}
		else RemoveList.push_back(*itr);
	}

	while (RemoveList.size())
	{
		for (std::list<Unit*>::iterator itr = CombatList.begin(); itr != CombatList.end(); ++itr)
		{
			if (*itr == *RemoveList.begin())
			{
				CombatList.erase(itr);
				RemoveList.pop_front();
				break;
			}
		}
	}
	if (CombatList.empty())
		SetInCombat(false);
}

void Player::Revive()
{
	SetUnitInt32Value(Curr_HP, GetUnitInt32Value(Max_HP) / 2);
	SetDeathStatus(Alive);
}

void Player::JustDead()
{
	sDeadTalkClass->Show();
}

void Player::LevelUp()
{
	SetUnitInt32Value(UnitValue_Level, GetLevel() + 1);
	m_Exp = 0;
	UpdateUnitValues();
	SetUnitInt32Value(Curr_HP, GetPlayerTotalInt32Value(Max_HP));
	SendUpdateValueRequire();
	SetNextLevelRequireExp(sGame->GetCurrentExpPerLevel(GetLevel() + 1));
	SaveToDB();
}

uint32 Player::GetPlayerTotalInt32Value(UnitInt32Value _val)
{
	return GetEquipItemTotalValusForKey(_val) + GetUnitInt32Value(_val);;
}

void Player::AddExp(uint32& _exp)
{
	if (m_Exp + _exp > GetNextLevelRequireExp())
	{
		_exp -= (GetNextLevelRequireExp() - m_Exp);
		LevelUp();
		AddExp(_exp);
		return;
	}

	m_Exp += _exp;
}

bool Player::CanEquipItem(Item* pItem)
{
	if (pItem->GetTemplate()->RequireLevel > GetLevel())
	{
		sNotifyMgr->ShowNotify("Level Error");
		return false;
	}

	if (pItem->GetTemplate()->RequireClass != GetClass())
	{
		sNotifyMgr->ShowNotify("Class Error");
		return false;
	}

	return true;
}

bool Player::CanAcceptQuest(const uint32& QuestID)
{
	const QuestTemplate* _QuestTemplate = sQuestMgr->GetQuestTemplate(QuestID);
	if (!_QuestTemplate)
		return false;

	if (GetLevel() < _QuestTemplate->AcceptNeedLevel)
		return false;

	if (HasQuest(QuestID))
		return false;
	//if (_QuestTemplate->AcceptNeedRepID)
	//缺少检查阵营声望

	return true;
}

bool Player::HasQuest(const uint32& questid)
{
	if (m_QuestsStat.find(questid) != m_QuestsStat.end())
		return true;
	return false;
}

void Player::AcceptQuest(const uint32& QuestId)
{
	const QuestTemplate* _QuestTemplate = sQuestMgr->GetQuestTemplate(QuestId);
	if (!_QuestTemplate)
		return;

	if (!CanAcceptQuest(QuestId))
		return;

	//缺少完成过任务的判断
	//sQuestBook->AddNewQuestToBook(_QuestTemplate);

	PlayerQuestStatus _PlayerQuestStatus;
	_PlayerQuestStatus.QuestRequire = sQuestMgr->GetQuestRequire(QuestId);
	for (int i = 0; i != 4; i++)
		_PlayerQuestStatus.FinishCount.push_back(0);

	m_QuestsStat[QuestId] = _PlayerQuestStatus;
	char msg[255];//										0				1			2			3					
	snprintf(msg, 255, "INSERT INTO player_quest_status(guid,quest_id) VALUES(%d,%d);", GetGuid(), QuestId);
	sDataMgr->PExcute(msg);
	sQuestBook->AddNewQuestToBook(_QuestTemplate);
}

void Player::SaveQuestStatusInfoToDB()
{
	for (m_Questitr = m_QuestsStat.begin(); m_Questitr != m_QuestsStat.end(); m_Questitr++)
	{
		if (m_Questitr->second.IsCompleted)
			continue;

		char msg[255];//										0				1			2			3					
		snprintf(msg, 255, "UPDATE player_quest_status SET count_1 = %d,count_2 = %d, count_3 = %d, count_4 = %d WHERE quest_id = %d AND guid = %d", m_Questitr->second.FinishCount.at(0), m_Questitr->second.FinishCount.at(1), m_Questitr->second.FinishCount.at(2), m_Questitr->second.FinishCount.at(3), m_Questitr->first, sPlayer->GetGuid());
		sDataMgr->PExcute(msg);
	}
}

void Player::SaveCharacterInfoToDB()
{
	char msg[255];
	snprintf(msg, 255, "UPDATE characters SET Money = %d, Exp = %d, Level = %d, Mapid = %d, Pos_X = %f, Pos_Y = %f WHERE guid = %d", GetMoney(), GetExp(), GetLevel(), GetMapid(), getPositionX(), getPositionY(), GetGuid());
	sDataMgr->PExcute(msg);
}

void Player::CalcItemValues()
{
	m_ItemTotalValues.clear();
	//InitLizs
	for (int i = SLOT_WEAPON; i != SLOT_END; i++)
		m_ItemTotalValues[(UnitInt32Value)i] = 0;

	for (int i = SLOT_WEAPON; i != SLOT_END; i++)
	{
		Slot* pSlot = (Slot*)sPlayerEquip->getChildByTag(i);
		if (pSlot && pSlot->GetItem())
		{
			const std::map<uint32, uint32> ItemValue = pSlot->GetItem()->GetTemplate()->Values;
			{
				for (std::map<uint32, uint32>::const_iterator itr = ItemValue.begin(); itr != ItemValue.end(); itr++)
					m_ItemTotalValues[(UnitInt32Value)itr->first] += itr->second;
			}
			//暂缺物品Min/Max Damage计算
		}
	}
}

uint32 Player::GetEquipItemTotalValusForKey(UnitInt32Value _val)						
{
	if (m_ItemTotalValues.find(_val) != m_ItemTotalValues.end())
		return m_ItemTotalValues[_val];
	return 0;
}

uint32 Player::GetItemTotalAttack()
{
	uint32 _attack = GetEquipItemTotalValusForKey(Base_Att);
	switch (GetClass())
	{
	case Saber:
		_attack += GetEquipItemTotalValusForKey(Base_Str) + GetEquipItemTotalValusForKey(Base_Dex) / 4;
		break;
	case Archer:
		_attack += GetEquipItemTotalValusForKey(Base_Str) / 4 + GetEquipItemTotalValusForKey(Base_Dex);
		break;
	case Caster:
		_attack += GetEquipItemTotalValusForKey(Base_Dex) / 4 + GetEquipItemTotalValusForKey(Base_Int);
		break;
	case Lancer:
		_attack += GetEquipItemTotalValusForKey(Base_Str) / 2 + GetEquipItemTotalValusForKey(Base_Dex) / 2;
		break;
	case Assasin:
		_attack += GetEquipItemTotalValusForKey(Base_Int) / 4 + GetEquipItemTotalValusForKey(Base_Dex);
		break;
	case Rider:
		_attack += GetEquipItemTotalValusForKey(Base_Str) / 3 + GetEquipItemTotalValusForKey(Base_Dex) + GetEquipItemTotalValusForKey(Base_Int) / 3;
		break;
	case Avenger:
		_attack += GetEquipItemTotalValusForKey(Base_Dex) / 4 + GetEquipItemTotalValusForKey(Base_Str) / 4 + GetEquipItemTotalValusForKey(Base_Dex) / 4 + GetEquipItemTotalValusForKey(Base_Int) / 4;
		break;
	case Berserker:
		_attack += GetEquipItemTotalValusForKey(Base_Str) * 2;
		break;
	}

	return _attack;
}

void Player::SetPlayerTarget(Unit* pUnit)
{
	if (!pUnit || m_Player_Target == pUnit)
		return;

	if (m_Player_Target && m_Player_Target->GetPlayerTargetSign())
		m_Player_Target->GetPlayerTargetSign()->removeFromParentAndCleanup(true);
	Sprite* TempSign = Sprite::create("Player_Target_Sign.png");
	TempSign->setPosition(pUnit->getContentSize().width / 2, 0);
	pUnit->SetPlayerTargetSign(TempSign);
	pUnit->addChild(TempSign);
	m_Player_Target = pUnit;
}

void Player::ReSetPlayerTarget()
{
	if (m_Player_Target)
	{
		m_Player_Target->GetPlayerTargetSign()->removeFromParentAndCleanup(true);
		m_Player_Target->ResetPlayerTargetSign();
		m_Player_Target = nullptr;
	}
}

void Player::CloseGossipMenu()
{
	PlayerTalkClass->ClearMenu();
	if (sPlayerTalkLayer)
	{
		sPlayerTalkLayer->CloseMenuWithCleanUp(true);
	}
}

void Player::SendGossipMenu(std::string Main_String, Creature* pCreature)
{
	if (sPlayerTalkLayer)
	{
		sPlayerTalkLayer->SendMenuToPlayer(Main_String);
		PlayerTalkClass->TalkingCreature = pCreature;
	}
}
void Player::LoadPlayerQuests()
{
	m_QuestsStat.clear();
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,"SELECT quest_id,is_completed,count_1,count_2,count_3,count_4 FROM player_quest_status WHERE guid = %u", GetGuid()))
	{
		if (!_Result.empty())
		{
			std::vector<RowInfo> row;
			for (Result::iterator itr = _Result.begin(); itr != _Result.end(); itr++)
			{
				row = itr->second;
				PlayerQuestStatus _PlayerQuestStatus;
				_PlayerQuestStatus.QuestRequire = sQuestMgr->GetQuestRequire(itr->second.at(0).GetInt());
				if (!_PlayerQuestStatus.QuestRequire)
					continue;

				_PlayerQuestStatus.IsCompleted = itr->second.at(1).GetBool();

				for (int i = 2; i != itr->second.size(); i++)
					_PlayerQuestStatus.FinishCount.push_back(itr->second.at(i).GetInt());
				m_QuestsStat[itr->second.at(0).GetInt()] = _PlayerQuestStatus;
			}
		}
	}
	else
	{

	}
}

void Player::ResetMoveKeyForRoker()
{
	for (int i = 0; i != MoveKey_Endl; i++)
		MoveKeyStatus[(MoveKeyValue)i] = false;
}
void Player::DealVirtualRoker(VirtualRockerOrginType _VirtualRockerOrginType)
{
	switch (_VirtualRockerOrginType)
	{
	case Roker_Up:
		for (int i = 0; i != MoveKey_Endl; i++)
			i == MoveKey_Up ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Up_Left:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Up || i == MoveKey_Left) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Up_Right:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Up || i == MoveKey_Right) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Left:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Left) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Left_Down:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Down || i == MoveKey_Left) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Down:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Down) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Down_Right:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Down || i == MoveKey_Right) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	case Roker_Right:
		for (int i = 0; i != MoveKey_Endl; i++)
			(i == MoveKey_Right) ? MoveKeyStatus[(MoveKeyValue)i] = true : MoveKeyStatus[(MoveKeyValue)i] = false;
		break;
	}

}

void Player::UpdateMoveStatus()
{
	if (!IsAlive())
		return;
	if (GetCastingSpell())
		return;
	bool NeedResetMoveStatus = true;
	for (int i = MoveKey_Left; i < MoveKey_Endl; i++)
	{
		if (MoveKeyStatus[(MoveKeyValue)i])
			NeedResetMoveStatus = false;
	}
	if (NeedResetMoveStatus && GetMoveType() != MoveType_None)
	{
		SetMoveType(MoveType_None);
		GetVision()->clearTracks();
		GetVision()->setAnimation(Track_Idle, "idle_normal", true);
		return;
	}
	//Calc
	float X_Modify = 0;
	float Y_Modify = 0;
	float Modify_Speed = (float)GetSpeed() / 100.0f;
	//log("speed %f", GetSpeed());
	if (MoveKeyStatus[MoveKey_Left]) 
		X_Modify -= Base_X_MovePoint * Modify_Speed;
	if (MoveKeyStatus[MoveKey_Right])
		X_Modify += Base_X_MovePoint * Modify_Speed;
	if (MoveKeyStatus[MoveKey_Up])
		Y_Modify += Base_Y_MovePoint * Modify_Speed;
	if (MoveKeyStatus[MoveKey_Down])
		Y_Modify -= Base_Y_MovePoint * Modify_Speed;

	if (GetMoveType() == MoveType_Run)
	{
		X_Modify = X_Modify * 1.5f;
		Y_Modify = Y_Modify * 1.5f;
	}
	else
	{
		if (X_Modify && GetMoveType() != MoveType_Walk)
		{
			SetMoveType(MoveType_Walk);
			GetVision()->clearTracks();
			GetVision()->setAnimation(0, "walk", true);
		}
	}

	CheckMoveTo X_MoveFront;
	CheckMoveTo Y_MoveFront;
	float CheckPosX = 0;
	float CheckPosY = 0;
	if (X_Modify)
	{
		if (X_Modify > 0)
		{
			SetFacing(Facing_Right);
			X_MoveFront = Move_To_Right;
		}
		else
		{
			SetFacing(Facing_Left);
			X_MoveFront = Move_To_Left;
		}
		if (sMoveMgr->CanMoveTo(this,X_MoveFront, abs(X_Modify)))
			setPositionX(getPositionX() + X_Modify);
	}

	if (Y_Modify)
	{
		Y_Modify > 0 ? Y_MoveFront = Move_To_Up : Y_MoveFront = Move_To_Down;
		if (sMoveMgr->CanMoveTo(this,Y_MoveFront, abs(Y_Modify)))
			setPositionY(getPositionY() + Y_Modify);
	}
	if (X_Modify || Y_Modify)
		sMiniMap->UpdateSingleUnitSign(this);
}

void Player::update(float diff)
{
	if (GetCastingSpell())
		GetCastingSpell()->update(diff);
	UpdateMoveStatus();
	if (IsInCombat())
		AutoUpdateCombatList();
	//Clear Key
	if (KeyVectorClearTimer <= diff)
	{
		PlayerActionMgr()->ClearKey();
		KeyVectorClearTimer = Base_Clear_Key_Time;
	}
	else KeyVectorClearTimer -= diff;


	if (m_NeedUpdateValueNumber)
	{
		CalcItemValues();
		sPlayerUi->ResetAllUIValuesNumber();
		m_NeedUpdateValueNumber = false;
	}
}

bool Player::CanCancelActionForMove()
{
	switch (GetDoingAction())
	{
	case Action_None:
		return true;
	case Normal_Attack_0:
	case Normal_Attack_1:
	case Normal_Attack_2:
	case Normal_Attack_3:
	{
		if (GetLevel() > 5)
			return false;
		return true;
	}
	case Move_Run_Right:
	case Move_Run_Left:
		return true;
	default:
		return false;
	}
}

bool Player::LoadPlayerSpells()
{
	m_Spells.clear();
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,"SELECT spell,active FROM player_spells WHERE guid = %u", GetGuid()))
	{
		if (_Result.empty())
			return true;
		std::vector<RowInfo> row;
		for (Result::iterator itr = _Result.begin(); itr != _Result.end(); itr++)
		{
			row = itr->second;
			PlayerSpellTemplate _template;
			_template.ID					= row.at(0).GetInt();
			_template.Active				= row.at(1).GetBool();
			m_Spells[row.at(0).GetInt()]	= _template;
		}
		return true;
	}
	else return false;
}

bool Player::CreatePlayer()
{
	if (LoadFromDB() && UpdateUnitValues())
		return true;

	_player = nullptr;
	removeFromParentAndCleanup(true);
	return false;
}

void Player::SetCurrentValues()
{
	SetUnitInt32Value(Curr_HP, GetPlayerTotalInt32Value(Max_HP));
}

bool Player::HasSpell(uint32 spellid)
{
	if (m_Spells.find(spellid) != m_Spells.end())
		return true;
	return false;
}

void Player::CheckPlayerSpellActive()
{
	for (PlayerSpells::iterator itr = m_Spells.begin(); itr != m_Spells.end(); itr++)
	{
		SpellInfo info = sSpellMgr->GetSpellInfo(itr->first);
		if (!info.ID)
			continue;
		uint32 NextLevelSpellID = sSpellMgr->GetSpellNextLevelID(info.ID, info.BaseLevelSpellID);

		if (!NextLevelSpellID || !HasSpell(NextLevelSpellID))
			itr->second.Active = true;
		else
			itr->second.Active = false;
	}
}

bool Player::LoadFromDB()
{
	if (!LoadPlayerSpells())
		return false;
	CheckPlayerSpellActive();
	return true;
}

void Player::SaveToDB()
{
	SaveQuestStatusInfoToDB();
	SaveCharacterInfoToDB();
}

uint16 Player::GetEmptyBagSlot()
{
	return sPlayerBag->GetEmptySlot();
}

void Player::AddItem(uint32 ItemEntry, uint32 Count)
{
	if (uint16 BagSlot = GetEmptyBagSlot())
	{
		uint8 Page = BagSlot / 100;
		uint8 __Slot = BagSlot % 100;
		Slot* pSlot = sPlayerBag->GetSlotByPageTag(Page, __Slot);
		Item* pItem = Item::CreateItem(ItemEntry, Page, __Slot);
		pItem->SaveToDB();
		pItem->SetCount(Count);
		pSlot->SetItem(pItem);
	}
	else
	{
		sNotifyMgr->ShowNotify("Inventory Is Full");
	}
}