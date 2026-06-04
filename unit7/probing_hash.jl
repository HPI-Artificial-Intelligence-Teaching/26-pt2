# ==========================================================================
# probing_hash.jl - Demonstration of hashing with linear probing
#
# 2026 by Ralf Herbrich
# Hasso Plattner Institut
# ==========================================================================

module ProbingHash

using Plots
using Colors

include("ip_utils.jl")
using .IPUtils

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
    key_distribution_plot(ips, hash_fct, M)

Insert IP addresses into a hash table of size `M` using linear probing
with `hash_fct`. Displays a heatmap of occupied slots and prints the
average number of probes per insertion.
"""
function key_distribution_plot(ips, hash_fct, M)
    y = Int64(ceil(sqrt(M / 2)))
    x = 2 * y
    M = x * y

    hash_set = zeros(M)
    probe_cnt = 0
    key_cnt = 0
    for key in map(ip_to_int64, ips)
        hash_key = hash_fct(key, M) + 1
        key_cnt += 1

        probe_cnt += 1
        while hash_set[hash_key] == 1
            probe_cnt += 1
            hash_key = mod(hash_key, M) + 1
        end

        hash_set[hash_key] = 1
    end

    p = heatmap(reshape(hash_set, y, x), color=(:greys))
    display(p)
    println("Average number of probes: ", probe_cnt / key_cnt)
end

"""
    demo_linear_probing_hash(file_name)

Read IP addresses from `file_name` and demonstrate clustering effects
of linear probing at load factor `α = 0.5` for both modular and
multiplicative hash functions.
"""
function demo_linear_probing_hash(file_name)
    ips = read_ips(file_name)
    M = length(ips) * 2    # ensures α = 0.5

    key_distribution_plot(ips, mod_hash, M)
    key_distribution_plot(ips, mult_hash, M)
end

"""
    main()

Run the linear probing demo on a CSV file of IP addresses.
Uses the bundled `geoip2-ipv4.csv` by default or an argument-provided path.
"""
function main()
    file_name = joinpath(@__DIR__, "geoip2-ipv4.csv")
    if length(ARGS) == 1
        file_name = ARGS[1]
    end
    demo_linear_probing_hash(file_name)
end

end # module ProbingHash

if abspath(PROGRAM_FILE) == @__FILE__
    ProbingHash.main()
end
