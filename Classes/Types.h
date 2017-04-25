#ifndef __TYPES_H__
#define __TYPES_H__

#include "cocos2d.h"
#include "Result.h"

#define	int8				signed char				
#define	int16				short
#define	int32				int
#define	int64				signed __int64

#define	uint8				unsigned char		
#define	uint16				unsigned short		
#define	uint32				unsigned int		
#define	uint64				unsigned __int64	

#define EnterGame_Layer_Tag				1000
#define Loading_Layer_Tag				2000
#define Typing_Layer_Tag				3000
#define Chose_Character_Layer_Tag		4000
#define Create_Character_Layer_Tag		5000
#define Main_Map_Layer_Tag				6000
#define Player_Talk_Layer_Tag			7000
#define Player_Talk_Showing_Zorder		100000
#define Player_Talk_Disappear_Zorder	-1
#define Base_X_MovePoint	2.0f
#define Base_Y_MovePoint	1.0f

class Unit;
typedef std::map<Unit*, float> CreatureThreadList;

enum TypeID
{
	TYPEID_PLAYER,
	TYPEID_MONSTER,
	TYPEID_NPC,
};

enum UnitSide
{
	UnitSide_Left,
	UnitSide_Right,
};
struct GossipMenu
{
	GossipMenu(uint8 ImageID, uint32 _sender, uint32 _action, std::string _TalkString)
	{
		ImageId = ImageID;
		sender = _sender;
		action = _action;
		TalkString = _TalkString;
	}
	uint32 ImageId = 0;
	uint32 sender = 0;
	uint32 action = 0;
	std::string TalkString = "";
};


enum ActionType
{
	Action_None					= 0,
	Normal_Attack_0				= 1,
	Normal_Attack_1				= 2,
	Normal_Attack_2				= 3,
	Normal_Attack_3				= 4,
	Sprint_Attack				= 5,
	Air_Sprint					= 6,
	Move_Run_Right				= 7,
	Move_Run_Left				= 8,
};

enum UnitClasses
{
	Class_None,
	Saber,
	Archer,
	Caster,
	Lancer,
	Assasin,
	Rider,
	Avenger,
	Berserker,
	Class_End,
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
	uint32 Faction;
};

struct ClassInfo
{
	std::string ClassName = "";
	float ModifyStrPerLevel;
	float ModifyDexPerLevel;
	float ModifyIntPerLevel;
	float ModifyDefPerLevel;
	float ModifyHpPerLevel;
	float BaseHp;
	float BaseStr;
	float BaseInt;
	float BaseDex;
	float BaseDef;
};

enum UnitAnimationTrack
{
	Track_Idle,
	Track_Move,
	Track_Run,
	Track_Normal_Attack_0,
	Track_Normal_Attack_1,
	Track_Normal_Attack_2,
	Track_Normal_Attack_3,
	Track_Normal_Attack_4,
};
enum Facing
{
	Facing_Left,
	Facing_Right,
};
enum MoveOrientation
{
	MoveOrgin_None,
	MoveOrgin_Left,
	MoveOrgin_Right,
	MoveOrgin_Up,
	MoveOrgin_Down,
};
enum MoveType
{
	MoveType_None,
	MoveType_Walk,
	MoveType_Run,
	MoveType_Jump,
	MoveType_Actioning,
};

struct FactionFrendlyInfo
{
	uint32 MatchFactionId;
	bool IsFrendly;
};

#define Base_Clear_Key_Time	0.5f
typedef std::map<int, std::vector<RowInfo>> Result;
#define Loading_Layer_Zorder	9999
#define Notify_Layer_Zorder		10001
#define UI_LAYER_ZORDER			9998
#define PLAYER_ZORDER 			10000
typedef std::vector<std::string> LoadAddress;
#endif