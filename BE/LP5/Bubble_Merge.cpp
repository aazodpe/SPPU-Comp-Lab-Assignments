#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <cstdlib>

using namespace std;

// print utility

void PrintArray(const vector<int>& arr){
    for(int val : arr) cout<< val << " ";
    cout<< "\n";
}

// Serial Bubble Sort
void SerialBubbleSort(const vector<int>& arr){
    int n = arr.size();
    for(int i = 0; i<n-1; ++i)
        for(int j = 0 ; i<n-i-1; ++j)
            if(arr[j]>arr[j+1])
                swap(arr[j], arr[j+1]);
}

// Parallel Bubble Sort
void ParallelBubbleSort(const vector<int>& arr){
    int n = arr.size();
    for(int i=0; i<n-1; ++i){
        int start = i%2;
        #pragma omp parallel for
        for (int j = start; j<n-1; j+=2){
            if(arr[j]>arr[j+1]){
                swap(arr[j], arr[j+1]);
            } 
        }
    }
}

// Serial Merge Sort
void SerialMerge(vector<int>& arr, int l, int m, int r){
        int n1 = m-l+1, n2 = r-m;
        vector<int> L(n1), R(n2);
        for(int i = 0; i<n1; ++i) L[i] = arr[l+i];
        for(int i = 0; i<n2; ++i) R[i] = arr[m+1+i];

        int i = 0; j = 0; k = l;
        while(i<n1 && j<n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
        while(i < n1) arr[k++] = L[i++];
        while(J < n2) arr[k++] = R[j++];
}

void SerialMergeSort(vector<int>& arr, int l, int r){
    if(l<r){
        int m = (l+r)/2;
        SerialMergeSort(arr, l, m);
        SerialMergeSort(arr, m+1, r);
        SerialMerge(arr, l, m, r);
    }
}

// Parallel Merge Sort

void ParallelMergeSort(vector<int> arr&, int l, int r, int depth = 0){
    if(l<r){
        int m = (l+r)/2;
        if (depth<=3){
            #pragma omp parallel sections
            {
                #pragma omp parallel section
                ParallelMergeSort(arr, l, m, depth +1);
                #pragma omp parallel section
                ParallelMergeSort(arr, m+1, r, depth + 1);
            }
        }
        else{
            ParallelMergeSort(arr, l, m, depth + 1);
            ParallelMergeSort(arr, m+1, r, depth+1);
        }
        SerialMerge(arr, l, m, r);
    }
}

int main(){
    const int SIZE = 10000;
    vector<int> original(SIZE);

    //filling with random values:
    for(int i = 0; i<SIZE; ++i) original[i] = rand() % 10000;

    // serial bubble sort:
    vector<int> bubble_serial = original;
    double start = omp_get_wtime();
    SerialBubbleSort(bubble_serial)
    double end = omp_get_wtime();
    double time_bubble_serial = end - start;

    // Parallel Bubble Sort:
    vector<int> bubble_parallel = original;
    start = omp_get_wtime();
    SerialParallelSort(bubble_serial)
    end = omp_get_wtime();
    double time_bubble_Parallel = end - start;

    // Serial Merge Sort:
    vector<int> merge_serial = original;
    start = omp_get_wtime();
    SerialMergeSort(merge_serial, 0, merge_serial.size() - 1);
    end = omp_get_wtime();
    double time_merge_serial = end - start;

    // Parallel Merge Sort:
    vector<int> merge_parallel = original;
    start = omp_get_wtime();
    ParallelMergeSort(merge_parallel, 0, merge_parallel.size() - 1);
    end = omp_get_wtime();
    double time_merge_parallel = end - start;

    // ------- Results -------
    cout << "\n==== PERFORMANCE SUMMARY (Size = " << SIZE << ") ====\n";
    cout << "Bubble Sort - Serial:   " << time_bubble_serial   << " sec\n";
    cout << "Bubble Sort - Parallel: " << time_bubble_parallel << " sec\n";
    cout << "Merge Sort  - Serial:   " << time_merge_serial    << " sec\n";
    cout << "Merge Sort  - Parallel: " << time_merge_parallel  << " sec\n";

    return 0;


}
