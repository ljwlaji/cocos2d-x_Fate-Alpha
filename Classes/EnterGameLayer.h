#ifndef __ENTER_GAME_LAYER_H__
#define __ENTER_GAME_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

enum EnterGameButtonTags
{
	button_entergame,
	button_diffcute,
	button_settings,
	button_exit,
	

	menu_settings,
	menu_diffcute,
	//取消
	setting_button_cancel,
	//难度
	setting_button_one,
	//开关音效
	setting_button_two,
	//开启连招预览
	setting_button_three,
	setting_button_four,
	setting_button_five,
	setting_button_end,

	Diffcute_button_cancel,
	Diffcute_button_one,
	Diffcute_button_two,
	Diffcute_button_three,
	Diffcute_button_four,
	Diffcute_button_five,
	Diffcute_button_end,
	end_of_button_tag,
};

class EnterGameLayer :public Layer
{
public:
	EnterGameLayer();
	~EnterGameLayer();
	CREATE_FUNC(EnterGameLayer);
private:
	bool RunActionWithButton(int _tag);
	void onTouchEnded(Touch *touch, Event *unused_event);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void InitListener();
	void InitSettingMenu();
	void InitDiffcuteMenu();
	virtual bool init();
	void EveryThingFadeIn();
	void EveryThingFadeOut();
	void MainMenuShow();
	void MainMenuDisAppear();

	Sprite* SettingMenu;
	Sprite* DiffcuteMenu;
	int m_TouchedButton;
	Vec2 Visablesize;
	Vec2 OrginSize;
};



#endif