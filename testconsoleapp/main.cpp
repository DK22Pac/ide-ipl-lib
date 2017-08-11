#include "ide\gtasa.h"
#include <filesystem>
#include <array>

using namespace std::experimental;

char const *flagNames[] = {
    "IS_ROAD",
    "DO_NOT_FADE",
    "DRAW_LAST",
    "ADDITIVE",
    "IS_TUNNEL",
    "IGNORE_LIGHTING",
    "NO_ZBUFFER_WRITE",
    "DONT_RECEIVE_SHADOWS",
    "IGNORE_DRAW_DISTANCE",
    "IS_GLASS_TYPE_1",
    "IS_GLASS_TYPE_2",
    "IS_GARAGE_DOOR",
    "IS_DAMAGABLE",
    "",
    "IS_VEGETATION",
    "",
    "",
    "",
    "",
    "",
    "IS_TAG",
    "DISABLE_BACKFACE_CULLING",
    "IS_BREAKABLE_STATUE",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

int main()
{
    std::array<std::vector<std::tuple<int, std::string, float>>, 32> ideFlags;

    // scan all ide files
    for (const auto& p : filesystem::recursive_directory_iterator(filesystem::current_path())) {
        if (p.path().extension() == ".ide") {
            ide::gtasa::ide_file ideFile(p.path().string());
            ideFile.forAllObjects([&](ide::gtasa::tObject &obj, ide::gtasa::object_type) {
                for (unsigned int i = 0; i < 32; i++) {
                    if (obj.flags.int_value & (1 << i))
                        ideFlags[i].emplace_back(obj.id, obj.modelName, obj.drawDistance);
                }
            });
        }
    }

    // sort & output
    if (!filesystem::exists(filesystem::current_path() / "flags"))
        filesystem::create_directory(filesystem::current_path() / "flags");
    for (unsigned int i = 0; i < 32; i++) {
        if (!ideFlags[i].empty()) {
            std::sort(ideFlags[i].begin(), ideFlags[i].end(), [](std::tuple<int, std::string, float> const &p1, std::tuple<int, std::string, float> const &p2) {
                return std::get<0>(p1) < std::get<0>(p2);
            });
        }
        std::ofstream output(filesystem::current_path() / "flags" / ("flag_" + std::to_string(1 << i) + "_" + flagNames[i] + ".txt"));
        for (auto const &p : ideFlags[i])
            output << std::get<0>(p) << ", " << std::get<1>(p) << ", " << std::get<2>(p) << "\n";
    }

    return 0;
}