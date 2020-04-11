struct position {
    int x, y;
};

struct field {
    position pos;
    int value;
};

bool operator == (field a, field b)
{
    return a.pos.x == b.pos.x && a.pos.y == b.pos.y;
}

void find_route(std::vector<field> route)
{
    std::cout << "The route length is: " << route.size() << '\n';
    std::cout << "The coordinates of route there are: " << '\n';
    for (auto& field : route)
        std::cout << field.pos.x << " " << field.pos.y << std::endl;
}

