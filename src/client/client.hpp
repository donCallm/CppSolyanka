#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <vector>

class client
{
private:
    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::socket _socket;
    

public:
    client(): _socket(_io_service){connect();}
    void connect();
    void sent_request();
    void get_response();
};
