# ==========================================================================
# birthday.jl - Visualization of the birthday paradox
#
# 2026 by Ralf Herbrich
# Hasso Plattner Institut
# ==========================================================================

module BirthdayParadox

using Plots

"""
    no_collision_prob(n, M) -> Float64

Compute the probability that `n` items drawn uniformly from `M` slots
produce no collision, using the recursive identity
`q(n, M) = (n+1)/M · q(n+1, M)` with base case `q(M+1, M) = 1`.
"""
function no_collision_prob(n, M)
    if n == M + 1
        return 1.0
    else
        return (n + 1) / M * no_collision_prob(n + 1, M)
    end
end

"""
    main()

Plot the probability of at least one birthday collision among `n` people
out of `M = 365` possible days, for `n = 1, …, 80`.
"""
function main()
    M = 365
    N = 80

    p = plot(
        1:N,
        n -> 1 - no_collision_prob(M - n, M),
        legend=false,
        linewidth=3,
    )
    scatter!(1:N, map(n -> 1 - no_collision_prob(M - n, M), 1:N))
    ylabel!("P(Collision|M=365)")
    xlabel!("n")
    display(p)
end

end # module BirthdayParadox

if abspath(PROGRAM_FILE) == @__FILE__
    BirthdayParadox.main()
end
