#ifndef __ITEM_H__
#define __ITEM_H__

#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

class Item
{
public:
	Item();
	~Item();
	std::string GetIconUrl()	{ return m_Icon_Url; }
private:
	std::string m_Icon_Url;
};


#endif