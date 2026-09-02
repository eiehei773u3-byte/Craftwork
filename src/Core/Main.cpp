#include "../Classes/Root.hpp"
#include "../Classes/functions.hpp"

#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <nlohmann/json.hpp>



int main() {
    Root Root("Craftwork", 800, 600);
    std::ifstream file("settings.json");
    nlohmann::json settings;
    file >> settings;
    bool Console_enabled = settings["Console"];
    if (Console_enabled) {
        Console();
    }

    return 0;
}