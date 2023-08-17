#include "client.hpp"

int main()
{
    std::cout << "START" << std::endl;
    client user;
    user.connect();
    std::cout << "END" << std::endl;
}