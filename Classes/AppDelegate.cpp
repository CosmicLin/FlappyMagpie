#include "AppDelegate.h"
#include "GameScene.h"
#include "Configuration.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    Director* director = Director::getInstance();
    GLView* glview = director->getOpenGLView();
    if (!glview)
    {
        glview = GLViewImpl::create("Flappy Magpie");
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);
    director->setAnimationInterval(1.f / GAME_FPS);

    glview->setFrameSize(GAME_WIDTH, GAME_HEIGHT);
    glview->setDesignResolutionSize(GAME_WIDTH, GAME_HEIGHT, ResolutionPolicy::NO_BORDER);

    Scene* scene = GameScene::createScene();
    director->runWithScene(scene);
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}
