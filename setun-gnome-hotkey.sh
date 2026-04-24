#!/bin/bash
HOTKEY_NAME="Ctrl+Y Hotkey"
HOTKEY_COMMAND="dbus-send --session --dest=ru.example.Hotkey /Hotkey ru.example.Hotkey.HandleHotkey"
HOTKEY_BINDING="Control+y"

# 1. Подсчитать количество существующих
COUNT=$(gsettings get org.gnome.settings-daemon.plugins.media-keys custom-keybindings | grep -o 'custom[0-9]\+' | wc -l)

# 2. Новый слот
NEW_SLOT=$COUNT
NEW_PATH="/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/custom${NEW_SLOT}/"

# 3. Добавить к существующим
gsettings set org.gnome.settings-daemon.plugins.media-keys custom-keybindings "['$NEW_PATH']"

# 4. Настроить
gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:$NEW_PATH name "$HOTKEY_NAME"
gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:$NEW_PATH command "$HOTKEY_COMMAND"
gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:$NEW_PATH binding "$HOTKEY_BINDING"

echo "✅ Hotkey $HOTKEY_BINDING = custom$NEW_SLOT"