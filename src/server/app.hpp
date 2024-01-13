#pragma once

#include <boost/asio.hpp>
#include <memory>

namespace core
{
    class state;
    class hub;

    class app : public std::enable_shared_from_this<app>
    {
        public:
            explicit app(boost::asio::io_service& io_service);

            boost::asio::io_service& get_service()  { return _io_service; }
            std::shared_ptr<state> get_state()      { return _state; }

        private:
            boost::asio::io_service     _default_io_service;
            boost::asio::io_service&    _io_service;
            std::shared_ptr<hub>        _hub;
            std::shared_ptr<state>      _state;
    };
}
