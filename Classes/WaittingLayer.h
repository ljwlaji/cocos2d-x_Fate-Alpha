#ifndef __WAITTING_LAYER_H__
#define __WAITTING_LAYER_H__
#include "Common.h"
#include "UILoader.h"

#define sWaittingLayer WaittingLayer::GetInstance()
class WaittingLayer : public Layer
{
public:
	static WaittingLayer* GetInstance();
	void ShowUp(const uint32& LayerTag);
private:
	WaittingLayer();
	~WaittingLayer();
	virtual bool init();
	CREATE_FUNC(WaittingLayer);
	virtual void update(float diff);
	void updatelaodprogress();
private:
	Sprite* LoadingIcon;
	Sprite* ShowTextImage;
	LabelTTF* TempLoadProggess;
	std::list<UILoadFunction> LoadList;
	uint32 TotalLoadCount;
	uint32 LoadedCount;
	uint32 LoadLayer;
};

#endif