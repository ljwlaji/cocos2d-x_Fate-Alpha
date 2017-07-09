#include "LoginTypeSprite.h"
#include "WorldPacket.h"
#include "SocketClient.h"
#include "NotifyMgr.h"
#pragma once
#pragma execution_character_set("utf-8")
static LoginTypeSprite* _LoginTypeSprite = nullptr;

LoginTypeSprite::LoginTypeSprite()
{
	initWithFile("Empty_Full_Screen.png");
	autorelease();
}

LoginTypeSprite::~LoginTypeSprite()
{
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
	_LoginTypeSprite = nullptr;
}

LoginTypeSprite* LoginTypeSprite::GetInstance()
{
	if (!_LoginTypeSprite)
		_LoginTypeSprite = new LoginTypeSprite();
	return _LoginTypeSprite;
}

void LoginTypeSprite::ShowUp()
{

	Title = Sprite::create("Login_Title.png");
	Title->setPosition(getContentSize().width / 2, getContentSize().height - Title->getBoundingBox().size.height * 1.5f);
	addChild(Title);

	ButtonLogin = Sprite::create("Login_Button_Login.png");
	ButtonLogin->setPosition(getContentSize().width / 2, getContentSize().height * 0.4f);

	addChild(ButtonLogin);
	ButtonLogin->setOpacity(0);
	ButtonLogin->runAction(FadeIn::create(1.0f));


	AccountFrame = Sprite::create("Login_Input.png");
	AccountFrame->SetRealPosition(getContentSize().width / 2, getContentSize().height * 0.55f);
	AccountFrame->setTag(0);
	addChild(AccountFrame);
	AccountTyping = TextFieldTTF::textFieldWithPlaceHolder("战网账号", "arial", 30);
	AccountTyping->setHorizontalAlignment(TextHAlignment::LEFT);
	AccountTyping->setTag(1);
	AccountTyping->setPosition(AccountFrame->getContentSize().width / 2, AccountFrame->getContentSize().height * 1.2f);
	AccountFrame->addChild(AccountTyping);
	AccountFrame->runAction(FadeIn::create(1.0f));

	PassWordFrame = Sprite::create("Login_Input.png");
	PassWordFrame->SetRealPosition(getContentSize().width / 2, getContentSize().height * 0.48f);
	PassWordFrame->setTag(0);
	addChild(PassWordFrame);
	PassWordTyping = TextFieldTTF::textFieldWithPlaceHolder("战网密码", "arial", 30);
	PassWordTyping->setHorizontalAlignment(TextHAlignment::LEFT);
	PassWordTyping->setTag(1);
	PassWordTyping->setPosition(PassWordFrame->getContentSize().width / 2, PassWordFrame->getContentSize().height * 1.2f);
	PassWordFrame->addChild(PassWordTyping);
	PassWordFrame->runAction(FadeIn::create(1.0f));

	RePassWord = Sprite::create("Login_RePassWord.png");
	RePassWord->setPosition(ButtonLogin->getPositionX(), ButtonLogin->getPositionY() - ButtonLogin->getBoundingBox().size.height);
	addChild(RePassWord);

	SignUp = Sprite::create("Login_SignUp.png");
	SignUp->setPosition(getContentSize().width / 2, SignUp->getBoundingBox().size.height * 1.5f);
	addChild(SignUp);
}

bool LoginTypeSprite::OnUITouchBegin(Touch* pTouch)
{
	if (AccountFrame->IsContectPoint(pTouch->getLocation()))
	{
		AccountTyping->attachWithIME();
	}
	else if (PassWordFrame->IsContectPoint(pTouch->getLocation()))
	{
		PassWordTyping->attachWithIME();
	}
	else if (ButtonLogin->IsContectPoint(pTouch->getLocation()))
	{
		HandlePlayerLogin();
	}
	return false; 
}
void LoginTypeSprite::OnUITouchMoved(Touch* pTouch)
{
}
void LoginTypeSprite::OnUITouchEnded(Touch* pTouch)
{
}

void LoginTypeSprite::HandlePlayerLogin()
{
	std::string Name = AccountTyping->getString();
	std::string Pass = PassWordTyping->getString();

	//if (Name.empty() || Pass.empty())
	//{
	//	//Send Notify
	//	return;
	//}
	WorldPacket Packet(CMSG_LOGIN_REQUIRE);
	Packet << (std::string)"123123";
	Packet << (std::string)"123123";
	//Packet << Name;
	//Packet << Pass;
	sNotifyMgr->ShowUpWithDisabaledTouching(3.0f, "正在与服务器进行连接...");
	sSocket->SendPacket(&Packet);

}

void LoginTypeSprite::OnFadeOut()
{
	_fadeout(AccountFrame);
	_fadeout(PassWordFrame);
	_fadeout(ButtonLogin);
	_fadeout((Sprite*)AccountTyping);
	_fadeout((Sprite*)PassWordTyping);
	_fadeout(RePassWord);
	_fadeout(SignUp);
	_fadeout(Title);
}

void LoginTypeSprite::_fadeout(Sprite* pSprte)
{
	if (pSprte)
		pSprte->runAction(FadeOut::create(1.0f));
}