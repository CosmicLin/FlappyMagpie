#include "GameScene.h"
#include "Configuration.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    Scene* scene = Scene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!LayerColor::initWithColor(Color4B(GAME_COLOUR)))
        return false;

    cocos2d::Sprite* bg = Sprite::create("sky.png");
    float s = GAME_WIDTH / bg->getTexture()->getPixelsWide();
    bg->setScale(s);
    bg->setAnchorPoint(Vec2(0, 0));
    bg->setPosition(Vec2(0, 0));

    addChild(bg, 1);
    return true;
}
