#include <iostream>
#include <string>
#include <regex>
#include <windows.h>
#include <wininet.h>
#include <sstream>
#include <algorithm>
#include "imdb_search_gui.h"

#pragma comment(lib, "wininet.lib")

std::string url_encode(const std::string &value) {
    std::string result;
    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            result += c;
        } else {
            char buf[4];
            sprintf_s(buf, "%%%02X", (unsigned char)c);
            result += buf;
        }
    }
    return result;
}

std::string http_get(const std::string& url) {
    std::string response;
    std::cout << "Sending request to: " << url << std::endl;
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hConnect) {
            char buffer[1024];
            DWORD bytesRead;
            while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
                response.append(buffer, bytesRead);
            }
            InternetCloseHandle(hConnect);
        } else {
            std::cout << "Failed to connect to URL. Error code: " << GetLastError() << std::endl;
        }
        InternetCloseHandle(hInternet);
    } else {
        std::cout << "Failed to initialize WinINet. Error code: " << GetLastError() << std::endl;
    }
    std::cout << "Response length: " << response.length() << std::endl;
    return response;
}

std::string search_imdb_api(const std::string& query, const std::string& year = "") {
    std::string encoded_query = url_encode(query);
    std::string search_url = "https://v3.sg.media-imdb.com/suggestion/x/" + encoded_query + ".json";
    
    std::string response = http_get(search_url);
    
    std::regex json_regex("\\{.*\\}");
    std::smatch json_match;
    if (std::regex_search(response, json_match, json_regex)) {
        std::string json_response = json_match.str();
        
        std::regex id_regex("\"id\":\"(tt\\d+)\"");
        std::regex title_regex("\"l\":\"([^\"]+)\"");
        std::regex year_regex("\"y\":(\\d{4})");
        
        std::sregex_iterator it(json_response.begin(), json_response.end(), id_regex);
        std::sregex_iterator end;
        
        while (it != end) {
            std::string id = (*it)[1];
            ++it;
            
            std::smatch title_match;
            if (std::regex_search(json_response, title_match, title_regex)) {
                std::string title = title_match[1];
                std::transform(title.begin(), title.end(), title.begin(), ::tolower);
                
                std::string query_lower = query;
                std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);
                
                if (title == query_lower) {
                    std::smatch year_match;
                    if (std::regex_search(json_response, year_match, year_regex)) {
                        std::string result_year = year_match[1];
                        if (year.empty() || result_year == year) {
                            return id;
                        }
                    }
                }
            }
        }
    }
    
    return "";
}

std::string search_imdb(const std::string& query) {
    std::cout << "Searching for: " << query << std::endl;
    
    std::regex year_regex("\\((\\d{4})\\)$");
    std::smatch year_match;
    std::string year;
    std::string cleaned_query = query;
    
    if (std::regex_search(query, year_match, year_regex)) {
        year = year_match[1];
        cleaned_query = query.substr(0, year_match.position());
        cleaned_query = cleaned_query.substr(0, cleaned_query.find_last_not_of(" \t") + 1);
    }
    
    std::cout << "Query: " << cleaned_query << ", Year: " << (year.empty() ? "Not specified" : year) << std::endl;
    
    std::string imdb_id = search_imdb_api(cleaned_query, year);
    if (!imdb_id.empty()) {
        std::cout << "Found via API: " << imdb_id << std::endl;
        return imdb_id;
    }
    
    std::cout << "No results found." << std::endl;
    return "";
}

void copy_to_clipboard(const std::string& text) {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hg) {
            memcpy(GlobalLock(hg), text.c_str(), text.size() + 1);
            GlobalUnlock(hg);
            SetClipboardData(CF_TEXT, hg);
            CloseClipboard();
            std::cout << "Movie ID copied to clipboard!" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: imdb_search.exe <movie/show name> [(year)]" << std::endl;
        std::cout << "       imdb_search.exe --gui" << std::endl;
        std::cout << "Example: imdb_search.exe \"The Shawshank Redemption (1994)\"" << std::endl;
        return 1;
    }
    
    if (std::string(argv[1]) == "--gui") {
        IMDbSearchGUI gui;
        gui.Show();
        return 0;
    }
    
    std::string query;
    for (int i = 1; i < argc; ++i) {
        query += argv[i];
        if (i < argc - 1) query += " ";
    }
    
    std::string imdb_id = search_imdb(query);
    
    if (!imdb_id.empty()) {
        std::cout << "Final result: " << imdb_id << std::endl;
        copy_to_clipboard(imdb_id);
    } else {
        std::cout << "No results found or an error occurred." << std::endl;
    }
    
    return 0;
}