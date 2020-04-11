#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <fstream>

#include "route_output.cpp"
#include "read_file.cpp"
#include "implementation_hex.cpp"
#include "breadth_search_and_dijkstras.cpp"
#include "sales_man_problem_hamilton.cpp"
#include "route_find.cpp"


int main()
{
    //input, file reading
    std::string text_file;
    std::cout << "Please enter the file name: ";
    std::cin >> text_file;
    std::cout << "\n";
    int n, radius = 0;
    std::vector<field> numbered_fields;
    readfile(text_file, n, radius, numbered_fields);

    std::cout << "Please enter what you want to do: " << std::endl;
    std::cout << "- Route drawing in hex between points (enter 1) OR" << std::endl;
    std::cout << "- Hamilton route (enter 2)" << std::endl;
    int choice;
    std::cin >> choice;
    std::cout << "\n";

    if(choice == 1)
    {
        route(n, radius, numbered_fields);
    }
    else if(choice == 2)
    {
        Hamilton(n, radius, numbered_fields);
    }

    return 0;
}
