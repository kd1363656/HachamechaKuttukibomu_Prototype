#pragma once
#ifdef _DEBUG

#include "../SingletonBase.h"

class ImGuiManager : public SingletonBase<ImGuiManager>
{
public:

	ImGuiManager ()         = default;
	~ImGuiManager()override = default;

	void Init  ();
	void Update();

};
#endif