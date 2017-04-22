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

bool MovementMgr::CanMoveTo(Unit* pUnit, CheckMoveTo front, float modify)
{
	std::vector<Sprite*> SpriteVector = sMainMap->GetGroundSprites();

	if (SpriteVector.empty())
		return false;

	if (!CheckEdge(pUnit, front))
		return false;

	Sprite* Checked = nullptr;
	//先找到X轴合适的判断目标
	for (int i = 0; i != SpriteVector.size(); i++)
	{
		if (Checked = SpriteVector.at(i))
		{
			Rect UnitRect = pUnit->getBoundingBox();
			switch (front)
			{
			case Move_To_Left:
				UnitRect.origin.x -= modify;
				break;
			case Move_To_Right:
				UnitRect.size.width += modify;
				break;
			case Move_To_Up:
				UnitRect.size.height += modify;
				break;
			case Move_To_Down:
				UnitRect.origin.y -= modify;
				break;
			}

			if (UnitRect.intersectsRect(Checked->getBoundingBox()))
				return false;
		}
	}
	return true;
}

bool MovementMgr::CheckEdge(Unit* pUnit, CheckMoveTo front)
{
	switch (front)
	{
	case Move_To_Left:
		if (pUnit->getBoundingBox().origin.x < 0)
			return false;
		return true;
	case Move_To_Right:
		if (pUnit->getBoundingBox().origin.x + pUnit->getBoundingBox().size.width > (sMainMap->GetGroundSprites().at(0)->getBoundingBox().size.width * sMainMap->GetGroundSprites().size()))
			return false;
		return true;
	case Move_To_Up:
		if (pUnit->getBoundingBox().origin.y + pUnit->getBoundingBox().size.height > Director::getInstance()->getVisibleSize().height)
			return false;
		return true;
	case Move_To_Down:
		if (pUnit->getBoundingBox().origin.x < 0)
			return false;
		return true;
	}
	return false;
}