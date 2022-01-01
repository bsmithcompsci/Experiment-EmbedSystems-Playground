using Native;
using System;

namespace ExampleCS
{
    public class MainClass : IEmbed
    {
        #region Searching
        public int Array_Search_Linear(int[] _array, int _needle)
        {
            for (int i = 0; i < _array.Length; ++i)
            {
                if (_array[i] == _needle)
                    return i;
            }
            return -1;
        }

        static int _Array_Search_Binary(ref int[] _array, int _needle, int _index, int _endex)
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
                return _Array_Search_Binary(ref _array, _needle, _index, mid - 1);
            return _Array_Search_Binary(ref _array, _needle, mid + 1, _endex);
        }
        public int Array_Search_Binary(int[] _array, int _needle)
        {
            return _Array_Search_Binary(ref _array, _needle, 0, _array.Length - 1);
        }
        public int Array_Search_Jump(int[] _array, int _needle)
        {
            int step = (int)Math.Round(Math.Sqrt(_array.Length));
            int prev = 0;
            while (_array[Math.Min(step, _array.Length) - 1] < _needle)
            {
                prev = step;
                step += (int)Math.Round(Math.Sqrt(_array.Length));
                if (prev >= _array.Length)
                    return -1;
            }
            while (_array[prev] < _needle)
            {
                prev++;
                if (prev == Math.Min(step, _array.Length))
                    return -1;
            }
            if (_array[prev] == _needle)
                return prev;

            // Consider the array to be unordered.
            return -1;
        }
        public int Array_Interpolation_Search(int[] _array, int _needle)
        {
            int lo = 0, hi = (_array.Length - 1);

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
        public int Array_Exponential_Search(int[] _array, int _needle)
        {
            if (_array[0] == _needle)
                return 0;
            int i = 1;
            while (i < _array.Length && _array[i] <= _needle)
                i *= 2;

            return _Array_Search_Binary(ref _array, _needle, i / 2, Math.Min(i, _array.Length - 1));
        }
        #endregion

        #region Sorting
        static void Swap<T>(ref T lhs, ref T rhs)
        {
            T temp = lhs;
            lhs = rhs;
            rhs = temp;
        }
        public void Array_selectionSort(int[] _array)
        {
            int i, j, min_idx;
            for (i = 0; i < _array.Length - 1; i++)
            {
                min_idx = i;
                for (j = i + 1; j < _array.Length; j++)
                {
                    if (_array[j] < _array[min_idx])
                        min_idx = j;
                }
                Swap(ref _array[min_idx], ref _array[i]);
            }
            Console.WriteLine(string.Join(",", _array));
        }
        public void Array_bubbleSort(int[] _array)
        {
            for (int i = 0; i < _array.Length - 1; i++)
            {
                for (int j = 0; j < _array.Length - i - 1; j++)
                {
                    if (_array[j] > _array[j + 1])
                        Swap(ref _array[j], ref _array[j + 1]);
                }
            }
            Console.WriteLine(string.Join(",", _array));
        }

        public void Array_insertionSort(int[] _array)
        {
            for (int i = 0; i < _array.Length; i++)
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
            Console.WriteLine(string.Join(",", _array));
        }
        
        static int Partition(ref int[] _array, int _low, int _high)
        {
            int pivot = _array[_high];
            int i = (_low - 1);
            for (int j = _low; j < _high; j++)
            {
                if (_array[j] < pivot)
                {
                    i++;
                    Swap(ref _array[i], ref _array[j]);
                }
            }
            Swap(ref _array[i + 1], ref _array[_high]);
            return (i + 1);
        }
        static void _Array_quickSort(ref int[] _array, int _low, int _high)
        {
            if (_low < _high)
            {
                int part = Partition(ref _array, _low, _high);
                _Array_quickSort(ref _array, _low, part - 1);
                _Array_quickSort(ref _array, part + 1, _high);
            }
        }

        public void Array_quickSort(int[] _array)
        {
            _Array_quickSort(ref _array, 0, _array.Length - 1);
            Console.WriteLine(string.Join(",", _array));
        }

        static int getMax(int[] _array)
        {
            int mx = _array[0];
            for (int i = 1; i < _array.Length; i++)
                if (_array[i] > mx)
                    mx = _array[i];
            return mx;
        }
        static void Array_countSort(ref int[] _array, int _exp, bool _shouldPrint = true)
        {
            int[] output = new int[_array.Length];
            int[] count = new int[10];

            for (int i = 0; i < _array.Length; i++)
                count[(_array[i] / _exp) % 10]++;
            for (int i = 1; i < 10; i++)
                count[i] += count[i - 1];

            for (int i = _array.Length - 1; i >= 0; i--)
            {
                output[count[(_array[i] / _exp) % 10] - 1] = _array[i];
                count[(_array[i] / _exp) % 10]--;
            }

            for (int i = 0; i < _array.Length; i++)
                _array[i] = output[i];

            if (_shouldPrint)
                Console.WriteLine(string.Join(",", _array));
        }

        public void Array_radixSort(int[] _array)
        {
            int m = getMax(_array);
            for (int exp = 1; m / exp > 0; exp *= 10)
                Array_countSort(ref _array, exp, false);

            Console.WriteLine(string.Join(",", _array));
        }

        #endregion
    }
}
