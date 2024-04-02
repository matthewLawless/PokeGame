#include "heap.h"

class Pokemon{

    public:
        int id;
        char * identifier;
        int species_id;
        int height;
        int weight;
        int base_experience;
        int order;
        int is_default;

    Pokemon(int id, char * identifier, int species_id, int height, int weight, int base_experience, int order, int is_default){

        this->id = id;
        this->identifier = identifier;
        this->species_id = species_id;
        this->height = height;
        this->weight = weight;
        this->base_experience = base_experience;
        this->order = order;
        this->is_default = is_default;

    }

};

class Move{

    public:
        int id;
        char * identifier;
        int generation_id;
        int type_id;
        int power;
        int pp;
        int accuracy;
        int priority;
        int target_id;
        int damage_class_id;
        int effect_id;
        int effect_chance;
        int contest_type_id;
        int super_contest_effect_id;

    Move(int id,
        char * identifier,
        int generation_id,
        int type_id,
        int power,
        int pp,
        int accuracy,
        int priority,
        int target_id,
        int damage_class_id,
        int effect_id,
        int effect_chance,
        int contest_type_id,
        int super_contest_effect_id){

            this->id = id;
            this->identifier = identifier;
            this->generation_id = generation_id;
            this->type_id = type_id;
            this->power = power;
            this->pp = pp;
            this->accuracy = accuracy;
            this->priority = priority;
            this->target_id = target_id;
            this->damage_class_id = damage_class_id;
            this->effect_id = effect_id;
            this->effect_chance = effect_chance;
            this->contest_type_id = contest_type_id;
            this->super_contest_effect_id = super_contest_effect_id;


        }

};


class NPC{

    public:

        int row;
        int col;

        int prow;
        int pcol;

        char type;

        int Path;
        int PMart;
        int PCntr;
        int TGras;
        int SGras;
        int Mtn;
        int Forest;

        //0 if false, 1 if true
        int foughtPC;


}NPC_t;

typedef struct npcNode{

    int costOfNextMove;

    NPC *npc;

    int sequenceNumber;

}npcNode_t;

typedef struct playerChar{

    int row;
    int col;
    heap_node_t *heapNode;
    

}playerChar_t;

struct Point{
    int x;
    int y;
};

class Map{
    public:
        int eG;
        int wG;
        int nG;
        int sG;
        struct Point worldCoordinates;
        char screen[21][80];
        char terrainOnly[21][80];
        NPC *characterTracker[21][80];
        int rivalHeatMap[21][80];
        int hikerHeatMap[21][80];
        int npcCount;
        NPC *npcList[1000];
        playerChar_t *pc;
        npcNode_t *heapNodePC;
        heap_t *turnQ;
        npcNode_t *heapNodeList[1000];

}map_t;

typedef struct World{
    Map *worldMaps[401][401];
    struct Point currentCoordinates;
}world_t;

