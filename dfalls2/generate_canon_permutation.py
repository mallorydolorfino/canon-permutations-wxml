import itertools

def generate_tableaux(n, k):
    valid_arrays = []
    matrix = [[0] * k for _ in range(n)]
    matrix[0][0] = 1
    used = {1}

    def fill_cell(cell_index):
        if cell_index == n * k:
            flat_array = [matrix[r][c] for r in range(n) for c in range(k)]
            valid_arrays.append(tuple(flat_array))
            return

        r = cell_index // k
        c = cell_index % k
        m = r + 1
        l = c + 1

        if m == 1 and l > 1:
            lower_bound = matrix[0][c - 1]
            upper_bound = n * k - n * (k - l + 1) + 1
        elif m > 1 and l == 1:
            lower_bound = matrix[r - 1][0]
            upper_bound = n * k - (n - m + 1) * k + 1
        else:
            lower_bound = max(matrix[r][c - 1], matrix[r - 1][c])
            upper_bound = n * k - (n - m + 1) * (k - l + 1) + 1

        for val in range(lower_bound + 1, upper_bound + 1):
            if val not in used:
                matrix[r][c] = val
                used.add(val)
                fill_cell(cell_index + 1)
                used.remove(val)
                matrix[r][c] = 0

    fill_cell(1)
    return valid_arrays


def array_to_canon_permutation(sigma_i, flat_array, n, k):
    permutation = [0] * (n * k)
    for r in range(n):
        for c in range(k):
            pos = flat_array[r * k + c]
            permutation[pos - 1] = sigma_i[r]
    return tuple(permutation)


def cannon_permutations(n, k):
    if n <= 0 or k <= 0:
        raise ValueError("n and k must be greater than 0.")

    counts = {}
    permutations_by_sigma = {}
    sn = list(itertools.permutations(range(1, n + 1)))

    for i, sigma_i in enumerate(sn, start=1):
        arrays = generate_tableaux(n, k)
        unique_arrays = list(dict.fromkeys(arrays))
        counts[i] = len(unique_arrays)
        permutations_by_sigma[i] = (
            sigma_i,
            [array_to_canon_permutation(sigma_i, arr, n, k) for arr in unique_arrays]
        )

    total_count = sum(counts.values())
    print(f"The total number of cannon permutations for {n} and {k} is {total_count}.")

    for i in range(1, len(sn) + 1):
        sigma_i, perms = permutations_by_sigma[i]
        print(f"These are the cannon permutations corresponding to {sigma_i}")
        for p in perms:
            print(p)

    return total_count