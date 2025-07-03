#include "Factory.h"

#include "../GameObject/Camera/FPSCamera/FPSCamera.h"
#include "../GameObject/Camera/TPSCamera/TPSCamera.h"

#include "../GameObject/Actor/Player/Player.h"

void Factory::Init()
{
	RegisterGameObjectFactoryMethod();
}

void Factory::RegisterGameObjectFactoryMethod()
{
#ifdef _DEBUG
	KdDebugGUI::Instance().AddLog("============ Start register gameObject factory ============\n\n");
#endif // _DEBUG

	RegisterGameObjectFuctoryMethod<FPSCamera>();
	RegisterGameObjectFuctoryMethod<TPSCamera>();

	RegisterGameObjectFuctoryMethod<Player>();

#ifdef _DEBUG
	KdDebugGUI::Instance().AddLog("\n============ End register gameObject factory ==============\n\n\n\n");
#endif // _DEBUG
}