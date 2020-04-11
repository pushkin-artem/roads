struct GridLocation
{
    int x, y;
};

namespace std
{
/* implement hash function so we can put GridLocation into an unordered_set */
    template <> struct hash<GridLocation>
    {
        typedef GridLocation argument_type;
        typedef std::size_t result_type;
        std::size_t operator()(const GridLocation& id) const noexcept
        {
            return std::hash<int>()(id.x ^ (id.y << 6));
        }
    };
}

struct HexGrid
{
    static std::array<GridLocation, 6> DIRS;

    int radius;
    std::unordered_set<GridLocation> walls;
    HexGrid(){};
    HexGrid(int radius_) : radius(radius_) {}

    bool in_bounds(GridLocation id) const
    {
        return abs(id.x) <= radius && abs(id.y) <= radius;
    }

    bool passable(GridLocation id) const
    {
        return walls.find(id) == walls.end();
    }

    std::vector<GridLocation> neighbors(GridLocation id) const
    {
    std::vector<GridLocation> results;

    for (GridLocation dir : DIRS)
    {
        GridLocation next{id.x + dir.x, id.y + dir.y};
        if (in_bounds(next) && passable(next))
        {
            results.push_back(next);
        }
    }

    if ((id.x + id.y) % 2 == 0)
    {
        // aesthetic improvement on grids
        std::reverse(results.begin(), results.end());
    }

    return results;
    }
};

std::array<GridLocation, 6> HexGrid::DIRS = {GridLocation{0, -1}, GridLocation{1, -1}, GridLocation{1, 0}, GridLocation{0, 1}, GridLocation{-1, 1}, GridLocation{-1, 0}};

// Helpers for GridLocation

