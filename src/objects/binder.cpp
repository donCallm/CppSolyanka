#include "binder.hpp"
#include "mempool.hpp"

namespace core
{
    binder::binder(mempool& tx_pool_, state& st_) :
        _tx_pool(tx_pool_),
        _st(st_)
    {}

    void binder::validate(std::unique_ptr<tx>&& new_tx)
    {
        if (new_tx->validate(_st))
        {
            _tx_pool.add(std::move(new_tx));
            _tx_pool.executing(_st);
        }
    }
}
