#ifndef __MAIN_UI_LAYER_NEW_H__
#define __MAIN_UI_LAYER_NEW_H__

#include "UISprite.h"
#include "Common.h"
#define sNews	MainNewsSprite::GetInstance()
class MainNewsSprite : public UISprite
{
public:
	static MainNewsSprite* GetInstance();
	void InitSprite();
	void NewsInput(const uint32& time, const std::string& title, std::string& message);
private:
	MainNewsSprite();
	~MainNewsSprite();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	void SendNewsRequire();
	Sprite* Button;
	uint16 CurrentNew;

	LabelTTF* NewsTitle;
	LabelTTF* NewsDetail;
	LabelTTF* TimeTitle;
};

#endif