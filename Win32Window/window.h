#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 

// __ImageBase represents the DOS header of the module, which 
// happens to be what a Win32 module begins with. In other words,
// it’s the base address of the module. And the module base 
// address is the same as its HINSTANCE.
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

class window_messages
{
public:
    virtual void on_wm_command(WPARAM wparam, LPARAM lparam) = 0;
    virtual void on_wm_notify(WPARAM wparam, LPARAM lparam) = 0;
};

class window_controls
{
public:
    virtual void add_control(HWND hwnd) = 0;

};

HBRUSH brush = CreateSolidBrush(RGB(100, 0, 0));

class window_base : public window_controls, window_messages
{
public:
    void button_create(LPCWSTR title)
    {
        // Initialize the window class.
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = HINST_THISCOMPONENT;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = L"SampleClass";
        windowClass.hbrBackground = CreateSolidBrush(RGB(75, 75, 75));
        RegisterClassEx(&windowClass);

        // Create the window and store a handle to it.
        HWND hwnd = CreateWindow(L"SampleClass", L"Sample Window",
            WS_VISIBLE, CW_USEDEFAULT,
            CW_USEDEFAULT, 400, 400, nullptr, nullptr,
            HINST_THISCOMPONENT, this);

        add_control(hwnd);

        //remove all window styles, check MSDN for details
        SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);

        //display window
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    // DO NOT EDIT
    void OnNCCreate(HWND hwnd, LPARAM lParam)
    {
        // Save the Win32App* passed in to CreateWindow.
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }

    LRESULT OnNchitTest(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        LRESULT hit = DefWindowProc(hwnd, message, wParam, lParam);
        if (hit == HTCLIENT) hit = HTCAPTION;
        return hit;
    }
};

LRESULT CALLBACK window_base::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto pApp = reinterpret_cast<window_base*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CTLCOLORSTATIC:
        //make background of static control transparent
        SetBkMode((HDC)wParam, TRANSPARENT);
        return (INT_PTR)brush;

    case WM_NCCREATE:
        pApp->OnNCCreate(hwnd, lParam);
        return 1;

    case WM_NCHITTEST:
        return pApp->OnNchitTest(hwnd, message, wParam, lParam);

    case WM_NOTIFY:
        pApp->on_wm_notify(wParam, lParam);
        return 0;

    case WM_COMMAND:
        pApp->on_wm_command(wParam, lParam);
        return 1;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    // Handle any messages the switch statement didn't.
    return DefWindowProc(hwnd, message, wParam, lParam);
};
