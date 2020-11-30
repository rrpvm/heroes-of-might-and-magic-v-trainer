#pragma once
#include <memory>
#include <wtypes.h>

class GlobalVariables
{
public:
	GlobalVariables() {
	
	};
	~GlobalVariables() {};
	HINSTANCE hInst;
	WCHAR szTitle[256]={'H','O','M','M','5', ' ','T','R','A','I','N','E','R'};
	WCHAR szWindowClass[256] = {'H','O','M','M'};
	HWND hWnd;
};

inline GlobalVariables global_vars;