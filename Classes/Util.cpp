#include "Util.h"
#include "Configuration.h"

unsigned long Util::GetMSTime(unsigned long tick)
{
    return (tick / GAME_FPS) * 1000;
}

void Util::ScaleSpriteToScreen(cocos2d::Sprite& sp)
{
    float s = GAME_WIDTH / sp.getTexture()->getPixelsWide();
    sp.setScale(s);
}
