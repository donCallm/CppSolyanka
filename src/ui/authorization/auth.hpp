#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QApplication>
#include <memory>

namespace ui
{

class login_window
{
public:
    login_window();

public:
    void show();

private:
    void setup_window();

private:
    std::shared_ptr<QWidget>        _window;
    std::shared_ptr<QVBoxLayout>    _main_layout;
    std::shared_ptr<QHBoxLayout>    _login_layout;
    std::shared_ptr<QHBoxLayout>    _password_layout;
    std::shared_ptr<QHBoxLayout>    _button_layout;
    std::shared_ptr<QLineEdit>      _login_edit;
    std::shared_ptr<QLineEdit>      _password_edit;
    std::shared_ptr<QPushButton>    _login_button;
    std::shared_ptr<QPushButton>    _registration_button;
};

class app
{
public:
    app();

public:
    int start();
    int setup();

private:
    core::client user;
    std::shared_ptr<login_window> _lw;
};

}