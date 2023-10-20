#pragma once

#include "user.hpp"

namespace core
{
    class card
    {
        public:
            card(user us, uint64_t card_number, std::string validity): _us(us), _card_number(card_number), 
                _balance(0), _validity(validity) {}

        public:
            void top_up_balance(uint64_t sum);
            void debiting(uint64_t sum);

        private:
            user _us;
            uint64_t _card_number;
            int _balance;
            std::string _validity;
    };
}
