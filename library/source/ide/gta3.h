#pragma once
#include "..\types\ideipl_line.h"
#include "..\types\section.h"
#include "..\types\point.h"
#include "..\types\rgba.h"
#include <fstream>

namespace ide {

    struct gta3 {

        enum item_type { item_na, item_objs, item_tobj, item_hier };
        enum object_type { obj_objs, obj_tobj };

        struct tModelObject {
            int id = -1;
            char modelName[24];
            char txdName[24];
        };

        union tObjectFlags {
            struct {
                unsigned int : 1;
                unsigned int doNotFade : 1;
                unsigned int drawLast : 1;
                unsigned int additive : 1;
                unsigned int isTunnel : 1;
                unsigned int ignoreLighting : 1;
                unsigned int noZbufferWrite : 1;
            };
            unsigned int int_value = 0;
        };

        struct tObject : public tModelObject {
            float drawDistances[3] = { 0.0f, 0.0f, 0.0f };
            unsigned int objCount;
            tObjectFlags flags;

            tObject();

            tObject(ideipl_line &line);

            void read(ideipl_line &line);

            void write(ideipl_line &line);

            float getLargestLodDistance();
        };

        struct tTimeObject : public tObject {
            unsigned char timeOn = 0;
            unsigned char timeOff = 0;

            tTimeObject(ideipl_line &line);

            void read(ideipl_line &line);

            void write(ideipl_line &line);
        };

        struct tHierObject : public tModelObject {
            tHierObject(ideipl_line &line);

            void read(ideipl_line &line);

            void write(ideipl_line &line);
        };

        struct ide_file {
            ideipl_section<tObject> objs;
            ideipl_section<tTimeObject> tobj;
            ideipl_section<tHierObject> hier;

            ide_file(std::string const &filename);

            template<typename T>
            void forAllObjects(T callback) {
                for (auto &obj : objs)
                    callback(obj, obj_objs);
                for (auto &obj : tobj)
                    callback(obj, obj_tobj);
            }
        };
    };
}