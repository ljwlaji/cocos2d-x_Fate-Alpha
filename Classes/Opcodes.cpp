/*
* This file is part of the OregonCore Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Opcodes.h"
#include "SocketClient.h"

// Correspondence between opcodes and their names
OpcodeHandler opcodeTable[NUM_MSG_TYPES] =
{
	/*0x000*/{ "NullOpcode",				STATUS_NEVER,	&SocketClient::Handle_NULL },
	/*0x001*/{ "CMSG_HEARTBEAT_RESULT",		STATUS_NEVER,	&SocketClient::Handle_NULL },
	/*0x002*/{ "SMSG_HEARTBEAT_RESULT",		STATUS_AUTHED,	&SocketClient::HandleServerHeartBeatOpcode },
	/*0x003*/{ "CMSG_LOGIN_REQUIRE",		STATUS_AUTHED,	&SocketClient::Handle_NULL },
	/*0x004*/{ "SMSG_LOGIN_RESULT",			STATUS_AUTHED,	&SocketClient::HandleBattleNetLoginOpcode },
	/*0x005*/{ "CMSG_AUTH_DATA_RESULT",		STATUS_AUTHED,	&SocketClient::Handle_NULL },
	/*0x006*/{ "SMGS_AUTH_DATA_RESULT",		STATUS_AUTHED,	&SocketClient::HandlePlayerLoginOpcode },
	/*0x007*/{ "CMSG_MESSAGE_CHAT",			STATUS_AUTHED,	&SocketClient::Handle_NULL },
	/*0x008*/{ "SMSG_MESSAGE_CHAT",			STATUS_AUTHED,	&SocketClient::Handle_NULL },
	/*0x009*/{ "CMSG_NEWS_REQUIRE",			STATUS_AUTHED,	&SocketClient::Handle_NULL },
	/*0x010*/{ "SMSG_NEWS_REQUIRE",			STATUS_AUTHED,	&SocketClient::HandleNewsData },
	/*0x011*/{ "CMSG_ACTIONS_REQUIRE",		STATUS_AUTHED,	&SocketClient::Handle_NULL },
	/*0x012*/{ "SMSG_ACTIONS_REQUIRE",		STATUS_AUTHED,	&SocketClient::HandleActionsData },
	/*0x013*/{ "CMSG_AUCTION_LIST_ITEM",	STATUS_AUTHED,	&SocketClient::Handle_NULL },
	/*0x014*/{ "SMSG_AUCTION_LIST_ITEM",	STATUS_AUTHED,	&SocketClient::HandleAuctionGetItemListOpcode },

};

