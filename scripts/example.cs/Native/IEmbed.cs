namespace Native
{
    /// <summary>
    /// This class allows Native Host (Cpp) to look for any classes that have the IEmbed interface and immediately start executing code.
    /// ---
    /// For now we are using this interface to contract the user to these functions that we are going to be looking for in Cpp;
    /// of course this would be completely different, if we were working on a different project.
    /// 
    /// Overall this is Cpp -> CSharp communication.
    /// </summary>
    public interface IEmbed 
    {
        int Array_Search_Linear(int[] _array, int _needle);
        int Array_Search_Binary(int[] _array, int _needle);
        int Array_Search_Jump(int[] _array, int _needle);
        int Array_Interpolation_Search(int[] _array, int _needle);
        int Array_Exponential_Search(int[] _array, int _needle);

        void Array_selectionSort(int[] _array);
        void Array_bubbleSort(int[] _array);
        void Array_insertionSort(int[] _array);
        void Array_quickSort(int[] _array);
        void Array_radixSort(int[] _array);
    }
}
