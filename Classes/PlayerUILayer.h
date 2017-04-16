#ifndef __PLAYER_UI_LAYER_H__
#define __PLAYER_UI_LAYER_H__

#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

class PlayerUILayer : public Layer
{
public:
	PlayerUILayer();
	~PlayerUILayer();
	CREATE_FUNC(PlayerUILayer);
private:
	virtual bool init();
	void CreateVirtualRoker();
	float GetVirtualRokerOrgin(Vec2 CenterPoint,Vec2 RokerPoint);
	virtual bool onTouchRockerBegan(Touch* touches, Event *_event);
	virtual void onTouchRockerMoved(Touch* touches, Event *_event);
	virtual void onTouchRockerEnded(Touch* touches, Event *_event);
	void ResetVirtualRokerOrgin(float Orgin);
	Layer* m_VirtualRokerLayer;
	Sprite* m_VirtualRoker_BackGround;
	Sprite* m_VirtualRoker_Roker;
	Vec2 visiablesize;
	Vec2 RockerLastPostion;
};
#endif