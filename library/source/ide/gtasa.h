#pragma once
#include "..\types\ideipl_line.h"
#include "..\types\section.h"
#include "..\types\point.h"
#include "..\types\rgba.h"
#include <fstream>

namespace ide {

    struct gtasa {

        enum item_type { item_na, item_objs, item_tobj, item_hier, item_anim, item_weap, item_txdp };
        enum object_type { obj_objs, obj_tobj, obj_anim };

        struct tModelObject {
            int id = -1;
            char modelName[24];
            char txdName[24];
        };

        union tObjectFlags {
            struct {
                unsigned int isRoad : 1;
                unsigned int doNotFade : 1;
                unsigned int drawLast : 1;
                unsigned int additive : 1;
                unsigned int isSubway : 1;
                unsigned int dynamicLighting : 1;
                unsigned int noZbufferWrite : 1;
                unsigned int dontReceiveShadows : 1;
                unsigned int ignoreDrawDistance : 1;
                unsigned int glassType : 2;
            };
            unsigned int int_value = 0;
        };

        struct tObject : public tModelObject {
            float drawDistance = 0.0f;
            tObjectFlags flags;

            tObject();

            tObject(ideipl_line &line);

            void read(ideipl_line &line);

            void write(ideipl_line &line);
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

        struct tAnimObject : public tObject {
            char animName[16];

            tAnimObject();

            tAnimObject(ideipl_line &line);

            void read(ideipl_line &line);

            void write(ideipl_line &line);
        };

        struct tWeapObject : public tAnimObject {
            tWeapObject(ideipl_line &line);

            void read(ideipl_line &line);

            void write(ideipl_line &line);
        };

        struct tTxdParent {
            char txdName[24];
            char parentTxdName[24];

            tTxdParent(ideipl_line &line);

            void read(ideipl_line &line);

            void write(ideipl_line &line);
        };

        struct t2dEffect {
            enum eType {
                LIGHT,
                PARTICLE,
                PEDATTRACTOR = 3,
                SUNGLARE,
                ENTEREXIT = 6,
                ROADSIGN,
                SLOTMACHINEWHEEL,
                COVERPOINT,
                ESCALATOR
            };

            point offset;
            unsigned int type;

            union {
                struct tLight {
                    enum eCoronaMode {
                        CORONA_DEFAULT = 0,
                        CORONA_ALWAYS_AT_WET_WEATHER = 2,
                        CORONA_TRAFFICLIGHT = 7,
                        CORONA_TRAINCROSSLIGHT = 8,
                        CORONA_AT_RAIN_ONLY = 10
                    };

                    rgba color;
                    float coronaFarClip;
                    float pointlightRange;
                    float coronaSize;
                    float shadowSize;
                    eCoronaMode coronaShowMode;
                    unsigned char coronaEnableReflection;
                    unsigned char coronaFlareType;
                    unsigned char shadowColorMultiplier;
                    union {
                        struct {
                            unsigned short coronaCheckObstacles : 1;
                            unsigned short fogType : 2;
                            unsigned short withoutCorona : 1;
                            unsigned short onlyAtLongDistance : 1;
                            unsigned short atDay : 1;
                            unsigned short atNight : 1;
                            unsigned short blinking1 : 1;
                            unsigned short : 1;
                            unsigned short coronaOnlyFromBelow : 1;
                            unsigned short blinking2 : 1;
                            unsigned short updateHeightAboveGround : 1;
                            unsigned short checkViewVector : 1;
                            unsigned short blinking3 : 1;
                        };
                        unsigned short int_value;
                    } flags;
                    char coronaTexName[24];
                    char shadowTexName[24];
                    unsigned char shadowZDistance;
                    unsigned char viewVector[3];
                } light;

                struct tParticle {
                    char name[24];
                } particle;

