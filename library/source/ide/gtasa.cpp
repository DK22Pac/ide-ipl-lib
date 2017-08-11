#include "gtasa.h"

ide::gtasa::tObject::tObject() {}

ide::gtasa::tObject::tObject(ideipl_line & line) {
    read(line);
}

void ide::gtasa::tObject::read(ideipl_line & line) {
    if (line.param_count() == 6)
        line >> id >> modelName >> txdName >> ideipl_line::skip >> drawDistance >> flags.int_value;
    else if (line.param_count() == 7)
        line >> id >> modelName >> txdName >> ideipl_line::skip >> drawDistance >> ideipl_line::skip >> flags.int_value;
    else if (line.param_count() == 8)
        line >> id >> modelName >> txdName >> ideipl_line::skip >> drawDistance >> ideipl_line::skip >> ideipl_line::skip >> flags.int_value;
    else
        line >> id >> modelName >> txdName >> drawDistance >> flags.int_value;
}

void ide::gtasa::tObject::write(ideipl_line & line) {
    line << id << modelName << txdName << drawDistance << flags.int_value;
}

ide::gtasa::tTimeObject::tTimeObject(ideipl_line & line) {
    read(line);
}

void ide::gtasa::tTimeObject::read(ideipl_line & line) {
    if (line.param_count() == 8)
        line >> id >> modelName >> txdName >> ideipl_line::skip >> drawDistance >> flags.int_value >> timeOn >> timeOff;
    else if (line.param_count() == 9)
        line >> id >> modelName >> txdName >> ideipl_line::skip >> drawDistance >> ideipl_line::skip >> flags.int_value >> timeOn >> timeOff;
    else if (line.param_count() == 11)
        line >> id >> modelName >> txdName >> ideipl_line::skip >> drawDistance >> ideipl_line::skip >> ideipl_line::skip >> flags.int_value >> timeOn >> timeOff;
    else
        line >> id >> modelName >> txdName >> drawDistance >> flags.int_value >> timeOn >> timeOff;
}

void ide::gtasa::tTimeObject::write(ideipl_line & line) {
    line << id << modelName << txdName << drawDistance << flags.int_value << timeOn << timeOff;
}

ide::gtasa::tHierObject::tHierObject(ideipl_line & line) {
    read(line);
}

void ide::gtasa::tHierObject::read(ideipl_line & line) {
    line >> id >> modelName >> txdName;
}

void ide::gtasa::tHierObject::write(ideipl_line & line) {
    line << id << modelName << txdName;
}

ide::gtasa::tAnimObject::tAnimObject() {}

ide::gtasa::tAnimObject::tAnimObject(ideipl_line & line) {
    read(line);
}

void ide::gtasa::tAnimObject::read(ideipl_line & line) {
    line >> id >> modelName >> txdName >> animName >> drawDistance >> flags.int_value;
}

void ide::gtasa::tAnimObject::write(ideipl_line & line) {
    line << id << modelName << txdName << animName << drawDistance << flags.int_value;
}

ide::gtasa::tWeapObject::tWeapObject(ideipl_line & line) {
    read(line);
}

void ide::gtasa::tWeapObject::read(ideipl_line & line) {
    line >> id >> modelName >> txdName >> animName >> ideipl_line::skip >> drawDistance;
}

void ide::gtasa::tWeapObject::write(ideipl_line & line) {
    line << id << modelName << txdName << animName << 1 << drawDistance << 0;
}

ide::gtasa::tTxdParent::tTxdParent(ideipl_line & line) {
    read(line);
}

void ide::gtasa::tTxdParent::read(ideipl_line & line) {
    line >> txdName >> parentTxdName;
}

void ide::gtasa::tTxdParent::write(ideipl_line & line) {
    line << txdName << parentTxdName;
}

ide::gtasa::ide_file::ide_file(std::string const & filename) {
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
                    else if (!fileline.compare(0, 4, "anim"))
                        item = item_anim;
                    else if (!fileline.compare(0, 4, "weap"))
                        item = item_weap;
                    else if (!fileline.compare(0, 4, "txdp"))
                        item = item_txdp;
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
                        else if (item == item_anim)
                            anim.emplace_back(line);
                        else if (item == item_weap)
                            weap.emplace_back(line);
                        else if (item == item_txdp)
                            txdp.emplace_back(line);
                    }
                }
            }
        }
    }
}
