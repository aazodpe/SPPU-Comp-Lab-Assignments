# Function to check if placing a queen at board[row][col] is safe
def is_safe(board, row, col, n):
    # Check this column on upper side
    for i in range(row):
        if board[i][col] == 'Q':
            return False

    # Check upper diagonal on left side
    for i, j in zip(range(row, -1, -1), range(col, -1, -1)):
        if board[i][j] == 'Q':
            return False

    # Check upper diagonal on right side
    for i, j in zip(range(row, -1, -1), range(col, n)):
        if board[i][j] == 'Q':
            return False

    return True

# Function to print the solution
def print_solution(board, n):
    for row in board:
        print(" ".join(row))
    print()

# Function to solve the n-Queens problem using backtracking
def solve_nqueens(board, row, n):
    # If all queens are placed, print the solution
    if row == n:
        print_solution(board, n)
        return True

    # Try placing queens in all columns for the current row
    for col in range(n):
        if is_safe(board, row, col, n):
            # Place the queen at board[row][col]
            board[row][col] = 'Q'

            # Recursively place the rest of the queens
            if solve_nqueens(board, row + 1, n):
                return True

            # If placing the queen at board[row][col] doesn't lead to a solution
            # Backtrack by removing the queen
            board[row][col] = '.'

    return False

# Function to initialize the board with the first queen pre-placed
def place_first_queen(n, first_row, first_col):
    # Create an empty n x n board
    board = [['.' for _ in range(n)] for _ in range(n)]
    
    # Place the first queen at the given position
    board[first_row][first_col] = 'Q'

    return board

# Example usage
if __name__ == "__main__":
    n = 8  # Size of the board (n x n)
    first_row = 0  # Row where the first queen is pre-placed
    first_col = 0  # Column where the first queen is pre-placed

    # Initialize the board with the first queen placed
    board = place_first_queen(n, first_row, first_col)

    print("Initial board with the first queen placed:")
    print_solution(board, n)

    # Solve the n-Queens problem with the first queen placed
    if not solve_nqueens(board, first_row + 1, n):
        print("No solution exists for the given initial queen placement.")