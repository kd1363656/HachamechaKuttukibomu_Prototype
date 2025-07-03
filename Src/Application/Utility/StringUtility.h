#pragma once

namespace StringUtility
{
	// 多重定義を許す意味も"inline"にはある
	inline void StripClassPrefix(std::string& ClassName)
	{
		if (ClassName.empty())return;

		if (ClassName.starts_with("class "))
		{
			ClassName.erase(0, 6);
		}
		else
		{
			assert(false && "クラスの書き方を確認してください");
		}
	}
}