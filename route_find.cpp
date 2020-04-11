void route(int n, int radius, std::vector<field>& numbered_fields)
{
    std::vector<field> wall;
    HexGrid grid1;
    GridWithWeights grid2;
    std::unordered_map<GridLocation, GridLocation> came_from;
    std::unordered_map<GridLocation, double> cost_so_far;
    GridLocation start;
    GridLocation goal;
    std::vector<GridLocation> path;

    for(auto&& i = 0; i < n-1; i++)
    {
        came_from.clear();
        cost_so_far.clear();

        grid1 = make_diagram1(radius, wall);
        grid2 = make_diagram4(radius, wall);

        start = {numbered_fields[i].pos.x, numbered_fields[i].pos.y};
        goal = {numbered_fields[i+1].pos.x, numbered_fields[i+1].pos.y};

        dijkstra_search(grid1, grid2, start, goal, came_from, cost_so_far);
        std::vector<GridLocation> path = reconstruct_path(start, goal, came_from);
        //std::cout << i +1 << ".step: " << '\n';
        //draw_grid(grid1, 1, nullptr, nullptr, &path);
        //std::cout << '\n';

        for(auto& i : path)
        {
            field f ={i.x, i.y, 10};
            wall.push_back(f);
        }
        wall.pop_back();
    }

    field myfield = {numbered_fields[n-1].pos.x, numbered_fields[n-1].pos.y, 10};
    wall.push_back(myfield);

    grid1 = make_diagram1(radius, wall);
    grid2 = make_diagram4(radius, wall);

    std::cout << '\n';
    std::cout << "You can see the route: " << '\n';
    std::cout << '\n';
    draw_grid(grid1, 1, nullptr, nullptr, &path);
    std::cout << '\n';
    find_route(wall);
}