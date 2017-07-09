#ifndef __MAIN_UI_ACTION_SPRITE_H__
#define __MAIN_UI_ACTION_SPRITE_H__

#include "UISprite.h"
#include "Common.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"

using namespace network;

#define sActionSprite MainUIActionSprite::GetInstance()
class MainUIActionSprite : public UISprite
{
public:
	void ClearVector();
	void PushActionUrl(std::string url);
	void SetCount(uint8 Count)	{ m_ImageCount = Count; }
	static MainUIActionSprite* GetInstance();
	void InitSprite();
private:
	MainUIActionSprite();
	~MainUIActionSprite();
	virtual bool OnUITouchBegin(Touch* pTouch);
	virtual void OnUITouchEnded(Touch* pTouch);
	virtual void update(float diff);
	void SendActionRequire();
	void LoadFinished(HttpClient* sender, HttpResponse *response);
	bool WaitForLoading;
	float LoadTime;
	Sprite* DisPlayImage;
	Sprite* LoadingImage;
	uint8 m_CurrentPage;
	uint8 m_ImageCount;
	std::vector<std::string> UrlVector;
	std::vector<Sprite*> SignVector;
};
#endif