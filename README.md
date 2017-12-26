# dsImage
dsImage, 2017, NJU CS data struct final project by Zhi Sun.

## Module
- dsImage: image bluring and sobel processing.
- scrawler: download pics from specific website.
  - pybloom: Bloom Filter with basic functions.

## Requirements
| Module                                   | Desription                               |
| ---------------------------------------- | ---------------------------------------- |
| [bitstring](https://pypi.python.org/pypi/bitstring/3.1.3) | Simple construction, analysis and modification of binary data. |
| [bs4](https://www.crummy.com/software/BeautifulSoup/) | Beautiful Soup sits atop an HTML or XML parser, providing Pythonic idioms for iterating, searching, and modifying the parse tree. |
| [requests](https://github.com/requests/requests) | Requests is the only Non-GMO HTTP library for Python, safe for human consumption. |
| [stb](https://github.com/nothings/stb)   | single-file public domain (or MIT licensed) libraries for C/C++ |

## Quick Start

```shell
git clone https://github.com/imagecser/dsImage
cd dsimage
git clone https://github.com/nothings/stb
cd ../scrawler
mkdir file
python3 mm4.py
```

open *dsimage/main.cpp* and modify the directory.

**Unix**: The program is based on g++5.4.0 x64.

```shell
cd dsimage
g++ main.cpp
./a.out
```
**Windows**: 
open dsimage.sln and build it.

## Testcase
All my testcases lost after a sudden shutdown...

## Cons
[dsimage](https://www.icser.me/ds/)
