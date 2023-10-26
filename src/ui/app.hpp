#pragma once
#include <ui/authorization/auth.hpp>

namespace ui
{

    class app
    {
    public:
        app();

    public:
        int start();
        void setup();

    private:
        std::shared_ptr<login_window> _lw;
    };

}