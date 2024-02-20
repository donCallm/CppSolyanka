#include <chrono>
#include <server/database/database.hpp>
#include "mempool.hpp"

using namespace db;

namespace core
{
    void mempool::start()
    {
        _m_thread = std::thread([this](){
            this->executing();
        });
    }

    void mempool::add(tx& new_tx)
    {
        _transactions.push(new_tx);
    }

    void mempool::executing()
    {
        while (true)
        {
            if (!_transactions.empty())
            {
                tx transaction = _transactions.front();
                transaction.executing();

                nlohmann::json json_tx = transaction;
                DB()->write(database::transactions, std::to_string(transaction.id), json_tx.dump());
                
                _transactions.pop();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    mempool::~mempool() { _m_thread.join(); }
}
