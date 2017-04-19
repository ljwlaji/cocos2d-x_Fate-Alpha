#ifndef __CHOSE_CHARACTER_LAYER_H__
#define __CHOSE_CHARACTER_LAYER_H__

#include "cocos2d.h"
#include "Types.h"
USING_NS_CC;

#define sChoseCharacter Chose_Character_Layer::GetInstance()

enum TouchType
{
	None,
	Button,
	PlayerFrame,
};
struct CharacterEnumInfo
{
	uint32 guid = 0;
	std::string name = "";
	UnitClasses Class = Class_None;
	uint32 Money = 0;
	uint32 Exp = 0;
	uint32 Level = 0;
	uint16 Mapid = 0;
	float Pos_X = 0;
	float Pos_Y = 0;
};


class Chose_Character_Layer : public Layer
{
public:
	Chose_Character_Layer();
	~Chose_Character_Layer();
	virtual bool init();
	CREATE_FUNC(Chose_Character_Layer);
	static Chose_Character_Layer* GetInstance();
private:
	virtual void update(float diff);
	bool LoadCharacters();
	void InitFrames();
	void SetChoseedCharacter(uint32 CharacterGuid);
	void SwapChosedCharacter(uint32 CharacterGuid);
	void SpritesFadeIn();
	std::vector<Sprite*> DisplaySprites;

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	Sprite* Title;
	Sprite* Taiji;
	Sprite* CreateOrEnterGameButton;
	Sprite* CanCelButton;
	Sprite* CharacterEnumSprite;
	std::vector<Sprite*> CharacterEnumFrame;
	Vec2 VisableSize;
	uint8 TotalCharacterCount;
	Sprite* m_TouchedSprite;
	std::map<uint32, CharacterEnumInfo> CharacterEnumMap;
	CCParticleSystem* p;
	CharacterEnumInfo m_ChosedInfo;
	TouchType m_TouchType;
};




#endif