#pragma once

#include <iostream>
#include <server/state/state.hpp>
#include "tx.hpp"
#include "mempool.hpp"

namespace core
{

class tx;
class mempool;
class state;

class binder
{
    public:
        binder(mempool& tx_pool_, state& st_);
        
    public:
        void validate(std::unique_ptr<tx>&& new_tx);

    private:
        mempool& _tx_pool;
        state& _st;
};

}
