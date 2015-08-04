#include "GameScene.h"
#include "Configuration.h"
#include "Util.h"

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

    tickCounter = 0;
    groundY = 0;

    scheduleUpdate();
    CreateBackground();
    return true;
}

void GameScene::update(float dt)
{
    tick();
    ScrollBackground();
}

void GameScene::CreateBackground()
{
    bgScrollPrimary = Sprite::create("land.png");
    Util::ScaleSpriteToScreen(*bgScrollPrimary);
    bgScrollPrimary->setAnchorPoint(Vec2(0, 0));
    bgScrollPrimary->setPosition(Vec2(0, 0));
    groundY = bgScrollPrimary->getBoundingBox().size.height;

    bgScrollSecondary = Sprite::create("land.png");
    Util::ScaleSpriteToScreen(*bgScrollSecondary);
    bgScrollSecondary->setAnchorPoint(Vec2(0, 0));
    bgScrollSecondary->setPosition(Vec2(bgScrollPrimary->getBoundingBox().size.width, 0));

    bgStatic = Sprite::create("sky.png");
    Util::ScaleSpriteToScreen(*bgStatic);
    bgStatic->setAnchorPoint(Vec2(0, 0));
    bgStatic->setPosition(Vec2(0, groundY));

    addChild(bgScrollPrimary, BG_SCROLL_LAYER);
    addChild(bgScrollSecondary, BG_SCROLL_LAYER);
    addChild(bgStatic, BG_STATIC_LAYER);
}

void GameScene::ScrollBackground()
{
    bgScrollPrimary->setPosition(Vec2(bgScrollPrimary->getPosition().x - GAME_SCROLL_SPEED, bgScrollPrimary->getPosition().y));
    bgScrollSecondary->setPosition(Vec2(bgScrollSecondary->getPosition().x - GAME_SCROLL_SPEED, bgScrollSecondary->getPosition().y));

    if (bgScrollPrimary->getPosition().x < -bgScrollPrimary->getBoundingBox().size.width)
        bgScrollPrimary->setPosition(Vec2(bgScrollSecondary->getPosition().x + bgScrollSecondary->getBoundingBox().size.width, bgScrollPrimary->getPosition().y));

    if (bgScrollSecondary->getPosition().x < -bgScrollSecondary->getBoundingBox().size.width)
        bgScrollSecondary->setPosition(Vec2(bgScrollPrimary->getPosition().x + bgScrollPrimary->getBoundingBox().size.width, bgScrollSecondary->getPosition().y));
}
