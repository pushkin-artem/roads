void readfile(std::string text_file, int& n, int& radius, std::vector<field>& numbered_fields)
{
    int data;
    std::ifstream MyReadFile(text_file + ".txt");
    if(!MyReadFile)
    {
	std::cout << "Error in opening the file\n";
    }

    MyReadFile >> n;
    MyReadFile >> radius;
    while (MyReadFile.good())
    {
        field myfield;
        MyReadFile >> myfield.pos.x;
        MyReadFile >> myfield.pos.y;
        MyReadFile >> myfield.value;
        numbered_fields.push_back(myfield);
    }
    MyReadFile.close();
}
