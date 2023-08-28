#pragma once

#include <iostream>

class data_base
{
private:
    enum db {transaction = 1, client_info = 2};
public:
    data_base(/* args */);
    ~data_base();
};
