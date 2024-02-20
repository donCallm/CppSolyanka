#pragma once

#include <server/database/database.hpp>
#include <iostream>
#include <queue>
#include <thread>
#include "tx.hpp"

namespace core
{

class mempool
{
    public:
        ~mempool();

    public:
        void start();
        void add(tx& new_tx);
        void executing();

    private:
        std::thread _m_thread;
        std::queue<tx> _transactions;
};

}
