#include "main.h"
#include "cocos2d.h"
#include "AppDelegate.h"
#include "StringHelper.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	int len = wcslen(lpCmdLine);
	char *ascii_str = new char[len * 2 + 1];
	memset(ascii_str, 0, len * 2 + 1);
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, lpCmdLine, len, ascii_str, len, NULL, NULL);

    AppDelegate app;
	app.setPlistFils(stringhelper::Split(ascii_str, " "));
    return Application::getInstance()->run();
}
