#include "imdb_search_gui.h"
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <iostream>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// Forward declarations of external functions
std::string search_imdb(const std::string& query);
void copy_to_clipboard(const std::string& text);

// Window class name
const wchar_t* const WINDOW_CLASS_NAME = L"IMDbSearchGUIClass";

// Control IDs
const int ID_SEARCH_INPUT = 101;
const int ID_SEARCH_BUTTON = 102;
const int ID_RESULT_DISPLAY = 103;

// Helper function to convert std::string to std::wstring
std::wstring to_wstring(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// Helper function to convert std::wstring to std::string
std::string to_string(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

IMDbSearchGUI::IMDbSearchGUI() : m_hwnd(nullptr), m_searchInput(nullptr), m_searchButton(nullptr), m_resultDisplay(nullptr) {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = WINDOW_CLASS_NAME;
    RegisterClassW(&wc);
}

void IMDbSearchGUI::Show() {
    m_hwnd = CreateWindowExW(
        0, WINDOW_CLASS_NAME, L"IMDb Search",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        nullptr, nullptr, GetModuleHandle(nullptr), this
    );

    if (m_hwnd == nullptr) {
        std::cerr << "Failed to create window" << std::endl;
        return;
    }

    ShowWindow(m_hwnd, SW_SHOW);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK IMDbSearchGUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    IMDbSearchGUI* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<IMDbSearchGUI*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    } else {
        pThis = reinterpret_cast<IMDbSearchGUI*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->HandleMessage(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT IMDbSearchGUI::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            m_searchInput = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
                10, 10, 280, 25, hwnd, reinterpret_cast<HMENU>(ID_SEARCH_INPUT), nullptr, nullptr);
            m_searchButton = CreateWindowW(L"BUTTON", L"Search", WS_CHILD | WS_VISIBLE,
                300, 10, 80, 25, hwnd, reinterpret_cast<HMENU>(ID_SEARCH_BUTTON), nullptr, nullptr);
            m_resultDisplay = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                10, 45, 370, 210, hwnd, reinterpret_cast<HMENU>(ID_RESULT_DISPLAY), nullptr, nullptr);
            return 0;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_SEARCH_BUTTON) {
                PerformSearch();
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void IMDbSearchGUI::PerformSearch() {
    wchar_t query[256];
    GetWindowTextW(m_searchInput, query, sizeof(query)/sizeof(wchar_t));

    std::wstring result = L"Searching for: " + std::wstring(query) + L"\r\n";
    SetWindowTextW(m_resultDisplay, result.c_str());

    std::string imdb_id = search_imdb(to_string(query));

    if (!imdb_id.empty()) {
        result += L"Final result: " + to_wstring(imdb_id) + L"\r\n";
        result += L"Movie ID copied to clipboard!";
        copy_to_clipboard(imdb_id);
    } else {
        result += L"No results found or an error occurred.";
    }

    SetWindowTextW(m_resultDisplay, result.c_str());
}