bool operator == (GridLocation a, GridLocation b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator != (GridLocation a, GridLocation b)
{
    return !(a == b);
}

bool operator < (GridLocation a, GridLocation b)
{
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

std::basic_iostream<char>::basic_ostream& operator<<(std::basic_iostream<char>::basic_ostream& out, const GridLocation& loc)
{
    out << '(' << loc.x << ',' << loc.y << ')';
    return out;
}

// This outputs a grid. Pass in a distances map if you want to print
// the distances, or pass in a point_to map if you want to print
// arrows that point to the parent location, or pass in a path vector
// if you want to draw the path.

template<class Graph>
void draw_grid(const Graph& graph, int radius,
                std::unordered_map<GridLocation, double>* distances=nullptr,
                std::unordered_map<GridLocation, GridLocation>* point_to=nullptr,
                std::vector<GridLocation>* path=nullptr,
                std::vector<field>* neighbors=nullptr
               )
{
    int p = 0;
    for(int y = p - graph.radius; y <= 0; y++)
    {
        for(int i = graph.radius - p; i >= 0; i--)
        {
            std::cout << " ";
        }

        p += graph.radius;
        for(int x = graph.radius - p; x <= graph.radius; x++)
        {
            GridLocation id {x, y};
            field id2{id.x, id.y};

            if (graph.walls.find(id) != graph.walls.end())
            {
                std::cout << " # ";
            }
            else if (distances != nullptr && distances->count(id))
            {
                std::cout << " " << (*distances)[id] << " ";
            }
            else if (path != nullptr && find(path->begin(), path->end(), id) != path->end())
            {
                std::cout << " @ ";
            }
            else if(neighbors != nullptr && find(neighbors->begin(), neighbors->end(), id2) != neighbors->end())
            {
                std::cout << " " << neighbors->at(std::distance(neighbors->begin(), find(neighbors->begin(), neighbors->end(), id2))).value << " ";
            }
            else
            {
                std::cout << " x ";
            }
        }
        p -= graph.radius;
        std::cout << '\n';
        p++;
    }

    p = 1;
    for(int y = p; y <= graph.radius; y++)
    {
        p += 1;
        for(int i = graph.radius - p; i <= graph.radius; i++)
        {
            std::cout << " ";
        }
        p -= 1;

        for(int x = -graph.radius; x <= graph.radius-p; x++)
        {
            GridLocation id {x, y};
            field id2{id.x, id.y};

            if (graph.walls.find(id) != graph.walls.end())
            {
                std::cout << " # ";
            }
            else if (distances != nullptr && distances->count(id))
            {
                std::cout << " " << (*distances)[id] << " ";
            }
            else if (path != nullptr && find(path->begin(), path->end(), id) != path->end())
            {
            std::cout << " @ ";
            }
            else if(neighbors != nullptr && find(neighbors->begin(), neighbors->end(), id2) != neighbors->end())
            {
                std::cout << " " << neighbors->at(std::distance(neighbors->begin(), find(neighbors->begin(), neighbors->end(), id2))).value << " ";
            }
            else
            {
                std::cout << " x ";
            }
        }
        std::cout << '\n';
        p++;
    }
}

void add_rect(HexGrid& grid, int x1, int y1)
{
    GridLocation loc = {x1, y1};
    grid.walls.insert(loc);
}

HexGrid make_diagram1(int radius, std::vector<field> wall)
{
    HexGrid grid(radius);
    for(size_t i = 0; i < wall.size(); i++)
        add_rect(grid, wall[i].pos.x, wall[i].pos.y);

    return grid;
}

struct GridWithWeights: HexGrid
{
    std::unordered_set<GridLocation> forests;
    GridWithWeights(){};
    GridWithWeights(int r): HexGrid(r) {}
    double cost(GridLocation from_node, GridLocation to_node) const
    {
    return forests.find(to_node) != forests.end()? 5 : 1;
    }
};

GridWithWeights make_diagram4(int radius, std::vector<field> wall)
{
    GridWithWeights grid(radius);

    for(size_t i = 0; i < wall.size(); i++)
        add_rect(grid, wall[i].pos.x, wall[i].pos.y);

    typedef GridLocation L;
    grid.forests = std::unordered_set<GridLocation>
    {
    L{3, 4}, L{3, 5}, L{4, 1}, L{4, 2},
    L{4, 3}, L{4, 4}, L{4, 5}, L{4, 6},
    L{4, 7}, L{4, 8}, L{5, 1}, L{5, 2},
    L{5, 3}, L{5, 4}, L{5, 5}, L{5, 6},
    L{5, 7}, L{5, 8}, L{6, 2}, L{6, 3},
    L{6, 4}, L{6, 5}, L{6, 6}, L{6, 7},
    L{7, 3}, L{7, 4}, L{7, 5}
    };
    return grid;
}

template<typename T, typename priority_t>
struct PriorityQueue
{
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>,
    std::greater<PQElement>> elements;

    inline bool empty() const
    {
    return elements.empty();
    }

    inline void put(T item, priority_t priority)
    {
        elements.emplace(priority, item);
    }

    T get()
    {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

template<typename Location, typename Graph>
void dijkstra_search
    (Graph graph1,
    GridWithWeights graph2,
    Location start,
    Location goal,
    std::unordered_map<Location, Location>& came_from,
    std::unordered_map<Location, double>& cost_so_far)
{
    PriorityQueue<Location, double> frontier;
    frontier.put(start, 0);

    came_from[start] = start;
    cost_so_far[start] = 0;

    while (!frontier.empty())
    {
        Location current = frontier.get();

        if (current == goal)
        {
            break;
        }

        for (Location next : graph1.neighbors(current))
        {
            double new_cost = cost_so_far[current] + graph2.cost(current, next);
            if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next])
            {
                cost_so_far[next] = new_cost;
                came_from[next] = current;
                frontier.put(next, new_cost);
            }
        }
    }
}

template<typename Location>
std::vector<Location> reconstruct_path(Location start, Location goal, std::unordered_map<Location, Location> came_from)
{
    std::vector<Location> path;
    Location current = goal;
    while (current != start)
    {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start); // optional
    std::reverse(path.begin(), path.end());

    return path;
}
