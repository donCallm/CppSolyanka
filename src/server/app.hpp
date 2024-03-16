#pragma once

#include <boost/asio.hpp>
#include <memory>

namespace core
{
    class state;
    class hub;
    class mempool;
    class binder;

    class app : public std::enable_shared_from_this<app>
    {
        public:
            app(boost::asio::io_service& io_service);
            virtual ~app() = default;

            virtual boost::asio::io_service& get_service()  { return _io_service; }
            virtual std::shared_ptr<state> get_state()      { return _state; }
            std::shared_ptr<mempool> get_mempool()          { return _tx_pool; }
            std::shared_ptr<binder> get_binder()            { return _bnr; }

        private:
            boost::asio::io_service&    _io_service;
            std::shared_ptr<hub>        _hub;
            std::shared_ptr<state>      _state;
            std::shared_ptr<mempool>    _tx_pool;
            std::shared_ptr<binder>     _bnr;
    };
}
