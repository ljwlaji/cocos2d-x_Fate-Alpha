#ifndef __LOGIN_TYPE_SPRITE_H__
#define __LOGIN_TYPE_SPRITE_H__

#include "UISprite.h"

#define sLoginTypeSprite LoginTypeSprite::GetInstance()

class LoginTypeSprite : public UISprite
{
public:
	static LoginTypeSprite* GetInstance();
	void ShowUp();
	void OnFadeOut();
private:
	LoginTypeSprite();
	~LoginTypeSprite();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchMoved(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	void _fadeout(Sprite* pSprte);
private:
	void HandlePlayerLogin();
	Sprite* ButtonLogin;
	TextFieldTTF* AccountTyping;
	TextFieldTTF* PassWordTyping;
	Sprite* AccountFrame;
	Sprite* PassWordFrame;


	Sprite* Title;
	Sprite* RePassWord;
	Sprite* SignUp;
};
#endif