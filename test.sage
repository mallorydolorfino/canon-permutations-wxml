# Basic SageMath test

print("factor(2026) =", factor(2026))

# Symmetric group S4
G = SymmetricGroup(4)
print("Order of S4:", G.order())

# A permutation in cycle notation
p = G("(1,2,3)(4)")
print("p =", p, "| order:", p.order(), "| sign:", p.sign())

# All permutations of [1,2,3]
P = Permutations(3)
print("Permutations of 3:", P.list())

# Symbolic calculus
x = var('x')
print("d/dx sin(x)^2 =", diff(sin(x)^2, x))