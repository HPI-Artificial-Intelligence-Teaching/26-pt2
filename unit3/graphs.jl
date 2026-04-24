# ==========================================================================
# graphs.jl - Growth function and complexity class visualizations
#
# 2026 by Ralf Herbrich
# Hasso Plattner Institut
# ==========================================================================

module GrowthPlots

using Plots
using LaTeXStrings

"""
    plot_growth(fs, labels)

Plot multiple growth functions `fs` with corresponding `labels` over the
range 1K–4K, with highlighted sample points at 1K, 2K, and 4K.
"""
function plot_growth(
    fs=[n -> 128 * n, n -> 64 * n * log2(n), n -> n^2],
    labels=[L"128 \cdot n", L"64 \cdot n \cdot \log_2(n)", L"n^2"],
)
    size = range(start=1000, stop=4000, length=300)
    size2 = [1000, 2000, 4000]

    p = plot(
        size,
        fs[1],
        linewidth=2,
        label=labels[1],
        xformatter=x -> "$(x/1000)K",
        yformatter=y -> "$(y/1000000)M",
        xlabel=L"n",
        ylabel="Laufzeit",
        legendfontsize=12,
        xtickfontsize=14,
        ytickfontsize=14,
        xguidefontsize=16,
        yguidefontsize=16,
    )
    for i = 2:length(fs)
        plot!(size, fs[i], label=labels[i], linewidth=2)
    end

    for i = 1:length(fs)
        scatter!(size2, fs[i], label=false, markersize=6, markercolor=:black)
    end

    display(p)
end

"""
    plot_sum3_runtime(; xs, ys, log_scale)

Plot empirical 3-sum runtimes and a power-law regression line.
`xs` and `ys` are the input sizes and measured times; `log_scale`
selects linear (`:identity`) or logarithmic (`:log10`) axes.
"""
function plot_sum3_runtime(;
    xs=[1000, 2000, 4000, 8000],
    ys=[0.02104, 0.107, 0.6583, 4.904],
    log_scale=:normal,
)
    log2xs = log2.(xs)
    log2ys = log2.(ys)

    # linear regression in log-space to estimate the power-law exponent
    slope =
        (length(log2xs) * sum(log2xs .* log2ys) - sum(log2xs) * sum(log2ys)) /
        (length(log2xs) * sum(log2xs .^ 2) - sum(log2xs)^2)
    intercept = (sum(log2ys) - slope * sum(log2xs)) / length(log2xs)
    println("slope: $slope, intercept: $intercept")

    p = plot(
        range(start=minimum(xs), stop=maximum(xs), length=300),
        n -> 2^(intercept) * n^slope,
        label=false,
        linewidth=3,
        xlabel=L"n",
        ylabel=L"T(n)",
        yscale=log_scale,
        xscale=log_scale,
        xtickfontsize=14,
        ytickfontsize=14,
        xguidefontsize=16,
        yguidefontsize=16,
    )

    scatter!(
        xs,
        ys,
        yscale=log_scale,
        xscale=log_scale,
        label=false,
        markersize=6,
        markercolor=:black,
    )

    display(p)
end

"""
    plot_order_of_growth(; f, g, min_n, max_n, ratio)

Plot a function `g(n)` against its leading-term approximation `f(n)`.
When `ratio=true`, plot `f(n)/g(n)` instead to show convergence to 1.
"""
function plot_order_of_growth(;
    f=n -> 1 / 6 * n^3,
    g=n -> 1 / 6 * n^3 - 1 / 2 * n^2 + 1 / 3 * n,
    min_n=10,
    max_n=50,
    ratio=false,
)
    ns = range(start=min_n, stop=max_n, length=300)

    if ratio
        p = plot(
            ns,
            n -> f(n) / g(n),
            linewidth=3,
            label=false,
            xlabel=L"n",
            ylabel=L"f(n)/g(n)",
            legendfontsize=12,
            xtickfontsize=14,
            ytickfontsize=14,
            xguidefontsize=16,
            yguidefontsize=16,
        )

        display(p)
    else
        p = plot(
            ns,
            f,
            linewidth=3,
            label=L"f(n)",
            xlabel=L"n",
            ylabel="Laufzeit",
            legendfontsize=12,
            xtickfontsize=14,
            ytickfontsize=14,
            xguidefontsize=16,
            yguidefontsize=16,
        )
        plot!(ns, g, linewidth=3, label=L"g(n)")

        display(p)
    end
end

"""
    plot_growth_classes(; ns, fs, labels, min_n, max_n)

Plot standard complexity classes (constant through exponential)
on a log₂–log₂ scale to illustrate their relative growth rates.
"""
function plot_growth_classes(;
    ns=[
        range(1, 2^48, 300),
        2.0 .^ (0:48),
        range(1, 2^48, 300),
        2.0 .^ (0:48),
        range(1, sqrt(2^48), 300),
        range(1, (2^48)^(1 / 3), 300),
        range(1, 48, 48),
    ],
    fs=[n -> 1, n -> log2(n), n -> n, n -> n * log2(n), n -> n^2, n -> n^3, n -> 2^n],
    labels=[
        "konstant",
        "logarithmisch",
        "linear",
        "linearithmisch",
        "quadratisch",
        "kubisch",
        "exponentiell",
    ],
    min_n=1,
    max_n=2^48,
)
    p = plot(
        ns[1],
        fs[1],
        linewidth=3,
        label=labels[1],
        legend=:bottomright,
        xlabel=L"n",
        ylabel=L"\Theta(n)",
        xscale=:log2,
        yscale=:log2,
        legendfontsize=12,
        xtickfontsize=14,
        ytickfontsize=14,
        xguidefontsize=16,
        yguidefontsize=16,
    )

    for i = 2:length(fs)
        plot!(ns[i], fs[i], linewidth=3, label=labels[i])
    end

    xlims!(min_n, max_n)
    ylims!(min_n, max_n)

    display(p)
end

"""
    main()

Generate all growth and complexity plots and save them as SVGs.
"""
function main()
    plot_growth()
    savefig("~/Downloads/growth.svg")
    plot_sum3_runtime(log_scale=:identity)
    savefig("~/Downloads/sum3_growth.svg")
    plot_sum3_runtime(log_scale=:log10)
    savefig("~/Downloads/sum3_growth_log.svg")
    plot_order_of_growth(ratio=false)
    savefig("~/Downloads/order_of_growth.svg")
    plot_order_of_growth(ratio=true)
    savefig("~/Downloads/order_of_growth_ratio.svg")
    plot_growth_classes()
    savefig("~/Downloads/growth_classes.svg")
end

end # module GrowthPlots

if abspath(PROGRAM_FILE) == @__FILE__
    GrowthPlots.main()
end
