#include "Factory.h"

#include "../GameObject/Actor/Player/Player.h"

void Factory::Init()
{
	RegisterGameObjectFactoryMethod();
}

void Factory::StripClassPrefix(std::string& ClassName)
{
	if (ClassName.empty())return;

	if (ClassName.starts_with("class ")) 
	{ 
		ClassName.erase(0 , 6);							   
	}
	else
	{ 
		assert(false && "クラスの書き方を確認してください"); 
	}
}

void Factory::RegisterGameObjectFactoryMethod()
{
#ifdef _DEBUG
	KdDebugGUI::Instance().AddLog("=== Start register gameObject factory ===\n");
#endif // _DEBUG

	RegisterGameObjectFuctoryMethod<Player>();

#ifdef _DEBUG
	KdDebugGUI::Instance().AddLog("=== End register gameObject factory =====\n");
#endif // _DEBUG
}
