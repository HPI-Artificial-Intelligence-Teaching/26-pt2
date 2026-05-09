# ==========================================================================
# sorts.jl - Animated visualizations of sorting algorithms
#
# 2026 by Ralf Herbrich
# Hasso Plattner Institut
# ==========================================================================

module SortPlots

using Plots
using Random
using LaTeXStrings

"""
    plot_array(a; mark_idx=10) -> Plot

Plot an integer array `a` as a bar chart, highlighting the bar
at index `mark_idx` in red.
"""
function plot_array(a::Vector{Int64}; mark_idx=10)
    p = bar(
        a,
        label=false,
        xlabel="Index",
        ylabel="Value",
        color=ifelse.(1:length(a) .== mark_idx, :red, :blue),
        xtickfontsize=14,
        ytickfontsize=14,
        xguidefontsize=16,
        yguidefontsize=16,
    )
    return p
end

"""
    selection_sort_plot(a) -> Animation

Animate selection sort on array `a`, highlighting the current
minimum element at each step.
"""
function selection_sort_plot(a)
    anim = Animation()

    for i = 1:length(a)
        min = i
        for j = i+1:length(a)
            if a[j] < a[min]
                min = j
            end
        end
        p = plot_array(a, mark_idx=min)
        frame(anim, p)
        a[i], a[min] = a[min], a[i]
    end

    return anim
end

"""
    insertion_sort_plot(a) -> Animation

Animate insertion sort on array `a`, highlighting each element
as it is shifted into its correct position.
"""
function insertion_sort_plot(a)
    anim = Animation()

    for i = 1:length(a)
        for j = i:-1:2
            if a[j] < a[j-1]
                a[j], a[j-1] = a[j-1], a[j]
                p = plot_array(a, mark_idx=j - 1)
                frame(anim, p)
            else
                break
            end
        end
    end

    return anim
end

"""
    bubble_sort_plot(a) -> Animation

Animate bubble sort on array `a`, highlighting each swap
as the largest unsorted element bubbles up.
"""
function bubble_sort_plot(a)
    anim = Animation()

    for i = 1:length(a)
        for j = length(a):-1:(i+1)
            if a[j] < a[j-1]
                a[j], a[j-1] = a[j-1], a[j]
                p = plot_array(a, mark_idx=j - 1)
                frame(anim, p)
            end
        end
    end

    return anim
end

"""
    shell_sort_plot(a) -> Animation

Animate Shell sort on array `a` using the 3x+1 gap sequence,
highlighting each swap within a gap-sorted pass.
"""
function shell_sort_plot(a)
    anim = Animation()

    # 3x+1 increment sequence:  1, 4, 13, 40, 121, 364, 1093, ...
    h = 1
    while h < length(a) / 3
        h = 3 * h + 1
    end

    while h >= 1
        for i = h:length(a)
            for j = i:-h:(h+1)
                if a[j] < a[j-h]
                    a[j], a[j-h] = a[j-h], a[j]
                    p = plot_array(a, mark_idx=j - 1)
                    frame(anim, p)
                else
                    break
                end
            end
        end
        h ÷= 3
    end

    return anim
end

"""
    merge_sort_plot(a) -> Animation

Animate top-down (recursive) merge sort on array `a`,
highlighting the current merge position.
"""
function merge_sort_plot(a)
    anim = Animation()

    function merge(a, lo, mid, hi)
        aux = copy(a)
        i, j = lo, mid + 1
        for k = lo:hi
            if i > mid
                a[k] = aux[j]; j += 1
            elseif j > hi
                a[k] = aux[i]; i += 1
            elseif aux[j] < aux[i]
                a[k] = aux[j]; j += 1
            else
                a[k] = aux[i]; i += 1
            end
            p = plot_array(a, mark_idx=k)
            frame(anim, p)
        end
    end

    function sort(a, lo, hi)
        if hi <= lo
            return
        end
        mid = lo + (hi - lo) ÷ 2
        sort(a, lo, mid)
        sort(a, mid + 1, hi)
        merge(a, lo, mid, hi)
    end

    sort(a, 1, length(a))
    return anim
end

