#include "ChoseCharacterLayer.h"
#include "DataMgr.h"
#include "HelloWorldScene.h"
#include "MainMapLayer.h"
#include "CreateCharacterLayer.h"

static Chose_Character_Layer* _Chose_Character_Layer = nullptr;

Chose_Character_Layer::Chose_Character_Layer()
{
	_Chose_Character_Layer = this;
	setTag(Chose_Character_Layer_Tag);
	CharacterEnumSprite = nullptr;
	VisableSize = Director::getInstance()->getVisibleSize();
	TotalCharacterCount = 0;
	CharacterEnumMap.clear();
	Buttons.clear();
	m_TouchedSprite = nullptr;
	m_TouchType = None;
}

Chose_Character_Layer::~Chose_Character_Layer()
{
	_Chose_Character_Layer = nullptr;
	_eventDispatcher->removeEventListener(listener);
	removeAllChildrenWithCleanup(true);
	listener = nullptr;
}

bool Chose_Character_Layer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		CC_BREAK_IF(!LoadCharacters());
		//if (TotalCharacterCount)
		//	SetChoseedCharacter(CharacterEnumMap.begin()->second.guid);
		InitFrames();
		listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(Chose_Character_Layer::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(Chose_Character_Layer::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		sMusic->stopBackgroundMusic();
		sMusic->playBackgroundMusic("ChosePlayer.mp3", true);
		scheduleUpdate();
		bRef = true;
	} while (0);

	return bRef;
}

bool Chose_Character_Layer::onTouchBegan(Touch *touch, Event *unused_event)
{
	for (int i = 0; i != Buttons.size(); i++)
	{
		if (Sprite* Temp = Buttons.at(i))
		if (Temp->IsContectPoint(touch->getLocation()))
		{
			Temp->setScale(0.8f);
			m_TouchedSprite = Temp;
			m_TouchType = Button;
			return true;
		}
	}
	for (int i = 0; i != CharacterEnumFrame.size(); i++)
	{
		if (Sprite* Temp = CharacterEnumFrame.at(i))
		if (Temp->IsContectPoint(touch->getLocation()))
		{
			Temp->setScale(0.8f);
			m_TouchedSprite = Temp;
			m_TouchType = PlayerFrame;
			return true;
		}
	}
	return false;
}

void Chose_Character_Layer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (m_TouchedSprite)
		m_TouchedSprite->setScale(1.0f);

	if (!m_TouchedSprite->IsContectPoint(touch->getLocation()))
		return;
	switch (m_TouchType)
	{
	case Button:
		if (m_TouchedSprite == EnterButton)
		{
			if (m_ChosedInfo.guid)
			{
				SpritesFadeOut(EnterGame);
				return;
			}
		}
		else if (m_TouchedSprite == CanCelButton)
		{
			SpritesFadeOut(BackToMenu);
			return;
		}
		else
		{
			SpritesFadeOut(GoToCreate);
			return;
		}
		break;
	case PlayerFrame:
		if (m_TouchedSprite->getTag())
		{
			SetChoseedCharacter(m_TouchedSprite->getTag());
		}
		else
		{
			m_ChosedInfo.guid = 0;
			if (CharacterEnumSprite)
				CharacterEnumSprite->setVisible(false);
		}
		break;
	}
}

Chose_Character_Layer* Chose_Character_Layer::GetInstance()
{
	if (!_Chose_Character_Layer)
	{
		_Chose_Character_Layer = Chose_Character_Layer::create();
	}
	return _Chose_Character_Layer;
}

void Chose_Character_Layer::SetChoseedCharacter(uint32 guid)
{
	if (CharacterEnumMap.find(guid) != CharacterEnumMap.end())
	{
		m_ChosedInfo = CharacterEnumMap.find(guid)->second;
		SwapChosedCharacter(m_ChosedInfo.guid);
	}
}


