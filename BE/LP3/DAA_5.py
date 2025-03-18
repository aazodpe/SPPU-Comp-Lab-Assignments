import random
import time

# Function for the deterministic partition
def partition(arr, low, high):
    pivot = arr[high]  # Deterministic: Pivot is the last element
    i = low - 1        # Index of the smaller element

    for j in range(low, high):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]  # Swap elements

    arr[i+1], arr[high] = arr[high], arr[i+1]  # Place pivot in the correct position
    return i + 1

# Function for the randomized partition
def randomized_partition(arr, low, high):
    random_index = random.randint(low, high)  # Pick a random pivot
    arr[random_index], arr[high] = arr[high], arr[random_index]  # Swap with the last element
    return partition(arr, low, high)  # Use the deterministic partition

# Deterministic QuickSort
def quicksort_deterministic(arr, low, high):
    if low < high:
        pi = partition(arr, low, high)  # Partitioning index
        quicksort_deterministic(arr, low, pi - 1)  # Sort left part
        quicksort_deterministic(arr, pi + 1, high)  # Sort right part

# Randomized QuickSort
def quicksort_randomized(arr, low, high):
    if low < high:
        pi = randomized_partition(arr, low, high)  # Randomized partitioning index
        quicksort_randomized(arr, low, pi - 1)  # Sort left part
        quicksort_randomized(arr, pi + 1, high)  # Sort right part

# Utility function to generate a random array
def generate_random_array(size):
    return [random.randint(0, 10000) for _ in range(size)]

# Function to analyze and compare deterministic and randomized QuickSort
def analyze_quicksort():
    sizes = [1000, 5000, 10000, 20000]  # Different array sizes for testing

    for size in sizes:
        print(f"\nArray size: {size}")
        arr = generate_random_array(size)
        
        # Time deterministic QuickSort
        arr_copy = arr[:]
        start_time = time.time()
        quicksort_deterministic(arr_copy, 0, len(arr_copy) - 1)
        end_time = time.time()
        deterministic_time = end_time - start_time
        print(f"Deterministic QuickSort took: {deterministic_time:.6f} seconds")

        # Time randomized QuickSort
        arr_copy = arr[:]
        start_time = time.time()
        quicksort_randomized(arr_copy, 0, len(arr_copy) - 1)
        end_time = time.time()
        randomized_time = end_time - start_time
        print(f"Randomized QuickSort took: {randomized_time:.6f} seconds")

# Example usage
if __name__ == "__main__":
    analyze_quicksort()