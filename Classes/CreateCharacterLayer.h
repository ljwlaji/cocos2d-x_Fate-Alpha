#ifndef __CREATE_CHARACTER_LAYER_H__
#define __CREATE_CHARACTER_LAYER_H__

#include "cocos2d.h"
#include "Types.h"
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
	void Create_Character();
	void InitFrame();
	void EveryThingFadeOut();
	void VirtualAction(Sprite* pSprite);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void update(float diff);
	Sprite* Title;
	Sprite* Taiji;
	Sprite* CreateOrEnterGameButton;
	Sprite* CanCelButton;
	Vec2 VisableSize;

	TextFieldTTF* NameTyping;
	Sprite* CreateNameFrame;
	UnitClasses ChoseedClass;
	Sprite* TempTouchedSprite;
	Sprite* ActionSprite;
	std::vector<Sprite*> ClassFrame;
	Vec2 ReturnPos;
	std::string CreateName;
};


#endif