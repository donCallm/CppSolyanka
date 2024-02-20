#include <chrono>
#include <server/database/database.hpp>
#include "mempool.hpp"

using namespace db;

namespace core
{
    void mempool::add(tx& new_tx)
    {
        _transactions.push(new_tx);
    }

    void mempool::executing()
    {
        if (_transactions.size() == QUEUE_SIZE_THRESHOLD)
        {
            while (!_transactions.empty())
            {
                tx transaction = _transactions.front();
                transaction.executing();

                nlohmann::json json_tx = transaction;
                DB()->write(database::transactions, std::to_string(transaction.id), json_tx.dump());
                
                _transactions.pop();
            }
        }
    }
}
