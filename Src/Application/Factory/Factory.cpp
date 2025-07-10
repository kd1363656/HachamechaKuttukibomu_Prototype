#include "Factory.h"

#include "../GameObject/Camera/FPSCamera/FPSCamera.h"
#include "../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../GameObject/Camera/DebugCamera/DebugCamera.h"
#include "../GameObject/Actor/Player/Player.h"
#include "../GameObject/Map/MapTile/Terrain/MapTileTerrain.h"
#include "../GameObject/Map/BackGround/BackGround.h"
#include "../GameObject/Projectile/StickyBomb/StickyBomb.h"

void Factory::Init()
{
	RegisterGameObjectFactoryMethod();
}

void Factory::RegisterGameObjectFactoryMethod()
{
#ifdef _DEBUG
	KdDebugGUI::Instance().AddLog("============ Start register gameObject factory ============\n\n");
#endif // _DEBUG

	RegisterGameObjectFactoryMethod<FPSCamera>  ();
	RegisterGameObjectFactoryMethod<TPSCamera>  ();
	RegisterGameObjectFactoryMethod<DebugCamera>();
	
	RegisterGameObjectFactoryMethod<Player>();

	RegisterGameObjectFactoryMethod<MapTileTerrain>();

	RegisterGameObjectFactoryMethod<BackGround>();

	RegisterGameObjectFactoryMethod<StickyBomb>();

#ifdef _DEBUG
	KdDebugGUI::Instance().AddLog("\n============ End register gameObject factory ==============\n\n\n\n");
#endif // _DEBUG
}