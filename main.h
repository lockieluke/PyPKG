#pragma once

#define FILE_EXITMENU 1
#define BTN_CLOSE 2
#define BTN_INSTALL_PKGS 3

#define WIN32_LEAN_AND_MEAN

void makeMenus(HWND hwnd);

void makeControls(HWND hwnd);
void resizeControls(HWND hwnd);

void initListViewColumns(HWND hWndListView);
void insertPackagesToListView(HWND control, const char* name);

void changeCtrlFont(HWND control);
void changeFontSize(HWND control, HFONT font);

void initPythonPackages(HWND hwnd);


std::string pathAppend(const std::string& p1, const std::string& p2);

HWND closeButton;
HWND installBtn;
HWND pkView;
HWND installedPKGLabel;
HWND packageNameBox;
HMENU hMenu = CreateMenu();
HMENU hFileMenu = CreateMenu();
RECT rcClient;

HFONT largerFont = CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
HFONT mediumFont = CreateFont(30, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
HFONT smallFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");