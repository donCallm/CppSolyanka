#pragma once

#include <iostream>
#include <redis++/redis++.h>

namespace db
{
    class database {
public:
    static database& instance() {
        static database instance; 
        return instance;
    }

private:
    database() {}

    database(const database&) = delete;
    database& operator=(const Singleton&) = delete;
};
}
