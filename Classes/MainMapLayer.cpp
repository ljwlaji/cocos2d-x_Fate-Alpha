#include "MainMapLayer.h"
#include "Player.h"
#include "Npc.h"
#include "Unit.h"
#include "Monster.h"
#include "DataMgr.h"
#include "LoadingUILayer.h"
#include "NotifyMgr.h"
#include "HelloWorldScene.h"
#include "PlayerUILayer.h"

Main_Map_Layer* _Main_Map_Layer = nullptr;

Main_Map_Layer::Main_Map_Layer(int MapId)
{
	_Main_Map_Layer = this;
	init();
	autorelease();
	Visablesize = Director::getInstance()->getVisibleSize();
	setTag(Main_Map_Layer_Tag);
	m_Mapid = MapId;
	m_WaitForLoadingNpcs.clear();
	m_WaitForLoadingMonsters.clear();
	for (int i = Object_GroundSprite; i != Object_Endl; i++)
	{
		std::vector<WaitForLoadingObjectTemplate> TempSprite;
		TempSprite.clear();
		m_WaitForLoadingObjects[(MapObjectType)i] = TempSprite;
	}
	SwapMap(m_Mapid);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Main_Map_Layer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Main_Map_Layer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	m_TouchedSprite = nullptr;
	m_TouchedType = Touch_None;
}

Main_Map_Layer::~Main_Map_Layer()
{
	_Main_Map_Layer = nullptr;
}

Main_Map_Layer* Main_Map_Layer::GetInstance()
{
	if (!_Main_Map_Layer)
		return nullptr;

	return _Main_Map_Layer;
}


bool Main_Map_Layer::onTouchBegan(Touch *touch, Event *unused_event)
{
	m_TouchedType = Touch_None;
	m_TouchedSprite = nullptr;
	if (!sPlayer)
		return false;
	for (int i = 0; i != m_NpcVector.size(); i++)
	{
		if (Sprite* Temp = m_NpcVector.at(i))
		{
			if (Temp->IsContectPoint(touch->getLocation()))
			{
				m_TouchedType = Touch_Npc;
				m_TouchedSprite = Temp;
				return true;
			}
		}
	}
	for (int i = 0; i != m_MonsterVector.size(); i++)
	{
		if (Sprite* Temp = m_MonsterVector.at(i))
		{
			if (Temp->IsContectPoint(touch->getLocation()))
			{
				m_TouchedType = Touch_Monster;
				m_TouchedSprite = Temp;
				return true;
			}
		}
	}

	return false;
}

void Main_Map_Layer::onTouchEnded(Touch *touch, Event *unused_event)
{
	switch (m_TouchedType)
	{
	case Touch_Npc:
		if (Npc* pNpc = (Npc*)m_TouchedSprite)
		{
			pNpc->OnGossipHello(sPlayer);
		}
		break;
	case Touch_Monster:
		if (Monster* pMonster = (Monster*)m_TouchedSprite)
		{
			sPlayer->SetPlayerTarget(pMonster);
			//pMonster->OnGossipHello(sPlayer);
		}
		break;
	case Touch_Player:
		break;
	}
}

bool Main_Map_Layer::SwapMap(int insteadid, bool FirstLoad)
{
	if (!insteadid)
		return false;
	sPlayer->ReSetPlayerTarget();
	m_MaxSize = 0;
	setTouchEnabled(false);
	unscheduleUpdate();
	sLoadingLayer->Show();
	m_Mapid = insteadid;
	if (!FirstLoad)
	{
		stopAllActions();
		if (sPlayer && sPlayer->getParent())
		{
			sPlayer->retain();
			sPlayer->removeFromParent();
		}
	}
	removeAllChildrenWithCleanup(true);
	if (!FirstLoad)
	{
		addChild(sPlayer);
		sPlayer->setPosition(Visablesize.x * 0.4f, Visablesize.y * 0.25f);
		sPlayer->setLocalZOrder(PLAYER_ZORDER);
		sPlayer->scheduleUpdate();
	}
	else
	{
		sPlayerUi->setLocalZOrder(UI_LAYER_ZORDER);
		sGame->addChild(sPlayerUi);
	}
	ClearVectors();
	FillLoadVectors(insteadid);
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
	scheduleUpdate();
	setTouchEnabled(true);
	return true;
}

