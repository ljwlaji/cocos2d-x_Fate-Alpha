#include "QuestBook.h"
#include "Player.h"
#include "QuestMgr.h"

static QuestBook* _QuestBook = nullptr;

QuestBook::QuestBook()
{
	initWithFile(QuestBookBaseFrame);
	autorelease();
	InitFrame();
	if (sPlayer)
		InitQuestFrame(sPlayer->GetQuests());
	CurrentPage = 0;

	setVisible(false);
	SetTouchType(PlayerUITouch_QuestBook);
}

QuestBook::~QuestBook()
{
	_QuestBook = nullptr;
	removeAllChildrenWithCleanup(true);
	removeFromParentAndCleanup(true);
}

QuestBook* QuestBook::GetInstance()
{
	if (!_QuestBook)
		_QuestBook = new QuestBook();
	return _QuestBook;
}

void QuestBook::InitFrame()
{
	//	QuestBookBaseFrame
	//	QuestBookRewardFrame
	//	QuestBookCloseButton
	//	QuestBookQuestCheckFrameLittle
	//	QuestBookQuestCheckFrameBig
	//	QuestBookQuestCheckArrow
	//	QuestBookScorllBarHandler
	//	QuestBookScrollBar_Bar
	//	QuestBookScrollBar_Uper_Arrow
	//	QuestBookScrollBar_Buttom_Arrow
	//	QuestBookSingleQuestFrame
	Sprite* RewardFrame = Sprite::create(QuestBookRewardFrame);
	RewardFrame->setPosition(getContentSize().width * 0.975f - RewardFrame->getBoundingBox().size.width / 2, getContentSize().height * 0.22f);
	addChild(RewardFrame);

	ScrollBar_Frame = Sprite::create(QuestBookScrollBar_Bar);
	ScrollBar_Frame->setPosition(getContentSize().width * 0.373f, ScrollBar_Frame->getBoundingBox().size.height * 0.532f);
	addChild(ScrollBar_Frame);

	for (int i = Scroll_Bar_Uper_Arrow; i != Button_End; i++)
	{
		char msg[255];
		snprintf(msg, 255, QuestBookButton, i);
		Sprite* ButtonTemp = Sprite::create(msg);
		switch (i)
		{
		case Scroll_Bar_Uper_Arrow:
			ButtonTemp->setPosition(ScrollBar_Frame->getContentSize().width / 2, ScrollBar_Frame->getContentSize().height - ButtonTemp->getBoundingBox().size.height / 2);
			ScrollBar_Frame->addChild(ButtonTemp);
			ButtonTemp->setTag(-1);
			ScrollBarHandler = Sprite::create(QuestBookScorllBarHandler);
			ScrollBarHandler->setAnchorPoint(Vec2(0.5f, 1.0f));
			ScrollBarHandler->setPosition(ScrollBar_Frame->getContentSize().width / 2, ButtonTemp->getBoundingBox().origin.y);
			ScrollBar_Frame->addChild(ScrollBarHandler);
			break;
		case Scroll_Bar_Button_Arrow:
			ButtonTemp->setPosition(ScrollBar_Frame->getContentSize().width / 2, ButtonTemp->getBoundingBox().size.height / 2);
			ScrollBar_Frame->addChild(ButtonTemp);
			ButtonTemp->setTag(-2);
			break;
		case Close_Button:
			ButtonTemp->setPosition(getContentSize().width - ButtonTemp->getBoundingBox().size.width, getContentSize().height - ButtonTemp->getBoundingBox().size.height);
			addChild(ButtonTemp);
			ButtonTemp->setTag(-3);
			break;
		}
		QuestBookButtons[(QuestBookTag)i] = ButtonTemp;
	}
	QuestTitleBig = LabelTTF::create("Quest Title: Null", "Arial", 40, Size::ZERO, TextHAlignment::LEFT);
	QuestTitleBig->setAnchorPoint(Vec2(0, 1.0f));
	QuestTitleBig->setPosition(getContentSize().width * 0.43f, getContentSize().height * 0.82f);
	addChild(QuestTitleBig);

	QuestString = LabelTTF::create("Quest Detail: Null\nNull\nNull\nNull\nNull\nNull\n...", "Arial", 28, Size::ZERO, TextHAlignment::LEFT);
	QuestString->setAnchorPoint(Vec2(0, 1.0f));
	QuestString->setPosition(getContentSize().width * 0.43f, getContentSize().height * 0.72f);
	addChild(QuestString);

	for (int i = 0; i != 3; i++)
	{
		QuestRewardText[i] = LabelTTF::create("", "Arial", 28, Size::ZERO, TextHAlignment::LEFT);
		QuestRewardText[i]->setAnchorPoint(Vec2(0, 1.0f));
		QuestRewardText[i]->setPosition(getContentSize().width * 0.5f + (i * getContentSize().width / 5.5f), getContentSize().height * 0.225f);
		addChild(QuestRewardText[i]);
	}
}