void Chose_Character_Layer::SwapChosedCharacter(uint32 CharacterGuid)
{
	if (CharacterEnumSprite)
		CharacterEnumSprite->removeFromParentAndCleanup(true);
	CharacterEnumSprite = sGame->GetAnimationByClass(m_ChosedInfo.Class);
	if (!CharacterEnumSprite)
		return;
	CharacterEnumSprite->setLocalZOrder(5);
	CharacterEnumSprite->setAnchorPoint(Vec2(0.5, 0));
	CharacterEnumSprite->setPosition(VisableSize.x / 2, VisableSize.y * 0.25f);
	CharacterEnumSprite->setAnimation(0, "idle_chose_character", true);
	addChild(CharacterEnumSprite);
	//SpriteFrameCache::getInstance()->removeSpriteFrames();
	//SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}

void Chose_Character_Layer::InitFrames()
{
		//ChoseCharacterLayerMenuButton
	Sprite* BackGround = Sprite::create("BackGround.png");
	BackGround->SetRealPosition(VisableSize.x / 2, VisableSize.y / 2);
	BackGround->setLocalZOrder(-2);
	addChild(BackGround);


	FactionFrame = Sprite::create(ChoseCharacterLayerFactionFrame);
	FactionFrame->SetRealPosition(FactionFrame->getBoundingBox().size.width / 2, VisableSize.y - FactionFrame->getBoundingBox().size.height / 2);
	addChild(FactionFrame);

	FactionInfo = LabelTTF::create("Please Chose Your \nCharacter.", "Ariral", 28, Size::ZERO, TextHAlignment::LEFT);
	// A Way To Change Font Name Need Test At Device;
	/*FactionInfo->setFontName("微软雅黑");*/
	FactionInfo->setAnchorPoint(Vec2(0, 1));
	FactionInfo->setPosition(FactionFrame->getContentSize().width * 0.15f, FactionFrame->getContentSize().height * 0.6f);
	FactionFrame->addChild(FactionInfo);

	ClassFrame = Sprite::create(ChoseCharacterLayerClassFrame);
	ClassFrame->SetRealPosition(ClassFrame->getBoundingBox().size.width / 2, FactionFrame->getBoundingBox().origin.y - ClassFrame->getBoundingBox().size.height * 0.45f);
	addChild(ClassFrame);

	ClassInfo = LabelTTF::create("Please Chose Your \nCharacter.", "Ariral", 28, Size::ZERO, TextHAlignment::LEFT);
	ClassInfo->setAnchorPoint(Vec2(0, 1));
	ClassInfo->setPosition(ClassFrame->getContentSize().width * 0.15f, ClassFrame->getContentSize().height * 0.75f);
	ClassFrame->addChild(ClassInfo);

	ButtomBar = Sprite::create(ChoseCharacterLayerButtomBar);
	ButtomBar->SetRealPosition(VisableSize.x / 2, ButtomBar->getBoundingBox().size.height / 2);
	addChild(ButtomBar);

	EnterButton = Sprite::create(ChoseCharacterLayerEnterButton);
	EnterButton->SetRealPosition(VisableSize.x / 2 - EnterButton->getBoundingBox().size.width / 2, ButtomBar->getPositionY() * 0.9f);
	addChild(EnterButton);
	Buttons.push_back(EnterButton);

	CanCelButton = Sprite::create(ChoseCharacterLayerCancelButton);
	CanCelButton->SetRealPosition(VisableSize.x / 2 + CanCelButton->getBoundingBox().size.width / 2, ButtomBar->getPositionY() * 0.9f);
	addChild(CanCelButton);
	Buttons.push_back(CanCelButton);

	ChoseCharacterFrame = Sprite::create(ChoseCharacterLayerSelectFrame);
	ChoseCharacterFrame->setPosition(VisableSize.x - ChoseCharacterFrame->getBoundingBox().size.width / 2, VisableSize.y - ChoseCharacterFrame->getBoundingBox().size.height / 2);
	addChild(ChoseCharacterFrame);

	ChoseCharacterTitle = Sprite::create(ChoseCharacterLayerSelectTitle);
	ChoseCharacterTitle->setPosition(ChoseCharacterFrame->getContentSize().width / 2, ChoseCharacterFrame->getContentSize().height - ChoseCharacterTitle->getBoundingBox().size.height * 0.9f);
	ChoseCharacterFrame->addChild(ChoseCharacterTitle);

	float PosY = 0;
	int k = 0;
	for (std::map<uint32, CharacterEnumInfo>::iterator itr = CharacterEnumMap.begin(); itr != CharacterEnumMap.end(); itr++)
	{
		Sprite* TempFrame = Sprite::create(ChoseCharacterLayerCharacterFrame);
		TempFrame->setPosition(ChoseCharacterTitle->getPositionX() - TempFrame->getBoundingBox().size.width * 0.06f, ChoseCharacterTitle->getPositionY() - TempFrame->getBoundingBox().size.height * 0.7f - (k * TempFrame->getBoundingBox().size.height));
		ChoseCharacterFrame->addChild(TempFrame);
		TempFrame->setTag(itr->first);
		k++;
		CharacterEnumFrame.push_back(TempFrame);

		LabelTTF* CharacterName = LabelTTF::create(itr->second.name.c_str(), "Arial", 35);
		CharacterName->setAnchorPoint(Vec2(0, 0));
		CharacterName->setPosition(TempFrame->getContentSize().width * 0.3f, TempFrame->getContentSize().height / 2);
		TempFrame->addChild(CharacterName);

		LabelTTF* ClassString = LabelTTF::create(GetClassNameByClass(itr->second.Class).c_str(), "Arial", 28);
		ClassString->setAnchorPoint(Vec2(0, 1));
		ClassString->setPosition(TempFrame->getContentSize().width * 0.3f, TempFrame->getContentSize().height / 2);
		TempFrame->addChild(ClassString);

		char msg[255];
		snprintf(msg, 255, "Level: %d", itr->second.Level);
		LabelTTF* LevelText = LabelTTF::create(msg, "Arial", 28);
		LevelText->setAnchorPoint(Vec2(0, 1));
		LevelText->setPosition(TempFrame->getPositionX() + TempFrame->getBoundingBox().size.width * 0.2f, TempFrame->getContentSize().height / 2);
		TempFrame->addChild(LevelText);
	}
	
	CreateCharacterButton = Sprite::create(ChoseCharacterLayerCreateButton);
	CreateCharacterButton->setPosition(ChoseCharacterFrame->getContentSize().width / 2, CreateCharacterButton->getBoundingBox().size.height);
	ChoseCharacterFrame->addChild(CreateCharacterButton);
	Buttons.push_back(CreateCharacterButton);

	//SpritesFadeIn();
}

std::string Chose_Character_Layer::GetClassNameByClass(const UnitClasses& _class)
{
	std::string Name = "Unknow";
	switch (_class)
	{
	case Saber:		Name = "Saber     ";	break;
	case Archer:	Name = "Archer    ";	break;
	case Caster:	Name = "Caster    ";	break;
	case Lancer:	Name = "Lancer    ";	break;
	case Assasin:	Name = "Assasin   ";	break;
	case Rider:		Name = "Rider     ";	break;
	case Avenger:	Name = "Avenger   ";	break;
	case Berserker:	Name = "Berserker ";	break;
	}
	return Name;
}

void Chose_Character_Layer::_SwapLayer(FadeType _FadeType)
{
	int pLayer;
	switch (_FadeType)
	{
	case BackToMenu:
		pLayer = EnterGame_Layer_Tag;
		break;
	case GoToCreate:
		pLayer = Create_Character_Layer_Tag;
		break;
	case EnterGame:
		if (!sPlayer)
		{
			SkeletonAnimation* sk = sGame->GetAnimationByClass(m_ChosedInfo.Class, false);
			Player* _player = new Player(sk, m_ChosedInfo);
			if (!_player->CreatePlayer())
				return;
			_player->GetVision()->setAnimation(0, "idle_normal", true);
		}
		sGame->SwapLayer(Main_Map_Layer_Tag, getTag(), sPlayer->GetMapid());
		if (sMainMap && sPlayer)
		{
			sMainMap->addChild(sPlayer);
			sPlayer->SetCurrentValues();
		}
		return;
	}
	if (pLayer)
		sGame->SwapLayer(pLayer, getTag());
	return;
}

void Chose_Character_Layer::SpritesFadeOut(FadeType _FadeType)
{
	setTouchEnabled(false);
	_SwapLayer(_FadeType);
}

void Chose_Character_Layer::SpritesFadeIn()
{
	//float DelayTime = 0;
	//for (int i = 0; i != Buttons.size(); i++)
	//{
	//	if (Sprite* TempSprite = Buttons.at(i))
	//	{
	//		Sequence* TempSQ = Sequence::create(DelayTime::create(DelayTime), ScaleTo::create(0.5f, 1.0f), ScaleTo::create(0.2f, 0.5f), ScaleTo::create(0.2f, 1.0f), NULL);
	//		TempSprite->runAction(TempSQ);
	//	}
	//	DelayTime += 0.5f;
	//}
}

bool Chose_Character_Layer::LoadCharacters()
{
	Result _Result;
	if (sDataMgr->selectUnitDataList(_Result,"SELECT guid,name,Class,Money,Exp,Level,Mapid,Pos_X,Pos_Y,Faction FROM characters"))
	{
		if (_Result.empty()) return true;
		else
		{
			for (Result::iterator ito = _Result.begin(); ito != _Result.end(); ito++)
			{
				std::vector<RowInfo> row = ito->second;
				if (row.empty()) continue;
				CharacterEnumInfo _info;

				_info.guid												= row.at(0).GetInt();
				_info.name												= row.at(1).GetString();
				_info.Class												= (UnitClasses)row.at(2).GetInt();
				_info.Money												= row.at(3).GetInt();
				_info.Exp												= row.at(4).GetInt();
				_info.Level												= row.at(5).GetInt();
				_info.Mapid												= row.at(6).GetInt();
				_info.Pos_X												= row.at(7).GetFloat();
				_info.Pos_Y												= row.at(8).GetFloat();
				_info.Faction											= row.at(9).GetInt();
				CharacterEnumMap[(UnitClasses)row.at(0).GetInt()]		= _info;
				TotalCharacterCount++;
			}
            return true;
		}
	}
	else
		return false;
}

void Chose_Character_Layer::update(float diff)
{
	//p->setPositionX(getPositionX() + 1.0f);
	//Taiji->setRotation(Taiji->getRotation() - 1.5f);
}
