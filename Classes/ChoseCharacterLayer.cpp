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
	DisplaySprites.clear();
	m_TouchedSprite = nullptr;
	m_TouchType = None;
}

Chose_Character_Layer::~Chose_Character_Layer()
{
	removeAllChildrenWithCleanup(true);
	_Chose_Character_Layer = nullptr;
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

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(Chose_Character_Layer::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(Chose_Character_Layer::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		scheduleUpdate();
		bRef = true;
	} while (0);

	return bRef;
}

bool Chose_Character_Layer::onTouchBegan(Touch *touch, Event *unused_event)
{
	for (int i = 0; i != DisplaySprites.size(); i++)
	{
		if (Sprite* Temp = DisplaySprites.at(i))
		if (Temp->getBoundingBox().containsPoint(touch->getLocation()))
		{
			Temp->setScale(0.8f);
			m_TouchedSprite = Temp;
			m_TouchType = Button;
			return true;
		}
	}
	for (int i = 0; i < CharacterEnumFrame.size(); i++)
	{
		if (Sprite* Temp = CharacterEnumFrame.at(i))
		if (Temp->getBoundingBox().containsPoint(touch->getLocation()))
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

	if (!m_TouchedSprite->getBoundingBox().containsPoint(touch->getLocation()))
		return;
	switch (m_TouchType)
	{
	case Button:
		if (m_TouchedSprite == CreateOrEnterGameButton)
		{
			if (m_ChosedInfo.guid)
			{
				SpritesFadeOut(EnterGame);
				//if (Main_Map_Layer* layer = new Main_Map_Layer(m_ChosedInfo.Mapid))
				//{
				//	layer->init();
				//	layer->autorelease();
				//	sGame->SwapLayer(layer, getTag());
				//}
				return;
			}
			else
			{
				SpritesFadeOut(GoToCreate);
				//if (Create_Character_Layer* layer = Create_Character_Layer::create())
				//	sGame->SwapLayer(layer, getTag());
				return;
			}
		}
		else
		{
			SpritesFadeOut(BackToMenu);
			//EnterGameLayer* _layer = EnterGameLayer::create();
			//sGame->SwapLayer(_layer, getTag());
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
			CreateOrEnterGameButton->setTexture("Button_Create_Character.png");
			CreateOrEnterGameButton->setOpacity(140.0f);
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
		CreateOrEnterGameButton->setTexture("tet.png");
		CreateOrEnterGameButton->setOpacity(255.0f);
	}
}

void Chose_Character_Layer::SwapChosedCharacter(uint32 CharacterGuid)
{
	if (!CharacterEnumSprite)
	{
		CharacterEnumSprite = Sprite::create("test.png");
		CharacterEnumSprite->setZOrder(5);
		addChild(CharacterEnumSprite);
		CharacterEnumSprite->setAnchorPoint(Vec2(0.5, 0));
	}
	CharacterEnumSprite->setTexture("test.png");
	CharacterEnumSprite->setPosition(Taiji->getPosition());
	CharacterEnumSprite->setVisible(true);

}

void Chose_Character_Layer::InitFrames()
{
	Sprite* BackGround = Sprite::create("White_Back_Ground.png");
	BackGround->SetRealPosition(VisableSize.x / 2, VisableSize.y / 2);
	BackGround->setZOrder(-2);
	addChild(BackGround);

	Title = Sprite::create("Chose_Character_Title.png");
	Title->SetRealPosition(Title->getBoundingBox().size.width * 0.4f, VisableSize.y - Title->getBoundingBox().size.height * 0.3f/* - Title->getBoundingBox().size.height / 2*/);
	Title->setScale(0.0f);
	addChild(Title);
	DisplaySprites.push_back(Title);

	std::string url = "";
	m_ChosedInfo.guid ? url = "tet.png" : url = "Button_Create_Character.png";
	CreateOrEnterGameButton = Sprite::create(url.c_str());
	CreateOrEnterGameButton->SetRealPosition(VisableSize.x / 2 - CreateOrEnterGameButton->getBoundingBox().size.width / 2, VisableSize.y * 0.15f);
	CreateOrEnterGameButton->setScale(0.0f);
	CreateOrEnterGameButton->setOpacity(140.0f);
	addChild(CreateOrEnterGameButton);
	DisplaySprites.push_back(CreateOrEnterGameButton);

	CanCelButton = Sprite::create("test1.png");
	CanCelButton->SetRealPosition(VisableSize.x / 2 + CanCelButton->getBoundingBox().size.width / 2, VisableSize.y * 0.15f);
	CanCelButton->setScale(0.0f);
	CanCelButton->setOpacity(140.0f);
	addChild(CanCelButton);
	DisplaySprites.push_back(CanCelButton);

	for (int i = 0; i != 5; i++)
	{
		int Tag = 0;
		Sprite* TempFrame = Sprite::create("Chose_Character_Character_Frame.png");
		TempFrame->SetRealPosition(VisableSize.x - TempFrame->getBoundingBox().size.width / 2, VisableSize.y / 2 + (TempFrame->getBoundingBox().size.height * 2.5) - (i * TempFrame->getBoundingBox().size.height));
		addChild(TempFrame);
		CharacterEnumFrame.push_back(TempFrame);
		//Sprite* LevelFrame = Sprite::create("Chose_Character_Character_Level_Frame.png");
		//LevelFrame->setPosition(TempFrame->getBoundingBox().origin.x + TempFrame->getBoundingBox().size.width - LevelFrame->getBoundingBox().size.width / 2, TempFrame->getPositionY() - LevelFrame->getBoundingBox().size.height * 0.1f);
		//addChild(LevelFrame);

		Sprite* CharacterNameText;

		bool Match = false;
		int k = 0;
		for (std::map<uint32, CharacterEnumInfo>::iterator itr = CharacterEnumMap.begin(); itr != CharacterEnumMap.end(); itr++)
		{
			if (k != i)
			{
				k++;
				continue;
			}
			if (itr->second.Class)
			{
				char msg[255];
				snprintf(msg, 255, "Chose_Character_Character_%d.png", itr->second.Class);
				CharacterNameText = Sprite::create(msg);
				CharacterNameText->setPosition(TempFrame->getContentSize().width * 0.2f, TempFrame->getContentSize().height * 0.45f);
				TempFrame->addChild(CharacterNameText);
				k++;
				Match = true;
				Tag = itr->second.guid;
				break;
			}
		}
		TempFrame->setTag(Tag);
		if (!Match)
		{
			CharacterNameText = Sprite::create("Chose_Character_Create_Text.png");
			CharacterNameText->setPosition(TempFrame->getContentSize().width * 0.25f, TempFrame->getContentSize().height * 0.45f);
			TempFrame->addChild(CharacterNameText);
		}
	}
	Taiji = Sprite::create("taiji.png");
	Taiji->SetRealPosition(VisableSize.x * 0.45f, VisableSize.y * 0.28f);
	Taiji->setZOrder(4);
	Taiji->setScaleY(0.2f);
	addChild(Taiji);

	

	SpritesFadeIn();
}

void Chose_Character_Layer::_SwapLayer(FadeType _FadeType)
{
	Layer* pLayer;
	switch (_FadeType)
	{
	case BackToMenu:
		pLayer = EnterGameLayer::create();
		break;
	case GoToCreate:
		pLayer = Create_Character_Layer::create();
		break;
	case EnterGame:
		if (pLayer = new Main_Map_Layer(m_ChosedInfo.Mapid))
		{
			pLayer->init();
			pLayer->autorelease();
		}
		break;
	default:
		break;
	}
	if (pLayer)
		sGame->SwapLayer(pLayer, getTag());
	return;
}

void Chose_Character_Layer::SpritesFadeOut(FadeType _FadeType)
{
	setTouchEnabled(false);
	Taiji->runAction(ScaleTo::create(1.0f, 0.0f));
	for (int i = 0; i != CharacterEnumFrame.size(); i++)
	{
		if (Sprite* Temp = CharacterEnumFrame.at(i))
			Temp->runAction(MoveTo::create(1.0f, Vec2(Temp->getPositionX() + Temp->getBoundingBox().size.width, Temp->getPositionY())));
	}
	for (int i = 0; i != DisplaySprites.size(); i++)
	{
		if (Sprite* TempSprite = DisplaySprites.at(i))
		{
			Vec2 Pos;
			if (Title == TempSprite)
				Pos = Vec2(0 - TempSprite->getPositionX(), TempSprite->getPositionY());
			else
				Pos = Vec2(TempSprite->getPositionX(), 0 - TempSprite->getPositionY());

			Sequence* TempSQ = Sequence::create(CCMoveTo::create(1.0f, Pos), DelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(Chose_Character_Layer::_SwapLayer, this, _FadeType)), NULL);
			TempSprite->runAction(TempSQ);
		}
	}
}

void Chose_Character_Layer::SpritesFadeIn()
{
	float DelayTime = 0;
	for (int i = 0; i != DisplaySprites.size(); i++)
	{
		if (Sprite* TempSprite = DisplaySprites.at(i))
		{
			Sequence* TempSQ = Sequence::create(DelayTime::create(DelayTime), ScaleTo::create(0.5f, 1.0f), ScaleTo::create(0.2f, 0.5f), ScaleTo::create(0.2f, 1.0f), NULL);
			TempSprite->runAction(TempSQ);
		}
		DelayTime += 0.5f;
	}
}

bool Chose_Character_Layer::LoadCharacters()
{
	char msg[255];//			0	 1		2	 3	  4		5	 6		7	8
	snprintf(msg, 255, "SELECT guid,name,Class,Money,Exp,Level,Mapid,Pos_X,Pos_Y FROM characters");
	Result _Result;
	if (sDataMgr->selectUnitDataList(msg, _Result))
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
