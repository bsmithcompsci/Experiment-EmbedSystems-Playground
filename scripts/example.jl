function PrintArray(_array)
    output = ""
    for i in _array
        output = output .. i .. ","
    end
    print(output)
end

# Linear Array Search is O(n)
# Non-recursive
function Array_Search_Linear(_array, _needle)
    for i in _array
        if _array[i] == _needle then
            return i - 1
        end
    end
    return -1
end

# Binary Array Search is O(n), if unsorted but while sorted it is O(Log n).
# Recursive
function Array_Search_Binary(_array, _needle, _index, _endex)
    if _index == nil
        _index = 1
    end
    if _endex == nil
        _endex = #_array
    end

    if _array[_index] == _needle then
        return _index - 1
    end
    if _array[_endex] == _needle then
        return _endex - 1
    end

    if _index >= _endex then
        return -1
    end

    mid = math.floor((_index + (_endex - 1)) / 2)

    if _array[mid] == _needle then
        return mid - 1
    end

    if _array[mid] > _needle then
        return Array_Search_Binary(_array, _needle, _index, mid - 1)
    end

    return Array_Search_Binary(_array, _needle, mid + 1, _endex)
end

function Array_Search_Jump(_array, _needle)
    arraySize = #_array
    step = math.floor(math.sqrt(arraySize))

    prev = 1

    while(_array[math.min(step, arraySize) - 1] < _needle) 
        prev = step
        step = step + math.floor(math.sqrt(arraySize))
        if prev >= arraySize then
            return -1
        end
    end

    while(_array[prev] < _needle) 
        prev = prev + 1
        if prev == math.min(step, arraySize) then
            return -1
        end
    end

    if (_array[prev] == _needle) then
        return prev
    end
    return -1
end

function Array_Interpolation_Search(_array, _needle)
    low = 1
    high = #_array - 1

    while low <= high and _needle >= _array[low] and _needle <= _array[high] 
        if low == high then
            if _array[low] == _needle then
                return low
            end
            return -1
        end

        pos = low + math.floor(((high - low + 0.0) / (_array[high] - _array[low])) * (_needle - _array[low]))
        if _array[pos] == _needle then
            return pos
        end
        if _array[pos] < _needle then
            low = pos + 1
        else
            high = pos - 1
        end
    end

    return -1
end

function Array_Exponential_Search(_array, _needle)
    if _array[1] == _needle then
        return 1
    end

    i = 1
    while (i < #_array and _array[i] <= _needle) 
        i = i * 2
    end

    return Array_Search_Binary(_array, _needle, math.floor(i / 2), math.min(i, #_array - 1))
end

function Array_selectionSort(_array)
    min_idx = 1
    for i = 1, #_array - 1 
        min_idx = i
        for j = i+1, #_array 
            if _array[j] < _array[min_idx] then
                min_idx = j
            end
        end

        _array[min_idx], _array[i] = _array[i], _array[min_idx]
    end

    PrintArray(_array)
    return -1 # Ignore this, just to hide false-positive errors.
end

function Array_bubbleSort(_array)
    for i = 1, #_array 
        for j = 1, #_array - i - 1 
            if _array[j] > _array[j + 1] then
                _array[j], _array[j + 1] = _array[j + 1], _array[j]
            end
        end
    end

    PrintArray(_array)
    return -1 # Ignore this, just to hide false-positive errors.
end

function Array_insertionSort(_array)
    for i = 1, #_array 
        val = _array[i]
        j = i - 1
        while (j >= 1 and _array[j] > val) 
            _array[j + 1] = _array[j]
            j = j - 1
        end

        _array[j + 1] = val
    end

    PrintArray(_array)
    return -1 # Ignore this, just to hide false-positive errors.
end

function array_partition(_array, _low, _high)
    pivot = _array[_high]
    i = (_low - 1)
    for j = _low, _high 
        if _array[j] < pivot then
            i = i + 1
            _array[i], _array[j] = _array[j], _array[i]
        end
    end
    _array[i + 1], _array[_high] = _array[_high], _array[i + 1]
    return (i + 1)
end
function Array_quickSort(_array, _low, _high, _init)
    if _low == nil then _low = 1 end
    if _high == nil then _high = #_array end
    if _init == nil then _init = true end

    if _low < _high then
        part = array_partition(_array, _low, _high)
        Array_quickSort(_array, _low, part - 1, false)
        Array_quickSort(_array, part + 1, _high, false)
    end
    if _init then
        PrintArray(_array)
    end
    return -1 # Ignore this, just to hide false-positive errors.
end

function array_getMax(_array)
    mx = _array[1]
    for i = 2, #_array 
        if _array[i] > mx then
            mx = _array[i]
        end
    end
    return mx
end
function array_countsort(_array, _exp)
    output = table.clone(_array)
    count = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    for i = 1, #_array 
        cIndx = (math.floor(_array[i] / _exp) % 10) + 1
        count[cIndx] = count[cIndx] + 1
    end
    for i = 2, #count 
        count[i] = count[i] + count[i - 1]
    end

    for i = #_array, 1, -1 
        cIndx = (math.floor(_array[i] / _exp) % 10) + 1

        output[count[cIndx] - 1] = _array[i]
        count[cIndx] = count[cIndx] - 1
    end

    for i = 1, #_array 
        _array[i] = output[i]
    end
end
function Array_radixSort(_array)
    m = array_getMax(_array)
    exp = 1
    while m / exp > 0 
        array_countsort(_array, exp)
        exp = exp * 10
    end
    PrintArray(_array)
    return -1 # Ignore this, just to hide false-positive errors.
end