#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

#include <iostream>
#include <string>

/* look like:
	COLUMN | COLUMN  | COLUMN
	ITEM   | SUBITEM | SUBITEM
*/

struct ListViewInfo
{
	HWND parent;
	SIZE position, size = { 125, 100 };
};

class ListView
{
public:
	void init(ListViewInfo info)
	{
		m_info = info;
		create();
	}

	void create()
	{
		m_hwnd = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_EDITLABELS,
			m_info.position.cx, m_info.position.cy, m_info.size.cx, m_info.size.cy, m_info.parent, NULL, NULL, NULL);
	};


	// add column in order its created
	void addColum(LPCSTR text, int width)
	{
		LVCOLUMN column;
		column.mask = LVCF_WIDTH | LVCF_TEXT;
		column.pszText = (LPSTR)text;
		column.cx = width;
		SendMessage(m_hwnd, LVM_INSERTCOLUMN, (WPARAM)++m_index, (LPARAM)&column);
	}

	// add item to first column
	void addItem(LPCSTR text)
	{
		LVITEM item = { 0 };
		item.mask = LVIF_TEXT;
		item.pszText = (LPSTR)text;
		SendMessage(m_hwnd, LVM_INSERTITEM, 0, (LPARAM)&item);
	}

	// add subitem to item
	void addSub(LPCSTR text, int column, int index)
	{
		LVITEM item = { 0 };
		item.mask = LVIF_TEXT;
		item.pszText = (LPSTR)text;
		item.iItem = index;
		item.iSubItem = column;
		SendMessage(m_hwnd, LVM_SETITEM, 0, (LPARAM)&item);
	}

	// remove selected item
	void remove()
	{
		int index = SendMessage(m_hwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
		SendMessage(m_hwnd, LVM_DELETEITEM, (WPARAM)index, 0);
	}

private:
	HWND m_hwnd;
	ListViewInfo m_info;
	int m_index;
};