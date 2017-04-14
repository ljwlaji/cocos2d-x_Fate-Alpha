#include "HelloWorldScene.h"
#include "ActionManager.h"
#include "MainMapLayer.h"
#include "DataMgr.h"
#include "NotifyMgr.h"
#include <iostream>
#include <fstream>

static  MainScene* _MainScene = nullptr;

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
		KeyBoardListener = EventListenerKeyboard::create();
		KeyBoardListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
		KeyBoardListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyBoardListener, this);
#else
#endif
		EnterLayer = EnterGameLayer::create();
		addChild(EnterLayer);

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

void MainScene::CheckDB()
{
	//std::string dbFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename("Datas.db");
	//std::string writablePath = CCFileUtils::getInstance()->getWritablePath() + "Datas.db";
	//FILE* fp = fopen(writablePath.c_str(),"w+");
	//if (fp)
	//{
	//	ssize_t dbSize;
	//	CCFileUtils::sharedFileUtils()->getFileData(writablePath.c_str(), "r", &dbSize);
	//	if (!dbSize){
	//		std::fstream fsCopee(dbFilePath.c_str(), std::ios::binary | std::ios::in);
	//		std::fstream fsCoper(writablePath.c_str(), std::ios::binary | std::ios::out);
	//		fsCoper << fsCopee.rdbuf();
	//	}
	//}
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
			SkeletonAnimation* _SkeletonAnimation = SkeletonAnimation::createWithJsonFile("spineboy.json", "spineboy.atlas", 0.5f);
			if (_SkeletonAnimation)
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