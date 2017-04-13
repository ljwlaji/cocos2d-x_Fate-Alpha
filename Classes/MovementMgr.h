#ifndef __MOVEMENT_MGR_H__
#define __MOVEMENT_MGR_H__

#include "MainMapLayer.h"

#define sMoveMgr MovementMgr::GetInstance()
enum CheckMoveTo
{
	Move_To_Left,
	Move_To_Right,
	Move_To_Up,
	Move_To_Down,
};
class MovementMgr
{
public:
	MovementMgr();
	~MovementMgr();
	static MovementMgr* GetInstance();
	bool CanMoveTo(CheckMoveTo front, float current, float modify);
private:

	bool NeedCheckThisGroundSprite(Sprite* pSprite, CheckMoveTo front);
	bool CheckEdge(CheckMoveTo front);
};



#endif