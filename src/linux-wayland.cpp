#include <dbus/dbus.h>
#include <iostream>
#include <cstring> 
#include <dbus/dbus.h>
#include <iostream>
#include <cstring>

DBusHandlerResult handle_message(DBusConnection *conn, DBusMessage *msg, void *data) {
    const char* interface = dbus_message_get_interface(msg);
    const char* member = dbus_message_get_member(msg);
    const char* path = dbus_message_get_path(msg);
    const int Type = dbus_message_get_type(msg);

    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Interface: " << (interface ? interface : "NULL") << std::endl;
    std::cout << "Member: " << (member ? member : "NULL") << std::endl;
    std::cout << "Path: " << (path ? path : "NULL") << std::endl;
    std::cout << "Type: " << (Type ? Type : 0) << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    
    if (dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_METHOD_CALL &&
        member && strcmp(member, "HandleHotkey") == 0) {
        
        std::cout << "🎉 Ctrl+Y!" << std::endl;
        
        DBusMessage *reply = dbus_message_new_method_return(msg);
        dbus_connection_send(conn, reply, NULL);
        dbus_connection_flush(conn);
        dbus_message_unref(reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main() {
    DBusError error;
    dbus_error_init(&error);
    
    DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (!conn) return 1;
    
    dbus_connection_add_filter(conn, (DBusHandleMessageFunction)handle_message, NULL, NULL);
    dbus_bus_request_name(conn, "ru.example.Hotkey", DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
    dbus_connection_flush(conn);
    
    std::cout << "✅ DBUS готов!" << std::endl;
    
    while (dbus_connection_read_write_dispatch(conn, -1));
    dbus_connection_unref(conn);
    return 0;
}