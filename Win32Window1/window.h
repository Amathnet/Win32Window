#pragma once
#include "winapi_ids.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// this is the HINSTANCE 
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

struct wnd_info
{
    LPCSTR title;
    SIZE position, size;
    COLORREF color = RGB(75, 75, 75);
};

class Window
{
public:
    void init(wnd_info info)
    {
        m_info = info;
        create();
    }

    // add window related stuff (buttons, etc.)
    virtual void add(HWND hwnd) {};

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    void create()
    {
        // Initialize the Window class.
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = HINST_THISCOMPONENT;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = "SampleClass";
        windowClass.hbrBackground = CreateSolidBrush(m_info.color);
        RegisterClassEx(&windowClass);

        // Create the Window and store a handle to it.
        HWND hwnd = CreateWindow("SampleClass", m_info.title, WS_VISIBLE, m_info.position.cx,
            m_info.position.cy, m_info.size.cx, m_info.size.cy, nullptr, nullptr, HINST_THISCOMPONENT, this);

        SetWindowText(hwnd, m_info.title);

        //remove all Window styles, check MSDN for details
        SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);

        add(hwnd);

        //display Window
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // DO NOT EDIT
    void OnNCCreate(HWND hwnd, LPARAM lParam)
    {
        // Save the Win32App* passed in to CreateWindow.
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }

    // to enable moving of the Window
    LRESULT OnNchitTest(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        LRESULT hit = DefWindowProc(hwnd, message, wParam, lParam);
        if (hit == HTCLIENT) hit = HTCAPTION;
        return hit;
    }

    wnd_info m_info;
};

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto ptr = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message)
    {

    case WM_NCCREATE:
        ptr->OnNCCreate(hwnd, lParam);
        return 1;

    case WM_NCHITTEST:
        return ptr->OnNchitTest(hwnd, message, wParam, lParam);

    case WM_NOTIFY: // 0x004E
        SendMessage(((NMHDR*)lParam)->hwndFrom, MY_NOTIFY, wParam, lParam);
        return 0;

    case WM_COMMAND:
        if (lParam != 0) SendMessage((HWND)lParam, MY_COMMAND, wParam, lParam);
        return 1;

    case WM_CTLCOLORSTATIC: // 0x0138
        SendMessage((HWND)lParam, MY_CTLCOLORSTATIC, wParam, lParam);
        return (LRESULT)CreateSolidBrush(ptr->m_info.color);;


    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    // Handle any messages the switch statement didn't.
    return DefWindowProc(hwnd, message, wParam, lParam);
};