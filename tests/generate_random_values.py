import random

# Number of entries in the array
num_entries = 100000

# Lower and upper bounds for the random values
lower_bound = 0
upper_bound = 100000

# Generating the array with random values
arr = [random.randint(lower_bound, upper_bound) for _ in range(num_entries)]

# Optionally, to print the array
for index, value in enumerate(arr):
    print(f'\tarr[{index}] = {value};')
