import heapq
from collections import defaultdict, Counter

# Node class for Huffman Tree
class HuffmanNode:
    def __init__(self, char, freq):
        self.char = char      # Character
        self.freq = freq      # Frequency of the character
        self.left = None      # Left child (HuffmanNode)
        self.right = None     # Right child (HuffmanNode)

    # Defining comparison operators for the priority queue
    def __lt__(self, other):
        return self.freq < other.freq

# Function to build the Huffman Tree
def build_huffman_tree(frequency_dict):
    # Create a priority queue (min-heap)
    heap = [HuffmanNode(char, freq) for char, freq in frequency_dict.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        # Remove the two nodes of lowest frequency
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)

        # Create a new internal node with these two nodes as children
        merged = HuffmanNode(None, left.freq + right.freq)
        merged.left = left
        merged.right = right

        # Add the new node to the heap
        heapq.heappush(heap, merged)

    # The remaining node is the root of the Huffman Tree
    return heap[0]

# Function to generate Huffman codes from the tree
def generate_huffman_codes(node, current_code="", codes=None):
    if codes is None:
        codes = {}

    # Base case: leaf node (contains a character)
    if node.char is not None:
        codes[node.char] = current_code
        return codes

    # Traverse left (assign "0")
    if node.left:
        generate_huffman_codes(node.left, current_code + "0", codes)

    # Traverse right (assign "1")
    if node.right:
        generate_huffman_codes(node.right, current_code + "1", codes)

    return codes

# Function to encode the input string using the Huffman codes
def huffman_encoding(data):
    if not data:
        return "", {}

    # Step 1: Calculate frequency of each character
    frequency_dict = Counter(data)

    # Step 2: Build Huffman Tree
    huffman_tree_root = build_huffman_tree(frequency_dict)

    # Step 3: Generate Huffman Codes
    huffman_codes = generate_huffman_codes(huffman_tree_root)

    # Step 4: Encode the input data
    encoded_data = ''.join([huffman_codes[char] for char in data])

    return encoded_data, huffman_codes

# Function to decode the encoded string
def huffman_decoding(encoded_data, huffman_codes):
    # Inverse the Huffman codes map (key = code, value = character)
    inverse_codes = {code: char for char, code in huffman_codes.items()}

    decoded_data = []
    current_code = ""

    for bit in encoded_data:
        current_code += bit
        if current_code in inverse_codes:
            decoded_data.append(inverse_codes[current_code])
            current_code = ""

    return ''.join(decoded_data)

# Example usage
if __name__ == "__main__":
    data = "huffman encoding algorithm"
    print(f"Original data: {data}")

    # Encode
    encoded_data, huffman_codes = huffman_encoding(data)
    print(f"Encoded data: {encoded_data}")
    print(f"Huffman Codes: {huffman_codes}")

    # Decode
    decoded_data = huffman_decoding(encoded_data, huffman_codes)
    print(f"Decoded data: {decoded_data}")