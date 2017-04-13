#include "HelloWorldScene.h"
#include "ActionManager.h"
static  MainScene* _MainScene = nullptr;
USING_NS_CC;

MainScene::MainScene()
{
	m_Diffcute = Rate_C;
	CombatSign = false;
	CanPlaySound = true;
	_MainScene = this;
	_player = nullptr;
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
			auto listener = EventListenerKeyboard::create();
			listener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
			listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		#else
		#endif
		

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
			_player->SetMoveKeyEnable(MoveKey_Left, true);
			break;
		case EventKeyboard::KeyCode::KEY_S:
			_player->SetMoveKeyEnable(MoveKey_Down, true);
			break;
		case EventKeyboard::KeyCode::KEY_D:
			_player->SetMoveKeyEnable(MoveKey_Right, true);
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_player->SetMoveKeyEnable(MoveKey_Up, true);
			break;
		}
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_R)
	{
		if (!sPlayer)
		{
			if (SkeletonAnimation* _SkeletonAnimation = SkeletonAnimation::createWithFile("spineboy.json", "spineboy.atlas", 0.5f))
			{
				_player = new Player(_SkeletonAnimation);
				EnterLayer->AddPlayer(_player);
			}
		}
	}
	else
	{
		if (sPlayer)
		{
			sPlayer->PlayerActionMgr()->OnPlayerPressKey(keyCode);
		}
	}
	//switch (keyCode)
	//{
	//	//移动//
	//case cocos2d::EventKeyboard::KeyCode::KEY_A:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_S:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_D:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_W:
	//	break;
	//	//移动//
	//
	//	//按键//
	//case cocos2d::EventKeyboard::KeyCode::KEY_J:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_L:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_I:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_U:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_O:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_P:
	//	break;
	//	//按键//
	//
	//	//快捷键//
	//case cocos2d::EventKeyboard::KeyCode::KEY_E:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_Q:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_F:
	//	break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_R:
	//	if (!_player)
	//	{
	//		if (SkeletonAnimation* _SkeletonAnimation = SkeletonAnimation::createWithFile("spineboy.json", "spineboy.atlas", 0.5f))
	//		{
	//			_player = new Player(_SkeletonAnimation);
	//			EnterLayer->AddPlayer(_player);
	//		}
	//	}
	//	break;
	//}
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (IsMoveKey(keyCode))
	{
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_A:
			_player->SetMoveKeyEnable(MoveKey_Left, false);
			break;
		case EventKeyboard::KeyCode::KEY_S:
			_player->SetMoveKeyEnable(MoveKey_Down, false);
			break;
		case EventKeyboard::KeyCode::KEY_D:
			_player->SetMoveKeyEnable(MoveKey_Right, false);
			break;
		case EventKeyboard::KeyCode::KEY_W:
			_player->SetMoveKeyEnable(MoveKey_Up, false);
			break;
		}
	}
}