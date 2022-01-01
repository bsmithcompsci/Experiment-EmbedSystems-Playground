// We aren't gonna pragma once, since this file is only used once for testing.

#pragma region Array
#pragma region Search
// O(n)
// Array Linear Search goes through each element in the given array, and proceeds to check each value for the needle.
// https://www.geeksforgeeks.org/linear-search/
static const int Array_Search_Linear(const int _array[], const int _arraySize, const int _needle)
{
	for (int i = 0; i < _arraySize; ++i)
	{
		if (_array[i] == _needle)
			return i;
	}
	return -1;
}
// O(logn)
// Array Binary search grabs three points, the index, endex, and the mid points; it will check each of those points for the needle.
// If the needle is lower than the mid point, then it will break a new block and look on the left of the last mid point;
// If the needle is higher than the mid point, then it will break a new block and look on the right of the last mid point.
// https://www.geeksforgeeks.org/binary-search/
static const int Array_Search_Binary(const int _array[], const int _arraySize, const int _needle, const int _index, const int _endex)
{
	// Prevent stack overflow edge-cases.
	if (_array[_index] == _needle)
		return _index;
	if (_array[_endex] == _needle)
		return _endex;

	// Make sure we are still within the array.
	if (_index >= _endex)
		return -1;

	// Recursively find the needle block.
	int mid = (_index + (_endex - 1)) / 2;
	if (_array[mid] == _needle)
		return mid;
	if (_array[mid] > _needle)
		return Array_Search_Binary(_array, _arraySize, _needle, _index, mid - 1);
	return Array_Search_Binary(_array, _arraySize, _needle, mid + 1, _endex);
}
// O((m/n) + (m - 1)) | m = sqrt(n)
// Array Jump Search will start at the zero index, and split the array into steps; then iterate until it hits the edge of the step, and check the needle.
// https://www.geeksforgeeks.org/jump-search/
static const int Array_Search_Jump(const int _array[], const int _arraySize, const int _needle)
{
	int step = (int)round(sqrt(_arraySize));
	int prev = 0;
	while (_array[(int)fmin(step, _arraySize) - 1] < _needle)
	{
		prev = step;
		step += (int)round(sqrt(_arraySize));
		if (prev >= _arraySize)
			return -1;
	}
	while (_array[prev] < _needle)
	{
		prev++;
		if (prev == (int)fmin(step, _arraySize))
			return -1;
	}
	if (_array[prev] == _needle)
		return prev;

	// Consider the array to be unordered.
	return -1;
}
static const int Array_Interpolation_Search(const int _array[], const int _arraySize, const int _needle)
{
	int lo = 0, hi = (_arraySize - 1);

	while (lo <= hi && _needle >= _array[lo] && _needle <= _array[hi])
	{
		if (lo == hi)
		{
			if (_array[lo] == _needle) return lo;
			return -1;
		}

		int pos = lo + (int)(((double)(hi - lo) / (_array[hi] - _array[lo])) * (_needle - _array[lo]));
		if (_array[pos] == _needle)
			return pos;
		if (_array[pos] < _needle)
			lo = pos + 1;
		else
			hi = pos - 1;
	}
	return -1;
}
static const int Array_Exponential_Search(const int _array[], const int _arraySize, const int _needle)
{
	if (_array[0] == _needle)
		return 0;
	int i = 1;
	while (i < _arraySize && _array[i] <= _needle)
		i *= 2;

	return Array_Search_Binary(_array, _arraySize, _needle, i / 2, (int)fmin(i, _arraySize - 1));
}
#pragma endregion // Search

#pragma region Sort
void Array_swap(int *_old, int *_new)
{
	int temp = *_old;
	*_old = *_new;
	*_new = temp;
}
void Array_selectionSort(int _array[], const int _arraySize)
{
	int i, j, min_idx;
	for (i = 0; i < _arraySize - 1; i++)
	{
		min_idx = i;
		for (j = i+1; j < _arraySize; j++)
		{
			if (_array[j] < _array[min_idx])
				min_idx = j;
		}

		Array_swap(&_array[min_idx], &_array[i]);
	}
}
void Array_bubbleSort(int _array[], const int _arraySize)
{
	for (size_t i = 0; i < _arraySize - 1; i++)
	{
		for (size_t j = 0; j < _arraySize-i-1; j++)
		{
			if (_array[j] > _array[j + 1])
				Array_swap(&_array[j], &_array[j + 1]);
		}
	}
}
void Array_insertionSort(int _array[], const int _arraySize)
{
	for (int i = 0; i < _arraySize; i++)
	{
		int value = _array[i];
		int j = i - 1;
		while (j >= 0 && _array[j] > value)
		{
			_array[j + 1] = _array[j];
			j = j - 1;
		}
		_array[j + 1] = value;
	}
}
const int Array_partition(int _array[], const int _low, const int _high)
{
	int pivot = _array[_high];
	int i = (_low - 1);
	for (int j = _low; j < _high; j++)
	{
		if (_array[j] < pivot)
		{
			i++;
			Array_swap(&_array[i], &_array[j]);
		}
	}
	Array_swap(&_array[i + 1], &_array[_high]);
	return (i + 1);
}
void Array_quickSort(int _array[], const int _low, const int _high)
{
	if (_low < _high)
	{
		const int part = Array_partition(_array, _low, _high);
		Array_quickSort(_array, _low, part - 1);
		Array_quickSort(_array, part + 1, _high);
	}
}
int Array_getMax(const int _array[], const int _arraySize)
{
	int mx = _array[0];
	for (int i = 1; i < _arraySize; i++)
		if (_array[i] > mx)
			mx = _array[i];
	return mx;
}
void Array_countSort(int _array[], const int _arraySize, const int _exp)
{
	int *output = new int[_arraySize];
	int i, count[10] = { 0 };

	for (i = 0; i < _arraySize; i++)
		count[(_array[i] / _exp) % 10]++;
	for (i = 1; i < 10; i++)
		count[i] += count[i - 1];

	for (i = _arraySize - 1; i >= 0; i--)
	{
		output[count[(_array[i] / _exp) % 10] - 1] = _array[i];
		count[(_array[i] / _exp) % 10]--;
	}

	for (i = 0; i < _arraySize; i++)
		_array[i] = output[i];

	delete[] output;
}
void Array_radixSort(int _array[], const int _arraySize)
{
	int m = Array_getMax(_array, _arraySize);
	for (int exp = 1; m / exp > 0; exp *= 10)
		Array_countSort(_array, _arraySize, exp);
}
#pragma endregion // Sort

#pragma endregion // Array
