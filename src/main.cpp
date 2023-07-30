#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main()
{
    json j {};
    std::ifstream json_file_stream("/home/nikita/CppSolyanka/src/HelloWorld.json");
    std::cout << (j = json::parse(json_file_stream))["hello"] << std::endl;
    
    return 0;
}