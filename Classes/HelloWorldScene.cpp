#include "HelloWorldScene.h"
#include "ActionManager.h"
#include "MainMapLayer.h"
#include "DataMgr.h"
#include "NotifyMgr.h"
#include "EnterGameLayer.h"
#include "LoadingUILayer.h"
#include "TypingLayer.h"
#include "ChoseCharacterLayer.h"
#include "CreateCharacterLayer.h"
#include "ScriptMgr.h"
#include "PlayerTalkLayer.h"
#include "SpellMgr.h"
#include "QuestMgr.h"
#include "LootMgr.h"
#ifdef __APPLE__
#include "spine/spine.h"
#include "cocos/editor-support/spine/SkeletonAnimation.h"
#else
#include "spine\spine.h"
#include "cocos\editor-support\spine\SkeletonAnimation.h"
#endif
#include <iostream>
#include <fstream>

using namespace spine;
static  MainScene* _MainScene = nullptr;

MainScene::MainScene()
{
	m_Diffcute = Rate_C;
	CombatSign = false;
	CanPlaySound = true;
	_MainScene = this;
	ClearCacheDelayTime = 10.0f;
	KeyBoardListener = nullptr;
	//网页跳转
	//Application::getInstance()->openURL("https://www.baidu.com/");
}
MainScene::~MainScene()
{
	removeAllChildrenWithCleanup(true);
}

MainScene* MainScene::GetInstance()
{
	if (!_MainScene)
	{
		_MainScene = new MainScene();
	}
	return _MainScene;
}

bool MainScene::init()
{
	bool bRef = false;

	do
	{
		CC_BREAK_IF(!Scene::init());

#ifdef WIN32 //Create KeyBoard Listner On Win32
		if (!KeyBoardListener)
		{
			KeyBoardListener = EventListenerKeyboard::create();
			KeyBoardListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
			KeyBoardListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyBoardListener, this);
		}
#else
#endif
		sSpellMgr->Init();
		sScriptMgr->InitScripts();
		LoadUnitClassInfo();
		LoadMapInfo();
		LoadFactionInfo();
		LoadItemTemplate();
		LoadQuestGiver();
		LoadExpPerLevelTemplate();
		LoadVendorTemplate();
		sQuestMgr;
		sLootMgr;
		addChild(sEnterGameLayer);

		sLoadingLayer->DisAppear();
		addChild(sLoadingLayer);

		sNotifyMgr->setLocalZOrder(Notify_Layer_Zorder);
		addChild(sNotifyMgr);
		scheduleUpdate();

		bRef = true;
	} while (0);
    
    
    return bRef;
}

const SingleMapInfo* MainScene::GetMapInfo(uint32 _mapid)
{
	if (m_MapInfo.find(_mapid) != m_MapInfo.end())
		return &m_MapInfo[_mapid];
	return nullptr;
}

void MainScene::LoadQuestGiver()
{
	m_QuestGivers.clear();
	Result _result;
	if (sDataMgr->selectUnitDataList(_result,"SELECT creature_id,quest_id FROM quest_giver_template"))
	{
		int CurrentCreature = 0;
		for (Result::iterator itr = _result.begin(); itr != _result.end(); itr++)
		{
			if (m_QuestGivers.empty() || m_QuestGivers.find(CurrentCreature) == m_QuestGivers.end())
			{
				std::list<uint32> TempList;
				m_QuestGivers[itr->second.at(0).GetInt()] = TempList;
				CurrentCreature = itr->second.at(0).GetInt();
			}
			m_QuestGivers[CurrentCreature].push_back(itr->second.at(1).GetInt());
		}
	}
	else
	{
		//Do Sth;
	}
}

uint32 MainScene::GetCurrentExpPerLevel(uint8 playerlevel)
{
	if (m_Exp_Per_Level.find(playerlevel) != m_Exp_Per_Level.end())
		return m_Exp_Per_Level[playerlevel];
	return 2100000000;
}