void Main_Map_Layer::FillLoadVectors(int id)
{
	//读取地图信息添加到待载入列表
	TotalLoadingSize = 0;
	std::string LoadTemplateName = "";
	for (std::map<MapObjectType, std::vector<WaitForLoadingObjectTemplate>>::iterator itr = m_WaitForLoadingObjects.begin(); itr != m_WaitForLoadingObjects.end(); itr++)
	{
		switch (itr->first)
		{
			case Object_GroundSprite:	LoadTemplateName = "map_ground_template";			break;
			case Object_BackGround:		LoadTemplateName = "map_back_ground_template";		break;
			case Object_ForeGround:		LoadTemplateName = "map_fore_ground_template";		break;
			case Object_FloatingObject:	LoadTemplateName = "map_floating_object_template";	break;
		}
		char msg[255];
		snprintf(msg, 255, "SELECT url,pos_x,pos_y FROM %s WHERE map_id = %d", LoadTemplateName.c_str(), id);
		Result _Result;
		if (sDataMgr->selectUnitDataList(msg, _Result))
		{
			if (_Result.empty())
			{
				char msg[255];
				snprintf(msg, 255, "ErrDB: LoadMapInfo: Empty Template %s For Loading Map %d", LoadTemplateName.c_str(), id);
				sNotifyMgr->ShowNotify(msg);	continue;
			}
			else
			{
				for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
				{
					std::vector<RowInfo> row = ito->second;
					if (row.empty()) continue;
					WaitForLoadingObjectTemplate _SingleTemplate;
					_SingleTemplate.url			= row.at(0).GetString();
					_SingleTemplate.pos_x		= row.at(1).GetFloat();
					_SingleTemplate.pos_y		= row.at(2).GetFloat();
					itr->second.push_back(_SingleTemplate);
					TotalLoadingSize++;
				}
			}
		}
		else
		{
			char msg[255];
			snprintf(msg, 255, "ErrDB: LoadMapInfo: Error Loading %s For Loading Map %d", LoadTemplateName.c_str(), id);
			sNotifyMgr->ShowNotify(msg);
			continue; 
		}
	}

	for (int i = 0; i != 2; i++)
	{
		i ? LoadTemplateName = "map_npc_template" : LoadTemplateName = "map_monster_template";
		char get[255];//			0	 1		2	 3		4	5	   6		7		8		9		10
		snprintf(get, 255, "SELECT json,atlas,pos_x,pos_y,guid,entry,scale,script_name,faction,level,class FROM %s WHERE map_id = %d", LoadTemplateName.c_str(), id);
		Result _Result;
		if (sDataMgr->selectUnitDataList(get, _Result))
		{
			if (_Result.empty())
			{
				log("ErrDB: LoadMapInfo: Empty Template %s For Loading Map %d", LoadTemplateName.c_str(), id);	continue;
			}
			else
			{
				for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
				{
					std::vector<RowInfo> row = ito->second;
					if (row.empty()) continue;
					WaitFroLoadingUnitTemplate _SingleTemplate;
					_SingleTemplate.json			= row.at(0).GetString();
					_SingleTemplate.atlas			= row.at(1).GetString();
					_SingleTemplate.pos_x			= row.at(2).GetFloat();
					_SingleTemplate.pos_y			= row.at(3).GetFloat();
					_SingleTemplate.guid			= row.at(4).GetInt();
					_SingleTemplate.entry			= row.at(5).GetInt();
					_SingleTemplate.Scale			= row.at(6).GetFloat();
					_SingleTemplate.ScriptName		= row.at(7).GetString();
					_SingleTemplate.faction			= row.at(8).GetInt();
					_SingleTemplate.Level			= row.at(9).GetInt();
					_SingleTemplate.Class			= row.at(10).GetInt();
					i ? m_WaitForLoadingNpcs.push_back(_SingleTemplate) : m_WaitForLoadingMonsters.push_back(_SingleTemplate);
					CreaturesTemplate[_SingleTemplate.guid] = _SingleTemplate;
					TotalLoadingSize++;
				}
			}
		}
		else { log("ErrDB: LoadMapInfo: Error Loading %s For Loading Map %d", LoadTemplateName.c_str(), id);	continue; }
	}
	LoadedSize = 0;
	NeedCreateObjects = true;
}

