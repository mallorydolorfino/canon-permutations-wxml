def syt_1(n, k, arr, used, i, out):
    if i >= n * k:                       # every square filled
        out.append(list(arr))
        return 1

    r, c = i // k, i % k
    left  = 0 if c == 0 else arr[i - 1]  # square to the left
    above = 0 if r == 0 else arr[i - k]  # square above
    low  = max(left, above) + 1
    high = n * k - (n - r) * (k - c) + 1

    acc = 0
    for m in range(low, high + 1):
        if used[m - 1]:
            continue
        arr[i] = m
        used[m - 1] = 1
        acc += syt_1(n, k, arr, used, i + 1, out)
        used[m - 1] = 0                  # backtrack
    return acc


def all_syt(n, k):
    out = []
    syt_1(n, k, [0] * (n * k), [0] * (n * k), 0, out)
    return out


def canon_from(sigma, T, n, k):
    pi = [0] * (n * k)
    for i in range(n * k):
        pi[T[i] - 1] = sigma[i // k]     # row i//k is the number sigma[i//k]
    return pi


def canon_permutations(n, k):
    tableaux = all_syt(n, k)
    result = []
    for sigma in Permutations(n):
        sigma = list(sigma)
        for T in tableaux:
            result.append(canon_from(sigma, T, n, k))
    return result, tableaux

def is_canon(pi, n, k):
    seen = [0] * (n + 1)
    voices = [[] for _ in range(k)]
    for x in pi:
        voices[seen[x]].append(x)
        seen[x] += 1
    return all(v == voices[0] for v in voices)


def print_tableau(T, n, k):
    for r in range(n):
        print("  " + " ".join(f"{T[r * k + c]:3d}" for c in range(k)))


# ---------- Main ----------
k = int(input("Enter k (copies of each number): "))
n = int(input("Enter n (largest number): "))

canons, tableaux = canon_permutations(n, k)

print(f"\n{len(tableaux)} standard Young tableaux of shape {n} x {k}. First one:")
print_tableau(tableaux[0], n, k)

for pi in canons[:200]:
    print(" ".join(str(x) for x in pi))
if len(canons) > 200:
    print(f"... ({len(canons) - 200} more not shown)")

print(f"\nTotal count: {len(canons)}  (= {n}! * {len(tableaux)})")

assert all(is_canon(pi, n, k) for pi in canons), "Found a non-canon permutation!"
assert len(set(tuple(pi) for pi in canons)) == len(canons), "Found a duplicate!"

check = StandardTableaux([k] * n).cardinality()
print(f"Sage's SYT count: {check}  ->  {'Match!' if check == len(tableaux) else 'MISMATCH'}")