#include "binder.hpp"
#include "mempool.hpp"

namespace core
{
    binder::binder(mempool& tx_pool_) :
        _tx_pool(tx_pool_)
    {}

    void binder::validate(tx& transaction)
    {
        if (transaction.validate())
            _tx_pool.add(transaction);
    }
}
