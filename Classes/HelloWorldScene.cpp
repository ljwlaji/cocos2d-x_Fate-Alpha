#include "HelloWorldScene.h"

static  MainScene* _MainScene = nullptr;
USING_NS_CC;

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
		auto listener = EventListenerKeyboard::create();
		listener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
		listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		

		EnterLayer = EnterGameLayer::create();
		addChild(EnterLayer);
		
		LoadingLayer = LoadingUILayer::create();
		LoadingLayer->DisAppear();
		addChild(LoadingLayer);

		bRef = true;
	} while (0);
    
    
    return bRef;
}

void MainScene::LoadEverything()
{

}

void MainScene::SwapLayer(Layer* _instead, int removetag)
{
	if (Layer* pLayer = (Layer*)getChildByTag(removetag))
	{
		pLayer->removeFromParentAndCleanup(true);
		addChild(_instead);
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

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	//if (!player) return;
	//if (sAction->CanDoAction(player))
	switch (keyCode)
	{
		//移动//
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		break;
		//移动//

		//按键//
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_L:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_I:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_U:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_O:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_P:
		break;
		//按键//

		//快捷键//
	case cocos2d::EventKeyboard::KeyCode::KEY_E:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_Q:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_F:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_R:
		break;
	}
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		break;
	}
}