#include "Mover.h"

static Mover* _Mover = nullptr;

Mover::Mover()
{
}

Mover::~Mover()
{
}

Mover* Mover::GetInstance()
{
	if (!_Mover)
	{
		if (!_Mover)
			_Mover = new Mover();
	}
	return _Mover;
}