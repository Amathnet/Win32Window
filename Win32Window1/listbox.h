#pragma once

#include <Windows.h>

struct ListBoxInfo
{
	HWND parent;
	SIZE position, size = { 125, 100 };
};

class ListBox
{
public:
	void init(ListBoxInfo info)
	{
		m_info = info;
		create();
	}

	void create()
	{
		m_hwnd = CreateWindow("Listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL, m_info.position.cx,
			m_info.position.cy, m_info.size.cx, m_info.size.cy, m_info.parent, NULL, NULL, NULL);
	};

	void add_string(LPCSTR text)
	{
		SendMessage(m_hwnd, LB_ADDSTRING, 0, (LPARAM)text);
	}

	void remove_selected()
	{
		int index = (int)SendMessage(m_hwnd, LB_GETCURSEL, 0, 0);
		SendMessage(m_hwnd, LB_DELETESTRING, index, 0);
	}

	LPCSTR get_selected()
	{
		int index = (int)SendMessage(m_hwnd, LB_GETCURSEL, 0, 0);

		char chBuffer[MAX_PATH];
		//SendDlgItemMessage(hwnd, iListBox, LB_GETTEXT, dwSel, (LPARAM)(LPSTR)chBuffer);
		SendMessage(m_hwnd, LB_GETTEXT, index, (LPARAM)(LPSTR)chBuffer);
		return chBuffer;
	}

private:
	HWND m_hwnd;
	ListBoxInfo m_info;
};