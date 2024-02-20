#pragma once

#include <iostream>
#include "tx.hpp"
#include "mempool.hpp"

namespace core
{

class tx;

class binder
{
    friend class mempool;

    public:
        binder(mempool& tx_pool_);

    private:
        void add(tx& transaction_);
        
    public:
        void validate(tx& transaction);

    private:
        mempool& _tx_pool;
};

}
