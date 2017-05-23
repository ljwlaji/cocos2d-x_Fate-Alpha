#ifndef __MINI_MAP_H__
#define __MINI_MAP_H__

#include "UISprite.h"


#define sMiniMap	MiniMap::GetInstance()

class MiniMap : public Sprite
{
public:
	static MiniMap* GetInstance();
	void InitWithMapID(uint32 Mapid);
private:
	MiniMap();
	~MiniMap();

	ClippingNode* clippingNode;
	Sprite* MiniMapSprite;
	Sprite* ClipFrame;
	float TotalX;
};



#endif