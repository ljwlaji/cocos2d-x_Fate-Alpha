#include "UILoader.h"


LoadHanler LoadTable[1024] =
{
	/*0x000*/{ "MainUI_Load_Title_Buttons",			&MainUILayer::HandleInitTitleButtons	},
	/*0x001*/{ "MainUI_Load_Uper_Buttons",			&MainUILayer::HandleInitUperButtons		},
	/*0x002*/{ "MainUI_Load_News_Sprite",			&MainUILayer::HandleInitNewsFrame		},
	/*0x002*/{ "MainUI_Load_Actions_Sprite",		&MainUILayer::HandleInitActionsFrame	},
	/*0x002*/{ "MainUI_Load_Account_Sprite",		&MainUILayer::HandleInitAccountFrame	},
	/*0x002*/{ "MainUI_Load_WorldChat_Sprite",		&MainUILayer::HandleInitWorldChatFrame	},
	/*0x002*/{ "MainUI_Load_Auction_Sprite",		&MainUILayer::HandleInitAuctionHouse	},
};