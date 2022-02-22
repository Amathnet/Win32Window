#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")


#include <string>
#include <vector>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)


// 1 state button
class button
{
public:
	void button_create(HWND parent, std::wstring title, SIZE position, SIZE size)
	{
		m_hwnd = CreateWindow(WC_BUTTON, title.c_str(),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, position.cx, position.cy, size.cx, size.cy,
			parent, nullptr, HINST_THISCOMPONENT, nullptr);

		m_title = title;
	}

	void button_set_color(COLORREF color)
	{
		m_color = color;
	}

	// add this to wm_notify of main window procedure
	void button_notify(WPARAM wparam, LPARAM lparam)
	{
		LPNMCUSTOMDRAW lpnmCD = (LPNMCUSTOMDRAW)lparam;
		if (((LPNMHDR)lparam)->hwndFrom == m_hwnd)
		{
				paint(lpnmCD, m_color);
		}
	}

	virtual void button_click() {};

	// add this to wm_command of main window procedure
	void button_command(WPARAM wparam, LPARAM lparam)
	{
		if (HWND(lparam) == m_hwnd)
		{
			button_click();
		}
	}

private:
	void paint(LPNMCUSTOMDRAW lpnmCD, COLORREF color)
	{
		// set background color
		SetDCBrushColor(lpnmCD->hdc, color);
		SelectObject(lpnmCD->hdc, GetStockObject(DC_BRUSH));

		// set border color
		SetDCPenColor(lpnmCD->hdc, color);
		SelectObject(lpnmCD->hdc, GetStockObject(DC_PEN));

		// draw it
		Rectangle(lpnmCD->hdc, lpnmCD->rc.left, lpnmCD->rc.top,
			lpnmCD->rc.right, lpnmCD->rc.bottom);
	}
	void best_size()
	{
		HDC hdc = GetDC(m_hwnd);
		RECT c = { 0 };
		DrawText(hdc, m_title.c_str(), m_title.length(), &c, DT_CALCRECT | DT_EXPANDTABS);
		ReleaseDC(m_hwnd, hdc);
		SetWindowPos(m_hwnd, 0, 50, 50, c.right + 10, c.bottom + 10,
			SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOCOPYBITS);
	}

	std::wstring m_title;
	HWND m_hwnd;
	COLORREF m_color = RGB(100, 200, 250);
};

// 2 state button
class toggle_button
{
public:
	void button_create(HWND parent, LPCWSTR title, SIZE position, SIZE size)
	{
		// Create the window and store a handle to it.
		m_hwnd = CreateWindow(WC_BUTTON, title,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, position.cx, position.cy, size.cx, size.cy, 
			parent, nullptr, HINST_THISCOMPONENT, nullptr);

		button_status = 0;
	}
	virtual void button_enabled() {};
	virtual void button_disabled() {};

	// default: green
	void button_enabled_color(COLORREF color)
	{
		clicked_color = color;
	};

	// default: red
	void button_disabled_color(COLORREF color)
	{
		not_clicked_color = color;
	};

	// add this to wm_command of main window procedure
	void button_command(WPARAM wparam, LPARAM lparam)
	{
		if (HWND(lparam) == m_hwnd)
		{
			set_status();

			if (button_status) button_enabled();
			else button_disabled();
		}
	}

	// add this to wm_notify of main window procedure
	void notify(WPARAM wparam, LPARAM lparam)
	{
		LPNMCUSTOMDRAW lpnmCD = (LPNMCUSTOMDRAW)lparam;
		if (((LPNMHDR)lparam)->hwndFrom == m_hwnd)
		{
			// check for toggle and stage of customdraw
			if (button_status && lpnmCD->dwDrawStage == CDDS_PREPAINT)
			{
				paint(lpnmCD, clicked_color);
			}
			else
			{
				//paint(lpnmCD, not_clicked_color);
			};
		}
	}

private:
	void set_status() { button_status = !button_status; }
	void paint(LPNMCUSTOMDRAW lpnmCD, COLORREF color)
	{
		// set background color
		SetDCBrushColor(lpnmCD->hdc, color);
		SelectObject(lpnmCD->hdc, GetStockObject(DC_BRUSH));

		// set border color
		SetDCPenColor(lpnmCD->hdc, color);
		SelectObject(lpnmCD->hdc, GetStockObject(DC_PEN));

		// draw it
		Rectangle(lpnmCD->hdc, lpnmCD->rc.left, lpnmCD->rc.top,
			lpnmCD->rc.right, lpnmCD->rc.bottom);
	}
	
	HWND m_hwnd;
	bool button_status;
	COLORREF clicked_color = RGB(0, 255, 0);
	COLORREF not_clicked_color = RGB(255, 0, 0);
};

