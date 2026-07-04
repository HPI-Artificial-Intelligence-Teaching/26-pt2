# ==========================================================================
# astar.jl - A* pathfinding algorithm demonstration on mazes
#
# 2026 by Ralf Herbrich
# Hasso Plattner Institut
# ==========================================================================

module AStarDemo

using Plots
using Mazes
using DataStructures
using Random

"""
    plot_maze(maze; start, goal, active, markersize, visited_list, path,
              wall_color, start_color, goal_color, active_color,
              visited_color, path_color) -> Plot

Render a maze with optional overlays for visited cells, the currently
active cell, and the current path from start to goal.
"""
function plot_maze(maze;
    start=(1, 1),
    goal=(maze.r, maze.c),
    active=nothing,
    markersize=10,
    visited_list=[],
    path=[],
    wall_color="black",
    start_color="green",
    goal_color="blue",
    active_color="orange",
    visited_color="yellow",
    path_color="red",
)
    p = plot(legend=false, aspect_ratio=:equal, axis=([], false))

    # plot the visited cells
    for (r, c) in visited_list
        plot!(Shape([c, c, c + 1, c + 1], [maze.r - r, maze.r - r - 1, maze.r - r - 1, maze.r - r]), color=visited_color, linewidth=0)
    end

    # plot the active cell
    if !isnothing(active)
        r, c = active
        plot!(Shape([c, c, c + 1, c + 1], [maze.r - r, maze.r - r - 1, maze.r - r - 1, maze.r - r]), color=active_color, linewidth=0)
    end

    # plot the path
    if !isempty(path)
        current = (0, 0)
        for (r, c) in path
            if current != (0, 0)
                plot!([current[2] + 0.5, c + 0.5], [maze.r - current[1] - 0.5, maze.r - r - 0.5], linewidth=3, color=path_color)
            end
            current = (r, c)
        end
    end

    # plot start and goal markers
    if !isnothing(start)
        plot!([start[2] + 0.5], [maze.r - start[1] - 0.5], marker=:circle, markersize=markersize, color=start_color)
    end
    if !isnothing(goal)
        plot!([goal[2] + 0.5], [maze.r - goal[1] - 0.5], marker=:circle, markersize=markersize, color=goal_color)
    end

    # plot the maze walls
    for r = 1:maze.r
        for c = 1:maze.c
            if r == 1
                plot!([c, c + 1], [maze.r - 1, maze.r - 1], color=wall_color, linewidth=2)
            end
            if c == 1
                plot!([c, c], [maze.r - r, maze.r - r - 1], color=wall_color, linewidth=2)
            end
            if !(((r, c), (r, c + 1)) in maze.T.E)
                plot!([c + 1, c + 1], [maze.r - r, maze.r - r - 1], color=wall_color, linewidth=2)
            end
            if !(((r, c), (r + 1, c)) in maze.T.E)
                plot!([c, c + 1], [maze.r - r - 1, maze.r - r - 1], color=wall_color, linewidth=2)
            end
        end
    end

    return p
end

"""
    manhattan(p1, p2) -> Int

Manhattan distance heuristic: `|x₁ − x₂| + |y₁ − y₂|`.
"""
manhattan((x1, y1), (x2, y2)) = abs(x1 - x2) + abs(y1 - y2)

"""
    euclidean(p1, p2) -> Float64

Euclidean distance heuristic: `√((x₁ − x₂)² + (y₁ − y₂)²)`.
"""
euclidean((x1, y1), (x2, y2)) = sqrt((x1 - x2)^2 + (y1 - y2)^2)

"""
    no_heuristic(p1, p2) -> Int

Trivial heuristic that always returns 0, reducing A* to Dijkstra's algorithm.
"""
no_heuristic((x1, y1), (x2, y2)) = 0

