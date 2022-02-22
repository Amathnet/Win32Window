#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

#include <iostream>
#include <string>

struct ToolBarInfo
{
	HWND parent;
	//SIZE position, size = { 125, 100 };
};

class ToolBar
{
public:
	void init(ToolBarInfo info)
	{
		m_info = info;
		create();
	}

	void create()
	{
       
	};


private:
	HWND m_hwnd;
	ToolBarInfo m_info;
};