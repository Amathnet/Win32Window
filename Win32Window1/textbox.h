#pragma once

#include <Windows.h>
#include <string>

struct TextBoxInfo
{
	HWND parent;
	LPCSTR text;
	SIZE position, size = { 125, 20 };
};

class TextBox
{
public:
	void init(TextBoxInfo info)
	{
		m_info = info;
		create();
	}

private:
	static LRESULT CALLBACK SubClassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		auto ptr = reinterpret_cast<TextBox*>(dwRefData);

		switch (msg)
		{
		case MY_CTLCOLORSTATIC:
			ptr->color(wParam, lParam);
			return 0;
		}

		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	void create()
	{
		m_hwnd = CreateWindow("STATIC", m_info.text, WS_CHILD | WS_VISIBLE | SS_CENTER, m_info.position.cx,
			m_info.position.cy, m_info.size.cx, m_info.size.cy, m_info.parent, NULL, NULL, NULL);

		SetWindowSubclass(m_hwnd, &SubClassProc, 1, reinterpret_cast<DWORD_PTR>(this));
	};

	void color(WPARAM wParam, LPARAM lParam)
	{
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, TRANSPARENT);
	}

	HWND m_hwnd;
	TextBoxInfo m_info;
};