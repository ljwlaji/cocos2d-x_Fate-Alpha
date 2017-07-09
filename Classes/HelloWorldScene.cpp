#include "HelloWorldScene.h"
#include "SocketClient.h"
#include "MainUILayer.h"
#include "EnterLayer.h"
#include "NotifyMgr.h"
#include "WaittingLayer.h"
#ifdef __APPLE__
#include "spine/spine.h"
#include "cocos/editor-support/spine/SkeletonAnimation.h"
#else
#include "spine\spine.h"
#include "cocos\editor-support\spine\SkeletonAnimation.h"
#endif
#include <iostream>
#include <fstream>

#pragma once
#pragma execution_character_set("utf-8")

using namespace spine;
static  MainScene* _MainScene = nullptr;

MainScene::MainScene()
{
	//网页跳转
	//Application::getInstance()->openURL("https://www.baidu.com/");
}
MainScene::~MainScene()
{
	removeAllChildrenWithCleanup(true);
}

MainScene* MainScene::GetInstance()
{
	if (!_MainScene)
	{
		_MainScene = MainScene::create();
	}
	return _MainScene;
}

bool MainScene::init()
{
	bool bRef = false;

	do
	{
		CC_BREAK_IF(!Scene::init());

		addChild(sEnterLayer);
		sEnterLayer->StartConnect();

		sNotifyMgr->setPosition(getContentSize() / 2);
		addChild(sNotifyMgr);
		sNotifyMgr->setVisible(false);

		addChild(sWaittingLayer);
		bRef = true;
	} while (0);
    
    
    return bRef;
}

void MainScene::SwapLayer(int OldLayer, Layer* Instead)
{
	if (Layer* _OldLayer = (Layer*)getChildByTag(OldLayer))
		_OldLayer->removeFromParentAndCleanup(true);
	else
		return;
	addChild(Instead);
	//SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	//Director::getInstance()->getTextureCache()->removeUnusedTextures();
}