const std::list<uint32>* MainScene::GetCreatureQuests(uint32 creatureid)
{
	if (m_QuestGivers.find(creatureid) != m_QuestGivers.end())
		return &m_QuestGivers[creatureid];
	return nullptr;
}

bool MainScene::IsQuestGiver(const uint32& _entey)
{
	if (m_QuestGivers.find(_entey) == m_QuestGivers.end())
		return false;
	return true;
}

void MainScene::LoadMapInfo()
{
	m_MapInfo.clear();
	Result _result;
	if (sDataMgr->selectUnitDataList(_result,"SELECT id,background_music_url,can_revive_player,revive_pos_x,revive_pos_y FROM map_template"))
	{
		std::vector<RowInfo> row;
		for (Result::iterator itr = _result.begin(); itr != _result.end(); itr++)
		{
			row = itr->second;
			SingleMapInfo _SingleMapInfo;

			_SingleMapInfo.BackGroundMusicUrl = row.at(1).GetString();
			_SingleMapInfo.CanRevive = row.at(2).GetBool();
			_SingleMapInfo.RevivePosX = row.at(3).GetFloat();
			_SingleMapInfo.RevivePosY = row.at(4).GetFloat();
			m_MapInfo[row.at(0).GetInt()] = _SingleMapInfo;
		}
	}
	else
	{
		//Do Sth;
	}
}

std::string MainScene::GetClassNameByClassID(UnitClasses _var)
{
	std::string Name = "Unknow";
	switch (_var)
	{
	case Saber:		Name = "Saber";		break;
	case Archer:	Name = "Archer";	break;
	case Caster:	Name = "Caster";	break;
	case Lancer:	Name = "Lancer";	break;
	case Assasin:	Name = "Assasin";	break;
	case Rider:		Name = "Rider";		break;
	case Avenger:	Name = "Avenger";	break;
	case Berserker:	Name = "Berserker";	break;
	}
	return Name;
}

std::string MainScene::GetUnitValueNameByID(UnitInt32Value _val)
{
	std::string Name = "UnKnow";
	switch (_val)
	{
	case Max_HP:			Name = "Heath";		break;
	case Max_Mana:			Name = "Mana";		break;
	case Base_Def:			Name = "Defance";	break;
	case Base_Str:			Name = "Strength";	break;
	case Base_Dex:			Name = "Agile";		break;
	case Base_Int:			Name = "Int";		break;
	case Base_Att:			Name = "Attack";	break;
	}
	return Name;
}

uint32 MainScene::FindNearestReviveMap(uint32 CurrentMapid)
{
	if (m_MapInfo[CurrentMapid].CanRevive)
		return CurrentMapid;
	uint32 Front = CurrentMapid - 1;
	uint32 Back = CurrentMapid + 1;
	bool CanCheckFront = true;
	bool CanCheckBack = true;
	while (true)
	{
		if (CanCheckFront)
		{
			if (m_MapInfo.find(Front) != m_MapInfo.end() && m_MapInfo[Front].CanRevive)
			{
				if (m_MapInfo[Front].CanRevive)
					return Front;
			}
			else CanCheckFront = false;
		}

		if (CanCheckBack)
		{
			if (m_MapInfo.find(Back) != m_MapInfo.end())
			{
				if (m_MapInfo[Back].CanRevive)
					return Back;
			}
			else CanCheckBack = false;
		}

		if (!CanCheckFront && !CanCheckBack)
			break;

		++Front; ++Back;
	}
	log("Error To Check Map Revive CurrentMapID = %d", CurrentMapid);
}

