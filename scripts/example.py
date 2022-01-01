from typing import List
import math

def Array_Search_Linear(_arr: List[int], _needle: int) -> int:
    i = 0
    for x in _arr:
        if x == _needle:
            return i
        i += 1
    return -1

def Array_Search_Binary(_arr: List[int], _needle: int, _index: int = None, _endex: int = None) -> int:
    if _index == None: _index = 0
    if _endex == None: _endex = len(_arr) - 1
    if _endex == 0:
        return -1
    if _arr[_index] == _needle:
        return _index
    if _arr[_endex] == _needle:
        return _endex
    mid = int((_index + (_endex - 1)) / 2)
    if mid == _index or mid == _endex:
        return -1
    if _arr[mid] == _needle:
        return mid
    if _arr[mid] > _needle:
        return Array_Search_Binary(_arr, _needle, _index, mid - 1)
    return Array_Search_Binary(_arr, _needle, mid + 1, _endex)

def Array_Search_Jump(_arr: List[int], _needle: int) -> int:
    arraySize = len(_arr)
    step = round(math.sqrt(arraySize))
    prev = 0
    while _arr[min(step, arraySize) - 1] < _needle:
        prev = step
        step += round(math.sqrt(arraySize))
        if prev >= arraySize:
            return -1
    while _arr[prev] < _needle:
        prev += 1
        if prev == min(step, arraySize):
            return -1
    if _arr[prev] == _needle:
        return prev
    return -1

def Array_Interpolation_Search(_arr: List[int], _needle: int) -> int:
    arraySize = len(_arr)
    low = 0
    high = arraySize - 1

    while low <= high and _needle >= _arr[low] and _needle <= _arr[high]:
        if low == high:
            if _arr[low] == _needle:
                return -1
        pos = low + int(((high - low + 0.0) / (_arr[high] - _arr[low])) * (_needle - _arr[low]))
        if _arr[pos] == _needle:
            return pos
        if _arr[pos] < _needle:
            low = pos + 1
        else:
            high = pos - 1
    return -1

def Array_Exponential_Search(_arr: List[int], _needle: int) -> int:
    arraySize = len(_arr)
    if _arr[0] == _needle:
        return 0
    i = 1
    while i < arraySize and _arr[i] <= _needle:
        i *= 2

    return Array_Search_Binary(_arr, _needle, int(i / 2), min(i, arraySize - 1))

def Array_selectionSort(_arr: List[int]):
    arraySize = len(_arr)
    min_idx = 0
    for i in range(0, arraySize):
        min_idx = i
        for j in range(i+1, arraySize):
            if _arr[j] < _arr[min_idx]:
                min_idx = j
        _arr[min_idx], _arr[i] = _arr[i], _arr[min_idx]
    print(",".join([str(x) for x in _arr]))

def Array_bubbleSort(_arr: List[int]):
    arraySize = len(_arr)
    for i in range(0, arraySize):
        for j in range(0, arraySize-i-1):
            if _arr[j] > _arr[j + 1]:
                _arr[j], _arr[j + 1] = _arr[j + 1], _arr[j]
    print(",".join([str(x) for x in _arr]))

def Array_insertionSort(_arr: List[int]):
    arraySize = len(_arr)
    for i in range(0, arraySize):
        val = _arr[i]
        j = i - 1
        while j >= 0 and _arr[j] > val:
            _arr[j + 1] = _arr[j]
            j = j - 1
        _arr[j + 1] = val
    print(",".join([str(x) for x in _arr]))

def Array_partition(_arr: List[int], _low: int, _high: int) -> int:
    pivot = _arr[_high]
    i = _low - 1
    for j in range(_low, _high - 1):
        if _arr[j] < pivot:
            i += 1
            _arr[i], _arr[j] = _arr[j], _arr[i]
    _arr[i + 1], _arr[_high] = _arr[_high], _arr[i + 1]
    return i + 1
def Array_quickSort(_arr: List[int], _low: int = None, _high: int = None, _init = True):
    if _low == None: _low = 0
    if _high == None: _high = len(_arr) - 1
    if _low < _high:
        part = Array_partition(_arr, _low, _high)
        Array_quickSort(_arr, _low, part - 1, False)
        Array_quickSort(_arr, part + 1, _high, False)
    if _init:
        print(",".join([str(x) for x in _arr]))

def Array_getMax(_arr: List[int]) -> int:
    mx = _arr[0]
    for i in _arr:
        if i > mx:
            mx = i
    return mx
def Array_countSort(_arr: List[int], _exp: int):
    arraySize = len(_arr)
    output: List[int] = _arr.copy()
    count: List[int] = [0] * 10
    for i in range(0, arraySize):
        count[int(_arr[i] / _exp) % 10] += 1
    for i in range(1, 10):
        count[i] += count[i - 1]

    for i in reversed(range(0, arraySize)):
        output[count[int(_arr[i] / _exp) % 10] - 1] = _arr[i]
        count[int(_arr[i] / _exp) % 10] -= 1

    for i in range(0, arraySize):
        _arr[i] = output[i]

def Array_radixSort(_arr: List[int]):
    m = Array_getMax(_arr)
    exp = 1
    while m / exp > 0:
        Array_countSort(_arr, exp)
        exp *= 10
    print(",".join([str(x) for x in _arr]))
