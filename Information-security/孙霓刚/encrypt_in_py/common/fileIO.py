from collections import deque
# 按固定长度分割字符串
def cut_text(s, n):
    return [s[i:i + n] for i in range(0, len(s), n)]
# 文件编码为数字格式
def encode(fpointer):
    data = 1
    stack = deque()
    while True:
        raw_data = fpointer.read(1)
        if raw_data:
            stack.append(ord(raw_data))
        else:
            break
    stack.reverse()
    for raw_data in stack:
        data <<= 8
        data ^= raw_data
    return data
# 数字格式编码为文件
def decode(data, fpointer):
    while data != 1:
        raw_data = 1 << 8
        raw_data ^= (data & 0xff)
        raw_data -= 1 << 8
        data >>= 8
        fpointer.write(raw_data.to_bytes(1, byteorder='big'))
