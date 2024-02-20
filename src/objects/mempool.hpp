#pragma once

#include <server/database/database.hpp>
#include <iostream>
#include <queue>
#include <thread>
#include "tx.hpp"

constexpr const uint8_t QUEUE_SIZE_THRESHOLD = 10;

namespace core
{

class mempool
{
    public:
        void add(tx& new_tx);
        void executing();

    private:
        std::queue<tx> _transactions;
};

}
