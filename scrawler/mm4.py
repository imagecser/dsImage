# coing: utf-8
import re
import requests
import urllib.parse
from bs4 import BeautifulSoup
import threading
import queue
import os
import time
import pybloom


count = sum([len(x) for _, _, x in os.walk(os.path.dirname("../dsimage/file/"))])  # index of file
MAX_SIZE = 10000  # stop when the count reaches MAX_SIZE
INIT_URL = "http://www.mm4000.com/"
lock_count = threading.Lock()
pageset = pybloom.BloomFilter(100000)
imgset = pybloom.BloomFilter(100000)
cache = queue.Queue(0)
stack = queue.LifoQueue(0)
stack.put(INIT_URL)
urlheader = {
    'User-Agent': 'Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)',
    'Proxy-Connection': 'keep-alive',
    'Pragma': 'no-cache',
    'Cache-Control': 'no-cache',
    'Upgrade-Insecure-Requests': '1',
    'DNT': '1',
    'Accept-Encoding': 'gzip, deflate',
    'Accept-Language': 'zh-SG,zh;q=0.9,zh-CN;q=0.8,en;q=0.7,zh-TW;q=0.6'
}
start = time.time()


def parse_page():
    """
    recursively get imgs' urls
    :return: push imgs' urls to stack
    """
    global pageset, imgset, lock_count
    while True:
        url = stack.get()
        base_url = urllib.parse.urlparse(url).netloc
        urlheader['Host'] = base_url
        try:
            context = requests.get(url, headers=urlheader, timeout=3).content.decode('utf-8', 'ignore')
        except Exception as e:
            print(url + " " + str(e))
            continue
        soup = BeautifulSoup(context, 'lxml')
        for line in soup.find_all('img', attrs={'data-original': re.compile("http://.+\.(jpg|png|jpeg)$")}):
            # width = int(line.get('width'))
            # height = int(line.get('height'))
            src = str(line.get('data-original'))
            # if width > 150 and height > 150:
            tiny_pattern = re.compile(r'http://.+_(60|160)\.jpg')
            if tiny_pattern.match(src):
                continue
            if src not in imgset:
                cache.put([url, src])
                imgset.add(src)
        print("count: " + str(count) + " cache: " + str(cache.qsize()) + " stack: " + str(stack.qsize()))
        for line in soup.find_all('a', href=re.compile("http://www.mm4000.com/.+")):
            href = line.get('href')
            num_pattern = re.compile(r'(http://.+)_\d+\.html')
            href = num_pattern.sub(r'\1.html', href)
            loc_pattern = re.compile(r'(http://.+)#.+')
            href = loc_pattern.sub(r'\1', href)
            if href not in pageset:
                pageset.add(href)
                stack.put(href)


def save_img():
    """
    get img url from stack and parse it
    :return: save imgs to file/
    """
    while True:
        global count, lock_count
        [url, src] = cache.get()
        imgheader = urlheader
        imgheader['Referer'] = url
        imgheader['Host'] = urllib.parse.urlparse(src).netloc
        try:
            img = requests.get(src, headers=imgheader, timeout=3).content
        except Exception as e:
            print(src + " " + str(e))
            continue
        if str(img).count('mm4000.com'):
            continue
        with lock_count:
            count = count + 1
        if count > MAX_SIZE:
            print(time.time() - start)
            os._exit(0)
        with open("../dsimage/file/" + str(count) + '.jpg', 'wb') as f:
            f.write(img)


thread_save = []
thread_parse = []
for i in range(20):
    thread_parse.append(threading.Thread(target=parse_page, name="parse"))
for i in range(10):
    thread_save.append(threading.Thread(target=save_img, name="save" + str(i)))
for t in thread_parse:
    t.start()
for t in thread_save:
    t.start()
for t in thread_parse:
    t.join()
for t in thread_save:
    t.join()
# multiple threadings
