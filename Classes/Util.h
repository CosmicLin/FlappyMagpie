#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "cocos2d.h"

class Util
{
    public:
        static unsigned long GetMSTime(unsigned long tick);
        static void ScaleSpriteToScreen(cocos2d::Sprite& sp);
};

#endif // __UTILITY_H__
