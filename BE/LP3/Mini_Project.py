import threading
import time
import numpy as np

# Function for single-threaded matrix multiplication
def matrix_multiplication_single_thread(A, B):
    rows_A, cols_A = len(A), len(A[0])
    rows_B, cols_B = len(B), len(B[0])

    # Initialize result matrix with zeroes
    result = [[0 for _ in range(cols_B)] for _ in range(rows_A)]

    # Perform matrix multiplication
    for i in range(rows_A):
        for j in range(cols_B):
            for k in range(cols_A):
                result[i][j] += A[i][k] * B[k][j]
    
    return result

# Multithreading: One thread per row
def matrix_multiplication_per_row(A, B, result, row):
    cols_B = len(B[0])
    cols_A = len(A[0])

    for j in range(cols_B):
        result[row][j] = 0
        for k in range(cols_A):
            result[row][j] += A[row][k] * B[k][j]

# Multithreading: One thread per cell
def matrix_multiplication_per_cell(A, B, result, i, j):
    cols_A = len(A[0])
    result[i][j] = 0
    for k in range(cols_A):
        result[i][j] += A[i][k] * B[k][j]

# Function to analyze and compare the performance of different methods
def analyze_matrix_multiplication(A, B):
    # Single-threaded matrix multiplication
    start_time = time.time()
    result_single_thread = matrix_multiplication_single_thread(A, B)
    end_time = time.time()
    single_thread_time = end_time - start_time
    print(f"Single-threaded execution time: {single_thread_time:.6f} seconds")

    # Multithreading: One thread per row
    rows_A = len(A)
    cols_B = len(B[0])
    result_per_row = [[0 for _ in range(cols_B)] for _ in range(rows_A)]
    threads = []

    start_time = time.time()
    for i in range(rows_A):
        thread = threading.Thread(target=matrix_multiplication_per_row, args=(A, B, result_per_row, i))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    end_time = time.time()
    per_row_time = end_time - start_time
    print(f"Multithreaded (one thread per row) execution time: {per_row_time:.6f} seconds")

    # Multithreading: One thread per cell
    result_per_cell = [[0 for _ in range(cols_B)] for _ in range(rows_A)]
    threads = []

    start_time = time.time()
    for i in range(rows_A):
        for j in range(cols_B):
            thread = threading.Thread(target=matrix_multiplication_per_cell, args=(A, B, result_per_cell, i, j))
            threads.append(thread)
            thread.start()

    for thread in threads:
        thread.join()

    end_time = time.time()
    per_cell_time = end_time - start_time
    print(f"Multithreaded (one thread per cell) execution time: {per_cell_time:.6f} seconds")

    # Compare results
    print(f"Speedup (one thread per row): {single_thread_time / per_row_time:.2f}x")
    print(f"Speedup (one thread per cell): {single_thread_time / per_cell_time:.2f}x")

# Example usage
if __name__ == "__main__":
    # Define the matrix dimensions
    rows_A, cols_A = 100, 100  # Matrix A is 100x100
    rows_B, cols_B = 100, 100  # Matrix B is 100x100

    # Initialize matrices A and B with random values
    A = np.random.randint(1, 10, (rows_A, cols_A)).tolist()
    B = np.random.randint(1, 10, (rows_B, cols_B)).tolist()

    # Analyze and compare the performance of different matrix multiplication methods
    analyze_matrix_multiplication(A, B)