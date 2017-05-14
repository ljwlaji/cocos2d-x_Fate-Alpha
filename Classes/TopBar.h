#ifndef __TOP_BAR_H__
#define __TOP_BAR_H__

#include "UISprite.h"

#define sTopBar TopBar::GetInstance()

class TopBar : public UISprite
{
public:
	static TopBar* GetInstance();
	void ReSetTopBarString();

	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);

private:
	TopBar();
	~TopBar();
	void Init();
	enum TopButtonLabelTTF
	{
		MoneyTTF,
		AttackTTF,
		CashTTF,
		NameTTF,
		EndOfTopTTF,
	};
	std::vector<Sprite*> m_Buttom_Menus;
	std::map<TopButtonLabelTTF, LabelTTF*> TopMenuLabel;
};

#endif