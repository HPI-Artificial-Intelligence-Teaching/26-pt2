# ==========================================================================
# ip_utils.jl - Shared utilities for reading and hashing IPv4 addresses
#
# 2026 by Ralf Herbrich
# Hasso Plattner Institut
# ==========================================================================

module IPUtils

export read_ips, ip_to_int64

"""
    read_ips(file_name) -> Vector{NTuple{4,String}}

Read IPv4 addresses from a CSV file. Expects a header row followed by lines
where the first comma-separated field contains an IP in CIDR notation
(e.g. `1.0.0.0/24`). Returns a vector of 4-tuples of octet strings.
"""
function read_ips(file_name)
    ips = Vector{NTuple{4,String}}()
    open(file_name, "r") do file
        readline(file)  # skip header
        for line in eachline(file)
            ip_field = first(split(line, ','))
            octets = split(first(split(ip_field, '/')), '.')
            if length(octets) == 4
                push!(ips, (octets[1], octets[2], octets[3], octets[4]))
            end
        end
    end
    return ips
end

"""
    ip_to_int64(ip; base=256) -> Int64

Convert a 4-tuple of octet strings to a single 64-bit integer
by treating the octets as digits in the given base.
"""
function ip_to_int64((ip1, ip2, ip3, ip4); base=256)::Int64
    key = parse(Int64, ip1)
    key = key * base + parse(Int64, ip2)
    key = key * base + parse(Int64, ip3)
    key = key * base + parse(Int64, ip4)
    return key
end

end # module IPUtils
