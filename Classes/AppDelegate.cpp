#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "DataMgr.h"
#include <iostream>
#include <fstream>
using namespace std;
USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 500);
static cocos2d::Size smallResolutionSize = cocos2d::Size(900, 768);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1280, 760);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);
static cocos2d::Size big = cocos2d::Size(2560, 1440);
static cocos2d::Size asdf = cocos2d::Size(1920, 1080);
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {

	//检查数据库文件是否已经提取
	CopyData("Datas.db");//要使用的sqlite库文件
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("MyGame", Rect(0, 0, mediumResolutionSize.width, mediumResolutionSize.height));
#else
        glview = GLViewImpl::create("MyGame");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

    // Set the design resolution
	glview->setDesignResolutionSize(mediumResolutionSize.width, mediumResolutionSize.height, ResolutionPolicy::EXACT_FIT);
    Size frameSize = glview->getFrameSize();
	director->setContentScaleFactor(MIN(smallResolutionSize.height / mediumResolutionSize.height, smallResolutionSize.width / mediumResolutionSize.width));

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = MainScene::create();
    // run
    director->runWithScene(scene);
	scene->CheckDB();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::CopyData(const char* pFileName)
{
	std::string dbFilePath = FileUtils::getInstance()->fullPathForFilename("Datas.db");
	std::string writablePath = FileUtils::getInstance()->getWritablePath() + pFileName;

	ssize_t dbSize;
	FileUtils::getInstance()->getFileData(writablePath.c_str(), "r", &dbSize);
	if(!dbSize)
	{
		ssize_t asize = 0;
		Data data = FileUtils::getInstance()->getDataFromFile(dbFilePath.c_str());
		FileUtils::getInstance()->writeDataToFile(data, writablePath);
	}
}
