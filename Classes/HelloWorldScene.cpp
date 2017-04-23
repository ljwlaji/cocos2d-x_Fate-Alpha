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
MainScene::~MainScene(){}

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

#ifdef WIN32 //Win32下创建键盘监听
		KeyBoardListener = EventListenerKeyboard::create();
		KeyBoardListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
		KeyBoardListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyBoardListener, this);
#else
#endif
		sScriptMgr->InitScripts();
		LoadUnitClassInfo();
		EnterLayer = EnterGameLayer::create();
		addChild(EnterLayer);

		Player_Talk_Layer* TalkLayer = Player_Talk_Layer::create();
		addChild(TalkLayer);


		LoadingLayer = LoadingUILayer::create();
		LoadingLayer->DisAppear();
		addChild(LoadingLayer);

		sNotifyMgr->setZOrder(Notify_Layer_Zorder);
		addChild(sNotifyMgr);
		scheduleUpdate();

		bRef = true;
	} while (0);
    
    
    return bRef;
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
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber.json", "Black_Saber.atlas", 1.0f);
		break;
	case Archer:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber.json", "Black_Saber.atlas", 1.0f);
		break;
	case Caster:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber.json", "Black_Saber.atlas", 1.0f);
		break;
	case Lancer:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber.json", "Black_Saber.atlas", 1.0f);
		break;
	case Assasin:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber.json", "Black_Saber.atlas", 1.0f);
		break;
	case Rider:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber.json", "Black_Saber.atlas", 1.0f);
		break;
	case Avenger:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber.json", "Black_Saber.atlas", 1.0f);
		break;
	case Berserker:
		SkeletonAnimation = SkeletonAnimation::createWithJsonFile("Black_Saber.json", "Black_Saber.atlas", 1.0f);
		break;
	}
	return SkeletonAnimation;
}

void MainScene::SwapLayer(int _instead, int removetag,int mapid)
{
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
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
				NewLayer = LoadingUILayer::create();
				addChild(NewLayer);
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
				NewLayer = Chose_Character_Layer::create();
				addChild(NewLayer);
				break;
			}
			case Create_Character_Layer_Tag:
			{
				NewLayer = Create_Character_Layer::create();
				addChild(NewLayer);
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