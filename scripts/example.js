function PrintArray(_array)
{
    let output = ""
    for (let i = 0; i < _array.length; i ++)
    {
        output = output + _array[i] + ","
    }
    print(output)
}

// Linear Array Search is O(n)
// Non-recursive
function Array_Search_Linear(_array, _needle)
{
    for (let i = 0; i < _array.length; i++)
    {
        if (_array[i] == _needle)
        {
            return i;
        }
    }
    return -1;
}

// Binary Array Search is O(n), if unsorted; but while sorted it is O(Log n).
// Recursive
function Array_Search_Binary(_array, _needle, _index, _endex)
{
    if (_index == undefined) _index = 0;
    if (_endex == undefined) _endex = _array.length - 1;

    if (_array[_index] == _needle)
    {
        return _index;
    }
    if (_array[_endex] == _needle)
    {
        return _endex;
    }

    if (_index >= _endex)
    {
        return -1;
    }

    let mid = math.floor((_index + (_endex - 1)) / 2);

    if (_array[mid] == _needle)
    {
        return mid;
    }

    if (_array[mid] > _needle)
    {
        return Array_Search_Binary(_array, _needle, _index, mid - 1);
    }

    return Array_Search_Binary(_array, _needle, mid + 1, _endex);
}

function Array_Search_Jump(_array, _needle)
{
    let arraySize = _array.length;
    let step = math.floor(math.sqrt(arraySize));

    let prev = 0;

    while(_array[math.min(step, arraySize) - 1] < _needle)
    {

        prev = step;
        step = step + math.floor(math.sqrt(arraySize));
        if (prev >= arraySize)
        {
            return -1;
        }
    }

    while(_array[prev] < _needle)
    {
        prev = prev + 1;
        if (prev == math.min(step, arraySize))
        {
            return -1;
        }
    }

    if (_array[prev] == _needle)
    {
        return prev;
    }
    return -1;
}

function Array_Interpolation_Search(_array, _needle)
{
    let low = 1;
    let high = _array.length - 1;

    while (low <= high && _needle >= _array[low] && _needle <= _array[high])
    {
        if (low == high)
        {
            if (_array[low] == _needle)
            {
                return low;
            }
            return -1;
        }

        let pos = low + math.floor(((high - low + 0.0) / (_array[high] - _array[low])) * (_needle - _array[low]));
        if (_array[pos] == _needle)
        {
            return pos;
        }
        if (_array[pos] < _needle)
            low = pos + 1;
        else
            high = pos - 1;
    }

    return -1;
}

function Array_Exponential_Search(_array, _needle)
{
    if (_array[0] == _needle)
    {
        return 0;
    }

    let i = 1;
    while (i < _array.length && _array[i] <= _needle)
    {
        i = i * 2;
    }

    return Array_Search_Binary(_array, _needle, math.floor(i / 2), math.min(i, _array.length - 1));
}

function Array_selectionSort(_array)
{
    let min_idx = 0;
    for (let i = 0; i < _array.length - 1; i++)
    {
        min_idx = i;
        for (let j = i+1; j < _array.length; j++)
        {
            if (_array[j] < _array[min_idx])
            {
                min_idx = j;
            }
        }

        [_array[min_idx], _array[i]] = [_array[i], _array[min_idx]];
    }

    PrintArray(_array);
    return -1; // Ignore this, just to hide false-positive errors.
}

function Array_bubbleSort(_array)
{
    for (let i = 0; i < _array.length; i++)
    {
        for (let j = 0; j < _array.length - i - 1; j++)
        {
            if (_array[j] > _array[j + 1])
            {
                [_array[j], _array[j + 1]] = [_array[j + 1], _array[j]];
            }
        }
    }

    PrintArray(_array);
    return -1; // Ignore this, just to hide false-positive errors.
}

function Array_insertionSort(_array)
{
    for (let i = 0; i < _array.length; i++)
    {
        let val = _array[i];
        let j = i - 1;
        while (j >= 1 && _array[j] > val)
        {
            _array[j + 1] = _array[j];
            j = j - 1;
        }

        _array[j + 1] = val;
    }

    PrintArray(_array);
    return -1; // Ignore this, just to hide false-positive errors.
}

function array_partition(_array, _low, _high)
{
    let pivot = _array[_high];
    let i = (_low - 1);
    for (let j = _low; j < _high; j++)
    {
        if (_array[j] < pivot)
        {
            i = i + 1;
            [_array[i], _array[j]] = [_array[j], _array[i]];
        }
    }
    [_array[i + 1], _array[_high]] = [_array[_high], _array[i + 1]];
    return (i + 1);
}
function Array_quickSort(_array, _low, _high, _init)
{
    if (_low == undefined) { _low = 0; };
    if (_high == undefined) { _high = _array.length; };
    if (_init == undefined) { _init = true; };

    if (_low < _high)
    {
        part = array_partition(_array, _low, _high);
        Array_quickSort(_array, _low, part - 1, false);
        Array_quickSort(_array, part + 1, _high, false);
    }
    if (_init)
    {
        PrintArray(_array);
    }
    return -1; // Ignore this, just to hide false-positive errors.
}

function array_getMax(_array)
{
    mx = _array[0];
    for (let i = 1; i < _array.length; i++)
    {
        if (_array[i] > mx)
        {
            mx = _array[i];
        }
    }
    return mx;
}
function array_countsort(_array, _exp)
{
    let output = [];
    for (let i = 0; i < _array.length; i++)
    {
        output[i] = _array[i];
    }

    let count = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
    for (let i = 0; i < _array.length; i++)
    {
        cIndx = (math.floor(_array[i] / _exp) % 10);
        count[cIndx] = count[cIndx] + 1;
    }
    for (let i = 1; i < count.length; i++)
    {
        count[i] = count[i] + count[i - 1];
    }

    for (let i = _array.length - 1; i > 0; i--)
    {
        cIndx = (math.floor(_array[i] / _exp) % 10);

        output[count[cIndx] - 1] = _array[i]
        count[cIndx] = count[cIndx] - 1;
    }

    for (let i = 1; i < _array.length; i++)
    {
        _array[i] = output[i];
    }
}
function Array_radixSort(_array)
{
    let m = array_getMax(_array)
    let exp = 1
    while (m / exp > 0)
    {
        array_countsort(_array, exp)
        exp = exp * 10;
    }
    PrintArray(_array);
    return -1; // Ignore this, just to hide false-positive errors.
}