Sprite* QuestBook::CreateNewQuestFrame(const QuestTemplate* pTemplate)
{
	Sprite* SingleQuestFrame = Sprite::create(QuestBookSingleQuestFrame);
	SingleQuestFrame->setAnchorPoint(Vec2(0, 1.0f));
	SingleQuestFrame->setTag(pTemplate->ID);
	LabelTTF* QuestTitle = LabelTTF::create(sQuestMgr->GetQuestTemplate(pTemplate->ID)->Title.c_str(), "Arial", 30, Size::ZERO, TextHAlignment::LEFT);
	QuestTitle->setAnchorPoint(Vec2(0, 0.5f));
	QuestTitle->setPosition(SingleQuestFrame->getContentSize().width * 0.2f, SingleQuestFrame->getContentSize().height * 0.65f);
	SingleQuestFrame->addChild(QuestTitle);

	return SingleQuestFrame;
}

void QuestBook::AddNewQuestToBook(const QuestTemplate* pTemplate)
{
	Sprite* TempFrame = CreateNewQuestFrame(pTemplate);
	bool Added = false;
	for (std::map<uint32, std::vector<Sprite*>>::iterator itr = m_SingleQuestFrames.begin(); itr != m_SingleQuestFrames.end(); itr++)
	{
		if (itr->second.size() != 7)
		{
			TempFrame->setPosition(getContentSize().width * 0.015f, QuestBookButtons[Scroll_Bar_Uper_Arrow]->getBoundingBox().origin.y + QuestBookButtons[Scroll_Bar_Uper_Arrow]->getBoundingBox().size.height * 1.5f - itr->second.size() * TempFrame->getBoundingBox().size.height);
			itr->second.push_back(TempFrame);
			if (itr->first != CurrentPage)
				TempFrame->setVisible(false);
			Added = true;
			break;
		}
	}
	if (!Added)
	{
		std::vector<Sprite*> NewSpriteVector;
		m_SingleQuestFrames[m_SingleQuestFrames.size()] = NewSpriteVector;
		TempFrame->setPosition(getContentSize().width * 0.015f, QuestBookButtons[Scroll_Bar_Uper_Arrow]->getBoundingBox().origin.y + QuestBookButtons[Scroll_Bar_Uper_Arrow]->getBoundingBox().size.height * 1.5f - NewSpriteVector.size() * TempFrame->getBoundingBox().size.height);
		m_SingleQuestFrames[(m_SingleQuestFrames.size() - 1)].push_back(TempFrame);
	}
	addChild(TempFrame);
}

void QuestBook::InitQuestFrame(const std::map<uint32, PlayerQuestStatus>& quests)
{
	if (quests.empty())
		return;

	int k = 0;
	int q = 0;
	std::vector<Sprite*> TempVector;
	for (std::map<uint32, PlayerQuestStatus>::const_iterator itr = quests.begin(); itr != quests.end(); itr++)
	{
		Sprite* SingleQuestFrame = Sprite::create(QuestBookSingleQuestFrame);
		SingleQuestFrame->setAnchorPoint(Vec2(0, 1.0f));
		SingleQuestFrame->setPosition(getContentSize().width * 0.015f, QuestBookButtons[Scroll_Bar_Uper_Arrow]->getBoundingBox().origin.y + QuestBookButtons[Scroll_Bar_Uper_Arrow]->getBoundingBox().size.height * 1.5f - (k * SingleQuestFrame->getBoundingBox().size.height));
		SingleQuestFrame->setTag(itr->first);
		LabelTTF* QuestTitle = LabelTTF::create(sQuestMgr->GetQuestTemplate(itr->first)->Title.c_str(), "Arial", 30,Size::ZERO,TextHAlignment::LEFT);
		QuestTitle->setAnchorPoint(Vec2(0, 0.5f));
		QuestTitle->setPosition(SingleQuestFrame->getContentSize().width * 0.2f, SingleQuestFrame->getContentSize().height * 0.65f);
		SingleQuestFrame->addChild(QuestTitle);
		if (q)
			SingleQuestFrame->setVisible(false);
		addChild(SingleQuestFrame);
		TempVector.push_back(SingleQuestFrame);
		if (++k > 6)
		{
			k = 0;
			m_SingleQuestFrames[q] = TempVector;
			q++;
			TempVector.clear();
		}
	}
	if (TempVector.size())
		m_SingleQuestFrames[q] = TempVector;
}