"""
    astar(maze; start, goal, heuristic) -> Vector{Tuple{Int,Int}} or nothing

Find the shortest path in `maze` from `start` to `goal` using A*
with the given `heuristic` function. Returns the path as a vector
of `(row, col)` tuples, or `nothing` if no path exists.
"""
function astar(maze; start=(1, 1), goal=(maze.r, maze.c), heuristic=manhattan)
    neighbors(node) = maze.T.N[node]

    open_list = PriorityQueue{Tuple{Int,Int},Int}()
    dist_from_start = Dict{Tuple{Int,Int},Int}()
    came_from = Dict{Tuple{Int,Int},Tuple{Int,Int}}()

    for node in maze.T.V
        dist_from_start[node] = typemax(Int)
    end
    dist_from_start[start] = 0
    open_list[start] = dist_from_start[start] + heuristic(start, goal)

    while !isempty(open_list)
        current = dequeue!(open_list)

        if current == goal
            path = Vector{Tuple{Int,Int}}()
            while current != start
                push!(path, current)
                current = came_from[current]
            end
            push!(path, current)
            return reverse(path)
        end

        for neighbor in neighbors(current)
            if dist_from_start[current] + 1 < dist_from_start[neighbor]
                came_from[neighbor] = current
                dist_from_start[neighbor] = dist_from_start[current] + 1
                open_list[neighbor] = dist_from_start[neighbor] + heuristic(neighbor, goal)
            end
        end
    end

    return nothing
end

"""
    astar_movie(maze; start, goal, heuristic) -> Animation

Create an animated visualization of the A* algorithm exploring
`maze` from `start` to `goal`, showing visited cells, the current
path, and the active node at each step.
"""
function astar_movie(maze; start=(1, 1), goal=(maze.r, maze.c), heuristic=manhattan)
    anim = Animation()
    visited = Set{Tuple{Int,Int}}()
    neighbors(node) = maze.T.N[node]

    open_list = PriorityQueue{Tuple{Int,Int},Float64}()
    dist_from_start = Dict{Tuple{Int,Int},Int}()
    came_from = Dict{Tuple{Int,Int},Tuple{Int,Int}}()

    for node in maze.T.V
        dist_from_start[node] = typemax(Int)
    end
    dist_from_start[start] = 0
    open_list[start] = dist_from_start[start] + heuristic(start, goal)

    while !isempty(open_list)
        current = dequeue!(open_list)

        # reconstruct the shortest path from start to current
        path = Vector{Tuple{Int,Int}}()
        node = current
        while node != start
            push!(path, node)
            node = came_from[node]
        end
        push!(path, node)

        p = plot_maze(maze, visited_list=collect(visited), active=current, markersize=3, path=reverse(path))
        frame(anim, p)

        if current == goal
            return anim
        end

        for neighbor in neighbors(current)
            if dist_from_start[current] + 1 < dist_from_start[neighbor]
                came_from[neighbor] = current
                dist_from_start[neighbor] = dist_from_start[current] + 1
                open_list[neighbor] = dist_from_start[neighbor] + heuristic(neighbor, goal)
            end
        end

        push!(visited, current)
    end

    return anim
end

"""
    main()

Generate A* pathfinding animations on 15×15 and 25×25 mazes
with Manhattan, Euclidean, and no-heuristic strategies.
"""
function main()
    Random.seed!(42)
    mz = Maze(15, 15)
    mp4(astar_movie(mz, heuristic=manhattan), "~/Downloads/astar_manhattan.mp4", fps=12, loop=0)
    mp4(astar_movie(mz, heuristic=euclidean), "~/Downloads/astar_euclidean.mp4", fps=12, loop=0)
    mp4(astar_movie(mz, heuristic=no_heuristic), "~/Downloads/astar_no_heuristic.mp4", fps=12, loop=0)

    Random.seed!(24)
    mz = Maze(25, 25)
    mp4(astar_movie(mz, heuristic=manhattan), "~/Downloads/astar_big_manhattan.mp4", fps=25, loop=0)
    mp4(astar_movie(mz, heuristic=euclidean), "~/Downloads/astar_big_euclidean.mp4", fps=25, loop=0)
    mp4(astar_movie(mz, heuristic=no_heuristic), "~/Downloads/astar_big_no_heuristic.mp4", fps=25, loop=0)
end

end # module AStarDemo

if abspath(PROGRAM_FILE) == @__FILE__
    AStarDemo.main()
end
