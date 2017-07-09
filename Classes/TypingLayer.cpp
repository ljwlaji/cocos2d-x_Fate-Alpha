#include "TypingLayer.h"
#include "Types.h"

TypingLayer::TypingLayer()
{
	setTag(Typing_Layer_Tag);
}

TypingLayer::~TypingLayer()
{
	removeAllChildrenWithCleanup(true);
}

bool TypingLayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		Vec2 visablesize = Director::getInstance()->getVisibleSize();
		Sprite* AccountFrame = Sprite::create("Typing_Frame.png");
		AccountFrame->SetRealPosition(visablesize.x / 2, visablesize.y * 0.70f);
		AccountFrame->setTag(0);
		addChild(AccountFrame);
		AccountTyping = TextFieldTTF::textFieldWithPlaceHolder("Please input your account:", "Arial", 30);
		AccountTyping->setTag(1);
		AccountTyping->setPosition(AccountFrame->getContentSize().width / 2, AccountFrame->getContentSize().height / 2);
		AccountFrame->addChild(AccountTyping);

		Sprite* PassWordFrame = Sprite::create("Typing_Frame.png");
		PassWordFrame->SetRealPosition(visablesize.x / 2, visablesize.y * 0.60f);
		PassWordFrame->setTag(1);
		PassWordTyping = TextFieldTTF::textFieldWithPlaceHolder("Please input your password:", "Arial", 30);
		PassWordTyping->setTag(1);
		PassWordTyping->setPosition(PassWordFrame->getContentSize().width / 2, PassWordFrame->getContentSize().height / 2);
		PassWordFrame->addChild(PassWordTyping);
		addChild(PassWordFrame);


		TypingButtons.push_back(AccountFrame);
		TypingButtons.push_back(PassWordFrame);
		setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
		setTouchEnabled(true);

		bRef = true;
	} while (0);
	return bRef;
}


bool TypingLayer::onTouchBegan(Touch* touch, Event* ev)
{
	return true;
}


void TypingLayer::onTouchEnded(Touch* touch, Event* ev)
{
	for (int i = 0; i != TypingButtons.size(); i++)
	{
		Sprite* Temp = TypingButtons.at(i);
		if (Temp->getBoundingBox().containsPoint(touch->getLocation()))
		{
			if (TextFieldTTF* ttf = (TextFieldTTF*)Temp->getChildByTag(1))
				ttf->attachWithIME();
		}
	}
}