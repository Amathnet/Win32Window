#include "winapi_ids.h"

#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

#include <functional>

struct ButtonState
{
    COLORREF color = RGB(0, 255, 0);
    using func = std::function<void()>;
    func function;
};

struct ButtonInfo
{
    HWND parent;
    LPCSTR title;
    SIZE position, size = { 125, 25 };
    ButtonState on, off;
    bool toggle = 0;
};

class Button
{
public:
    void init(ButtonInfo info)
    {
        m_info = info;
        create();
    }

private:
    static LRESULT CALLBACK SubClassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
    {
        auto ptr = reinterpret_cast<Button*>(dwRefData);

        switch (msg)
        {

        case MY_COMMAND:
            ptr->toggle();
            return 0;

        case MY_NOTIFY:
            ptr->paint(wParam, lParam);
            return 0;
        }

        return DefSubclassProc(hWnd, msg, wParam, lParam);
    }

    void create()
    {
        // Create the Window and store a handle to it.
        HWND m_hwnd = CreateWindowEx(0, WC_BUTTON, m_info.title, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            m_info.position.cx, m_info.position.cy, m_info.size.cx, m_info.size.cy, m_info.parent, nullptr, nullptr, this);

        SetWindowSubclass(m_hwnd, &SubClassProc, 1, reinterpret_cast<DWORD_PTR>(this));
    };

    void toggle()
    {
        // this is just used to toggle m_state (first click set it to true, second set it to zero)
        if (m_info.toggle) m_state = !m_state; 
        else m_state = 1;

        // check state and if there is a callable target for std::function
        if(m_state && m_info.on.function) m_info.on.function();
        else if(m_info.off.function) m_info.off.function();
    }

    void coloring(LPNMCUSTOMDRAW lpnmCD, COLORREF color)
    {
        // set background color
        SetDCBrushColor(lpnmCD->hdc, color);
        SelectObject(lpnmCD->hdc, GetStockObject(DC_BRUSH)); // DC_BRUSH is the background brush

        // set border color
        SetDCPenColor(lpnmCD->hdc, color);
        SelectObject(lpnmCD->hdc, GetStockObject(DC_PEN)); // DC_PEN is the border brush

        // draw it
        Rectangle(lpnmCD->hdc, lpnmCD->rc.left, lpnmCD->rc.top, lpnmCD->rc.right, lpnmCD->rc.bottom);
    }

    void paint(WPARAM wParam, LPARAM lParam)
    {
        if (((LPNMHDR)lParam)->code == NM_CUSTOMDRAW)
        {
            LPNMCUSTOMDRAW lpnmCD = (LPNMCUSTOMDRAW)lParam;

            if(!m_info.toggle) coloring(lpnmCD, m_info.on.color);
            else
            {
                if (m_state) coloring(lpnmCD, m_info.on.color);
                else coloring(lpnmCD, m_info.off.color);
            }

        }
    }


    bool m_state;

    ButtonInfo m_info;
};
