int ary[10][10],completed[10],m,cost=0;

std::vector<std::vector<int>> distance_matrix (std::vector<field> numbered_fields)
{
    std::vector<int> inner_matrix;
    std::vector<std::vector<int>> matrix;
    for(auto&& field1: numbered_fields)
    {
        for(auto&& field2: numbered_fields)
        {
            auto&& distance = hex_distance(Hex(field1.pos.x, field1.pos.y), Hex(field2.pos.x, field2.pos.y));
            inner_matrix.push_back(distance);
        }
        matrix.push_back(inner_matrix);
        inner_matrix.clear();
    }
    return matrix;
}

void takeInput(int n, std::vector<std::vector<int>> matrix)
{
	int i,j;

	m = n;

	for(i=0; i < n; i++)
	{
		for( j=0;j < n;j++)
			ary[i][j] = matrix[i][j];

		completed[i]=0;
	}

	std::cout<<"The cost list is:";
	for( i=0;i < n;i++)
	{
		std::cout<<"\n";

		for(j=0;j < n;j++)
			std::cout<< " " <<ary[i][j];
	}
	std::cout<<"\n";
}

int least(int c)
{
	int i,nc=999;
	int min=999,kmin;

	for(i=0;i < m;i++)
	{
		if((ary[c][i]!=0)&&(completed[i]==0))
			if(ary[c][i]+ary[i][c] < min)
			{
				min=ary[i][0]+ary[c][i];
				kmin=ary[c][i];
				nc=i;
			}
	}

	if(min!=999)
		cost+=kmin;

	return nc;
}

std::vector<int> order;

void mincost(int city)
{
	int i,ncity;

	completed[city]=1;
    //printf("%d--->",city+1);
	order.push_back(city+1);
	ncity=least(city);

	if(ncity==999)
	{
		ncity=0;
		//printf("%d",ncity+1);
		order.push_back(ncity+1);
		cost+=ary[city][ncity];

		return;
	}

	mincost(ncity);
}

void Hamilton(int n, int radius, std::vector<field> numbered_fields)
{
    std::vector<std::vector<int>> matrix = distance_matrix(numbered_fields);
	takeInput(n, matrix);
    std::cout<<"\n";
	mincost(0);
    std::cout<<"Point order should to be: ";
    std::cout<<"\n";
	for(auto&& i : order)
    {
        numbered_fields.push_back(numbered_fields[i-1]);
        std::cout << numbered_fields[i-1].pos.x << ", " << numbered_fields[i-1].pos.y << ", " << numbered_fields[i-1].value << std::endl;
    }
    numbered_fields.erase (numbered_fields.begin(), numbered_fields.begin()+numbered_fields.size()/2);

    std::vector<field> wall;
    HexGrid grid1;
    GridWithWeights grid2;
    std::unordered_map<GridLocation, GridLocation> came_from;
    std::unordered_map<GridLocation, double> cost_so_far;
    GridLocation start;
    GridLocation goal;

    for(auto&& i = 0; i < numbered_fields.size()-1; i++)
    {
        if(i == numbered_fields.size()-2)
            break;

        came_from.clear();
        cost_so_far.clear();

        grid1 = make_diagram1(radius, wall);
        grid2 = make_diagram4(radius, wall);

        start = {numbered_fields[i].pos.x, numbered_fields[i].pos.y};
        goal = {numbered_fields[i+1].pos.x, numbered_fields[i+1].pos.y};

        dijkstra_search(grid1, grid2, start, goal, came_from, cost_so_far);
        std::vector<GridLocation> path = reconstruct_path(start, goal, came_from);
        //std::cout << i +1 << ".step: " << '\n';
        //draw_grid(grid2, 3, nullptr, nullptr, &path);
        //std::cout << '\n';

        for(auto& i : path)
        {
            field f ={i.x, i.y, 10};
            wall.push_back(f);
        }
        wall.pop_back();
    }

    field first_element = {wall[0].pos.x, wall[0].pos.y};
    wall.erase(wall.begin());

    grid1 = make_diagram1(radius, wall);
    grid2 = make_diagram4(radius, wall);

    came_from.clear();
    cost_so_far.clear();

    start = {numbered_fields[numbered_fields.size()-2].pos.x, numbered_fields[numbered_fields.size()-2].pos.y};
    goal = {numbered_fields[numbered_fields.size()-1].pos.x, numbered_fields[numbered_fields.size()-1].pos.y};

    dijkstra_search(grid1, grid2, start, goal, came_from, cost_so_far);
    std::vector<GridLocation> path = reconstruct_path(start, goal, came_from);
    //std::cout << "Last step: " << '\n';
    //draw_grid(grid2, 3, nullptr, nullptr, &path);
    //std::cout << '\n';

    for(auto& i:path)
    {
            field f ={i.x, i.y, 10};
            wall.push_back(f);
    }

    grid1 = make_diagram1(radius, wall);
    grid2 = make_diagram4(radius, wall);

    std::cout << '\n';
    std::cout << "You can see the Hamilton route: " << '\n';
    std::cout << '\n';
    draw_grid(grid2, 3, nullptr, nullptr, &path);
    std::cout << '\n';

    wall.pop_back();
    wall.insert(wall.begin(),first_element);
    std::vector<field> road = wall;
}
