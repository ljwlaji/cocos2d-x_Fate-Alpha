#include "MainUILayerActionSprite.h"
#include "SocketClient.h"
#include "WorldPacket.h"
#include "Common.h"
static MainUIActionSprite* _MainUIActionSprite = nullptr;

MainUIActionSprite::MainUIActionSprite() : m_CurrentPage(0), m_ImageCount(0)
{
	SetTouchType(Main_UISprite_Auction_Sprite);
	initWithFile(MainUILayer_News_Frame);
	autorelease();
	WorldPacket pack(CMSG_ACTIONS_REQUIRE);
	pack << (uint32)1;
	sSocket->SendPacket(&pack);
}

MainUIActionSprite::~MainUIActionSprite()
{
	_MainUIActionSprite = nullptr;
}

MainUIActionSprite* MainUIActionSprite::GetInstance()
{
	if (!_MainUIActionSprite)
		_MainUIActionSprite = new MainUIActionSprite();
	return _MainUIActionSprite;
}

void MainUIActionSprite::InitSprite()
{
	if (m_ImageCount)
	{
		float CurrentWeith = 0;
		float TempPosition = 0;
		for (int i = 0; i != m_ImageCount; i++)
		{
			Sprite* TempSprite = FrameCreate(MainUILayer_News_Button_HL);
			addChild(TempSprite);
			if (!CurrentWeith)
				CurrentWeith = TempSprite->getBoundingBox().size.width;

			TempSprite->setPosition(getContentSize().width - (CurrentWeith * m_ImageCount) + CurrentWeith / 2 + (i * CurrentWeith), CurrentWeith / 2);
			TempSprite->setLocalZOrder(1);
			SignVector.push_back(TempSprite);
		}

		DisPlayImage = Sprite::create(LogoImage);
		DisPlayImage->setPosition(getContentSize() / 2);
		addChild(DisPlayImage);
		DisPlayImage->setVisible(false);

		LoadingImage = Sprite::create(LogoImage);
		LoadingImage->setPosition(getContentSize() / 2);
		addChild(LoadingImage);

		WaitForLoading = true;
		LoadTime = 0;
		SendActionRequire();
	}
}

bool MainUIActionSprite::OnUITouchBegin(Touch* pTouch)
{
	if (++m_CurrentPage >= m_ImageCount)
		m_CurrentPage = 0;
	SendActionRequire();
	return true;
}

void MainUIActionSprite::OnUITouchEnded(Touch* pTouch)
{

}

void MainUIActionSprite::SendActionRequire()
{
	scheduleUpdate();
	LoadTime = 0;
	LoadingImage->setVisible(true);
	DisPlayImage->setVisible(false);
	for (int i = 0; i < SignVector.size(); i++)
		SignVector.at(i)->setOpacity(i == m_CurrentPage ? 255.0f : 80.0f);

	HttpClient* httpClient = HttpClient::getInstance();
	HttpRequest* httpReq = new HttpRequest();
	httpReq->setRequestType(HttpRequest::Type::GET);
	httpReq->setUrl(UrlVector.at(m_CurrentPage).c_str());
	httpReq->setResponseCallback(CC_CALLBACK_2(MainUIActionSprite::LoadFinished, this));
	httpClient->setTimeoutForConnect(30);
	httpClient->send(httpReq);
	httpReq->release();
	httpReq = NULL;
}

void MainUIActionSprite::LoadFinished(HttpClient* /*sender*/, HttpResponse *response)
{
	unscheduleUpdate();
	LoadingImage->setVisible(false);
	DisPlayImage->setVisible(true);
	//    判断是否响应成功
	if (!response->isSucceed())
	{
		return;
	}

	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	vector<char> *buffer = response->getResponseData();

	CCImage* img = new CCImage;
	img->initWithImageData((unsigned char*)buffer->data(), buffer->size());
	if (!img->getData())
	{
		return;
	}
	Texture2D* texture = new Texture2D();
	bool isImg = texture->initWithImage(img);
	img->release();
	if (!isImg)
		return;
	DisPlayImage->setTexture(texture);
	unscheduleUpdate();
	DisPlayImage->setRotation(0);
	texture->autorelease();
}

void MainUIActionSprite::update(float diff)
{
	if (!WaitForLoading)
		return;
	if (LoadTime > 30.0f)
	{
		unscheduleUpdate();
		return;
	}

	LoadingImage->setRotation(LoadingImage->getRotation() - 3.0f);
	LoadTime += diff;
}

void MainUIActionSprite::ClearVector()
{
	UrlVector.clear();
}
void MainUIActionSprite::PushActionUrl(std::string url)
{
	UrlVector.push_back(url);
}