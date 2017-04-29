#include "CreateCharacterLayer.h"
#include "ChoseCharacterLayer.h"
#include "HelloWorldScene.h"
#include "DataMgr.h"

static Create_Character_Layer* _Create_Character_Layer = nullptr;

Create_Character_Layer::Create_Character_Layer()
{
	ActionSprite = nullptr;
	_Create_Character_Layer = this;
	setTag(Create_Character_Layer_Tag);
	ClassFrame.clear();
	VisableSize = Director::getInstance()->getVisibleSize();
	ChoseedClass = Class_None;
	CreateName = "";
}

Create_Character_Layer::~Create_Character_Layer()
{
	removeAllChildrenWithCleanup(true);
	_Create_Character_Layer = nullptr;
}

bool Create_Character_Layer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		InitFrame();

		bRef = true;
	} while (0);

	return bRef;
}

void Create_Character_Layer::InitFrame()
{
	Sprite* BackGround = Sprite::create("White_Back_Ground.png");
	BackGround->SetRealPosition(VisableSize.x / 2, VisableSize.y / 2);
	BackGround->setZOrder(-2);
	addChild(BackGround);

	Title = Sprite::create("Create_Character_Title.png");
	Title->SetRealPosition(Title->getBoundingBox().size.width * 0.4f, VisableSize.y - Title->getBoundingBox().size.height * 0.3f/* - Title->getBoundingBox().size.height / 2*/);
	addChild(Title);


	Taiji = Sprite::create("Create_Taiji.png");
	Taiji->SetRealPosition(VisableSize.x * 0.45f, VisableSize.y * 0.28f);
	Taiji->setZOrder(4);
	Taiji->setScaleY(0.2f);
	Taiji->setOpacity(0.01f);
	Taiji->runAction(FadeIn::create(1.0f));
	addChild(Taiji);

	CreateOrEnterGameButton = Sprite::create("Button_Create_Character.png");
	CreateOrEnterGameButton->SetRealPosition(VisableSize.x / 2 - CreateOrEnterGameButton->getBoundingBox().size.width / 2, 0 - CreateOrEnterGameButton->getBoundingBox().size.height / 2);
	CreateOrEnterGameButton->setOpacity(140.0f);
	CreateOrEnterGameButton->runAction(MoveTo::create(1.0f, Vec2(CreateOrEnterGameButton->getPositionX(), VisableSize.y * 0.15f)));
	addChild(CreateOrEnterGameButton);

	CanCelButton = Sprite::create("test1.png");
	CanCelButton->SetRealPosition(VisableSize.x / 2 + CanCelButton->getBoundingBox().size.width / 2, 0 - CreateOrEnterGameButton->getBoundingBox().size.height / 2);
	CanCelButton->setOpacity(140.0f);
	CanCelButton->runAction(MoveTo::create(1.0f, Vec2(CanCelButton->getPositionX(), VisableSize.y * 0.15f)));
	addChild(CanCelButton);

	for (int i = Saber; i != Class_End; i++)
	{
		char msg[255];
		snprintf(msg, 255, "Create_Class_%d.jpg", i);
		Sprite* Temp = Sprite::create(msg);
		Temp->SetRealPosition(VisableSize.x / 2 - Temp->getBoundingBox().size.width * 1.5f + (i * Temp->getBoundingBox().size.width * 0.5f), VisableSize.y + Temp->getBoundingBox().size.height * 0.25f); //- Temp->getBoundingBox().size.height);
		addChild(Temp);
		ClassFrame.push_back(Temp);
		Temp->setTag(i);
		Temp->setZOrder(i - 1);
	}

	Sprite* ClickCreate = Sprite::create("Create_Chose_Class.png");
	ClickCreate->setPosition(VisableSize.x - ClickCreate->getBoundingBox().size.width, VisableSize.y - ClickCreate->getBoundingBox().size.height * 0.5f);
	addChild(ClickCreate);

	Sprite* CreateName = Sprite::create("Create_Character_Name.png");
	CreateName->SetRealPosition(VisableSize.x * 0.1f, VisableSize.y / 2);
	addChild(CreateName);

	CreateNameFrame = Sprite::create("Create_Character_Name_1.png");
	CreateNameFrame->setPosition(CreateName->getBoundingBox().size.width * 0.8f + CreateName->getBoundingBox().origin.x + CreateNameFrame->getBoundingBox().size.width / 2, CreateName->getBoundingBox().origin.y * 1.15f);
	addChild(CreateNameFrame);

	NameTyping = TextFieldTTF::textFieldWithPlaceHolder("Text Here", "Arial", 34);
	NameTyping->setColor(ccc3(0,0,0));
	NameTyping->setColorSpaceHolder(ccc3(0,0,0));
	NameTyping->setTag(12);
	NameTyping->setPosition(CreateNameFrame->getPositionX(), CreateNameFrame->getPositionY() + NameTyping->getBoundingBox().size.height * 0.5f);//().width / 2, CreateNameFrame->getContentSize().height / 2);
	addChild(NameTyping);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Create_Character_Layer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Create_Character_Layer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();
}

