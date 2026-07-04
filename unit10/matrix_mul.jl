# ==========================================================================
# matrix_mul.jl - Optimal matrix chain multiplication via dynamic programming
#
# 2026 by Ralf Herbrich
# Hasso Plattner Institut
# ==========================================================================

module MatrixMultiplication

using Random

"""
    optimal_multiply(As) -> (m, s)

Compute the optimal multiplication order for a chain of matrices `As`
using dynamic programming. Returns:
- `m`: cost matrix where `m[i,j]` is the minimum number of scalar
  multiplications needed to compute `Aᵢ · Aᵢ₊₁ · … · Aⱼ`
- `s`: split matrix where `s[i,j]` records the optimal split index `k`
"""
function optimal_multiply(As::Vector{Matrix{Float64}})
    for i in 1:length(As)-1
        if size(As[i])[2] != size(As[i+1])[1]
            error("Matrix dimensions do not match")
        end
    end

    n = length(As)
    m = zeros(Int, n, n)
    s = zeros(Int, n, n)
    for l in 2:n
        for i in 1:n-l+1
            j = i + l - 1
            m[i, j] = typemax(Int)
            for k in i:j-1
                q = m[i, k] + m[k+1, j] + size(As[i])[1] * size(As[k])[2] * size(As[j])[2]
                if q < m[i, j]
                    m[i, j] = q
                    s[i, j] = k
                end
            end
        end
    end
    return m, s
end

"""
    reconstruct(s, i, j) -> String

Reconstruct a parenthesized expression for the optimal multiplication
order of matrices `Aᵢ` through `Aⱼ` from the split matrix `s`.
"""
function reconstruct(s::Matrix{Int}, i::Int, j::Int)
    if i == j
        return "A$i"
    else
        k = s[i, j]
        o1 = reconstruct(s, i, k)
        o2 = reconstruct(s, k + 1, j)
        return "($o1 * $o2)"
    end
end

"""
    main()

Demonstrate optimal matrix chain multiplication on two example chains.
"""
function main()
    As = [randn(10, 100), randn(100, 10), randn(10, 1000)]
    m, s = optimal_multiply(As)
    println("Optimal matrix multiplication = ", reconstruct(s, 1, length(As)))
    println("Optimal number of multiplications = ", m[1, length(As)])

    As = [randn(1, 10), randn(10, 1), randn(1, 10), randn(10, 1)]
    m, s = optimal_multiply(As)
    println("Optimal matrix multiplication = ", reconstruct(s, 1, length(As)))
    println("Optimal number of multiplications = ", m[1, length(As)])
end

end # module MatrixMultiplication

if abspath(PROGRAM_FILE) == @__FILE__
    MatrixMultiplication.main()
end
