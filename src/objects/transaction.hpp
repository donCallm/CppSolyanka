#pragma once

#include <iostream>

namespace core
{
    class transaction
    {
        public:
            enum type { top_up = 0, debiting = 1 };
            transaction(uint64_t id, type t): _id(id), _type(t) {}

        private:
            uint64_t _id;
            type _type;
    };
}
