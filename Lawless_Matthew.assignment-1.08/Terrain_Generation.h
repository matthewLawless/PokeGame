#include "heap.h"

// class LivePokemon{
//     public:
//         int id;
//         std::string identifier;
//         int level;
//         std::vector<Move> moves;
//         int hp;
//         int attack;
//         int defense;
//         int speed;
//         int specialAttack;
//         int specialDefense;
//         //male = t, female = f
//         bool gender;
//         bool isShiny;

//     LivePokemon(int id, std::string identifier, int level, int hp, int attack, int defense, int speed, int specialAttack, int specialDefense, bool gender, bool isShiny){

//         this->id = id;
//         this->identifier = identifier;
//         this->level = level;
//         this->hp = hp;
//         this->attack = attack;
//         this->defense = defense;
//         this->speed = speed;
//         this->specialAttack = specialAttack;
//         this->specialDefense = specialDefense;
//         this->gender = gender;
//         this->isShiny = isShiny;


//     }

// };

class Pokemon{

    public:
        int id;
        std::string identifier;
        int species_id;
        int height;
        int weight;
        int base_experience;
        int order;
        int is_default;

    Pokemon(int id, std::string identifier, int species_id, int height, int weight, int base_experience, int order, int is_default){

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
        std::string identifier;
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
        int contest_effect_id;
        int super_contest_effect_id;

    Move(int id,
        std::string identifier,
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
        int contest_effect_id,
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
            this->contest_effect_id = contest_effect_id;
            this->super_contest_effect_id = super_contest_effect_id;


        }

};


class Pokemon_Move{

    public:
        int pokemon_id;
        int version_group_id;
        int move_id;
        int pokemon_move_method_id;
        int level;
        int order;

    Pokemon_Move(
        int pokemon_id,
        int version_group_id,
        int move_id,
        int pokemon_move_method_id,
        int level,
        int order){
            this->pokemon_id = pokemon_id;
            this->version_group_id = version_group_id;
            this->move_id = move_id;
            this->pokemon_move_method_id = pokemon_move_method_id;
            this->level = level;
            this->order = order;
        }
};

class Pokemon_Species{

    public:
        int id;
        std::string identifier;
        int generation_id;
        int evolves_from_species_id;
        int evolution_chain_id;
        int color_id;
        int shape_id;
        int habitat_id;
        int gender_rate;
        int capture_rate;
        int base_happiness;
        int is_baby;
        int hatch_counter;
        int has_gender_differences;
        int growth_rate_id;
        int forms_switchable;
        int is_legendary;
        int is_mythical;
        int order;
        int conquest_order;

    Pokemon_Species(
        int id,
        std::string identifier,
        int generation_id,
        int evolves_from_species_id,
        int evolution_chain_id,
        int color_id,
        int shape_id,
        int habitat_id,
        int gender_rate,
        int capture_rate,
        int base_happiness,
        int is_baby,
        int hatch_counter,
        int has_gender_differences,
        int growth_rate_id,
        int forms_switchable,
        int is_legendary,
        int is_mythical,
        int order,
        int conquest_order
    ){
        this->id = id;
        this->identifier = identifier;
        this->generation_id = generation_id;
        this->evolves_from_species_id = evolves_from_species_id;
        this->evolution_chain_id = evolution_chain_id;
        this->color_id = color_id;
        this->shape_id = shape_id;
        this->habitat_id = habitat_id;
        this->gender_rate = gender_rate;
        this->capture_rate = capture_rate;
        this->base_happiness = base_happiness;
        this->is_baby = is_baby;
        this->hatch_counter = hatch_counter;
        this->has_gender_differences = has_gender_differences;
        this->growth_rate_id = growth_rate_id;
        this->forms_switchable = forms_switchable;
        this->is_legendary = is_legendary;
        this->is_mythical = is_mythical;
        this->order = order;
        this->conquest_order = conquest_order;
    }
};


class Experience{

