#ifndef IMDB_SEARCH_GUI_H
#define IMDB_SEARCH_GUI_H

#include <windows.h>
#include <string>

class IMDbSearchGUI {
public:
    IMDbSearchGUI();
    void Show();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void PerformSearch();

    HWND m_hwnd;
    HWND m_searchInput;
    HWND m_searchButton;
    HWND m_resultDisplay;
};

// Helper function declarations
std::wstring to_wstring(const std::string& str);
std::string to_string(const std::wstring& wstr);

#endif // IMDB_SEARCH_GUI_H