#ifndef __MOVER_H__
#define __MOVER_H__
//全局单例对象 用来检测碰撞
#include "Types.h"

class Mover
{
public:
	Mover();
	~Mover();
	static Mover* GetInstance();
private:

};




#endif