#pragma once

#include <server/database/database.hpp>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <server/state/state.hpp>
#include <memory>
#include "tx.hpp"

constexpr const uint8_t QUEUE_SIZE_THRESHOLD = 10;

namespace core
{

class state;
class tx;
class tx_send;

class mempool
{
    public:
        void add(std::unique_ptr<tx>&& new_tx);
        void executing(state& st);

    private:
        std::queue<std::unique_ptr<tx>> _transactions;
        static std::mutex _mtx;
};

}
