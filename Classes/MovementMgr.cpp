#include "MovementMgr.h"
static MovementMgr* sMovement = nullptr;

MovementMgr::MovementMgr()
{
	sMovement = this;
}

MovementMgr::~MovementMgr()
{
}

MovementMgr* MovementMgr::GetInstance()
{
	if (!sMovement)
		sMovement = new MovementMgr();

	return sMovement;
}

bool MovementMgr::CanMoveTo(CheckMoveTo front, float current, float modify)
{
	return true;
	std::vector<Sprite*> SpriteVector = sMainMap->GetGroundSprites();

	if (SpriteVector.empty())
		return false;

	if (!CheckEdge(front))
		return false;

	Sprite* Checked = nullptr;
	//先找到X轴合适的判断目标
	for (int i = 0; i != SpriteVector.size(); i++)
	{
		if (Checked = SpriteVector.at(i))
		{
			if (NeedCheckThisGroundSprite(Checked, front))
				break;
			else Checked = nullptr;
		}
	}

	//判断
	if (Checked)
	{

	}

	return true;
}

bool MovementMgr::CheckEdge(CheckMoveTo front)
{
	switch (front)
	{
	case Move_To_Left:
	case Move_To_Right:
		break;
	case Move_To_Up:
		break;
	case Move_To_Down:
		break;
	}

	return true;
}

bool MovementMgr::NeedCheckThisGroundSprite(Sprite* pSprite, CheckMoveTo front)
{
	switch (front)
	{
	case Move_To_Left:
		break;
	case Move_To_Right:
		break;
	case Move_To_Up:
		break;
	case Move_To_Down:
		break;
	}

	return false;
}