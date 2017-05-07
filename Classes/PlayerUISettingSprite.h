#ifndef __PLAYER_UI_SETTING_SPRITE_H__
#define __PLAYER_UI_SETTING_SPRITE_H__
#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

#define sSettingMenu  SettingMenu::GetInstance()

class SettingMenu : public Sprite
{
public:
	static SettingMenu* GetInstance();
	void OnTouchBegin(Touch* pTouch);
	void OnTouchEnded(Touch* pTouch);
	void OnClickSettingButton();
private:
	void Init();
	SettingMenu();
	~SettingMenu();
	Sprite* m_TouchedButton;
};

#endif