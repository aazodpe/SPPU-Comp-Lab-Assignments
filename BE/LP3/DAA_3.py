# Function to solve the 0-1 Knapsack problem using dynamic programming
def knapsack_dynamic_programming(weights, values, capacity):
    n = len(values)  # Number of items

    # Create a 2D DP table to store the maximum value that can be achieved with a given capacity
    dp = [[0 for _ in range(capacity + 1)] for _ in range(n + 1)]

    # Build the DP table
    for i in range(1, n + 1):
        for w in range(1, capacity + 1):
            if weights[i-1] <= w:
                # Maximize value by including or excluding the current item
                dp[i][w] = max(dp[i-1][w], dp[i-1][w - weights[i-1]] + values[i-1])
            else:
                # Cannot include the current item, carry forward the previous value
                dp[i][w] = dp[i-1][w]

    # The last cell of dp table will contain the maximum value that can be achieved
    return dp[n][capacity], dp

# Function to trace back the selected items
def find_selected_items(dp, weights, capacity):
    n = len(weights)
    selected_items = []
    w = capacity

    # Trace back the items that were included
    for i in range(n, 0, -1):
        if dp[i][w] != dp[i-1][w]:
            selected_items.append(i-1)  # Item was included
            w -= weights[i-1]

    return selected_items

# Example usage
if __name__ == "__main__":
    weights = [2, 3, 4, 5]  # Weights of the items
    values = [3, 4, 5, 6]   # Values of the items
    capacity = 5            # Maximum capacity of the knapsack

    # Solve the 0-1 knapsack problem
    max_value, dp_table = knapsack_dynamic_programming(weights, values, capacity)

    # Find which items were selected
    selected_items = find_selected_items(dp_table, weights, capacity)

    # Output the results
    print(f"Maximum value that can be achieved: {max_value}")
    print(f"Selected item indices: {selected_items}")
    print(f"Selected item weights: {[weights[i] for i in selected_items]}")
    print(f"Selected item values: {[values[i] for i in selected_items]}")