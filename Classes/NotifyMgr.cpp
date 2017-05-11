#include "NotifyMgr.h"

static NotifyMgr* _NotifyMgr = nullptr;

NotifyMgr::NotifyMgr()
{
	_NotifyMgr = this;
	NotifyQueue.clear();
	visablesize = Director::getInstance()->getVisibleSize();
}

NotifyMgr::~NotifyMgr()
{
	removeAllChildrenWithCleanup(true);
	_NotifyMgr = nullptr;
}

bool NotifyMgr::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		bRef = true;
	} while (0);
	return bRef;
}

NotifyMgr* NotifyMgr::GetInstance()
{
	if (!_NotifyMgr)
		_NotifyMgr = NotifyMgr::create();

	return _NotifyMgr;
}

void NotifyMgr::ShowNotify(const char* args)
{
	LabelTTF* Temp = LabelTTF::create(args, "Arial", 38);
	Temp->setColor(ccc3(255, 0, 0));
	CCDelayTime* pdelay = CCDelayTime::create(2.0f);
	CCFadeOut* pFadeOut = CCFadeOut::create(1.0f);
	CCSequence* spawn = CCSequence::create(pdelay, pFadeOut, CallFunc::create(CC_CALLBACK_0(NotifyMgr::DestorySingleNotifyText, this, Temp)), NULL);
	Temp->SetRealPosition(visablesize.x / 2, visablesize.y * 0.85f + Temp->getBoundingBox().size.height);
	for (int i = 0; i != NotifyQueue.size(); i++)
		NotifyQueue.at(i)->setPositionY(NotifyQueue.at(i)->getPositionY() + Temp->getBoundingBox().size.height);
	NotifyQueue.push_back(Temp);
	addChild(Temp);
	Temp->runAction(spawn);
}

void NotifyMgr::DestorySingleNotifyText(LabelTTF* pLabelTTF)
{
	for (std::vector<LabelTTF*>::iterator i = NotifyQueue.begin(); i != NotifyQueue.end(); i++)
	{
		if (*i == pLabelTTF)
		{
			NotifyQueue.erase(i);
			pLabelTTF->removeFromParentAndCleanup(true);
			break;
		}
	}
}