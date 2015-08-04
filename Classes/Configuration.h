#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#define GAME_FPS 60.f
#define GAME_WIDTH 480.f
#define GAME_HEIGHT 800.f
#define GAME_COLOUR 0x4E, 0xC0, 0xCA, 0xFF // RGBA
#define GAME_SCROLL_SPEED 3.f

enum Layer
{
    BG_STATIC_LAYER,
    BG_SCROLL_LAYER,
};

#endif // __CONFIGURATION_H__
