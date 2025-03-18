# Non-recursive Fibonacci function to return the sequence
def fibonacci_non_recursive(n):
    fib_sequence = []
    
    if n <= 0:
        return fib_sequence
    
    # Initialize first two numbers
    a, b = 0, 1
    fib_sequence.append(a)
    
    if n > 1:
        fib_sequence.append(b)
    
    for _ in range(2, n):
        a, b = b, a + b
        fib_sequence.append(b)
    
    return fib_sequence

# Example usage for non-recursive
n = 10
print(f"Non-recursive Fibonacci sequence for n={n}: {fibonacci_non_recursive(n)}")



# Helper recursive function to calculate Fibonacci and append to the sequence
def fibonacci_recursive_helper(n, fib_sequence):
    if n == 0:
        fib_sequence.append(0)
    elif n == 1:
        fib_sequence.append(1)
    else:
        fibonacci_recursive_helper(n - 1, fib_sequence)
        fibonacci_recursive_helper(n - 2, fib_sequence)
        fib_sequence.append(fib_sequence[-1] + fib_sequence[-2])

# Wrapper function to return the Fibonacci sequence
def fibonacci_recursive(n):
    fib_sequence = []
    
    if n <= 0:
        return fib_sequence
    
    # Initialize the first two numbers
    fib_sequence = [0, 1] if n > 1 else [0]
    
    # Call the recursive function starting from the 3rd element
    for i in range(2, n):
        fib_sequence.append(fib_sequence[-1] + fib_sequence[-2])
    
    return fib_sequence

# Example usage for recursive
n = 10
print(f"Recursive Fibonacci sequence for n={n}: {fibonacci_recursive(n)}")
