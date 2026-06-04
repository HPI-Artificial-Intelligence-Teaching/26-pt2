# ==========================================================================
# bloom.jl - Bloom filter demonstration and analysis
#
# 2026 by Ralf Herbrich
# Hasso Plattner Institut
# ==========================================================================

module BloomFilter

using Plots
using Random

include("ip_utils.jl")
using .IPUtils

"""
    mult_hash(a, M) -> Function

Return a multiplicative hash function parameterized by multiplier `a`
that maps `Int64` keys to the range `[0, M)`.
"""
function mult_hash(a, M)
    return (key::Int64) -> Int64(floor(M * (key * a - floor(key * a))))
end

"""
    eval_bloom(ips, k, M)

Evaluate a Bloom filter with `k` independent multiplicative hash functions
and `M` bits on the given IP address tuples. Prints the observed false
positive rate (fraction of insertions where all `k` bits were already set).
"""
function eval_bloom(ips, k, M)
    hash_set = zeros(M)
    hash_fcts = map(a -> mult_hash(a, M), rand(Float64, (k, 1)))

    error_cnt = 0
    for key in map(ip_to_int64, ips)
        hash_keys = map(h -> h(key) + 1, hash_fcts)
        if all(hash_key -> hash_set[hash_key] == 1, hash_keys)
            error_cnt += 1
        end
        for hash_key in hash_keys
            hash_set[hash_key] = 1
        end
    end

    println("k=$k, M=$M: false positive rate = ", error_cnt / length(ips))
end

"""
    demo_bloom(file_name)

Read IP addresses from `file_name` and evaluate Bloom filters
with `k = 1, …, 10` hash functions at a load of `α = 0.1`.
"""
function demo_bloom(file_name)
    ips = read_ips(file_name)
    for k in 1:10
        eval_bloom(ips, k, 10 * length(ips))
    end
end

"""
    false_pos_rate(k, α) -> Float64

Compute the theoretical false positive rate of a Bloom filter
with `k` hash functions at load factor `α = n/M`.
"""
function false_pos_rate(k::Int64, α::Float64)
    return (1.0 - exp(-k * α))^k
end

"""
    bloom_plots()

Plot the Bloom filter false positive rate as a function of
the number of hash functions `J` (at fixed `α = 0.1`) and
as a function of the load factor `α` (at fixed `k = 10`).
"""
function bloom_plots()
    p = plot(1:20, map(k -> false_pos_rate(k, 0.1), 1:20), legend=false, linewidth=3)
    scatter!(1:20, map(k -> false_pos_rate(k, 0.1), 1:20))
    ylabel!("P(False Positive)")
    xlabel!("J")
    display(p)

    αs = range(0.0, 0.5, 100)
    p = plot(αs, map(α -> false_pos_rate(10, α), αs), legend=false, linewidth=3)
    scatter!(αs, map(α -> false_pos_rate(10, α), αs))
    ylabel!("P(False Positive)")
    xlabel!("α")
    display(p)
end

"""
    main()

Run the Bloom filter demo on a file (if provided as argument)
or plot theoretical false positive rates.
"""
function main()
    if length(ARGS) == 1
        demo_bloom(ARGS[1])
    else
        bloom_plots()
    end
end

end # module BloomFilter

if abspath(PROGRAM_FILE) == @__FILE__
    BloomFilter.main()
end
