#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

int main()
{
    nlohmann::json j {};
    std::ifstream json_file_stream("/home/nikita/CppSolyanka/src/hello_world.json");
    std::cout << (j = nlohmann::json::parse(json_file_stream))["hello"] << std::endl;
    
    return 0;
}