**IMDB Search Tool**

A simple Python script to search for IMDB IDs using either the official API or web scraping.

## Features

* Searches for IMDB IDs based on movie or show title
* Supports searching by year (optional)
* Uses both official API and web scraping as fallbacks

## Usage

To use this tool, simply clone this repository and run `python imdb_search.py <movie/show name> [(year)]`. Replace `<movie/show name>` with the actual title of the movie or show you're interested in.

Example: `python imdb_search.py 'Gen V (2023)'`

### Command-Line Arguments

* `query`: The movie or show title to search for
* `year`: Optional year of release (in parentheses)

## Requirements

This script requires:

* Python 3.x
* `requests` library (`pip install requests`)
* `beautifulsoup4` library (`pip install beautifulsoup4`)

Note: If you're using a virtual environment, make sure to activate it before running the script.

## API Documentation

The official IMDB API used by this tool is documented on [the IMDB website](https://imdb.com/). The API endpoint and response format are subject to change without notice. This script may stop working if the API changes incompatibly.

## Web Scraping Caveats

Web scraping is a gray area of ethics, and we encourage users to respect IMDB's terms of service. If you're using this tool for commercial purposes, please ensure you have the necessary permissions and follow IMDB's guidelines for web scraping.

## Contributing

Contributions are welcome! Please fork this repository, make your changes, and submit a pull request. We'll do our best to review and merge your contributions in a timely manner.

**MIT License**

Copyright (c) 2023 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.