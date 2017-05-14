#ifndef __PLAYER_UI_SETTING_SPRITE_H__
#define __PLAYER_UI_SETTING_SPRITE_H__
#include "Types.h"
#include "UISprite.h"

#define sSettingMenu  SettingMenu::GetInstance()

class SettingMenu : public UISprite
{
public:
	static SettingMenu* GetInstance();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	void OnClickSettingButton();
private:
	void Init();
	SettingMenu();
	~SettingMenu();
	Sprite* m_TouchedButton;
};

#endif