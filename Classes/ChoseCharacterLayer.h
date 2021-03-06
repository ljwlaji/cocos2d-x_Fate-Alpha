#ifndef __CHOSE_CHARACTER_LAYER_H__
#define __CHOSE_CHARACTER_LAYER_H__

#include "cocos2d.h"
#include "Types.h"
#ifdef __APPLE__
#include "spine/spine.h"
#include "cocos/editor-support/spine/SkeletonAnimation.h"
#else
#include "spine\spine.h"
#include "cocos\editor-support\spine\SkeletonAnimation.h"
#endif
using namespace spine;
USING_NS_CC;

#define sChoseCharacter Chose_Character_Layer::GetInstance()

enum TouchType
{
	None,
	Button,
	PlayerFrame,
};

enum FadeType
{
	BackToMenu,
	GoToCreate,
	EnterGame,
};

class Chose_Character_Layer : public Layer
{
public:
	static Chose_Character_Layer* GetInstance();
private:
	Chose_Character_Layer();
	~Chose_Character_Layer();
	virtual bool init();
	CREATE_FUNC(Chose_Character_Layer);
	virtual void update(float diff);
	bool LoadCharacters();
	void InitFrames();
	void SetChoseedCharacter(uint32 CharacterGuid);
	void SwapChosedCharacter(uint32 CharacterGuid);
	void SpritesFadeIn();
	void SpritesFadeOut(FadeType _FadeType);
	void _SwapLayer(FadeType _FadeType);
	std::string GetClassNameByClass(const UnitClasses& _class);
	std::vector<Sprite*> Buttons;

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	Sprite* ButtomBar;
	Sprite* EnterButton;
	Sprite* CanCelButton;
	Sprite* CreateButton;
	Sprite* ChoseCharacterFrame;
	Sprite* ChoseCharacterTitle;
	Sprite* CreateCharacterButton;

	Sprite* ClassFrame;
	Sprite* FactionFrame;

	LabelTTF* ClassInfo;
	LabelTTF* FactionInfo;
	SkeletonAnimation* CharacterEnumSprite;
	std::vector<Sprite*> CharacterEnumFrame;
	Vec2 VisableSize;
	uint8 TotalCharacterCount;
	Sprite* m_TouchedSprite;
	std::map<uint32, CharacterEnumInfo> CharacterEnumMap;
	ParticleSystem* p;
	CharacterEnumInfo m_ChosedInfo;
	TouchType m_TouchType;
	EventListenerTouchOneByOne* listener;
};




#endif