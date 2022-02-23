#include <iostream>

#include "window.h"
#include "button.h"
#include "editbox.h"
#include "textbox.h"
#include "listbox.h"
#include "listview.h"

class MyWindow : private Window
{
public:
    void init()
    {
        WindowInfo info = { 0 };
        info.title = "Test";
        info.size = { 666, 333 };
        Window::init(info);
    }

private:

    void a(HWND hwnd)
    {
        SendMessage(hwnd, WM_CLOSE, NULL, NULL);
    }

    void b(HWND hwnd)
    {
        ShowWindow(hwnd, 2);
    }

    void c()
    {
        std::cout << "ONn" << std::endl;
    }

    void d()
    {
        std::cout << "OFF" << std::endl;
    }

    void add(HWND hwnd)
    {
        COLORREF lightred = RGB(255, 100, 100);
        COLORREF lightblack = RGB(100, 100, 100);
        
        // close button
        ButtonInfo info = { 0 };
        info.parent = hwnd;
        info.title = "X";
        info.position = { 666 - 25, 0 };
        info.size = { 25, 25 };
        info.on.color = lightred;
        info.on.function = std::bind(&MyWindow::a, this, hwnd);
        button_1.init(info);
        
        // minimize/maximize window button
        info.title = "-";
        info.position = { 666 - 25 - 25, 0 };
        info.on.color = lightblack;
        info.on.function = std::bind(&MyWindow::b, this, hwnd);
        button_2.init(info);
      
        // toggle button
        info.title = "test";
        info.position = { 10, 10 };
        info.size = { 125, 25 };
        info.on.color = lightblack;
        info.toggle = 1;
        info.on.function = [=] { c(); };
        // or: info.on.function = std::bind(&MyWindow::c, this); 
        info.off.function = [=] { d(); };
        button_3.init(info);
        

        /*
        EditBoxInfo eb_info = { 0 };
        eb_info.parent = hwnd;
        eb_info.position = { 10, 45 };
        eb_1.init(eb_info);

        TextBoxInfo tb_info = { 0 };
        tb_info.parent = hwnd;
        tb_info.text = "TEXT";
        tb_info.position = { 280, 45 };
        tb_1.init(tb_info);
        */

        /*
        ListBoxInfo lb_info = { 0 };
        lb_info.parent = hwnd;
        lb_info.position = { 10, 80 };
        lb_1.init(lb_info);
        lb_1.add_string("a");
        
        // button for lb
        info.title = "get";
        info.position = { 10, 20 };
        info.on.color = lightblack;
        info.on.function = [=] {std::cout << lb_1.get_selected() << std::endl; };
        button_3.init(info);
        */

        /*
        ListViewInfo lv_info = { 0 };
        lv_info.parent = hwnd;
        lv_info.position = { 10, 50 };
        lv_info.size = { 800, 500 };
        lv_1.init(lv_info);
        lv_1.addColum("a", 50);
        lv_1.addColum("b", 50);
        lv_1.addItem("test");
        lv_1.addSub("sub", 1, 0);
        */
             
    }

    Button button_1, button_2, button_3;
    EditBox eb_1;
    TextBox tb_1;
    ListBox lb_1;
    ListView lv_1;
};

int main()
{
    MyWindow wnd;
    wnd.init();
}