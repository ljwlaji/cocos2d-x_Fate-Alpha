#ifndef __MAIN_UI_LAYER_UPER_SPRITE_H__
#define __MAIN_UI_LAYER_UPER_SPRITE_H__

#include "UISprite.h"

#define sMainUperSprite MainUperSprite::GetInstance()

class MainUperSprite : public UISprite
{
public:
	static MainUperSprite* GetInstance();
	void InitSprite();
private:
	enum ButtonType
	{
		Game,
		Shop,
		ExChange,
		End,
	};
	MainUperSprite();
	~MainUperSprite();
	virtual bool OnUITouchBegin(Touch* pTouch){ return true; }
	virtual void OnUITouchEnded(Touch* pTouch){};
};

#endif