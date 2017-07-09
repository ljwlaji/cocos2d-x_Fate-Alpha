#ifndef __TYPES_H__
#define __TYPES_H__

#include "cocos2d.h"
#include "Result.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;
#define MAX_QUERY_LEN   1024

#define sMusic									SimpleAudioEngine::getInstance()
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
#define Base_Falling_Speed	5.0f
class Unit;
typedef std::map<Unit*, float> CreatureThreadList;
typedef std::map<uint32, uint32> ItemValueInfo;

struct SingleTrainerSpell
{
	uint32 SpellID;
	uint32 NeedMoney;
	uint32 NeedLevel;
};
struct SingleLootInfo
{
	uint32 ItemID = 0;
	uint32 Refance = 0;
	uint32 MaxCount = 0;
	uint32 MinCount = 0;
};

struct SingleVendorInfo
{
	uint32 Item_Id;
	uint32 Exchange_Cost;
};

typedef std::list<SingleVendorInfo> VendorList;
struct SingleMapInfo
{
	std::string BackGroundMusicUrl;
	bool CanRevive;
	float RevivePosX;
	float RevivePosY;
};

enum QuestRequireType
{
	Require_None,
	Require_Monster,
	Require_Item,
	Require_Money,
	Require_Rep,
};

enum QuestRewardType
{
	Reward_None,
	Reward_Item,
	Reward_Rep,
};

enum NpcFlags
{
	NPC_Flag_GossipTalker = 1,
	NPC_Flag_QuestGiver = 2,
	NPC_Flag_Vendor = 4,
	NPC_Flag_Reqairer = 8,
	NPC_Flag_SpellTeacher = 16,
};

struct SingleQuestRequire
{
	QuestRequireType RequireType;
	uint32 RequireID;
	uint32 Count;
};

struct SingleQuestReWard
{
	QuestRewardType RewardType;
	uint32 RewardID;
	uint32 Count;
};

