#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

const int INSERTION_SORT_THRESHOLD = 16;

// ������� ������� ����������
int partition(vector<int>& arr, int low, int high) 
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) 
    {
        if (arr[j] < pivot) 
        {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) 
{
    if (low < high) 
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ������� ��� ����������� �������� "����" � ������ � �������
void heapify(vector<int>& arr, int n, int i) 
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) 
    {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) 
    {
        largest = right;
    }

    if (largest != i) 
    {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// ������� ���������� �����
void heapSort(vector<int>& arr) 
{
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--) 
    {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--) 
    {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ���������� IntroSort
void introSort(vector<int>& arr, int low, int high, int depthLimit) 
{
    if (low < high) 
    {
        if (depthLimit == 0) 
        {
            heapSort(arr);
        }
        else 
        {
            int pi = partition(arr, low, high);
            introSort(arr, low, pi - 1, depthLimit - 1);
            introSort(arr, pi + 1, high, depthLimit - 1);
        }
    }
}

void introSortHelper(vector<int>& arr) 
{
    int depthLimit = 2 * log2(arr.size());
    introSort(arr, 0, arr.size() - 1, depthLimit);
}

// ��������� ��� �������� ��� ���������

// ��������� ������� � ������������� ����
vector<int> generateSortedArray(int size) 
{
    vector<int> arr(size);
    for (int i = 0; i < size; i++) 
    {
        arr[i] = i;
    }
    return arr;
}

// ��������� ������� ����� � �������������� (� �������� ����������� �������)
vector<int> generateAlmostSortedArray(int size, int swapCount) 
{
    vector<int> arr = generateSortedArray(size);
    random_device rd;
    mt19937 g(rd());

    for (int i = 0; i < swapCount; i++) 
    {
        int idx1 = g() % size;
        int idx2 = g() % size;
        swap(arr[idx1], arr[idx2]);
    }

    return arr;
}

// ��������� ������� � ������� ������������� ����
vector<int> generateReverseSortedArray(int size) 
{
    vector<int> arr = generateSortedArray(size);
    reverse(arr.begin(), arr.end());
    return arr;
}

// ��������� ������� ����������
template<typename SortFunction>
long long measureSortingTime(SortFunction sortFunc, vector<int>& arr) 
{
    auto start = chrono::high_resolution_clock::now();
    sortFunc(arr);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

int main() 
{
    int size = 1000; // ������� �������� ��� �������������
    int swapCount = 10; // ���������� ������� ��� ����� ��������������� ��������

    vector<int> sortedArray = generateSortedArray(size);
    vector<int> almostSortedArray = generateAlmostSortedArray(size, swapCount);
    vector<int> reverseSortedArray = generateReverseSortedArray(size);
    vector<int> randomArray(size);

    // ��������� ���������� �������
    random_device rd;
    mt19937 g(rd());
    shuffle(sortedArray.begin(), sortedArray.end(), g);
    randomArray = sortedArray;

    long long timeIntroSortSorted = measureSortingTime(introSortHelper, sortedArray);
    long long timeIntroSortAlmostSorted = measureSortingTime(introSortHelper, almostSortedArray);
    long long timeIntroSortReverseSorted = measureSortingTime(introSortHelper, reverseSortedArray);
    long long timeIntroSortRandom = measureSortingTime(introSortHelper, randomArray);

    cout << "Size: " << size << "\n";
    cout << "IntroSort (Sorted): " << timeIntroSortSorted << " microseconds\n";
    cout << "IntroSort (Almost Sorted): " << timeIntroSortAlmostSorted << " microseconds\n";
    cout << "IntroSort (Reverse Sorted): " << timeIntroSortReverseSorted << " microseconds\n";
    cout << "IntroSort (Random): " << timeIntroSortRandom << " microseconds\n";

    return 0;
}
