#include <cstdio>
#include <iostream>
#include <string.h>
#include <vector>
#include <string>
#include <limits>

const std::string BASE_PATH = "/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings";

std::string parceVectorToArrayString(std::vector<std::string> vector)
{
    if (vector.empty())
    {
        return "@as []";
    }
    std::string string = "['" + vector[0];
    for (int i = 1; i < vector.size(); i++)
    {
        string.append("', '");
        string.append(vector[i]);
    }
    string.append("']");
    return string;
}

std::string exec_command_string(const char *cmd)
{
    std::string result;
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
    {
        std::cout << "faild to execute command '" << cmd << "'" << std::endl;
        return result;
    };
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe))
    {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

int configure_hotkey(std::string command, std::string binding, std::string hotkeyName, std::string Hotkey)
{
    std::string cmd;
    std::string result;
    cmd = "dconf write " + Hotkey + "binding" + " " + '"' + "'" + binding + "'" + '"';
    std::cout << cmd << std::endl;
    result = exec_command_string(cmd.c_str());
    std::cout << result << std::endl;
    cmd = "dconf write " + Hotkey + "command" + " " + '"' + "'" + command + "'" + '"';
    std::cout << cmd << std::endl;
    result = exec_command_string(cmd.c_str());
    std::cout << result << std::endl;
    cmd = "dconf write " + Hotkey + "name" + " " + '"' + "'" + hotkeyName + "'" + '"';
    std::cout << cmd << std::endl;
    result = exec_command_string(cmd.c_str());
    std::cout << result << std::endl;
    return 0;
}

int configure_hotkey_array(std::vector<std::string> userHotkeys)
{
    std::string cmd;
    std::string result;
    cmd = "dconf write '" + BASE_PATH + "' \"" + parceVectorToArrayString(userHotkeys) + "\"";
    std::cout << cmd << std::endl;
    result = exec_command_string(cmd.c_str());
    std::cout << result << std::endl;
    return 0;
}
int main(int argc, char *argv[])
{

    std::string hotkeyName = "DcustomHotkey";
    std::string command = "";
    std::string binding;
    std::string operation = "unset";

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h")
        {
            std::cout << "Usage: " << argv[0] << " --command <command> --binding <binding> [--name <name>] [--operation <add|remove>]\n"
                      << "  --command, -c <command>  Command to execute when the hotkey is pressed.\n"
                      << "  --binding, -b <binding>  Hotkey binding (e.g., '<Control>y', '<Alt>F4').\n"
                      << "  --name, -n <name>        Name of the hotkey (default: DcustomHotkey).\n"
                      << "  --operation, -o <add|remove>  Operation to perform (add or remove hotkey). Default is add."
                      << std::endl;
            return 0;
        }
        else if ((arg == "--command" || arg == "-c") && i + 1 < argc)
        {
            command = argv[++i];
        }
        else if ((arg == "--binding" || arg == "-b") && i + 1 < argc)
        {
            binding = argv[++i];
        }
        else if ((arg == "--name" || arg == "-n") && i + 1 < argc)
        {
            hotkeyName = argv[++i];
        }
        else if ((arg == "--operation" || arg == "-o") && i + 1 < argc)
        {
            operation = argv[++i];
        }
        else
        {
            std::cerr << "unknown argument: " << arg << "\n";
            return 1;
        }
    }
    if (!command.empty() && !binding.empty())
    {
        std::cout << "command: " << command << std::endl;
        std::cout << "binding: " << binding << "\n"
                  << std::endl;
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " --command <command> --binding <binding> [--name <name>]\n";
        return 1;
    }

    if (operation == "unset")
    {
        std::cout << "type of operations:\n"
                     "add - add new hotkey\n"
                     "remove - remove hotkey\n"
                     "edit - edit hotkey"
                  << std::endl;
        std::cout << "operation: ";
        std::cin >> operation;
    }

    std::cout << "operation: " << operation << std::endl;

    const char *GETUSERHOTKEYSCOMMAND =
        "/bin/bash -c \"dconf read /org/gnome/settings-daemon/plugins/media-keys/custom-keybindings | "
        "sed 's/\\[//g' | "
        "sed 's/\\]//g' | "
        "sed \\\"s/'//g\\\" | "
        "sed 's/, /\\n/g'\"";

    FILE *pipe = popen(GETUSERHOTKEYSCOMMAND, "r");
    if (!pipe)
    {
        std::cout << "faild to execute command '" << GETUSERHOTKEYSCOMMAND << "'" << std::endl;
        return 1;
    };

    char buffer[256];
    std::vector<std::string> userHotkeys;
    while (fgets(buffer, sizeof(buffer), pipe))
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }
        if (len > 0 && buffer[0] != '\n' && std::string(buffer) != "@as " && std::string(buffer) != "@as []")
        {
            userHotkeys.push_back(buffer);
        }
    }
    pclose(pipe);

    const std::string Hotkey = "/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/" + hotkeyName + "/";

    for (int i = 0; i < userHotkeys.size(); i++)
    {
        if (userHotkeys[i] == Hotkey)
        {
            if (operation == "add")
            {
                std::cout << "This hotkey already exists" << std::endl;
                return 1;
            }
            else if (operation == "remove")
            {
                userHotkeys.erase(userHotkeys.begin() + i);
                break;
            }
        }
    }

    std::string result;
    std::string cmd;

    if (operation == "add")
    {
        userHotkeys.push_back(Hotkey);
        configure_hotkey(command, binding, hotkeyName, Hotkey);
        configure_hotkey_array(userHotkeys);
    }
    else if (operation == "remove")
    {
        cmd = "dconf reset -f '" + Hotkey + "'";
        std::cout << cmd << std::endl;
        result = exec_command_string(cmd.c_str());
        configure_hotkey_array(userHotkeys);
    }
    else if (operation == "edit")
    {
        configure_hotkey(command, binding, hotkeyName, Hotkey);
    }

    return 0;
}