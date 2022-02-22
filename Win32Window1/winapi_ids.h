#pragma once

// for new control style

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 

// this is used to subclass 

constexpr auto MY_COMMAND = 0x0111 + 1; // WM_COMMAND 0x0111
constexpr auto MY_NOTIFY = 0x004E + 1; // WM_NOTIFY 0x004E
constexpr auto MY_CTLCOLORSTATIC = 0x0138 + 1; // WM_CTLCOLORSTATIC 0x0138