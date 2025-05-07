#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <omp.h>
#include <iomanip>

using namespace std;
using namespace chrono;

// -------------------- Generate Random Array --------------------
void generateArray(vector<int>& arr, int n) {
    srand(time(0));
    arr.clear();
    for (int i = 0; i < n; ++i)
        arr.push_back(rand() % 100000);
}

// -------------------- Sequential Operations --------------------
int serialSum(const vector<int>& arr) {
    int sum = 0;
    for (int val : arr)
        sum += val;
    return sum;
}

int serialMin(const vector<int>& arr) {
    int min_val = arr[0];
    for (int val : arr)
        if (val < min_val)
            min_val = val;
    return min_val;
}

int serialMax(const vector<int>& arr) {
    int max_val = arr[0];
    for (int val : arr)
        if (val > max_val)
            max_val = val;
    return max_val;
}

// -------------------- Parallel Reduction Operations --------------------
int parallelSum(const vector<int>& arr) {
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); ++i)
        sum += arr[i];
    return sum;
}

int parallelMin(const vector<int>& arr) {
    int min_val = arr[0];
    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < arr.size(); ++i)
        if (arr[i] < min_val)
            min_val = arr[i];
    return min_val;
}

int parallelMax(const vector<int>& arr) {
    int max_val = arr[0];
    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < arr.size(); ++i)
        if (arr[i] > max_val)
            max_val = arr[i];
    return max_val;
}

// -------------------- Compare Performance --------------------
void compareReduction(int size) {
    vector<int> arr;
    generateArray(arr, size);

    int sum_serial, sum_parallel, min_serial, min_parallel, max_serial, max_parallel;
    double avg_serial, avg_parallel;

    // --- SUM ---
    auto start = high_resolution_clock::now();
    sum_serial = serialSum(arr);
    auto stop = high_resolution_clock::now();
    auto time_serial_sum = duration_cast<milliseconds>(stop - start).count();

    start = high_resolution_clock::now();
    sum_parallel = parallelSum(arr);
    stop = high_resolution_clock::now();
    auto time_parallel_sum = duration_cast<milliseconds>(stop - start).count();

    // --- MIN ---
    start = high_resolution_clock::now();
    min_serial = serialMin(arr);
    stop = high_resolution_clock::now();
    auto time_serial_min = duration_cast<milliseconds>(stop - start).count();

    start = high_resolution_clock::now();
    min_parallel = parallelMin(arr);
    stop = high_resolution_clock::now();
    auto time_parallel_min = duration_cast<milliseconds>(stop - start).count();

    // --- MAX ---
    start = high_resolution_clock::now();
    max_serial = serialMax(arr);
    stop = high_resolution_clock::now();
    auto time_serial_max = duration_cast<milliseconds>(stop - start).count();

    start = high_resolution_clock::now();
    max_parallel = parallelMax(arr);
    stop = high_resolution_clock::now();
    auto time_parallel_max = duration_cast<milliseconds>(stop - start).count();

    // --- AVERAGE ---
    avg_serial = (double)sum_serial / size;
    avg_parallel = (double)sum_parallel / size;

    // --- PRINT RESULTS ---
    cout << "Array Size: " << size << "\n\n";
    cout << left << setw(12) << "Operation"
         << setw(15) << "Serial Time (ms)"
         << setw(18) << "Parallel Time (ms)"
         << setw(12) << "Speedup" << "\n";
    cout << string(55, '-') << "\n";

    auto show = [](const string& name, long t1, long t2) {
        double speedup = (t2 == 0) ? 0 : (double)t1 / t2;
        cout << left << setw(12) << name
             << setw(15) << t1
             << setw(18) << t2
             << setw(12) << fixed << setprecision(2) << speedup << "\n";
    };

    show("Sum", time_serial_sum, time_parallel_sum);
    show("Min", time_serial_min, time_parallel_min);
    show("Max", time_serial_max, time_parallel_max);

    cout << "\nAverage (Serial): " << avg_serial << "\n";
    cout << "Average (Parallel): " << avg_parallel << "\n";
}

// -------------------- Main --------------------
int main() {
    int size = 10000000;  // Increase to see clearer performance gains
    compareReduction(size);
    return 0;
}

// compile with: g++ -fopenmp Parallel_Reductions.cpp -o Parallel_Reductions_Obj
// run with: ./Parallel_Reductions_Obj
