#ifndef __MINI_MAP_H__
#define __MINI_MAP_H__

#include "UISprite.h"


#define sMiniMap	MiniMap::GetInstance()
class Creature;
class MiniMap : public Sprite
{
public:
	static MiniMap* GetInstance();
	void SwapOpType();
	void InitWithMapID(uint32 Mapid);
	void UpdateSingleUnitSign(Unit* pUnit);
	void RemoveCreatureFromMiniMap(Creature* pCreature);
private:
	MiniMap();
	~MiniMap();
	enum MapOpTypeEnum
	{
		//100
		One,
		//66
		Two,
		//33
		Three,
		//0
		Four,
		//
		End,
	};
	std::map<Creature*, Sprite*> SingleCreatureSignMap;
	int MapOpType;
	bool IsLoadFinished;
	float TotalX;
	float CurrentX;
	float TotalY;
	ClippingNode* clippingNode;
	Sprite* MiniMapSprite;
	Sprite* ClipFrame;
	Sprite* PlayerMiniMapSingle;
};



#endif