#ifndef __OPCODES_H__
#define __OPCODES_H__
#include "SocketClient.h"
enum Opcodes
{
	MSG_NULLPTR				= 0x000,
	CMSG_HEARTBEAT_RESULT	= 0x001,
	SMSG_HEARTBEAT_RESULT	= 0x002,
	CMSG_LOGIN_REQUIRE		= 0x003,
	SMSG_LOGIN_RESULT		= 0x004,
	CMSG_AUTH_DATA_RESULT	= 0x005,
	SMGS_AUTH_DATA_RESULT	= 0X006,
	CMSG_MESSAGE_CHAT		= 0x007,
	SMSG_MESSAGE_CHAT		= 0x008,
	CMSG_NEWS_REQUIRE		= 0x009,
	SMSG_NEWS_REQUIRE		= 0x00A,
	CMSG_ACTIONS_REQUIRE	= 0x00B,
	SMSG_ACTIONS_REQUIRE	= 0x00C,
	CMSG_AUCTION_LIST_ITEM	= 0x00D,
	SMSG_AUCTION_LIST_ITEM	= 0x00E,
};

enum LoginResult
{
	Login_Success,
	Login_Failed,
};

// Don't forget to change this value and add opcode name to Opcodes.cpp when you add new opcode!
#define NUM_MSG_TYPES 0x424

/// Player state
enum SessionStatus
{
	STATUS_AUTHED = 0x01, //!< Player authenticated
	STATUS_LOGGEDIN = 0x02, //!< Player in game
	STATUS_TRANSFER_PENDING = 0x04, //!< Player transferring to another map
	STATUS_NEVER = 0x08, //!< Opcode not accepted from client (deprecated or server side only)
	STATUS_PROTECTED = 0x10  //!< Using this opcode is time protected
};

class WorldPacket;

struct OpcodeHandler
{
	char const* name;
	unsigned long status;
	void (SocketClient::*handler)(WorldPacket& recvPacket);
};

extern OpcodeHandler opcodeTable[NUM_MSG_TYPES];

#endif