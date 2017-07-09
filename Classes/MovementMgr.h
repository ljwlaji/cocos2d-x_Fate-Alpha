#ifndef __MOVEMENT_MGR_H__
#define __MOVEMENT_MGR_H__

#include "MainMapLayer.h"
#include "Unit.h"
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
	static MovementMgr* GetInstance();
	bool CanMoveTo(Unit* pUnit, CheckMoveTo front, float modify);
private:
	MovementMgr();
	~MovementMgr();
	bool CheckEdge(Unit* pUnit, CheckMoveTo front);
};



#endif