"""
    bottom_up_merge_sort_plot(a) -> Animation

Animate bottom-up (iterative) merge sort on array `a`,
highlighting the current merge position.
"""
function bottom_up_merge_sort_plot(a)
    anim = Animation()

    function merge(a, lo, mid, hi)
        aux = copy(a)
        i, j = lo, mid + 1
        for k = lo:hi
            if i > mid
                a[k] = aux[j]; j += 1
            elseif j > hi
                a[k] = aux[i]; i += 1
            elseif aux[j] < aux[i]
                a[k] = aux[j]; j += 1
            else
                a[k] = aux[i]; i += 1
            end
            p = plot_array(a, mark_idx=k)
            frame(anim, p)
        end
    end

    sz = length(a)
    len = 1
    while len < sz
        lo = 1
        while lo < sz - len
            merge(a, lo, lo + len - 1, min(lo + len + len - 1, sz))
            lo += len + len
        end
        len *= 2
    end

    return anim
end

"""
    quick_sort_plot(a) -> Animation

Animate standard two-way quick sort on array `a`,
highlighting the partition element and swap positions.
"""
function quick_sort_plot(a)
    anim = Animation()

    function partition(a, lo, hi)
        i, j = lo, hi + 1
        while true
            i += 1
            while a[i] < a[lo]
                i += 1
                if i == hi
                    break
                end
            end

            j -= 1
            while a[lo] < a[j]
                j -= 1
                if j == lo
                    break
                end
            end

            if i >= j
                break
            end
            a[i], a[j] = a[j], a[i]
            p = plot_array(a, mark_idx=i)
            frame(anim, p)
        end
        a[lo], a[j] = a[j], a[lo]
        p = plot_array(a, mark_idx=j)
        frame(anim, p)
        return j
    end

    function sort(a, lo, hi)
        if hi <= lo
            return
        end
        j = partition(a, lo, hi)
        sort(a, lo, j - 1)
        sort(a, j + 1, hi)
    end

    sort(a, 1, length(a))
    return anim
end

"""
    quick3_sort_plot(a) -> Animation

Animate three-way (Dutch National Flag) quick sort on array `a`,
which efficiently handles arrays with many duplicate keys.
"""
function quick3_sort_plot(a)
    anim = Animation()

    function sort3(a, lo, hi)
        if hi <= lo
            return
        end
        lt, gt = lo, hi
        v = a[lo]
        i = lo
        while i <= gt
            if a[i] < v
                a[lt], a[i] = a[i], a[lt]
                p = plot_array(a, mark_idx=i)
                frame(anim, p)
                lt += 1
                i += 1
            elseif a[i] > v
                a[i], a[gt] = a[gt], a[i]
                p = plot_array(a, mark_idx=i)
                frame(anim, p)
                gt -= 1
            else
                i += 1
            end
        end

        sort3(a, lo, lt - 1)
        sort3(a, gt + 1, hi)
    end

    sort3(a, 1, length(a))
    return anim
end

"""
    main()

Generate MP4 animations for all sorting algorithms on shuffled arrays.
"""
function main()
    Random.seed!(42)
    mp4(selection_sort_plot(shuffle(1:50)), "~/Downloads/selection_sort.mp4", fps=5, loop=0)

    Random.seed!(42)
    mp4(insertion_sort_plot(shuffle(1:50)), "~/Downloads/insertion_sort.mp4", fps=20, loop=0)

    Random.seed!(42)
    mp4(bubble_sort_plot(shuffle(1:50)), "~/Downloads/bubble_sort.mp4", fps=20, loop=0)

    Random.seed!(42)
    mp4(shell_sort_plot(shuffle(1:50)), "~/Downloads/shell_sort.mp4", fps=20, loop=0)

    Random.seed!(42)
    mp4(merge_sort_plot(shuffle(1:50)), "~/Downloads/merge_sort.mp4", fps=20, loop=0)

    Random.seed!(42)
    mp4(bottom_up_merge_sort_plot(shuffle(1:50)), "~/Downloads/bottom_up_merge_sort.mp4", fps=20, loop=0)

    Random.seed!(42)
    mp4(quick_sort_plot(shuffle(1:50)), "~/Downloads/quick_sort.mp4", fps=20, loop=0)

    Random.seed!(42)
    mp4(quick_sort_plot(shuffle(convert(Vector{Int}, vcat(ones(20), 2 * ones(20), 3 * ones(20))))), "~/Downloads/quick_sort_duplicate.mp4", fps=20, loop=0)

    Random.seed!(42)
    mp4(quick3_sort_plot(shuffle(convert(Vector{Int}, vcat(ones(20), 2 * ones(20), 3 * ones(20))))), "~/Downloads/quick3_sort_duplicate.mp4", fps=20, loop=0)
end

end # module SortPlots

if abspath(PROGRAM_FILE) == @__FILE__
    SortPlots.main()
end
