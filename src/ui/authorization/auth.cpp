#pragma once
#include <QMessageBox>
#include <spdlog/spdlog.h>
#include "auth.hpp"

namespace ui
{

login_window::login_window()
{
    spdlog::info("Create login_window");
    
    _window = std::make_shared<QWidget>();
    _window->setWindowTitle("Login Window");
    _window->setFixedSize(480, 240);
    
    _main_layout = std::make_shared<QVBoxLayout>(_window.get());
    _login_layout = std::make_shared<QHBoxLayout>(_window.get());
    _password_layout = std::make_shared<QHBoxLayout>(_window.get());
    _button_layout = std::make_shared<QHBoxLayout>(_window.get());

    setup_window();
}

void login_window::show()
{
    if(!_window)
        throw std::runtime_error("Qt app is not created");
    _window->show();
}

void login_window::setup_window()
{
    spdlog::info("Setup login_window");
    
    _login_edit = std::make_shared<QLineEdit>(_window.get());
    _login_edit->setPlaceholderText("Login");
    _login_layout->addWidget(_login_edit.get());
    _main_layout->addLayout(_login_layout.get());
    _main_layout->addSpacing(5);

    _password_edit = std::make_shared<QLineEdit>(_window.get());
    _password_edit->setPlaceholderText("Password");
    _password_edit->setEchoMode(QLineEdit::Password);
    _password_layout->addWidget(_password_edit.get());
    _main_layout->addLayout(_password_layout.get());
    _main_layout->addSpacing(5);

    _login_button = std::make_shared<QPushButton>("Login", _window.get());
    _registration_button = std::make_shared<QPushButton>("Registration", _window.get());

    // Link button login with some handler
    QObject::connect(_login_button.get(), &QPushButton::clicked, [&](){
        QString username = _login_edit->text();
        QString password = _password_edit->text();

        if (username == "user" && password == "password") {
            QMessageBox::information(_window.get(), "Login", "Login successful");
        } else {
            QMessageBox::critical(_window.get(), "Login", "Login failed");
        }
        
    });
    _button_layout->addWidget(_login_button.get());

    // Link button reg with some handler
    QObject::connect(_registration_button.get(), &QPushButton::clicked, [&](){
        QMessageBox::critical(_window.get(), "Registration", "Unimplemented button, please wait...");
    });
    _button_layout->addWidget(_registration_button.get());

    _main_layout->addLayout(_button_layout.get());
    
    _main_layout->setAlignment(Qt::AlignCenter);
}

}