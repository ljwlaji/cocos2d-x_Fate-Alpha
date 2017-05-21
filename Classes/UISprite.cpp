#include "UISprite.h"

UISprite::UISprite()
{
}

UISprite::~UISprite()
{
}

bool UISprite::IsSpriteOutOfRange(Sprite* pSprite)
{
	if (pSprite->getBoundingBox().origin.y < 0 || pSprite->getBoundingBox().origin.y + pSprite->getBoundingBox().size.height > getContentSize().height * 0.9f)
		return true;
	return false;
}