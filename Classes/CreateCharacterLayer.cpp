#include "CreateCharacterLayer.h"
#include "ChoseCharacterLayer.h"
#include "HelloWorldScene.h"

static Create_Character_Layer* _Create_Character_Layer = nullptr;

Create_Character_Layer::Create_Character_Layer()
{
	setTag(Create_Character_Layer_Tag);
	_Create_Character_Layer = this;
	VisableSize = Director::getInstance()->getVisibleSize();
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
	addChild(Taiji);

	CreateOrEnterGameButton = Sprite::create("Button_Create_Character.png");
	CreateOrEnterGameButton->SetRealPosition(VisableSize.x / 2 - CreateOrEnterGameButton->getBoundingBox().size.width / 2, VisableSize.y * 0.15f);
	CreateOrEnterGameButton->setOpacity(140.0f);
	addChild(CreateOrEnterGameButton);

	CanCelButton = Sprite::create("test1.png");
	CanCelButton->SetRealPosition(VisableSize.x / 2 + CanCelButton->getBoundingBox().size.width / 2, VisableSize.y * 0.15f);
	CanCelButton->setOpacity(140.0f);
	addChild(CanCelButton);
}

Create_Character_Layer* Create_Character_Layer::GetInstance()
{
	if (!_Create_Character_Layer)
	{
		_Create_Character_Layer = Create_Character_Layer::create();
	}
	return _Create_Character_Layer;
}