void QuestBook::SwapPage(bool pageup)
{
	if (pageup && !CurrentPage)//Min Page Return
		return;

	if (!pageup && CurrentPage == m_SingleQuestFrames.size() - 1) //Max Page Return
		return;

	for (int i = 0; i != m_SingleQuestFrames[CurrentPage].size(); i++)
		m_SingleQuestFrames[CurrentPage].at(i)->setVisible(false);

	pageup ? --CurrentPage : ++CurrentPage;

	for (int i = 0; i != m_SingleQuestFrames[CurrentPage].size(); i++)
		m_SingleQuestFrames[CurrentPage].at(i)->setVisible(true);

	float PlusY = QuestBookButtons[Scroll_Bar_Uper_Arrow]->getBoundingBox().size.height + ScrollBarHandler->getBoundingBox().size.height;
	float CurrentPosY = (float)CurrentPage / (float)(m_SingleQuestFrames.size() - 1.0f);
	if (!CurrentPosY)
		PlusY = 0 - QuestBookButtons[Scroll_Bar_Uper_Arrow]->getBoundingBox().size.height;
	ScrollBarHandler->setPositionY(ScrollBar_Frame->getBoundingBox().size.height - (CurrentPosY * ScrollBar_Frame->getBoundingBox().size.height) + PlusY);
}

void QuestBook::SwapVisable()
{
	isVisible() ? setVisible(false) : setVisible(true);
}

std::string itoc(const int& int_)
{
	char msg[255];
	snprintf(msg, 255, "%d", int_);
	return msg;
}

char* ToChar(const std::string& _str)
{
	char* re = "";
	return re;
}

void QuestBook::SwapQuestDetail(uint32 QuestID)
{
	if (const QuestTemplate* pTemplate = sQuestMgr->GetQuestTemplate(QuestID))
	{
		QuestTitleBig->setString(pTemplate->Title.c_str());
		QuestString->setString(pTemplate->QuestText.c_str());
		QuestRewardText[0]->setString(itoc(pTemplate->RewardMoney).c_str());
		QuestRewardText[1]->setString(itoc(pTemplate->RewardCash).c_str());
		QuestRewardText[2]->setString(itoc(pTemplate->RewardExp).c_str());
	}
}

bool QuestBook::OnUITouchBegin(Touch* Loc)
{
	if (!isVisible())
		return false;

	m_TouchedSprite = nullptr;
	for (std::map<uint32, std::vector<Sprite*>>::iterator itr = m_SingleQuestFrames.begin(); itr != m_SingleQuestFrames.end(); itr++)
	{
		if (itr->second.empty() || !itr->second.at(0)->isVisible())
			continue;

		for (int i = 0; i != itr->second.size(); i++)
		{
			if (itr->second.at(i)->IsContectPoint(Loc->getLocation()))
			{
				m_TouchedSprite = itr->second.at(i);
				return true;
			}
		}
	}

	for (int i = Scroll_Bar_Uper_Arrow; i != Button_End; i++)
	{
		if (QuestBookButtons[(QuestBookTag)i]->IsContectPoint(Loc->getLocation()))
		{
			m_TouchedSprite = QuestBookButtons[(QuestBookTag)i];
			return true;
		}
	}
}

void QuestBook::OnUITouchEnded(Touch* Loc)
{
	if (!m_TouchedSprite || !m_TouchedSprite->IsContectPoint(Loc->getLocation()))
		return;

	if (m_TouchedSprite->getTag() < 0)
	{
		switch (m_TouchedSprite->getTag())
		{
		case -1:
			SwapPage(true);
			break;
		case -2:
			SwapPage(false);
			break;
		case -3:
			SwapVisable();
			break;
		}
	}
	else
	{
		SwapQuestDetail(m_TouchedSprite->getTag());
	}
}