void Main_Map_Layer::ClearVectors()
{
	m_NpcVector.clear();
	m_MonsterVector.clear();
	m_MapGroundSpriteVector.clear();
	m_MapFloatingObjectVector.clear();
	m_MapBackGroundVector.clear();
	m_MapForeGroundVector.clear();
	m_WaitForLoadingNpcs.clear();
	m_WaitForLoadingMonsters.clear();
	CreaturesTemplate.clear();
	for (std::map<MapObjectType, std::vector<WaitForLoadingObjectTemplate>>::iterator itr = m_WaitForLoadingObjects.begin(); itr != m_WaitForLoadingObjects.end(); itr++)
		itr->second.clear();
}

void Main_Map_Layer::CreateObjects()
{
	for (std::map<MapObjectType, std::vector<WaitForLoadingObjectTemplate>>::iterator itr = m_WaitForLoadingObjects.begin(); itr != m_WaitForLoadingObjects.end(); itr++)
	{
		if (!itr->second.empty())
		{
			WaitForLoadingObjectTemplate _template = itr->second.at(itr->second.size() - 1);
			Sprite* Temp = Sprite::create(_template.url.c_str());
			if (itr->first == Object_GroundSprite)
				Temp->SetRealPosition(Temp->getBoundingBox().size.width * 0.5 + (itr->second.size() - 1) * Temp->getBoundingBox().size.width, Temp->getBoundingBox().size.height / 2);
			else
				Temp->SetRealPosition(_template.pos_x, _template.pos_y);
			addChild(Temp);
			switch (itr->first)
			{
				case Object_GroundSprite:	m_MapGroundSpriteVector.push_back(Temp);	break;
				case Object_BackGround:		m_MapBackGroundVector.push_back(Temp);		break;
				case Object_ForeGround:		m_MapForeGroundVector.push_back(Temp);		break;
				case Object_FloatingObject: m_MapFloatingObjectVector.push_back(Temp);	break;
			}
			itr->second.pop_back();
			return;
		}
	}
	if (!m_WaitForLoadingNpcs.empty())
	{
		WaitFroLoadingUnitTemplate _template = m_WaitForLoadingNpcs.at(m_WaitForLoadingNpcs.size() - 1);
		SkeletonAnimation* sk = spine::SkeletonAnimation::createWithJsonFile(_template.json, _template.atlas, _template.Scale);
		Npc* Temp = new Npc(sk, _template.entry, _template.guid);
		Temp->SetRealPosition(_template.pos_x, _template.pos_y);
		sk->setAnimation(0, "idle", true);
		m_WaitForLoadingNpcs.pop_back();
		m_NpcVector.push_back(Temp);
		addChild(Temp);
		return;
	}
	if (!m_WaitForLoadingMonsters.empty())
	{
		WaitFroLoadingUnitTemplate _template = m_WaitForLoadingMonsters.at(m_WaitForLoadingMonsters.size() - 1);
		SkeletonAnimation* sk = spine::SkeletonAnimation::createWithJsonFile(_template.json, _template.atlas, _template.Scale);
		Monster* Temp = new Monster(sk, _template.entry, _template.guid);
		Temp->SetRealPosition(Visablesize.x * _template.pos_x / 100, Visablesize.y * _template.pos_y / 100);
		sk->setAnimation(0, "idle", true);
		m_WaitForLoadingMonsters.pop_back();
		m_MonsterVector.push_back(Temp);
		addChild(Temp);
		return;
	}

	Size s = Director::getInstance()->getWinSize();
	m_MaxSize = 0;
	for (int i = 0; i != m_MapGroundSpriteVector.size(); i++)
		m_MaxSize += m_MapGroundSpriteVector.at(i)->getBoundingBox().size.width;

	runAction(Follow::create(sPlayer, Rect(0, 0, m_MaxSize, s.height)));

	NeedCreateObjects = false;

	m_Next_Map_Door = Sprite::create("TeleportDoor.png");
	m_Next_Map_Door->setScaleX(-1.0f);
	m_Next_Map_Door->setPosition(m_MaxSize - m_Next_Map_Door->getBoundingBox().size.width * 1.5, Visablesize.y * 0.4f);
	addChild(m_Next_Map_Door);

	m_Older_Map_Door = Sprite::create("TeleportDoor.png");
	m_Older_Map_Door->setPosition(m_Older_Map_Door->getBoundingBox().size.width, Visablesize.y * 0.4f);
	addChild(m_Older_Map_Door);

}

