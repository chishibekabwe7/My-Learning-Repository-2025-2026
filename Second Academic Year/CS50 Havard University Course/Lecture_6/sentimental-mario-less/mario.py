from cs50 import get_int

def main():
    # Prompt user for height between 1 and 8
    while True:
        height = get_int("Height: ")
        if 1 <= height <= 8:
            break

    # Print pyramid
    for i in range(height):
        spaces = height - i - 1
        hashes = i + 1
        print(" " * spaces + "#" * hashes)

if __name__ == "__main__":
    main()
