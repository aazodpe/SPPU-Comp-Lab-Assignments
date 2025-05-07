#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <omp.h>
#include <numeric>

using namepspace std;

// Serial Reduction Operations:

void SerialOps(const vector<int>& arr, int& minVal, int& maxVal, long long& sum, double& avg){
    minVal = INT_MAX;
    maxVal = INT_MIN;
    sum = 0;
    for(int val : arr){
        if (val<minVal) minVal = val;
        if (val>maxVal) maxVal = val;
        sum += val;
    }
    avg = static_cast<double>(sum)/arr.size();
}

// Parallel Reduction Operations

void ParallelOps(const vector<int>& arr, int& minVal, int&maxVal, long long& sum, double& avg){
    minVal = INT_MAX;
    maxVal = INT_MIN;
    sum = 0;
    #pragma omp parallel for reduction(min:localMin) reduction(max:localMax) reduction(+:localSum)
    for(int i = 0; i<arr.size(); ++i){
        if(arr[i] < localMin) localMin = arr[i];
        if(arr[i] > localMax) localMax = arr[i];
        localSum+=arr[i];
    }
    minVal = localMin;
    maxVal = localMax;
    sum = localSum;
    avg = static_cast<double>(sum) / arr.size();
}

int main(){
    const int SIZE = 10000000;
    vector<int> arr(SIZE);
    for(int i = 0; i<SIZE; ++i)
        arr[i] = rand() % 100000;
    
    int min_s, max_s;
    long long sum_s;
    double avg_s;

    int min_p, max_p;
    long long sum_p;
    double avg_p;

    // serial execution:

    double start = omp_get_wtime();
    SerialOps(arr, min_s, max_s, sum_s, avg_s);
    double time_serial = omp_get_wtime() - start;

    // ---------- Parallel Execution ----------
    start = omp_get_wtime();
    parallelOps(arr, min_p, max_p, sum_p, avg_p);
    double time_parallel = omp_get_wtime() - start;

    // ---------- Output ----------
    cout << "\n==== RESULTS ====\n";
    cout << "Serial   -> Min: " << min_s << ", Max: " << max_s << ", Sum: " << sum_s << ", Avg: " << avg_s << "\n";
    cout << "Parallel -> Min: " << min_p << ", Max: " << max_p << ", Sum: " << sum_p << ", Avg: " << avg_p << "\n";

    cout << "\n==== PERFORMANCE ====\n";
    cout << "Serial Time:   " << time_serial   << " sec\n";
    cout << "Parallel Time: " << time_parallel << " sec\n";

    return 0;


}