                struct tAttractor {
                    enum eType {
                        ATM_ATTRACTOR,
                        SEAT_ATTRACTOR,
                        STOP_ATTRACTOR,
                        PIZZA_ATTRACTOR,
                        SHELTER_ATTRACTOR,
                        TRIGGER_SCRIPT_ATTRACTOR,
                        LOOK_AT_ATTRACTOR,
                        SCRIPTED_ATTRACTOR,
                        PARK_ATTRACTOR,
                        STEP_ATTRACTOR
                    };

                    eType attractorType;
                    point right;
                    point top;
                    point at;
                    char scriptName[8];
                    unsigned char existingProbability;
                    unsigned char unknown1;
                    unsigned char unknown2;
                } pedAttractor;

                struct SunGlare {
                } sunGlare;

                //struct EnEx {
                //    gtaRwReal angle;
                //    gtaRwReal radiusX;
                //    gtaRwReal radiusY;
                //    gtaRwV3d exitPosn;
                //    gtaRwReal exitMarkerAngle;
                //    gtaRwUInt16 interiorId;
                //    union {
                //        gtaRwUInt8 flags1;
                //        struct {
                //            gtaRwUInt8 unknownInterior : 1;
                //            gtaRwUInt8 unknownPairing : 1;
                //            gtaRwUInt8 createLinkedPairing : 1;
                //            gtaRwUInt8 rewardInterior : 1;
                //            gtaRwUInt8 usedRewardIntrance : 1;
                //            gtaRwUInt8 carsAndAircraft : 1;
                //            gtaRwUInt8 bikesAndMotorcycles : 1;
                //            gtaRwUInt8 disableOnFoot : 1;
                //        };
                //    };
                //    gtaRwUInt8 skyColor;
                //    gtaRwChar interiorName[8];
                //    gtaRwUInt8 timeOn;
                //    gtaRwUInt8 timeOff;
                //    union {
                //        gtaRwUInt8 flags2;
                //        struct {
                //            gtaRwUInt8 acceptNpcGroup : 1;
                //            gtaRwUInt8 foodDateFlag : 1;
                //            gtaRwUInt8 unknownBurglary : 1;
                //            gtaRwUInt8 disableExit : 1;
                //            gtaRwUInt8 burglaryAccess : 1;
                //            gtaRwUInt8 enteredWithoutExit : 1;
                //            gtaRwUInt8 enableAccess : 1;
                //            gtaRwUInt8 deleteEnex : 1;
                //        };
                //    };
                //} enex;

                struct Roadsign {
                    enum eLinesNumber {
                        LINES_4,
                        LINES_1,
                        LINES_2,
                        LINES_3,
                    };

                    enum eSymbolsInLine {
                        SYMBOLS_16,
                        SYMBOLS_2,
                        SYMBOLS_4,
                        SYMBOLS_8,
                    };

                    enum eTextColor {
                        WHITE,
                        BLACK,
                        GREY,
                        RED
                    };

                    point2d scale;
                    point rotation;
                    eLinesNumber linesNumber;
                    eSymbolsInLine symbolsInLine;
                    eTextColor textColor;
                    char textLines[4][16];
                } roadsign;

                struct Wheel {
                    int modelId;
                } wheel;

                struct Cover {
                    point2d direction;
                    unsigned int type;
                } cover;

                struct Escalator {
                    enum eDirection {
                        DOWN,
                        UP
                    };

                    point bottom;
                    point top;
                    point end;
                    eDirection direction;
                } escalator;
            };
        };

        struct ide_file {
            ideipl_section<tObject> objs;
            ideipl_section<tTimeObject> tobj;
            ideipl_section<tHierObject> hier;
            ideipl_section<tAnimObject> anim;
            ideipl_section<tWeapObject> weap;
            ideipl_section<tTxdParent> txdp;

            ide_file(std::string const &filename);

            template<typename T>
            void forAllObjects(T callback) {
                for (auto &obj : objs)
                    callback(obj, obj_objs);
                for (auto &obj : tobj)
                    callback(obj, obj_tobj);
                for (auto &obj : anim)
                    callback(obj, obj_anim);
            }
        };
    };

}