    public:
        int growth_rate_id;
        int level;
        int experience;

        Experience(

            int growth_rate_id,
            int level,
            int experience

        ){

            this->growth_rate_id = growth_rate_id;
            this->level = level;
            this->experience = experience;

        }
};

class Type_Name{

    public:
        int type_id;
        int local_language_id;
        std::string name;

        Type_Name(
            int type_id,
            int local_language_id,
            std::string name
        ){

            this->type_id = type_id;
            this->local_language_id = local_language_id;
            this->name = name;

        }
};

class Pokemon_Stat{

    public:
        int pokemon_id;
        int stat_id;
        int base_stat;
        int effort;

        Pokemon_Stat(

            int pokemon_id,
            int stat_id,
            int base_stat,
            int effort

        ){

            this->pokemon_id = pokemon_id;
            this->stat_id = stat_id;
            this->base_stat = base_stat;
            this->effort = effort;


        }


};

class Stat{

    public:
        int id;
        int damage_class_id;
        std::string identifier;
        int is_battle_only;
        int game_index;

        Stat(

            int id,
            int damage_class_id,
            std::string identifier,
            int is_battle_only,
            int game_index

        ){

            this->id = id;
            this->damage_class_id = damage_class_id;
            this->identifier = identifier;
            this->is_battle_only = is_battle_only;
            this->game_index = game_index;


        }

};

class Pokemon_Type{

    public:
        int pokemon_id;
        int type_id;
        int slot;

        Pokemon_Type(

            int pokemon_id,
            int type_id,
            int slot

        ){
            this->pokemon_id = pokemon_id;
            this->type_id = type_id;
            this->slot = slot;
        }


};


class LivePokemon{
    public:
        int id;
        std::string identifier;
        int level;
        std::vector<Move> moves;
        int hpIV;
        int attackIV;
        int defenseIV;
        int speedIV;
        int specialAttackIV;
        int specialDefenseIV;
        //male = t, female = f
        bool gender;
        bool isShiny;

        int baseHP;
        int baseAttack;
        int baseDefense;
        int baseSpeed;
        int baseSpecialAttack;
        int baseSpecialDefense;

        int hp;
        int attack;
        int defense;
        int speed;
        int specialAttack;
        int specialDefense;


    LivePokemon(int id, std::string identifier, int level, int hpIV, int attackIV, int defenseIV, int speedIV, int specialAttackIV, int specialDefenseIV, bool gender, bool isShiny, int baseHP, int baseAttack, int baseDefense, int baseSpeed, int baseSpecialAttack, int baseSpecialDefense){

        this->id = id;
        this->identifier = identifier;
        this->level = level;
        this->hp = hp;
        this->attack = attack;
        this->defense = defense;
        this->speed = speed;
        this->specialAttack = specialAttack;
        this->specialDefense = specialDefense;
        this->gender = gender;
        this->isShiny = isShiny;
        this->baseHP = baseHP;
        this->baseAttack = baseAttack;
        this->baseDefense = baseDefense;
        this->baseSpeed = baseSpeed;
        this->baseSpecialAttack = baseSpecialAttack;
        this->baseSpecialDefense = baseSpecialDefense;

        this->hp = floor((((this->baseHP + this->hpIV) * 2) * this->level) / 100) + this->level + 10;

        this->attack = floor((((this->baseAttack + this->attackIV) * 2) * this->level) / 100) + 5;

        this->defense = floor((((this->baseDefense + this->defenseIV) * 2) * this->level) / 100) + 5;

        this->speed = floor((((this->baseSpeed + this->speedIV) * 2) * this->level) / 100) + 5;

        this->specialAttack = floor((((this->baseSpecialAttack + this->specialAttackIV) * 2) * this->level) / 100) + 5;

        this->specialDefense = floor((((this->baseSpecialDefense + this->specialDefenseIV) * 2) * this->level) / 100) + 5;


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
    // std::vector<LivePokemon> pokemon;
    

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

