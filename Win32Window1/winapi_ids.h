#pragma once

// for new control style

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 

// this is used to subclass 

#define MY_COMMAND  (WM_COMMAND + 1)
#define MY_NOTIFY   (WM_NOTIFY + 1)
#define MY_CTLCOLORSTATIC	(WM_CTLCOLORSTATIC + 1)