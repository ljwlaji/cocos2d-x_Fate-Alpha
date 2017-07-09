#include "WaittingLayer.h"
#include "UILoader.h"
#include "MainUILayer.h"

static WaittingLayer* _WaittingLayer = nullptr;

WaittingLayer::WaittingLayer()
{
	setVisible(false);
}

WaittingLayer::~WaittingLayer()
{
	_WaittingLayer = nullptr;
}

WaittingLayer* WaittingLayer::GetInstance()
{
	if (!_WaittingLayer)
		_WaittingLayer = WaittingLayer::create();
	return _WaittingLayer;
}

bool WaittingLayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		LoadingIcon = Sprite::create(LogoImage);
		LoadingIcon->setPosition(getContentSize() / 2);
		addChild(LoadingIcon);

		ShowTextImage = Sprite::create(WaittingLayer_Show_Text);
		ShowTextImage->setPosition(LoadingIcon->getPositionX(), LoadingIcon->getPositionY() - LoadingIcon->getBoundingBox().size.height);
		addChild(ShowTextImage);

		TempLoadProggess = LabelTTF::create("0", "arial", 35);
		TempLoadProggess->setPosition(ShowTextImage->getPositionX(), ShowTextImage->getPositionY() - TempLoadProggess->getBoundingBox().size.height);
		addChild(TempLoadProggess);

		bRef = true;
	} while (0);

	return bRef;
}

void WaittingLayer::ShowUp(const uint32& LayerTag)
{
	TotalLoadCount = 0;
	LoadLayer = 0;
	LoadedCount = 0;
	switch (LayerTag)
	{
	case MainUILayerTag:
		LoadList.push_back(MainUI_Load_Title_Buttons);
		LoadList.push_back(MainUI_Load_Uper_Buttons);
		LoadList.push_back(MainUI_Load_Actions_Sprite);
		LoadList.push_back(MainUI_Load_News_Sprite);
		LoadList.push_back(MainUI_Load_Account_Sprite);
		LoadList.push_back(MainUI_Load_WorldChat_Sprite);
		LoadList.push_back(MainUI_Load_Auction_Sprite);
		LoadLayer = LayerTag;
		break;
	default:
		break;
	}

	TotalLoadCount = LoadList.size();
	setVisible(true);
	scheduleUpdate();
}

void WaittingLayer::update(float diff)
{
	if (LoadList.empty())
	{
		unscheduleUpdate();
		setVisible(false);
		return;
	}

	LoadHanler const& opHandle = LoadTable[*LoadList.begin()];
	switch (LoadLayer)
	{
	case MainUILayerTag:
		(sMainUI->*opHandle.MainUILayerHandler)();
		++LoadedCount;
		updatelaodprogress();
		break;
	}

	LoadList.pop_front();
}

void WaittingLayer::updatelaodprogress()
{
	uint32 progess = ((float)LoadedCount / (float)TotalLoadCount) * 100.0f;
	char msg[20];
	snprintf(msg, 20, "%d%%", progess);
	TempLoadProggess->setString(msg);
}