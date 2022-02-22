#include <iostream>

#include "window.h"
#include "button.h"
#include "timer.h"
#include "text_file.h"
#include "listbox.h"

#include <algorithm>

class sample_button : public button, listbox
{
public:
	void button_click() 
	{
	
	}

	void Init(HWND hwnd)
	{
		button_create(hwnd, L"Sample Button", { 10, 10 }, { 125, 30 });
		listbox_create(hwnd, { 150, 10 }, { 125, 300 });
		for (int i = 0; i < str.size(); i++) listbox_add_line(str[i]);
	}

	void Command(WPARAM wparam, LPARAM lparam)
	{
		button_command(wparam, lparam);
	}

private:
	std::vector<int> str = { 50, 7, 5, 8, 50000, 1 };
};

class Window : public window_base
{
	sample_button sb;

	void add_control(HWND hwnd)
	{
		sb.Init(hwnd);	
	}

	void on_wm_command(WPARAM wparam, LPARAM lparam)
	{
		sb.Command(wparam, lparam);
	}

	void on_wm_notify(WPARAM wparam, LPARAM lparam) 
	{
		sb.button_notify(wparam, lparam);
	}

};

int main()
{
	Window window;
	window.button_create(L"Sample Window");
	return 0;
}