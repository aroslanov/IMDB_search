import sys
import re
import json
from urllib.parse import quote_plus

try:
    import requests
    from bs4 import BeautifulSoup
    from PyQt6.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QWidget, QLineEdit, QPushButton, QLabel, QTextEdit
    from PyQt6.QtCore import Qt
    from PyQt6.QtGui import QClipboard
except ImportError:
    print("Error: Required libraries are not installed.")
    print("Please install the required libraries using the following command:")
    print("pip install requests beautifulsoup4 PyQt6")
    sys.exit(1)

def search_imdb_api(query, year=None):
    base_url = "https://v3.sg.media-imdb.com/suggestion/x/"
    search_url = f"{base_url}{quote_plus(query)}.json"
    
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36"
    }
    
    try:
        response = requests.get(search_url, headers=headers)
        response.raise_for_status()
        data = response.json()
        
        if 'd' in data:
            for item in data['d']:
                if item.get('q') in ['TV series', 'feature', 'TV mini-series']:
                    title = item.get('l', '').lower()
                    item_year = item.get('y')
                    
                    if title == query.lower() and (not year or str(item_year) == year):
                        return item.get('id')
        
        return None
    except requests.RequestException as e:
        print(f"API Error: Unable to connect to IMDB API. {str(e)}")
        return None

def search_imdb_web(query, year=None):
    encoded_query = quote_plus(query)
    search_url = f"https://www.imdb.com/search/title/?title={encoded_query}"
    if year:
        search_url += f"&release_date={year},{year}"

    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36"
    }
    
    try:
        response = requests.get(search_url, headers=headers)
        response.raise_for_status()
        soup = BeautifulSoup(response.content, 'html.parser')
        
        results = soup.find_all('div', class_='lister-item mode-advanced')
        
        for result in results:
            title_element = result.find('h3', class_='lister-item-header').find('a')
            title = title_element.text.strip()
            href = title_element['href']
            
            year_element = result.find('span', class_='lister-item-year')
            result_year = re.search(r'\d{4}', year_element.text).group() if year_element else None
            
            if title.lower() == query.lower() and (not year or year == result_year):
                match = re.search(r'/title/(tt\d+)/', href)
                if match:
                    return match.group(1)
        
        return None
    except requests.RequestException as e:
        print(f"Web Error: Unable to connect to IMDB. {str(e)}")
        return None

def search_imdb(query):
    print(f"Searching for: {query}")
    year_match = re.search(r'\((\d{4})\)$', query)
    year = year_match.group(1) if year_match else None
    if year:
        query = query.replace(f"({year})", "").strip()
    
    print(f"Query: {query}, Year: {year}")
    
    # Try API search first
    imdb_id = search_imdb_api(query, year)
    if imdb_id:
        print(f"Found via API: {imdb_id}")
        return imdb_id
    
    # Fall back to web scraping
    print("API search failed. Trying web scraping...")
    imdb_id = search_imdb_web(query, year)
    if imdb_id:
        print(f"Found via web scraping: {imdb_id}")
        return imdb_id
    
    print("No results found.")
    return None

class IMDbSearchGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("IMDb Search")
        self.setGeometry(100, 100, 400, 300)

        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        layout = QVBoxLayout()

        input_layout = QHBoxLayout()
        self.search_input = QLineEdit()
        self.search_input.returnPressed.connect(self.perform_search)  # Connect Enter key to search
        self.search_button = QPushButton("Search")
        self.search_button.clicked.connect(self.perform_search)
        input_layout.addWidget(self.search_input)
        input_layout.addWidget(self.search_button)

        self.result_display = QTextEdit()
        self.result_display.setReadOnly(True)

        layout.addLayout(input_layout)
        layout.addWidget(self.result_display)

        central_widget.setLayout(layout)

    def perform_search(self):
        query = self.search_input.text()
        self.result_display.clear()
        self.result_display.append(f"Searching for: {query}")
        
        imdb_id = search_imdb(query)
        
        if imdb_id:
            self.result_display.append(f"Final result: {imdb_id}")
            # Copy the movie ID to clipboard
            clipboard = QApplication.clipboard()
            clipboard.setText(imdb_id)
            self.result_display.append("Movie ID copied to clipboard!")
        else:
            self.result_display.append("No results found or an error occurred.")

if __name__ == "__main__":
    if "--gui" in sys.argv:
        app = QApplication(sys.argv)
        window = IMDbSearchGUI()
        window.show()
        sys.exit(app.exec())
    elif len(sys.argv) < 2:
        print("Usage: python imdb_search_gui.py <movie/show name> [(year)]")
        print("       python imdb_search_gui.py --gui")
        print("Example: python imdb_search_gui.py 'Gen V (2023)'")
        sys.exit(1)
    else:
        query = ' '.join(sys.argv[1:])
        imdb_id = search_imdb(query)
        
        if imdb_id:
            print(f"Final result: {imdb_id}")
            # Copy the movie ID to clipboard in CLI mode as well
            if sys.platform == "win32":
                import subprocess
                subprocess.run(["clip"], input=imdb_id.encode("utf-16"), check=True)
                print("Movie ID copied to clipboard!")
            elif sys.platform == "darwin":
                import subprocess
                subprocess.run("pbcopy", input=imdb_id.encode("utf-8"), check=True)
                print("Movie ID copied to clipboard!")
            elif sys.platform == "linux":
                import subprocess
                subprocess.run(["xclip", "-selection", "clipboard"], input=imdb_id.encode("utf-8"), check=True)
                print("Movie ID copied to clipboard!")
            else:
                print("Clipboard functionality not supported on this platform.")
        else:
            print("No results found or an error occurred.")