bool MainScene::GetFactionFriendly(uint32 factionA, uint32 FactionB)
{
	if (factionA == FactionB || (factionA == 35 || FactionB == 35))
		return true;
	if (factionA == 14 || FactionB == 14)
		return false;
	if (m_Faction_Friendly_Info.find(factionA) != m_Faction_Friendly_Info.end() && m_Faction_Friendly_Info[factionA].find(FactionB) != m_Faction_Friendly_Info[factionA].end())
	{
		return m_Faction_Friendly_Info[factionA][FactionB];
	}
	else if (m_Faction_Friendly_Info.find(FactionB) != m_Faction_Friendly_Info.end() && m_Faction_Friendly_Info[FactionB].find(factionA) != m_Faction_Friendly_Info[FactionB].end())
	{
		return m_Faction_Friendly_Info[FactionB][factionA];
	}
	return false;
}

void MainScene::LoadItemTemplate()
{
	m_ItemTemplate.clear(); 
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,"SELECT entry,name,slot_type,url,quality,buy_price,sell_price,require_class,require_level,max_count,item_set,min_damage,max_damage,stat_type1,stat_value1,stat_type2,stat_value2,stat_type3,stat_value3,stat_type4,stat_value4,stat_type5,stat_value5 FROM item_template"))
	{
		if (_Result.empty())
		{
			char msg[255];
			snprintf(msg, 255, "ErrDB: LoadMapInfo: Empty Template For Loading Template 'item_template'");
			sNotifyMgr->ShowNotify(msg);
			return;
		}
		else
		{
			std::vector<RowInfo> info;
			for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
			{
				info = ito->second;
				ItemTemplate _template;
				_template.Entry			= info.at(0).GetInt();
				_template.Name			= info.at(1).GetString();
				_template.SlotType		= (PlayerEquipSlots)info.at(2).GetInt();
				_template.Url			= info.at(3).GetString();
				_template.Quality		= info.at(4).GetInt();
				_template.BuyPrice		= info.at(5).GetInt();
				_template.SellPrice		= info.at(6).GetInt();
				_template.RequireClass	= (UnitClasses)info.at(7).GetInt();
				_template.RequireLevel	= info.at(8).GetInt();
				_template.MaxCount		= info.at(9).GetInt();
				_template.ItemSet		= info.at(10).GetInt();
				_template.MinDamage		= info.at(11).GetInt();
				_template.MaxDamage		= info.at(12).GetInt();
				for (int i = 13; i != 23; i++)
				{
					switch (info.at(i).GetInt())
					{
					case Max_HP:
					case Max_Mana:
					case Base_Def:
					case Base_Str:
					case Base_Dex:
					case Base_Int:
					{
						uint32 Type = info.at(i).GetInt();
						uint32 Value = info.at(++i).GetInt();
						log("%d,%d", Type, Value);
						_template.Values[Type] = Value;
						break;
					}
					default:
						log("Unknow Item Value Type %d , Ingore...", info.at(i).GetInt());
						++i;
						break;
					}
				}
				m_ItemTemplate[_template.Entry] = _template;
			}
		}
	}
}

const ItemTemplate* MainScene::GetItemTemplate(const uint32& entry)
{
	if (m_ItemTemplate.find(entry) != m_ItemTemplate.end())
		return &m_ItemTemplate[entry];
	return nullptr;
}

void MainScene::LoadExpPerLevelTemplate()
{
	m_Exp_Per_Level.clear();
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,"SELECT level,exp FROM level_exp"))
	{
		if (_Result.empty())
		{
			char msg[255];
			snprintf(msg, 255, "ErrDB: LoadMapInfo: Empty Template For Loading Map 'level_exp'");
			sNotifyMgr->ShowNotify(msg);
			return;
		}
		else
		{
			int tempexp = 0;
			for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
				m_Exp_Per_Level[ito->second.at(0).GetInt()] = ito->second.at(1).GetInt();
		}
	}
}

const VendorList* MainScene::GetCreatureVendorInfo(uint32 creature_id)
{
	if (m_VendorTemplate.find(creature_id) != m_VendorTemplate.end())
		return &m_VendorTemplate[creature_id];
	return nullptr;
}

