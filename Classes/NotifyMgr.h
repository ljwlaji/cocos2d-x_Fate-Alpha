#ifndef __NOTIFY_MGR_H__
#define __NOTIFY_MGR_H__

#include "cocos2d.h"
USING_NS_CC;

#define sNotifyMgr NotifyMgr::GetInstance()
class NotifyMgr : public Layer
{
public:
	NotifyMgr();
	~NotifyMgr();
	virtual bool init();
	static NotifyMgr* GetInstance();
	CREATE_FUNC(NotifyMgr);
	void ShowNotify(const char* args);
private:

	void DestorySingleNotifyText(LabelTTF* pLabelTTF);
	std::vector<LabelTTF*> NotifyQueue;
	Vec2 visablesize;
};


#endif