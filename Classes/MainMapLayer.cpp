#include "MainMapLayer.h"

Main_Map_Layer* _Main_Map_Layer = nullptr;
Main_Map_Layer::Main_Map_Layer()
{
	_Main_Map_Layer = this;
}

Main_Map_Layer::~Main_Map_Layer()
{
}

Main_Map_Layer* Main_Map_Layer::GetInstance()
{
	if (!_Main_Map_Layer)
		_Main_Map_Layer = new Main_Map_Layer();

	return _Main_Map_Layer;
}