void MainScene::LoadVendorTemplate()
{
	m_VendorTemplate.clear();
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,"SELECT creature_id,item_id,exchange_cost FROM npc_vendor"))
	{
		if (_Result.empty())
		{

		}
		else
		{
			std::vector<RowInfo> info;
			uint32 NowCreature = 0;
			for (Result::iterator itr = _Result.begin(); itr != _Result.end(); itr++)
			{
				info = itr->second;
				SingleVendorInfo _vendorinfo;

				if (NowCreature != info.at(0).GetInt())
				{
					std::list<SingleVendorInfo> __info;
					m_VendorTemplate[info.at(0).GetInt()] = __info;
					NowCreature = info.at(0).GetInt();
				}
				_vendorinfo.Item_Id = info.at(1).GetInt();
				_vendorinfo.Exchange_Cost = info.at(0).GetInt();
				m_VendorTemplate[info.at(0).GetInt()].push_back(_vendorinfo);
			}
		}
	}
}

void MainScene::LoadFactionInfo()
{
	m_Faction_Friendly_Info.clear();
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,"SELECT main_faction_id,match_faction_id,isfrendly FROM faction_template"))
	{
		if (_Result.empty())
		{
			char msg[255];
			snprintf(msg, 255, "ErrDB: LoadMapInfo: Empty Template For Loading Map 'faction_template'");
			sNotifyMgr->ShowNotify(msg);
			return;
		}
		else
		{
			for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
			{
				if (m_Faction_Friendly_Info.find(ito->second.at(0).GetInt()) == m_Faction_Friendly_Info.end())
				{
					std::map<uint32, bool> m_map;
					m_Faction_Friendly_Info[ito->second.at(0).GetInt()] = m_map;
				}
				if (m_Faction_Friendly_Info[ito->second.at(0).GetInt()].find(ito->second.at(1).GetInt()) == m_Faction_Friendly_Info[ito->second.at(0).GetInt()].end())
				{
					bool IsFriend = false;
					IsFriend = ito->second.at(2).GetBool();
					m_Faction_Friendly_Info[ito->second.at(0).GetInt()][ito->second.at(1).GetInt()] = IsFriend;
				}
				else
				{
					bool IsFriend = false;
					IsFriend = ito->second.at(2).GetBool();
					m_Faction_Friendly_Info[ito->second.at(0).GetInt()][ito->second.at(1).GetInt()] = IsFriend;
				}
			}
		}
	}
}

void MainScene::LoadUnitClassInfo()
{
	m_UnitClasses_Class_Info.clear();
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,"SELECT classid,str_pre_level,dex_pre_level,int_pre_level,def_pre_level,hp_pre_level,name,base_hp,base_str,base_dex,base_int,base_def FROM unit_class_info"))
	{
		if (_Result.empty())
		{
			char msg[255];
			snprintf(msg, 255, "ErrDB: LoadMapInfo: Empty Template %s For Loading Map 'unit_class_info'");
			sNotifyMgr->ShowNotify(msg);
			return;
		}
		else
		{
			for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
			{
				std::vector<RowInfo> row = ito->second;
				if (row.empty()) continue;
				ClassInfo _info;
				_info.ModifyStrPerLevel										= row.at(1).GetFloat();
				_info.ModifyDexPerLevel										= row.at(2).GetFloat();
				_info.ModifyIntPerLevel										= row.at(3).GetFloat();
				_info.ModifyDefPerLevel										= row.at(4).GetFloat();
				_info.ModifyHpPerLevel										= row.at(5).GetFloat();
				_info.ClassName												= row.at(6).GetString();
				_info.BaseHp												= row.at(7).GetInt();
				_info.BaseStr												= row.at(8).GetInt();
				_info.BaseDex												= row.at(9).GetInt();
				_info.BaseInt												= row.at(10).GetInt();
				_info.BaseDef												= row.at(11).GetInt();
				m_UnitClasses_Class_Info[(UnitClasses)row.at(0).GetInt()]	= _info;
			}
		}
	}
}

