#ifndef __COMMON_H__
#define __COMMON_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define ScriptManaGer ScriptEngineManager::getInstance()

//typedef signed char			int8;
//typedef short				int16;
//typedef	int					int32;
//typedef	signed long         int64;
//typedef	unsigned char		uint8;
//typedef	unsigned short		uint16;
//typedef	unsigned int		uint32;
//typedef	unsigned long       uint64;

enum BattleNet_Message_Channel
{
	Whisper_Channel,
	World_Channel,
	Guild_Channel,
};

enum BattleNet_Message_Status
{
	Status_Wait,
	Status_Success,
	Status_UnSuccess,
	Status_PlayerOffLine,
};

#ifdef __APPLE__
typedef signed char int8;
typedef short int16;		
typedef	int int32;
typedef	signed long int64;		
typedef	unsigned char uint8;		
typedef	unsigned short uint16;		
typedef	unsigned int uint32;		
typedef	unsigned long uint64;		
#else
#define int8		signed char		
#define int16		short			
#define	int32		int				
#define	int64		signed long     
#define	uint8		unsigned char	
#define	uint16		unsigned short	
#define	uint32		unsigned int	
#define	uint64		unsigned long   
#endif
#define FrameCreate(a) Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(a))

#define LoadCacheFrame(a) SpriteFrameCache::getInstance()->addSpriteFramesWithFile(a);
#define EnterLayerTag 1000
#define MainUILayerTag 2000
#define IP_SERVER "192.168.0.102"
#define PORT_SERVER 6000

#define LogoImage						"Logo.png"
#define TitleFrame						"Title_Frame.png"

#define WaittingLayer_Show_Text			"WaittingLayer_ShowText.png"

#define MainUILayer_Title_Menu_Icon		"MainUILayer_Uper_Menu_Icon.png"
#define MainUILayer_Title_Title_Text	"MainUILayer_Title_Title_Text.png"
#define MainUILayer_Title_Chat_Icon		"MainUILayer_Title_Chat_Icon.png"

#define MainUILayer_Uper_Frame				"MainUILayer_Uper_Frame.png"
#define MainUILayer_Uper_Big_Button_Frame	"MainUILayer_Uper_Big_Frame_HL.png"
#define MainUILayer_Uper_Big_Button_Text	"MainUILayer_Uper_Button_Text_%d.png"


#define MainUILayer_Info_Frame				"MainUILayer_Infomation_Frame.png"
#define MainUILayer_Info_Button				"MainUILayer_Infomation_Next_Button.png"


#define MainUILayer_News_Button_HL		"MainUILayer_News_Sprite_Button_HL.png"
#define MainUILayer_News_Button			"MainUILayer_News_Sprite_Button.png"
#define MainUILayer_News_Frame			"MainUILayer_News_Sprite_Frame.png"

#define NotifyMgr_BackGround "NotifyMgr_BackGround.png"
#define NotifyMgr_Frame	"NotifyMgr_Frame.png"
#define NotifyMgr_Button "NotifyMgr_Button.png"


#define Account_Sprite_Frame					"Account_BackGround.png"
#define Account_Sprite_Logout_Button			"Account_Button_Logout.png"
#define Account_Sprite_Arrow					"Account_Arrow.png"
#define Account_Sprite_Number_More				"9+.png"
#define Account_Sprite_Number					"Account_Notify_Text_%d.png"
#define Account_Sprite_Main_ButtonFrame			"Account_Main_Button_Frame.png"
#define Account_Sprite_Sub_ButtonFrame			"Account_Sub_Burron_Frame.png"
#define Account_Sprite_Title					"Account_Top_Title.png"

#define Account_Sprite_Text_Auction				"Account_Text_Auction.png"
#define Account_Sprite_Text_Guild				"Account_Text_Guild.png"
#define Account_Sprite_Text_MyAccount			"Account_Text_MyAccount.png"
#define Account_Sprite_Text_Notify				"Account_Text_Notify.png"
#define Account_Sprite_Text_AccountSettings		"Account_Text_Settings.png"
#define Account_Sprite_Text_Sub_Text			"Account_Sub_Text_%d.png"

#define Account_Sprite_Notify_Frame				"Account_Text_Notify_Frame.png"


#define World_Chat_Frame				"World_Chat_Frame.png"
#define World_Chat_Back_Arrow			"World_Chat_Back_Arrow.png"
#define World_Chat_Buttom_Frame			"World_Chat_Buttom_Frame.png"
#define World_Chat_Channel_Frame		"World_Chat_Channel_Frame.png"
#define World_Chat_Channel_Guild_Text	"World_Chat_Channel_Guild.png"
#define World_Chat_Channel_World_Text	"World_Chat_Channel_World.png"
#define World_Chat_Text_Input_Frame		"World_Chat_Input_Frame.png"
#define World_Chat_Send_Message_Icon	"World_Chat_Send_Text.png"
#define World_Chat_Title_Frame			"World_Chat_Title_Frame.png"
#define World_Chat_Title_Text			"World_Chat_Title_Text.png"
#define World_Chat_Chat_Frame_Other		"World_Frame_Other.png"
#define World_Chat_Chat_Frame_Self		"World_Frame_Self.png"


#define Auction_Title					"Acution_Text_Title.png"
#define Auction_Title_FrameImage		World_Chat_Title_Frame
#define Auction_Button_Frame			"Acution_Sprite_Button.png"			
#define Auction_Button_Text				"Acution_Text_%d.png"
#define Auction_Serch_Frame				"Auction_SerchBar_Frame.png"
#define Auction_Serch_Input_Frame		"Auction_Serch_Input_Frame.png"
#define Auction_Button_Sign				"Auction_Sign_ICON.png"
#define Auction_Single_Frame			"Auction_Single_Frame.png"
#define Auction_Serch_Back_Button		"Auction_Back_Button.png"
#define Auction_Serch_Next_Button		"Auction_Next_Button.png"
#define Auction_Serch_Button			"Auction_Serch.png"
#define Auction_Serch_Icon				"Auction_SerchBar_Icon.png"

#endif
