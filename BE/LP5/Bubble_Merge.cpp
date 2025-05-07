#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <omp.h>
#include <algorithm>

using namespace std;
using namespace chrono;

// -------------------- RANDOM ARRAY GENERATOR --------------------
void generateArray(vector<int>& arr, int n) {
    srand(time(0));
    arr.clear();
    for (int i = 0; i < n; ++i)
        arr.push_back(rand() % 100000);
}

// -------------------- SERIAL BUBBLE SORT --------------------
void serialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// -------------------- PARALLEL BUBBLE SORT --------------------
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for
        for (int j = i % 2; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// -------------------- SERIAL MERGE SORT --------------------
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    vector<int> L(arr.begin() + l, arr.begin() + m + 1);
    vector<int> R(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

void serialMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        serialMergeSort(arr, l, m);
        serialMergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// -------------------- PARALLEL MERGE SORT --------------------
void parallelMergeSort(vector<int>& arr, int l, int r, int depth = 0) {
    if (l < r) {
        int m = l + (r - l) / 2;

        if (depth <= 4) {
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, l, m, depth + 1);

                #pragma omp section
                parallelMergeSort(arr, m + 1, r, depth + 1);
            }
        } else {
            serialMergeSort(arr, l, m);
            serialMergeSort(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

// -------------------- PERFORMANCE COMPARISON --------------------
void compareSorts(int size) {
    vector<int> arr, arr_copy;

    // Bubble Sort
    generateArray(arr, size);
    arr_copy = arr;
    auto start = high_resolution_clock::now();
    serialBubbleSort(arr);
    auto stop = high_resolution_clock::now();
    cout << "Serial Bubble Sort Time: " << duration_cast<milliseconds>(stop - start).count() << " ms\n";

    arr = arr_copy;
    start = high_resolution_clock::now();
    parallelBubbleSort(arr);
    stop = high_resolution_clock::now();
    cout << "Parallel Bubble Sort Time: " << duration_cast<milliseconds>(stop - start).count() << " ms\n";

    // Merge Sort
    generateArray(arr, size);
    arr_copy = arr;
    start = high_resolution_clock::now();
    serialMergeSort(arr, 0, arr.size() - 1);
    stop = high_resolution_clock::now();
    cout << "Serial Merge Sort Time: " << duration_cast<milliseconds>(stop - start).count() << " ms\n";

    arr = arr_copy;
    start = high_resolution_clock::now();
    parallelMergeSort(arr, 0, arr.size() - 1);
    stop = high_resolution_clock::now();
    cout << "Parallel Merge Sort Time: " << duration_cast<milliseconds>(stop - start).count() << " ms\n";
}

// -------------------- MAIN --------------------
int main() {
    int size = 10000; // Bubble sort: keep ≤10K for reasonable time
    cout << "Sorting " << size << " elements\n\n";
    compareSorts(size);
    return 0;
}

// Compile with: g++ -fopenmp Bubble_Merge.cpp -o Bubble_Merge_Obj
// Run with: ./Bubble_Merge_Obj
