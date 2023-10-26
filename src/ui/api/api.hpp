#pragma once

#include <client/client.hpp>
#include <optional>

namespace ui
{
    class api
    {
    public:
        api() {}

    public:
        std::optional<core::user> login(const std::string& log, const std::string& pass);
        std::optional<core::user> registration(const std::string& log, const std::string& pass);

    private:
        core::client _cli;
    };
}