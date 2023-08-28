#pragma once

#include <iostream>

class client
{
private:
    std::string _name;
    std::string _sername;
    std::string _patronymic;
    int _id;
public:
    client(std::string name, std::string sername, std::string patronymic, int id): _name(name), _sername(sername),
    _patronymic(patronymic), _id(id) {}
    std::string get_nsp();
    int get_id();
};
