#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "Common.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define sGame		MainScene::GetInstance()

class Player;
class MainScene : public Scene
{
public:
	MainScene();
	~MainScene();
    virtual bool init();
	static MainScene* GetInstance();
	CREATE_FUNC(MainScene);
	void SwapLayer(int OldLayer, Layer* Instead);
};

#endif
