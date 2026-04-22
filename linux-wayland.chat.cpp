#include <dbus/dbus.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

static void free_user_data(void *data) {
    // Функция очистки (может быть NULL)
    std::cout << "Очистка user_data" << std::endl;
}

DBusHandlerResult handle_message(DBusConnection *conn, DBusMessage *msg, void *user_data) {
    std::cout << "📨 Получено сообщение!" << std::endl;
    
    if (dbus_message_is_method_call(msg, "ru.example.Hotkey", "HandleHotkey")) {
        std::cout << "🎉 HOTKEY Ctrl+Y сработал!" << std::endl;
        
        // Уведомление (установите libnotify-bin если нужно)
        system("notify-send 'Ctrl+Y Hotkey!' || echo 'notify-send не установлен'");
        
        DBusMessage *reply = dbus_message_new_method_return(msg);
        dbus_connection_send(conn, reply, NULL);
        dbus_message_unref(reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main() {
    dbus_threads_init_default();
    
    DBusError error;
    dbus_error_init(&error);
    
    std::cout << "🚀 Запуск DBUS Hotkey демона..." << std::endl;
    
    DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (!conn) {
        std::cerr << "❌ DBUS ошибка: " << error.message << std::endl;
        dbus_error_free(&error);
        return 1;
    }
    
    dbus_connection_add_filter(conn, 
                              (DBusHandleMessageFunction)handle_message, 
                              NULL,  // user_data
                              free_user_data);  // free_function
    
    int ret = dbus_bus_request_name(conn, "ru.example.Hotkey", 
                                   DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        std::cerr << "❌ Регистрация имени провалилась: " << ret << std::endl;
        dbus_connection_unref(conn);
        dbus_error_free(&error);
        return 1;
    }
    
    std::cout << "✅ Сервис ru.example.Hotkey готов!" << std::endl;
    std::cout << "🧪 dbus-send --session --dest=ru.example.Hotkey /Hotkey ru.example.Hotkey.HandleHotkey" << std::endl;
    
    // ✅ Правильный цикл обработки
    while (dbus_connection_read_write_dispatch(conn, -1)) {
        // Обработка продолжается...
    }
    
    dbus_connection_remove_filter(conn, (DBusHandleMessageFunction)handle_message, NULL);
    dbus_connection_unref(conn);
    dbus_error_free(&error);
    return 0;
}