bool Create_Character_Layer::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (ActionSprite)
		return false;

	if (CreateNameFrame->getBoundingBox().containsPoint(touch->getLocation()))
		return true;
	if (CanCelButton->getBoundingBox().containsPoint(touch->getLocation()))
	{
		TempTouchedSprite = CanCelButton;
		return true;
	}
	else if (CreateOrEnterGameButton->getBoundingBox().containsPoint(touch->getLocation()))
	{
		TempTouchedSprite = CreateOrEnterGameButton;
		return true;
	}
	for (int i = ClassFrame.size() - 1; i != -1; i--)
	{
		if (Sprite* TempSprite = ClassFrame.at(i))
		{
			if (TempSprite->getBoundingBox().containsPoint(touch->getLocation()))
			{
				TempSprite->setZOrder(10);
				TempTouchedSprite = TempSprite;
				return true;
			}
		}
	}
	return false;
}

void Create_Character_Layer::Create_Character()
{
	uint32 guid = 0;
	Result _result;
	if (sDataMgr->selectUnitDataList("SELECT MAX(guid) FROM characters", _result))
	{
		if (!_result.empty())
		{
			Result::iterator itr = _result.begin();
			{
				guid = itr->second.at(0).GetInt() + 1;
			}
		}
		char msg[255];
		snprintf(msg, 255, "INSERT INTO characters (guid,name,Class,Mapid) VALUES(%u,'%s',%u,1)", guid, NameTyping->getString().c_str(), ChoseedClass);
		sDataMgr->PExcute(msg);
	}


}

void Create_Character_Layer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (CreateNameFrame->getBoundingBox().containsPoint(touch->getLocation()))
	{
		NameTyping->attachWithIME();
		return;
	}
	if (TempTouchedSprite)
	{
		if (TempTouchedSprite == CanCelButton)
		{
			sGame->SwapLayer(Chose_Character_Layer_Tag, getTag());
			return;
		}
		else if (TempTouchedSprite == CreateOrEnterGameButton)
		{
			//Do DB Work And Return;
			if (ChoseedClass)
			{
				if (NameTyping->getString().empty())
					return;
				Create_Character();
				sGame->SwapLayer(Chose_Character_Layer_Tag, getTag());
				return;
			}
		}
		else
		{
			for (int i = 0; i < ClassFrame.size(); i++)
			{
				if (TempTouchedSprite->getTag() == ChoseedClass || ActionSprite)
					break;
				if (TempTouchedSprite == ClassFrame.at(i))
				{
					TempTouchedSprite->setZOrder(TempTouchedSprite->getTag() - 1);
					if (ChoseedClass)
					{
						Sprite* Return = ClassFrame.at(ChoseedClass - 1);
						if (Return)
						{
							Return->setScaleX(1.0f);
							Return->setPosition(ReturnPos);
							Return->setZOrder(Return->getTag() - 1);
						}
					}
					if (TempTouchedSprite->getBoundingBox().containsPoint(touch->getLocation()))
					{
						Sequence* sequence = Sequence::create(MoveTo::create(0.8f, Vec2(Taiji->getPositionX(), Taiji->getPositionY() + TempTouchedSprite->getBoundingBox().size.height / 2)), CallFunc::create(CC_CALLBACK_0(Create_Character_Layer::VirtualAction, this, TempTouchedSprite)), NULL);
						ActionSprite = TempTouchedSprite;
						ChoseedClass = (UnitClasses)TempTouchedSprite->getTag();
						TempTouchedSprite->runAction(sequence);
						ReturnPos = TempTouchedSprite->getPosition();
					}
					else
					{
						ChoseedClass = Class_None;
					}
					break;
				}
			}
		}
	}

	TempTouchedSprite = nullptr;
}

void Create_Character_Layer::VirtualAction(Sprite* pSprite)
{
	ActionSprite->setScaleX(-1.0f);
	ActionSprite = nullptr;
}

Create_Character_Layer* Create_Character_Layer::GetInstance()
{
	if (!_Create_Character_Layer)
	{
		_Create_Character_Layer = Create_Character_Layer::create();
	}
	return _Create_Character_Layer;
}

void Create_Character_Layer::EveryThingFadeOut()
{

}

void Create_Character_Layer::update(float diff)
{
	if (ActionSprite && ActionSprite->getScaleX() > -1.0f)
		ActionSprite->setScaleX(ActionSprite->getScaleX() - 0.05f);
}