SkeletonAnimation* MainScene::GetAnimationByClass(UnitClasses _class)
{
	SkeletonAnimation* SkeletonAnimation = nullptr;
	switch (_class)
	{
	case Saber:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber_Thrid_View.json", "Black_Saber_Thrid_View.atlas", 0.4f);
		break;
	case Archer:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber_Thrid_View.json", "Black_Saber_Thrid_View.atlas", 0.4f);
		break;
	case Caster:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber_Thrid_View.json", "Black_Saber_Thrid_View.atlas", 0.4f);
		break;
	case Lancer:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber_Thrid_View.json", "Black_Saber_Thrid_View.atlas", 0.4f);
		break;
	case Assasin:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber_Thrid_View.json", "Black_Saber_Thrid_View.atlas", 0.4f);
		break;
	case Rider:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber_Thrid_View.json", "Black_Saber_Thrid_View.atlas", 0.4f);
		break;
	case Avenger:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber_Thrid_View.json", "Black_Saber_Thrid_View.atlas", 0.4f);
		break;
	case Berserker:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber_Thrid_View.json", "Black_Saber_Thrid_View.atlas", 0.4f);
		break;
	}
	return SkeletonAnimation;
}

void MainScene::SwapLayer(int _instead, int removetag,int mapid)
{
	if (Layer* OldLayer = (Layer*)getChildByTag(removetag))
	{
		OldLayer->removeAllChildrenWithCleanup(true);
		OldLayer->removeFromParentAndCleanup(true);
		Layer* NewLayer = nullptr;
		switch (_instead)
		{
			case EnterGame_Layer_Tag:
			{
				sPlayerTalkLayer->removeFromParentAndCleanup(true);
				addChild(sEnterGameLayer);
				break;
			}
			case Loading_Layer_Tag:
			{
				addChild(sLoadingLayer);
				break;
			}
			case Typing_Layer_Tag:
			{
				//NewLayer = TypingLayer::create();
				//addChild(NewLayer);
				break;
			}
			case Chose_Character_Layer_Tag:
			{
				addChild(sChoseCharacter);
				break;
			}
			case Create_Character_Layer_Tag:
			{
				addChild(sCreate_Character);
				break;
			}
			case Main_Map_Layer_Tag:
			{
				NewLayer = new Main_Map_Layer(mapid);
				addChild(NewLayer);
				break;
			}
		}
	}
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

Sprite* MainScene::ShowDiffcuteImage()
{
	if (Sprite* Temp = Sprite::create("Diffcute_Frame.png"))
	{
		Temp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		char msg[255];
		snprintf(msg, 255, "diffcute_%d.png", m_Diffcute);
		Sprite* Rate = Sprite::create(msg);
		Rate->setPosition(Temp->getContentSize().width / 2, Temp->getContentSize().height / 2);
		Temp->addChild(Rate);
		Temp->runAction(CCSequence::create(CCFadeOut::create(1.0f), CCDelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(MainScene::DisTorySprite, this, Temp)), NULL));
		Rate->runAction(CCSequence::create(CCFadeOut::create(1.0f), CCDelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(MainScene::DisTorySprite, this, Rate)), NULL));
		return Temp;
	}
	return nullptr;
}

Sprite* MainScene::ShowCombatSignImage()
{
	if (Sprite* Temp = Sprite::create("Diffcute_Frame.png"))
	{
		Temp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		std::string url = "";
		CombatSign ? url = "Combat_Sign_0.png" : url = "Combat_Sign_1.png";
		Sprite* Rate = Sprite::create(url.c_str());
		Rate->setPosition(Temp->getContentSize().width / 2, Temp->getContentSize().height / 2);
		Temp->addChild(Rate);
		Temp->runAction(CCSequence::create(CCFadeOut::create(1.0f), CCDelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(MainScene::DisTorySprite, this, Temp)), NULL));
		Rate->runAction(CCSequence::create(CCFadeOut::create(1.0f), CCDelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(MainScene::DisTorySprite, this, Rate)), NULL));
		return Temp;
	}
	return nullptr;
}

