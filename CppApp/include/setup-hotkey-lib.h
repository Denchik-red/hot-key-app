#pragma once

int setup_hotkey_linux(std::string command, std::string binding, std::string hotkeyName = "DCustomHotkey", std::string operation = "add", bool verbose = false);
int is_hotkey_exists(std::string hotkeyName);