enum PlayerUITouchType
{
	PlayerUITouch_None,
	PlayerUITouch_UperButton,
	PlayerUITouch_Bag,
	PlayerUITouch_SpellBook,
	PlayerUITouch_Roker,
	PlayerUITouch_Buttom_Menu,
	PlayerUITouch_Button_SpellSlot,
	PlayerUITouch_Equip_Window,
	PlayerUITouch_SettingMenu,
	PlayerUITouch_SwapTopButton,
	PlayerUITouch_QuestBook,
	PlayerUITouch_DeathTalkClass,
	PlayerUITouch_DeathSign,
	PlayerUITouch_TopBar,
	PlayerUITouch_LootingSprite,
	PlayerUITouch_ItemDetailSprite,
	PlayerUITouch_NpcVendorSprite,
	PlayerUITouch_LearnSpellSprite,
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

enum PlayerEquipSlots
{
	SLOT_WEAPON,
	SLOT_SECOND_WEAPON,
	SLOT_LEFT_HAND,
	SLOT_RIGHT_HAND,
	SLOT_RING_1,
	SLOT_RING_2,
	SLOT_HEAD,
	SLOT_SHOULDER,
	SLOT_END,
};

struct ItemTemplate
{
	uint32 Entry;
	std::string Name;
	PlayerEquipSlots SlotType;
	std::string Url;
	uint8 Quality;
	uint32 BuyPrice;
	uint32 SellPrice;
	UnitClasses RequireClass;
	uint32 RequireLevel;
	uint32 MaxCount;
	uint32 ItemSet;
	uint32 MinDamage;
	uint32 MaxDamage;
	ItemValueInfo Values;
};

struct PlayerSpellTemplate
{
	uint32 ID;
	bool Active;
};
enum ButtomMenuInfo
{
	Button_Menu_Setting,
	Button_Menu_Spell,
	Button_Menu_Quest,
	Button_Menu_Bag,
	Button_Menu_Equip,
	Button_Menu_Character,
	Button_Menu_End,
};
enum PlayerBagPage
{
	Page_None = -1,
	Page_One,
	Page_Two,
	Page_Three,
	Page_Four,
	Page_Five,
	Page_Six,
	Page_Seven,
	Page_Eight,
	Page_Nine,
	End_Of_Player_Bag_Page,
	Page_EquipWindow = 100,
};

typedef std::map<uint32, cocos2d::Sprite*> SlotMap;



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
#define Loading_Layer_Zorder			9999
#define Notify_Layer_Zorder				10001
#define UI_LAYER_ZORDER					9998
#define PLAYER_ZORDER 					10000
#define SlotRowCount					8
#define SlotFieldCount					9
#define PlayerBagImage					"Player_UI_Bag.png"
#define PlayerSlotImage					"Player_UI_Bag_Slot.png"
#define PlayerBagPageImage				"Player_UI_Bag_Page.png"
#define PlayerBagPageSelectorImage		"Player_UI_Bag_Page_Selector.png"
#define PlayerUIButtomMenuImage			"Player_UI_Buttom_Menu.png"
#define PlayerUIButtonMenuListImage		"Player_UI_Buttom_Menu_"
#define PlayerUIButtonMenuSwapButton	"Player_UI_TopBar_Swap_Button.png"
#define PlayerUIEquipFrame				"Player_UI_Equip_Character_Big_Frame.png"
#define PlayerUIEquipValueFrame			"Player_UI_Equip_Character_Value_Frame.png"
#define PlayerUISpellBookFrame			"Player_UI_Spell_Frame.png"
#define PlayerUISpellDefaultFrame		"Player_UI_SpellBook_Default_Frame.png"
#define SpellInfoText
#define PlayerUIButtonSpellBarImage		"Player_UI_ButtomSpell_Bar.png"

#define PlayerUICharacterHeadImage			"Player_UI_Character_Head.png"
#define PlayerUICharacterHPProccessImage	"Player_UI_Character_HP_score.png"
#define PlayerUICharacterMPPorccessImage	"Player_UI_Character_MP_Score.png"
#define PlayerUICharacterLevelFrame			"Player_UI_Character_Level.png"
#define PlayerUICastingBarImage				"Player_UI_Casting.png"
#define PlayerUICastingBarFrame				"Player_UI_Character_Casting_Frame.png"
#define PlayerUICastingBarPorccessImage		"Player_UI_Character_Casting_Score.png"


#define PlayerUISettingMenuFrame			"Player_Setting_UI_Setting_Frame.png"
#define PlayerUISettingMenuButtonNormal		"Player_Setting_UI_Normal_Frame.png"
#define PlayerUISettingMenuButtonSelected	"Player_Setting_UI_Selected_Frame.png"


#define EnterGameLayerBackGroundImage		"FateAlpha.png"
#define EnterGameLayerLeftFrameBig			"NotificationWindow.png"
#define EnterGameLayerLeftFrameSmall		"NotificationWindow1.png"
#define EnterGameLayerCopyingRight			"CopingRight.png"


#define ChoseCharacterLayerCancelButton		"Chose_Character_CancelButton.png"
#define ChoseCharacterLayerEnterButton		"Chose_Character_EnterButton.png"
#define ChoseCharacterLayerCreateButton		"Chose_Character_CreateButton.png"
#define ChoseCharacterLayerCharacterFrame	"Chose_Character_SingleCharacterFrame.png"
#define ChoseCharacterLayerButtomBar		"Chose_Character_ButtomBar.png"
#define ChoseCharacterLayerClassFrame		"Chose_Character_ClassFrame.png"
#define ChoseCharacterLayerFactionFrame		"Chose_Character_FactionFrame.png"
#define ChoseCharacterLayerMenuButton		"Chose_Character_MenuButton.png"
#define ChoseCharacterLayerSelectFrame		"Chose_Character_SelectFrame.png"
#define ChoseCharacterLayerSelectTitle		"Chose_Character_SelectTitle.png"



#define QuestBookBaseFrame						"QuestBook_BaseFrame.png"
#define QuestBookRewardFrame					"QuestBook_SrcollBar_Quest_Reward_Frame.png"
#define QuestBookQuestCheckFrameBig				"QuestBook_SrcollBar_Check_Frame_Sign.png"
#define QuestBookQuestCheckArrow				"QuestBook_SrcollBar_Check_Sign.png"
#define QuestBookScorllBarHandler				"QuestBook_SrcollBar_Scroll_Handler.png"
#define QuestBookScrollBar_Bar					"QuestBook_SrcollBar_scroll_bar.png"
#define QuestBookSingleQuestFrame				"QuestBook_SrcollBar_SingleQuestFrame.png"
#define QuestBookSingleCheckFrameLittle			"QuestBook_SrcollBar_Check_Frame_Little.png"
#define QuestBookButton							"QuestBook_Button_%d.png"

#define PlayerUIExpBarFrame						"Player_UI_Exp_Bar_Frame.png"
#define PlayerUIExpBarScroll					"Player_UI_Exp_Bar_Scroll.png"

#define MonsterHpProccessBar					"Monster_Hp_Proccess.png"
#define MonsterCastingProccessBar				"Monster_Casting_Proccess.png"

#define DamageTextUrl							"number_%d.png"
#define HealingTextUrl							"number_%d,png"


#define DeadTalkClassFrame						"Player_UI_Dead_Frame.png"
#define DeadTalkClassButtonFrame				"Player_UI_Dead_Frame_Button.png"
#define DeadTalkClassOkButton					"Player_UI_Dead_Button_Yes.png"
#define DeadTalkClassCancelButton				"Player_UI_Dead_Button_No.png"


#define PlayerUILootingFrame					"Player_UI_Loot_Frame.png"
#define PlayerUILootingCloseButton				"Player_UI_Loot_Close_Button.png"
#define PlayerUILootingSingleIconFrame			"Player_UI_Loot_Icon_Frame.png"
#define PlayerUILootingSingleFrame				"Player_UI_Loot_Single_Loot_Frame.png"
#define PlayerUILootingScoreBarFrame			"Player_UI_Loot_Scroll_Frame.png"
#define PlayerUILootingScoreBarController		"Player_UI_Loot_Scroll_Controller.png"
#define PlayerUILootingScoreBarUpButton			"Player_UI_Loot_Scroll_Top_Button.png"
#define PlayerUILootingScoreBarButtomButton		"Player_UI_Loot_Scroll_Buttom_Button.png"

#define PlayerUIItemDetailFrameTop				"Player_UI_Item_Detail_Frame_Top.png"
#define PlayerUIItemDetailFrameButtom			"Player_UI_Item_Detail_Frame_Buttom.png"
#define PlayerUIItemDetailFrameMiddle			"Player_UI_Item_Detail_Frame_Middle.png"

#define PlayerUIVendorSingleIconFrame			"Player_UI_Vendor_Single_Icon_Frame.png"
#define PlayerUIVendorSingleFrame				"Player_UI_Vendor_Single_Frame.png"
#define PlayerUIVendorFrame						"Player_UI_Vendor_Frame.png"
#define PlayerUIVendorCloseButton				"Player_UI_Vendor_Close_Button.png"
#define PlayerUIVendorRepairButton				"Player_UI_Vendor_Repaire_Button.png"

#define PlayerUIMiniMapFrame					"Player_UI_Mini_Map_Frame.png"
#define PlayerUIMiniMapSearch					"Player_UI_Mini_Map_Search.png"
#define PlayerUIMiniMapSkipFrame				"Player_UI_Mini_Map_ClipFrame.png"

#define PlayerUILearnSpellSpriteSingleFrame		"Player_UI_LearnSpell_Sprite_SingleFrame.png"
#define PlayerUILearnSpellSpriteFrame			"Player_UI_LearnSpell_Sprite_Frame.png"

enum SettingButtonTag
{
	TAG_RESUME_GAME = 1,
	TAG_EXIT_GAME,
	TAG_LOG_OUT,
	TAG_GAME_SETTING,
	TAG_SYS_SETTING,
	MaxSettingButton,
};

#define MaxButtomSpellBarSlot			8
#define SingleSlotTagStart				1
#define SingleSlotTagEnded				73
#define MaxSlotPerSpellBookPage			14
typedef std::map<uint32, PlayerSpellTemplate> PlayerSpells;
typedef std::map<uint32, std::vector<uint32>> SpellChainMap;
typedef std::vector<std::string> LoadAddress;
#endif