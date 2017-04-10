#ifndef __TYPES_H__
#define __TYPES_H__

#include "cocos2d.h"

#define	int8				signed char				
#define	int16				short
#define	int32				int
#define	int64				signed __int64

#define	uint8				unsigned char		
#define	uint16				unsigned short		
#define	uint32				unsigned int		
#define	uint64				unsigned __int64	


#define EnterGame_Layer_Tag			1000
#define Loading_Layer_Tag			2000
#define Typing_Layer_Tag			3000
#define EnterGame_Layer_Tag			4000


#define Loading_Layer_Zorder	9999
typedef std::vector<std::string> LoadAddress;
#endif