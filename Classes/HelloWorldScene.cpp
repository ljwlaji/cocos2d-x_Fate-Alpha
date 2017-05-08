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
		KeyBoardListener = EventListenerKeyboard::create();
		KeyBoardListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
		KeyBoardListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyBoardListener, this);
#else
#endif
		sSpellMgr->Init();
		sScriptMgr->InitScripts();
		LoadUnitClassInfo();

		LoadFactionInfo();
		LoadItemTemplate();
		EnterLayer = EnterGameLayer::create();
		addChild(EnterLayer);

		addChild(sPlayerTalkLayer);


		sLoadingLayer->DisAppear();
		addChild(sLoadingLayer);

		sNotifyMgr->setLocalZOrder(Notify_Layer_Zorder);
		addChild(sNotifyMgr);
		scheduleUpdate();

		bRef = true;
	} while (0);
    
    
    return bRef;
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
	Result _Result;//							0	1		2		3	4		5			6			7			8				9		10		11			12			13		14				15			16			17		18			19			20			21			22
	if (sDataMgr->selectUnitDataList("SELECT entry,name,slot_type,url,quality,buy_price,sell_price,require_class,require_level,max_count,item_set,min_damage,max_damage,stat_type1,stat_value1,stat_type2,stat_value2,stat_type3,stat_value3,stat_type4,stat_value4,stat_type5,stat_value5 FROM item_template", _Result))
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
				for (int i = 13, k = 0; i != 23; i++, k++)
				{
					_template.Values[k] = info.at(++i).GetInt();
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

void MainScene::LoadFactionInfo()
{
	m_Faction_Friendly_Info.clear();
	char msg[255];
	snprintf(msg, 255, "SELECT main_faction_id,match_faction_id,isfrendly FROM faction_template");
	Result _Result;
	if (sDataMgr->selectUnitDataList(msg, _Result))
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
	char msg[255];//			0			1			2				3				4			5		6		7		8		9		10			11
	snprintf(msg, 255, "SELECT classid,str_pre_level,dex_pre_level,int_pre_level,def_pre_level,hp_pre_level,name,base_hp,base_str,base_dex,base_int,base_def FROM unit_class_info");
	Result _Result;
	if (sDataMgr->selectUnitDataList(msg, _Result))
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
		OldLayer->removeFromParentAndCleanup(true);
		Layer* NewLayer = nullptr;
		switch (_instead)
		{
			case EnterGame_Layer_Tag:
			{
				NewLayer = EnterGameLayer::create();
				addChild(NewLayer);
				break;
			}
			case Loading_Layer_Tag:
			{
				addChild(sLoadingLayer);
				break;
			}
			case Typing_Layer_Tag:
			{
				NewLayer = TypingLayer::create();
				addChild(NewLayer);
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

Sprite* MainScene::GetNumberSpriteByInt(int number)
{
	std::vector<int> numberVector;
	Sprite* TempSprite;
	if (!number)
	{
		TempSprite = Sprite::create("number_0.png");
		return TempSprite;
	}
	while (number)
	{
		int temp = number % 10;
		numberVector.push_back(temp);
		number = number / 10;
	}
	TempSprite = Sprite::create("1.png");
	int i = 0;
	while (numberVector.size())
	{
		char msg[255];
		snprintf(msg, 255, "number_%d.png", numberVector.at(numberVector.size() - 1));
		Sprite* Temp = Sprite::create(msg);
		Temp->setPosition(Temp->getBoundingBox().size.width / 2 + (i * Temp->getBoundingBox().size.width / 2), TempSprite->getContentSize().height / 2);
		TempSprite->addChild(Temp);
		numberVector.pop_back();
		i++;
	}
	return TempSprite;
}