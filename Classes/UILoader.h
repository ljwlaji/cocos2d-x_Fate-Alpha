#ifndef __UI_LOADER_H__
#define __UI_LOADER_H__

#include "UISprite.h"
#include "MainUILayer.h"
#include "EnterLayer.h"

enum UILoadFunction
{
	MainUI_Load_Title_Buttons,
	MainUI_Load_Uper_Buttons,
	MainUI_Load_News_Sprite,
	MainUI_Load_Actions_Sprite,
	MainUI_Load_Account_Sprite,
	MainUI_Load_WorldChat_Sprite,
	MainUI_Load_Auction_Sprite,
};

struct LoadHanler
{
	char const* name;
	void (MainUILayer::*MainUILayerHandler)();
	void (EnterLayer::*EnterLayerHandler)();
};

extern LoadHanler LoadTable[1024];

#endif