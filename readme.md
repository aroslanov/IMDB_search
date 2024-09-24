# IMDb Search Tool

This C++ program allows you to search for movies or TV shows on IMDb and retrieve their IMDb ID. It includes both a command-line interface and a graphical user interface (GUI).

## Compilation

To compile the program with GUI support, use the following command:

```
g++ -std=c++17 -O2 imdb_search.cpp imdb_search_gui.cpp -o imdb_search.exe -lwininet -lgdi32 -luser32
```

Make sure you have a C++ compiler (like MinGW-w64) installed and properly set up in your system PATH.

## Usage

### Command-line Interface

Run the compiled executable from the command line, providing the movie or TV show title as an argument. You can optionally include the year in parentheses for more accurate results.

```
.\imdb_search.exe "Movie Title (Year)"
```

Example:
```
.\imdb_search.exe "The Shawshank Redemption (1994)"
```

The program will search for the movie or TV show and, if found, display the IMDb ID. The ID will also be automatically copied to your clipboard.

### Graphical User Interface

To launch the GUI version of the program, use the following command:

```
.\imdb_search.exe --gui
```

In the GUI:
1. Enter the movie or TV show title in the input field.
2. Click the "Search" button or press Enter to perform the search.
3. The results will be displayed in the text area below.
4. If a match is found, the IMDb ID will be automatically copied to your clipboard.

## Notes

- This program uses the IMDb suggestion API to search for titles.
- Internet connection is required for the program to work.
- The program is designed for Windows and uses the WinINet library for HTTP requests and Windows API for the GUI.
- The GUI is a simple Windows native interface and does not require additional GUI libraries.
- To see and interact with the GUI, you need to run the program on a Windows machine with a graphical interface.

## Requirements

- Windows operating system
- C++17 compatible compiler
- WinINet library (usually included with Windows)
- Windows API (GDI and User32 libraries, included with Windows)

## Troubleshooting

If you encounter any issues running the executable, make sure you're in the correct directory and use `.\` before the executable name, like this:

```
.\imdb_search.exe --gui
```

or

```
.\imdb_search.exe "Movie Title"
```