Sprite* MainScene::ShowSoundImage()
{
	if (Sprite* Temp = Sprite::create("Diffcute_Frame.png"))
	{
		Temp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		std::string url = "";
		CanPlaySound ? url = "Combat_Sign_0.png" : url = "Combat_Sign_1.png";
		Sprite* Rate = Sprite::create(url.c_str());
		Rate->setPosition(Temp->getContentSize().width / 2, Temp->getContentSize().height / 2);
		Temp->addChild(Rate);
		Temp->runAction(CCSequence::create(CCFadeOut::create(1.0f), CCDelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(MainScene::DisTorySprite, this, Temp)), NULL));
		Rate->runAction(CCSequence::create(CCFadeOut::create(1.0f), CCDelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(MainScene::DisTorySprite, this, Rate)), NULL));
		return Temp;
	}
	return nullptr;
}

void MainScene::DisTorySprite(Sprite* pSprite)
{
	if (pSprite)
	{
		pSprite->removeAllChildrenWithCleanup(true);
		pSprite->removeFromParentAndCleanup(true);
	}
}

bool MainScene::IsMoveKey(EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_A:
		case EventKeyboard::KeyCode::KEY_S:
		case EventKeyboard::KeyCode::KEY_D:
		case EventKeyboard::KeyCode::KEY_W:
		case EventKeyboard::KeyCode::KEY_K:
			return true;
	}
	return false;
}
void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (sPlayer && IsMoveKey(keyCode))
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_A:
			sPlayer->SetMoveKeyEnable(MoveKey_Left, true);
			break;
		case EventKeyboard::KeyCode::KEY_S:
			sPlayer->SetMoveKeyEnable(MoveKey_Down, true);
			break;
		case EventKeyboard::KeyCode::KEY_D:
			sPlayer->SetMoveKeyEnable(MoveKey_Right, true);
			break;
		case EventKeyboard::KeyCode::KEY_W:
			sPlayer->SetMoveKeyEnable(MoveKey_Up, true);
			break;
		}
	}
	if (sPlayer)
	{
		sPlayer->PlayerActionMgr()->OnPlayerPressKey(keyCode);
	}
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (sPlayer && IsMoveKey(keyCode))
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_A:
			sPlayer->SetMoveKeyEnable(MoveKey_Left, false);
			break;
		case EventKeyboard::KeyCode::KEY_S:
			sPlayer->SetMoveKeyEnable(MoveKey_Down, false);
			break;
		case EventKeyboard::KeyCode::KEY_D:
			sPlayer->SetMoveKeyEnable(MoveKey_Right, false);
			break;
		case EventKeyboard::KeyCode::KEY_W:
			sPlayer->SetMoveKeyEnable(MoveKey_Up, false);
			break;
		}
	}
}

std::vector<Sprite*> MainScene::GetNumberSpriteByInt(int number)
{
	std::vector<int> numberVector;
	std::vector<Sprite*> SpriteVector;
	if (!number)
	{
		Sprite* TempSprite = Sprite::create("number_0.png");
		SpriteVector.push_back(TempSprite);
		return SpriteVector;
	}
	while (number)
	{
		int temp = number % 10;
		numberVector.push_back(temp);
		number = number / 10;
	}
	int i = 0;
	while (numberVector.size())
	{
		char msg[255];
		snprintf(msg, 255, "number_%d.png", numberVector.at(numberVector.size() - 1));
		Sprite* Temp = Sprite::create(msg);
		SpriteVector.push_back(Temp);
		numberVector.pop_back();
		i++;
	}
	return SpriteVector;
}

void MainScene::update(float diff)
{

}