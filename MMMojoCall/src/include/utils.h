#pragma once

#include <string>

namespace qqimpl
{
namespace utils
{
	//һЩ���ߺ���
	
	//����Ϊchar��wchar��ת��
	std::wstring Utf8ToUnicode(std::string utf8_str);
	std::string UnicodeToUtf8(std::wstring utf16_str);

	//����0->������ 1->���ļ��� 2->���ļ�
	int CheckPathInfo(std::string path);
	int CheckPathInfo(std::wstring path);

	std::string string_format(const std::string fmt_str, ...);
}
}
