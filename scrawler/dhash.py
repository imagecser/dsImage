# codingL utf-8
from PIL import Image


def dhash(image, size=(17, 16)):
    result = ""
    image.resize(size).convert('L')
    for x in range(size[0] - 1):
        for y in range(size[1]):
            left_value = image.getpixel((x, y))
            right_value = image.getpixel((x + 1, y))
            result += "1" if right_value > left_value else "0"
    return result


def dhash_cmp(dest, src):
    return sum([1 if dest[ite] == src[ite] else 0 for ite in range(len(dest))])


if __name__ == '__main__':
    record = {}
    img1 = Image.open("file/1.jpg")
    for i in range(8, 20):
        img2 = Image.open("file/%d.jpg" % i)
        record[i] = dhash_cmp(dhash(img1), dhash(img2))
    record = sorted(record.items(), key=lambda x: x[1], reverse=True)
    with open("record", "w") as f:
        for item in record:
            f.write(str(item) + "\n")

__all__ = ['dhash', 'dhash_cmp']