bool GetBigger(const Unit*a, const Unit*b)
{
	return a->getBoundingBox().origin.y > b->getBoundingBox().origin.y;
}

void Main_Map_Layer::ReCheckZorder()
{
	CalcZorderList.clear();
	for (int i = 0; i != m_NpcVector.size(); i++)
		CalcZorderList.push_back(m_NpcVector.at(i));
	for (int i = 0; i != m_MonsterVector.size(); i++)
		CalcZorderList.push_back(m_MonsterVector.at(i));
	if (sPlayer)
		CalcZorderList.push_back(sPlayer);

	CalcZorderList.sort(GetBigger);
	int Zorder = 100;
	for (Listitr = CalcZorderList.begin(); Listitr != CalcZorderList.end(); Listitr++, Zorder++)
		(*Listitr)->setZOrder(Zorder);
}

void Main_Map_Layer::update(float diff)
{
	if (NeedCreateObjects)
	{
		uint8 tage = ((float)LoadedSize / (float)TotalLoadingSize) * 100.0f;
		sLoadingLayer->SetPretage(tage);
		CreateObjects();
		LoadedSize++;
		return;
	}
	else
	{
		if (!sPlayer)
			return;

		if (m_Next_Map_Door->getBoundingBox().intersectsRect(sPlayer->getBoundingBox()))
			SwapMap(m_Mapid + 1, false);
		else if (m_Older_Map_Door->getBoundingBox().intersectsRect(sPlayer->getBoundingBox()))
			SwapMap(m_Mapid - 1, false);

		if (m_CheckZorderTimer <= diff)
		{
			ReCheckZorder();
			m_CheckZorderTimer = 1.0f;
		}
		else m_CheckZorderTimer -= diff;
	}
}

WaitFroLoadingUnitTemplate Main_Map_Layer::GetCreatureTemplate(uint32 guid)
{ 
	WaitFroLoadingUnitTemplate _template; 
	if (CreaturesTemplate.find(guid) != CreaturesTemplate.end()) 
		_template = CreaturesTemplate.find(guid)->second; 
	return _template; 
}

Unit* Main_Map_Layer::GetNearestUnitForUnit(Unit* pUnit, bool SelectForTarget, bool CheckAlive)
{
	Unit* NearestUnit = nullptr;
	float NearestDistance = 9999.0f;
	for (int i = 0; i != m_MonsterVector.size(); i++)
	{
		if (CheckAlive && !m_MonsterVector.at(i)->IsAlive()) continue;
		if (SelectForTarget && pUnit->IsFrendlyTo(m_MonsterVector.at(i))) continue;
		if (m_MonsterVector.at(i) == pUnit) continue;

		float TempDis = abs(pUnit->getPositionX() - m_MonsterVector.at(i)->getPositionX());
		if (TempDis < NearestDistance)
		{
			NearestDistance = TempDis;
			NearestUnit = m_MonsterVector.at(i);
		}
	}

	for (int i = 0; i != m_NpcVector.size(); i++)
	{
		if (CheckAlive && !m_NpcVector.at(i)->IsAlive()) continue;
		if (SelectForTarget && pUnit->IsFrendlyTo(m_NpcVector.at(i))) continue;
		if (m_NpcVector.at(i) == pUnit) continue;

		float TempDis = abs(pUnit->getPositionX() - m_NpcVector.at(i)->getPositionX());
		if (TempDis < NearestDistance)
		{
			NearestDistance = TempDis;
			NearestUnit = m_NpcVector.at(i);
		}
	}

	if (pUnit != sPlayer && abs(pUnit->getPositionX() - sPlayer->getPositionX()) < NearestDistance)
	{
		if (SelectForTarget && pUnit->IsFrendlyTo(sPlayer))
			return NearestUnit;
		if (CheckAlive && sPlayer->IsAlive()) 
			return sPlayer;
	}

	return NearestUnit;
}