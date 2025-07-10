#pragma once
#include <regex>

namespace StringUtility
{
	// 多重定義を許す意味も"inline"にはある
	inline void StripClassPrefix(std::string& ClassName)
	{
		if (ClassName.empty())return;

		// "\s*"は戦闘の空白があってもなくても"OK"あった場合削除対象
		// "class"m_typeNameに格納する際に"class Player"の用に代入されるから
		// "\s+"
		std::regex  stripString_(R"(\s*class\s+)");
		ClassName = std::regex_replace(ClassName, stripString_, "");
	}
}