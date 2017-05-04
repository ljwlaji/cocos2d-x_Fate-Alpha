#ifndef __NOTIFY_MGR_H__
#define __NOTIFY_MGR_H__

#include "cocos2d.h"
USING_NS_CC;

#define sNotifyMgr NotifyMgr::GetInstance()
class NotifyMgr : public Layer
{
public:
	static NotifyMgr* GetInstance();
	void ShowNotify(const char* args);
private:
	NotifyMgr();
	~NotifyMgr();
	virtual bool init();
	CREATE_FUNC(NotifyMgr);
	void DestorySingleNotifyText(LabelTTF* pLabelTTF);
	std::vector<LabelTTF*> NotifyQueue;
	Vec2 visablesize;
};


#endif