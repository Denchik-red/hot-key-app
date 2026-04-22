#!/bin/bash
HOTKEY_NAME="Ctrl+Y Hotkey"
HOTKEY_COMMAND="dbus-send --session --dest=ru.example.Hotkey /Hotkey ru.example.Hotkey.HandleHotkey"
HOTKEY_BINDING="Control+y"

# Получить текущий список
CUSTOM_KEYS=$(gsettings get org.gnome.settings-daemon.plugins.media-keys custom-keybindings | sed "s/[[\]//g" | sed "s/[\]]//g" | sed "s/'/ /g")

# Добавить новый ключ
NEW_PATH="/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/custom${#CUSTOM_KEYS}/"
echo $CUSTOM_KEYS $NEW_PATH | tr ' ' '\n' | grep -v '^$' | tr '\n' ',' | sed 's/,$//'

gsettings set org.gnome.settings-daemon.plugins.media-keys custom-keybindings "[$NEW_PATH]"

# Настроить
gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:$NEW_PATH name "$HOTKEY_NAME"
gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:$NEW_PATH command "$HOTKEY_COMMAND"
gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:$NEW_PATH binding "$HOTKEY_BINDING"

echo "✅ Hotkey $HOTKEY_BINDING установлен!"