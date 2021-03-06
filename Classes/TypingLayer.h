#ifndef __TYPING_LAYER_H__
#define __TYPING_LAYER_H__


#include "cocos2d.h"
USING_NS_CC;

class TypingLayer : public Layer
{
public:
	TypingLayer();
	~TypingLayer();
	CREATE_FUNC(TypingLayer);

private:
	virtual bool init();
	TextFieldTTF* AccountTyping;
	TextFieldTTF* PassWordTyping;
	bool onTouchBegan(Touch* touch, Event* ev);
	void onTouchEnded(Touch* touch, Event* ev);

	std::vector<Sprite*> TypingButtons;
};
#endif