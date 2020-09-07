#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include "main.h"
#include <string>
#include "console.h"
#include <filesystem>

#define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE         ((DPI_AWARENESS_CONTEXT)-2)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    const char CLASS_NAME[]  = "Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "PyPKG",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
            nullptr,
            nullptr,
            hinstance,
            nullptr
            );

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CREATE:
            std::cout << "Created Window" << std::endl;
            SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
            std::cout << "Set DPI-Aware" << std::endl;
            makeMenus(hwnd);
            makeControls(hwnd);
            std::cout << "Finished making controls" << std::endl;
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);

        case WM_PAINT:
        {
            PAINTSTRUCT  ps;
            HDC  hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_COMMAND:
            switch (wParam) {
                //For Actions like menu bar button presses

                case FILE_EXITMENU:
                    DestroyWindow(hwnd);
                    break;

                case BTN_CLOSE:
                    DestroyWindow(hwnd);
                    break;

                case BTN_INSTALL_PKGS: {
                    char* pkgname = "";
                    TCHAR buff[256];
                    GetWindowText(packageNameBox, buff, GetWindowTextLength(packageNameBox) + 1);
                    pkgname = buff;
                    std::string basecmd = "pip install ";
                    basecmd += pkgname;
                    system(basecmd.c_str());
                    break;
                }
            }

        case WM_SIZING:
            resizeControls(hwnd);
            break;

        case WM_SIZE:
            resizeControls(hwnd);
            break;

        case WM_GETMINMAXINFO: {
            LPMINMAXINFO lpminmaxinfo = (LPMINMAXINFO)lParam;
            lpminmaxinfo->ptMinTrackSize.x = 300;
            lpminmaxinfo->ptMinTrackSize.y = 300;
            break;
        }

        case WM_SHOWWINDOW: {
            initPythonPackages(hwnd);
            break;
        }

        case WM_CTLCOLORSTATIC: {
            break;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void makeMenus(HWND hwnd) {
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
    AppendMenu(hFileMenu, MF_STRING, FILE_EXITMENU, "Exit");
    SetMenu(hwnd, hMenu);
}

void makeControls(HWND hwnd) {
    GetClientRect(hwnd, &rcClient);

    closeButton = CreateWindow(
            WC_BUTTON,
            "Close",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
            rcClient.right - 180,
            rcClient.bottom - 100,
            100,
            50,
            hwnd,
            (HMENU)BTN_CLOSE,
            (HINSTANCE)GetModuleHandle(NULL),
            nullptr
            );
    changeFontSize(closeButton, smallFont);

    installBtn = CreateWindow(
            WC_BUTTON,
            "Install Packages...",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
            rcClient.left + 100,
            rcClient.bottom - 100,
            150,
            50,
            hwnd,
            (HMENU)BTN_INSTALL_PKGS,
            (HINSTANCE)GetModuleHandle(NULL),
            nullptr
            );
    changeFontSize(installBtn, smallFont);

    pkView = CreateWindow(
            WC_LISTVIEW,
            "",
            WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
            rcClient.left + 100,
            rcClient.top + 100,
            300,
            300,
            hwnd,
            nullptr,
            (HINSTANCE)GetModuleHandle(NULL),
            nullptr
            );
    initListViewColumns(pkView);

    installedPKGLabel = CreateWindow(
            WC_STATIC,
            "Installed Python Packages",
            WS_VISIBLE | WS_CHILD | SS_LEFT,
            rcClient.left + 100,
            rcClient.top + 15,
            300,
            25,
            hwnd,
            nullptr,
            (HINSTANCE)GetModuleHandle(NULL),
            nullptr
            );
    SetBkColor(GetDC(installedPKGLabel), RGB(230,230,230));

    packageNameBox = CreateWindow(
            WC_EDIT,
            "",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
            rcClient.left + 275,
            rcClient.bottom - 85,
            150,
            25,
            hwnd,
            (HMENU)BTN_INSTALL_PKGS,
            (HINSTANCE)GetModuleHandle(NULL),
            nullptr
    );
    changeFontSize(packageNameBox, smallFont);
}

void changeCtrlFont(HWND control) {
    SendMessage(control, WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), true);
}

void changeFontSize(HWND control, HFONT font) {
    SendMessage(control, WM_SETFONT, WPARAM (font), true);
}

void resizeControls(HWND hwnd) {
    GetClientRect(hwnd, &rcClient);
    RECT rect;
    unsigned int width;
    unsigned int height;
    if(GetWindowRect(hwnd, &rect))
    {
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }

    SetWindowPos(closeButton, nullptr, rcClient.right - 180, rcClient.bottom - 100, 100, 50, SWP_NOZORDER);
    SetWindowPos(installBtn, nullptr, rcClient.left + 100, rcClient.bottom - 100, 150, 50, SWP_NOZORDER);
    SetWindowPos(pkView, nullptr, rcClient.left + 100, rcClient.top + 50, width - 200, height - 250, SWP_NOZORDER);
    SetWindowPos(installedPKGLabel, nullptr, rcClient.left + 100, rcClient.top + 15, 300, 25, SWP_NOZORDER);
    SetWindowPos(packageNameBox, nullptr, rcClient.left + 275, rcClient.bottom - 85, 150, 25, SWP_NOZORDER);
}

void initPythonPackages(HWND hwnd) {
    namespace fs = std::filesystem;
    fs::path path = getenv("LOCALAPPDATA");
    path /= "Programs";
    path /= "Python";
    for (const auto & entry : fs::directory_iterator(path)) {
        if (entry.exists() && entry.is_directory()) {
            path = entry.path();
            break;
        } else {
            MessageBox(hwnd, "Python installation not found", "PyPKG", MB_OK | MB_ICONINFORMATION);
        }
    }
    path /= "Lib";
    path /= "site-packages";
    std::vector<std::string> packages[1000] = {};
    for (const auto & entry : fs::directory_iterator(path)) {
        if (entry.exists() && entry.is_directory()) {
            packages->push_back(entry.path().string());
            insertPackagesToListView(pkView, entry.path().string().c_str());
            std::cout << entry.path() << std::endl;
        }
    }
}

void initListViewColumns(HWND hWndListView)
{
    char szText[256] ="buffer";     // Temporary buffer.
    LVCOLUMN lvc;

    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM| LVS_REPORT;
    lvc.pszText = "Package Name";
    lvc.cx = 800;
    lvc.cxMin = 800;
    lvc.iOrder = 0;
    ListView_InsertColumn(hWndListView, 0, &lvc);
}

void insertPackagesToListView(HWND control, const char* name) {
    LVITEM item;

    item.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    item.pszText = (char*)name;
    item.iItem = 2;
    item.iSubItem = 0;
    item.cchTextMax = 512;
    ListView_InsertItem(control, &item);

    std::cout << "Added Item " << name << std::endl;
}

std::string pathAppend(const std::string& p1, const std::string& p2) {
    using namespace std;
    char sep = '/';
    string tmp = p1;

#ifdef _WIN32
    sep = '\\';
#endif

    if (p1[p1.length()] != sep) { // Need to add a
        tmp += sep;                // path separator
        return(tmp + p2);
    }
    else
        return(p1 + p2);
}