import sys
import random
import time

# Add the directory containing the .pyd file
sys.path.append(r'C:/Users/Chris/source/repos/parseintpy/build/Release')

from parseint import parse_integers

def generate_random_values(num_values=100000):
    values = []
    for _ in range(num_values):
        # Randomly decide whether to create an integer, decimal, or exponential string
        rand_choice = random.choice(['int', 'decimal', 'exponential'])
        if rand_choice == 'int':
            # Generate a random integer as a string
            values.append(str(random.randint(0, 10**6)))
        elif rand_choice == 'decimal':
            # Generate a random decimal as a string
            values.append(f"{random.randint(0, 10**6)}.{random.randint(0, 99)}")
        else:
            # Generate a random number in scientific notation as a string
            base = random.randint(1, 10**6)
            exponent = random.randint(-5, 5)
            values.append(f"{base}e{exponent}")
    return values

def validate_parsed_values(parsed_values, original_values):
    errors = []  # List to store validation errors
    for parsed, original in zip(parsed_values, original_values):
        try:
            expected = int(float(original))  # Use float to handle decimals and exponentials
            if parsed != expected:
                errors.append(f"Parsed value {parsed} does not match expected value for input '{original}'")
        except ValueError as e:
            errors.append(f"Error parsing '{original}': {e}")
    return errors

def benchmark_parse_functions(num_tests=100000):
    test_values = generate_random_values(num_tests)

    # Parse with C++ function
    start_parseint = time.perf_counter()
    parsed_integers = parse_integers(test_values)  # Call the batch processing function
    end_parseint = time.perf_counter()
    parseint_duration = end_parseint - start_parseint
    print(f"parse_integers took {parseint_duration:.6f} seconds for {num_tests} parses.")

    # Validate C++ parsed integers
    parseint_errors = validate_parsed_values(parsed_integers, test_values)
    if parseint_errors:
        print("Validation failed for C++ parsed integers:")
        for error in parseint_errors:
            print(f"  - {error}")
    else:
        print("All C++ parsed integers validated successfully.")

    # Parse with Python's built-in int
    start_builtin = time.perf_counter()
    builtin_parsed = []
    for val in test_values:
        try:
            builtin_parsed.append(int(float(val)))  # Use float to handle decimals and exponentials
        except ValueError:
            builtin_parsed.append(None)  # Mark failed parses
    end_builtin = time.perf_counter()
    builtin_duration = end_builtin - start_builtin
    print(f"int() took {builtin_duration:.6f} seconds for {num_tests} parses.")

    # Validate Python parsed integers
    builtin_errors = validate_parsed_values(builtin_parsed, test_values)
    if builtin_errors:
        print("Validation failed for Python parsed integers:")
        for error in builtin_errors:
            print(f"  - {error}")
    else:
        print("All Python parsed integers validated successfully.")

# Run the benchmark
if __name__ == "__main__":
    benchmark_parse_functions()
