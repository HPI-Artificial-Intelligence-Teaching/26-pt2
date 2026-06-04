# ==========================================================================
# mod_hash.jl - Demonstration of modular and multiplicative hashing
#
# 2026 by Ralf Herbrich
# Hasso Plattner Institut
# ==========================================================================

module ModHash

using Plots

include("ip_utils.jl")
using .IPUtils

"""
    key_distribution_plot(ips, hash_fct, M) -> Plot

Plot the distribution of hash keys for the given IP address tuples
using `hash_fct(key, M)` with table size `M`. An ideal uniform
distribution line is overlaid for comparison.
"""
function key_distribution_plot(ips, hash_fct, M)
    counts = zeros(M)
    for key in map(ip_to_int64, ips)
        counts[hash_fct(key, M)+1] += 1
    end

    p = bar(1:length(counts), counts, legend=false)
    plot!([1, M], [sum(counts) / M, sum(counts) / M], linewidth=3)
    ylabel!("Count")
    xlabel!("Hash Key")
    return p
end

"""
    mod_hash(key, M) -> Int64

Compute the modular hash `key mod M`.
"""
function mod_hash(key::Int64, M)::Int64
    return mod(key, M)
end

"""
    mult_hash(key, M) -> Int64

Compute the multiplicative hash of `key` with table size `M`
using the golden ratio constant `a = (√5 − 1) / 2`.
"""
function mult_hash(key::Int64, M)::Int64
    a = (sqrt(5) - 1) / 2
    return Int64(floor(M * (key * a - floor(key * a))))
end

"""
    mult_shift_hash(key, M) -> Int64

Compute the multiply-shift hash of `key` for table size `M`
(which must be a power of two) using Knuth's constant `2654435769`.
"""
function mult_shift_hash(key::Int64, M)::Int64
    a = 2654435769
    l = Int(log(M) / log(2))
    return (mod(key * a, 2^32)) >> (32 - l)
end

"""
    demo_hash(file_name)

Read IP addresses from `file_name` and interactively display hash key
distributions for modular, multiplicative, and multiply-shift hash
functions at various table sizes.
"""
function demo_hash(file_name)
    ips = read_ips(file_name)

    plts = [
        key_distribution_plot(ips, mod_hash, 96),
        key_distribution_plot(ips, mod_hash, 97),
        key_distribution_plot(ips, mod_hash, 98),
        key_distribution_plot(ips, mult_hash, 96),
        key_distribution_plot(ips, mult_hash, 97),
        key_distribution_plot(ips, mult_hash, 98),
        key_distribution_plot(ips, mult_hash, 4),
        key_distribution_plot(ips, mult_hash, 8),
        key_distribution_plot(ips, mult_hash, 16),
    ]

    for p in plts
        println("Enter for next. q to close.")
        display(p)
        var = readline()
        if var == "q"
            exit()
        end
    end
end

"""
    main()

Run the hashing demo on a CSV file of IP addresses.
Uses the bundled `geoip2-ipv4.csv` by default or an argument-provided path.
"""
function main()
    file_name = joinpath(@__DIR__, "geoip2-ipv4.csv")
    if length(ARGS) == 1
        file_name = ARGS[1]
    end
    demo_hash(file_name)
end

end # module ModHash

if abspath(PROGRAM_FILE) == @__FILE__
    ModHash.main()
end
