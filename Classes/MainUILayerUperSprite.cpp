#include "MainUILayerUperSprite.h"
#include "Common.h"

static MainUperSprite* _MainUperSprite = nullptr;

MainUperSprite::MainUperSprite()
{
	initWithFile(MainUILayer_Uper_Frame);
	autorelease();
	SetTouchType(Main_UISprite_Uper_Sprite);
}

MainUperSprite::~MainUperSprite()
{
	_MainUperSprite = nullptr;
}
MainUperSprite* MainUperSprite::GetInstance()
{
	if (!_MainUperSprite)
		_MainUperSprite = new MainUperSprite();
	return _MainUperSprite;
}

void MainUperSprite::InitSprite()
{
	for (int i = Game; i != End; i++)
	{
		Sprite* TempButtonFrame = Sprite::create(MainUILayer_Uper_Big_Button_Frame);
		TempButtonFrame->setPosition(TempButtonFrame->getBoundingBox().size.width / 2 + (i * TempButtonFrame->getBoundingBox().size.width), getContentSize().height / 2);
		addChild(TempButtonFrame);
		setTag(i);

		char msg[255];
		snprintf(msg, 255, MainUILayer_Uper_Big_Button_Text, i);
		Sprite* Text = Sprite::create(msg);
		Text->setPosition(TempButtonFrame->getContentSize() / 2);
		TempButtonFrame->addChild(Text);
	}
}