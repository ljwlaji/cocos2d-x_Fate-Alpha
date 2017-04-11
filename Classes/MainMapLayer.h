#ifndef __MAIN_MAP_LAYER_H__
#define __MAIN_MAP_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;
class Player;
class Main_Map_Layer : public Layer
{
public:
	Main_Map_Layer();
	~Main_Map_Layer();

private:
	Player* _Player;
};

#endif