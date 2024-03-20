#include <chrono>
#include "mempool.hpp"

using namespace db;

namespace core
{
    std::mutex mempool::_mtx;

        void mempool::add(std::unique_ptr<tx>&& new_tx)
        {
            std::lock_guard<std::mutex> lock(_mtx);
            switch (new_tx->tx_type)
            {
                case tx::tx_send:
                {
                    _transactions.push(std::move(new_tx));
                    break;
                }
            }
        }

    void mempool::executing(state& st)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_transactions.size() == QUEUE_SIZE_THRESHOLD)
        {
            while (!_transactions.empty())
            {
                auto _tx = _transactions.front().get();
                _tx->executing(st);

                nlohmann::json json_tx = *_tx;
                DB()->write(database::transactions, std::to_string(_tx->id), json_tx.dump());

                _transactions.pop();
            }
        }
    }
}
