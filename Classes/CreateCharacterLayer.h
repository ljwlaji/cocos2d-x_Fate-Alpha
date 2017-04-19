#ifndef __CREATE_CHARACTER_LAYER_H__
#define __CREATE_CHARACTER_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

#define sCreate_Character Create_Character_Layer::GetInstance();
class Create_Character_Layer : public Layer
{
public:
	Create_Character_Layer();
	~Create_Character_Layer();
	static Create_Character_Layer* GetInstance();
	virtual bool init();
	CREATE_FUNC(Create_Character_Layer);
private:
	void InitFrame();


	Sprite* Title;
	Sprite* Taiji;
	Sprite* CreateOrEnterGameButton;
	Sprite* CanCelButton;
	Vec2 VisableSize;
};


#endif