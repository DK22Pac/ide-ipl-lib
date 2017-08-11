#include "gtavc.h"

ide::gtavc::tObject::tObject() {}

ide::gtavc::tObject::tObject(ideipl_line & line) {
    read(line);
}

void ide::gtavc::tObject::read(ideipl_line & line) {
    if (line.param_count() == 6)
        line >> id >> modelName >> txdName >> objCount >> drawDistances[0] >> flags.int_value;
    else if (line.param_count() == 7)
        line >> id >> modelName >> txdName >> objCount >> drawDistances[0] >> drawDistances[1] >> flags.int_value;
    else if (line.param_count() == 8)
        line >> id >> modelName >> txdName >> objCount >> drawDistances[0] >> drawDistances[1] >> drawDistances[2] >> flags.int_value;
}

void ide::gtavc::tObject::write(ideipl_line & line) {
    if (objCount == 1)
        line << id << modelName << txdName << 1 << drawDistances[0] << flags.int_value;
    else if (objCount == 2)
        line << id << modelName << txdName << 2 << drawDistances[0] << drawDistances[1] << flags.int_value;
    else if (objCount == 3)
        line << id << modelName << txdName << 3 << drawDistances[0] << drawDistances[1] << drawDistances[2] << flags.int_value;
}

float ide::gtavc::tObject::getLargestLodDistance() {
    if (drawDistances[0] > drawDistances[1])
        return drawDistances[0] > drawDistances[2] ? drawDistances[0] : drawDistances[2];
    else if (drawDistances[1] > drawDistances[2])
        return drawDistances[1];
    return drawDistances[2];
}

ide::gtavc::tTimeObject::tTimeObject(ideipl_line & line) {
    read(line);
}

void ide::gtavc::tTimeObject::read(ideipl_line & line) {
    if (line.param_count() == 6)
        line >> id >> modelName >> txdName >> objCount >> drawDistances[0] >> flags.int_value >> timeOn >> timeOff;
    else if (line.param_count() == 7)
        line >> id >> modelName >> txdName >> objCount >> drawDistances[0] >> drawDistances[1] >> flags.int_value >> timeOn >> timeOff;
    else if (line.param_count() == 8)
        line >> id >> modelName >> txdName >> objCount >> drawDistances[0] >> drawDistances[1] >> drawDistances[2] >> flags.int_value >> timeOn >> timeOff;
}

void ide::gtavc::tTimeObject::write(ideipl_line & line) {
    if (objCount == 1)
        line << id << modelName << txdName << 1 << drawDistances[0] << flags.int_value << timeOn << timeOff;
    else if (objCount == 2)
        line << id << modelName << txdName << 2 << drawDistances[0] << drawDistances[1] << flags.int_value << timeOn << timeOff;
    else if (objCount == 3)
        line << id << modelName << txdName << 3 << drawDistances[0] << drawDistances[1] << drawDistances[2] << flags.int_value << timeOn << timeOff;
}

ide::gtavc::tHierObject::tHierObject(ideipl_line & line) {
    read(line);
}

void ide::gtavc::tHierObject::read(ideipl_line & line) {
    line >> id >> modelName >> txdName;
}

void ide::gtavc::tHierObject::write(ideipl_line & line) {
    line << id << modelName << txdName;
}

ide::gtavc::tWeapObject::tWeapObject(ideipl_line & line) {
    read(line);
}

void ide::gtavc::tWeapObject::read(ideipl_line & line) {
    line >> id >> modelName >> txdName >> animName >> ideipl_line::skip >> drawDistances[0];
}

void ide::gtavc::tWeapObject::write(ideipl_line & line) {
    line << id << modelName << txdName << animName << 1 << drawDistances[0] << 0;
}

ide::gtavc::ide_file::ide_file(std::string const & filename) {
    std::ifstream stream(filename);
    item_type item = item_na;
    if (stream.is_open()) {
        for (std::string fileline; getline(stream, fileline); ) {
            if (!fileline.empty() && fileline[0] != '#' && fileline.find_first_not_of(" \t") != std::string::npos) {
                if (item == item_na) {
                    if (!fileline.compare(0, 4, "objs"))
                        item = item_objs;
                    else if (!fileline.compare(0, 4, "tobj"))
                        item = item_tobj;
                    else if (!fileline.compare(0, 4, "hier"))
                        item = item_hier;
                    else if (!fileline.compare(0, 4, "weap"))
                        item = item_weap;
                }
                else {
                    if (!fileline.compare(0, 3, "end"))
                        item = item_na;
                    else {
                        ideipl_line line(fileline);
                        if (item == item_objs)
                            objs.emplace_back(line);
                        else if (item == item_tobj)
                            tobj.emplace_back(line);
                        else if (item == item_hier)
                            hier.emplace_back(line);
                        else if (item == item_weap)
                            weap.emplace_back(line);
                    }
                }
            }
        }
    }
}
