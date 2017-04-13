#ifndef __MAIN_MAP_LAYER_H__
#define __MAIN_MAP_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

#define sMainMap Main_Map_Layer::GetInstance()
class Player;
class Main_Map_Layer : public Layer
{
public:
	Main_Map_Layer();
	~Main_Map_Layer();

	static Main_Map_Layer* GetInstance();
	Sprite* GetCurrentGroundSprite(int id)		{ if (GroundSpriteVector.size() + 1 <= id) return GroundSpriteVector.at(id); return nullptr; }
	std::vector<Sprite*> GetGroundSprites()		{ return GroundSpriteVector; }
private:
	Player* _Player;

	std::vector<Sprite*> GroundSpriteVector;
};

#endif