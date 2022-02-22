#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

#include <string>
#include <vector>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

class listbox
{
public:
	void listbox_create(HWND parent, SIZE position, SIZE size)
	{
		// Create the window and store a handle to it.
		m_hwnd = CreateWindow(WC_LISTBOX, L"",
			WS_VISIBLE | WS_BORDER | WS_CHILD | LBS_STANDARD | LBS_NOTIFY, position.cx, position.cy, size.cx, size.cy,
			parent, nullptr, HINST_THISCOMPONENT, nullptr);
	}

	template<typename T>
	void listbox_add_line(T content)
	{
		if constexpr (std::is_same_v<T, std::string>)
		{
			SendMessage(m_hwnd, LB_INSERTSTRING, 0, (LPARAM)content.c_str());
			return;
		}
		else if constexpr (std::is_same_v<T, std::wstring>)
		{
			SendMessage(m_hwnd, LB_INSERTSTRING, 0, (LPARAM)content.c_str());
			return;
		}
		auto buffer = std::to_wstring(content);
		SendMessage(m_hwnd, LB_INSERTSTRING, 0, (LPARAM)buffer.c_str());
	}

	// return element index
	int listbox_remove_selected()
	{
		int current_select = SendMessage(m_hwnd, LB_GETCURSEL, 0, 0);
		SendMessage(m_hwnd, LB_DELETESTRING, current_select, 0);
		return current_select;
	}

	// add this to wm_command of main window procedure
	void listbox_command(WPARAM wparam, LPARAM lparam)
	{
		if (HWND(lparam) == m_hwnd)
		{
			// needed to remove selected line
			std::wstring buffer;
			int index = SendMessage((HWND)lparam, LB_GETCARETINDEX, 0, 0);
			SendMessage((HWND)lparam, LB_GETTEXT, (LPARAM)index, (WPARAM)buffer.c_str());
			SetWindowText(m_hwnd, buffer.c_str());
		}
	}

private:
	HWND m_hwnd;
};

// int i = listbox_remove_selected();
// str.erase(str.begin() + i);