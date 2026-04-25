#include <dbus/dbus.h>
#include <iostream>
#include <cstring>
#include <dbus/dbus.h>
#include <iostream>
#include <cstring>
#include <limits>
#include <csignal>
#include "setup-hotkey-lib.h"
#include "capture_key_terminal.h"

// Глобальный флаг для управления главным циклом
volatile sig_atomic_t g_running = 1;
bool main_verbose = false;

// Обработчик сигнала для Ctrl+C (SIGINT)
void signal_handler(int signum)
{
    g_running = 0;
}

DBusHandlerResult handle_message(DBusConnection *conn, DBusMessage *msg, void *data)
{
    const char *interface = dbus_message_get_interface(msg);
    const char *member = dbus_message_get_member(msg);
    const char *path = dbus_message_get_path(msg);
    const int Type = dbus_message_get_type(msg);

    if (main_verbose)
    {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Interface: " << (interface ? interface : "NULL") << std::endl;
        std::cout << "Member: " << (member ? member : "NULL") << std::endl;
        std::cout << "Path: " << (path ? path : "NULL") << std::endl;
        std::cout << "Type: " << (Type ? Type : 0) << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
    }

    if (dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_METHOD_CALL &&
        member && strcmp(member, "DDeathCounter") == 0)
    {

        std::cout << "🎉 Ctrl+Y!" << std::endl;

        DBusMessage *reply = dbus_message_new_method_return(msg);
        dbus_connection_send(conn, reply, NULL);
        dbus_connection_flush(conn);
        dbus_message_unref(reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main(int argc, char *argv[])
{
    std::string hotkeyName = "DDeathCounter";
    std::string binding;
    std::string operation = "add";

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h")
        {
            std::cout << "Usage: " << argv[0] << " [--binding <binding>] [--name <name>]\n"
                      << "  --binding, -b <binding>  Hotkey binding (e.g., '<Control>y', '<Alt>y', <Control>y).\n"
                      << "  --name, -n <name>        Name of the hotkey (default: DDeathCounter).\n"
                      << std::endl;
            return 0;
        }
        else if (arg == "--verbose")
        {
            main_verbose = true;
        }
        else if ((arg == "--binding" || arg == "-b") && i + 1 < argc)
        {
            binding = argv[++i];
        }
        else if ((arg == "--name" || arg == "-n") && i + 1 < argc)
        {
            hotkeyName = argv[++i];
        }
        else
        {
            std::cerr << "unknown argument: " << arg << "\n";
            return 1;
        }
    }
    std::cout << "is_hotkey_exists: " << is_hotkey_exists(hotkeyName) << std::endl;
    if (is_hotkey_exists(hotkeyName))
    {
        std::cout << "This hotkey already exists" << std::endl;
        std::cout << "what do you want to do:\n"
                     "1. edit hotkey\n"
                     "2. remove hotkey\n"
                     "3. exit"
                  << std::endl;
        std::cout << "operation: ";
        std::cin >> operation;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очищаем ввод
    }
    int status;
    std::string command = "dbus-send --session --dest=ru.example.Hotkey --print-reply /Hotkey ru.example.Hotkey." + hotkeyName;

    if (operation == "remove" || operation == "2")
    {
        status = setup_hotkey_linux(command, binding, hotkeyName, "remove", main_verbose);
        if (status == 1)
        {
            std::cout << "faild to remove hotkey" << std::endl;
            return 1;
        }
        return 0;
    }
    else if (operation == "exit" || operation == "3")
    {
        return 0;
    }
    else
    {
        if (binding.empty())
        {
            std::cout << "Hotkey binding:\n";
            binding = capture_key_terminal();
            std::cout << "binding: " << binding << std::endl;
        }

        if (operation == "edit" || operation == "1")
        {
            status = setup_hotkey_linux(command, binding, hotkeyName, "edit", main_verbose);
            if (status == 1)
            {
                std::cout << "faild to edit hotkey" << std::endl;
                return 1;
            }
        }
        else if (operation == "add")
        {
            status = setup_hotkey_linux(command, binding, hotkeyName, "add", main_verbose);
            if (status == 1)
            {
                std::cout << "faild to add hotkey" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cout << "unknown operation: " << operation << std::endl;
            return 0;
        }
    }

    // Регистрируем обработчик для сигнала SIGINT (Ctrl+C)
    signal(SIGINT, signal_handler);

    DBusError error;
    dbus_error_init(&error);

    DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (!conn)
    {
        std::cerr << "Failed to connect to the D-Bus session bus: " << error.message << std::endl;
        dbus_error_free(&error);
        return 1;
    }

    dbus_connection_add_filter(conn, (DBusHandleMessageFunction)handle_message, NULL, NULL);
    dbus_bus_request_name(conn, "ru.example.Hotkey", DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
    dbus_connection_flush(conn);

    std::cout << "✅ DBUS готов!" << std::endl;

    while (g_running)
    {
        dbus_connection_read_write_dispatch(conn, 100); // Проверяем сообщения с таймаутом 100 мс
    }

    std::cout << "\nExit... remove hotkey." << std::endl;
    status = setup_hotkey_linux(command, binding, hotkeyName, "remove", main_verbose);
    if (status == 1)
    {
        std::cout << "faild to remove hotkey" << std::endl;
        return 1;
    }
    dbus_connection_unref(conn);
    return 0;
}