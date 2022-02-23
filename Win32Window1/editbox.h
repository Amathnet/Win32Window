#pragma once

#include <Windows.h>
#include <string>

struct EditBoxInfo
{
	HWND parent;
	SIZE position, size = { 125, 20 };
};

class EditBox
{
public:
	void init(EditBoxInfo info)
	{
		m_info = info;
		create();
	}

	void create()
	{
		m_hwnd = CreateWindow("Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, m_info.position.cx,
			m_info.position.cy, m_info.size.cx, m_info.size.cy, m_info.parent, NULL, NULL, NULL);
	};

	std::string getText()
	{
		int len = GetWindowTextLength(m_hwnd);

		if (len == 0) return "edit box empty";
		
		std::string s;
		GetWindowText(m_hwnd, s.data(), len + 1);

		return s.c_str();
	}


private:
	HWND m_hwnd;
	EditBoxInfo m_info;
};