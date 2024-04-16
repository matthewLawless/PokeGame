#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <limits.h>
#include <sstream>
#include <cmath>
#include "Terrain_Generation.h"
#include <ncurses.h>


void dumpBoard(char gameBoard[21][80]);
int max(int a, int b);
int min(int a, int b);
struct Point findPair(struct Point p1, struct Point p2);
void fillRectangle(struct Point p1, struct Point p2, char gameBoard[21][80], char c);
void generatePaths(char gameBoard[21][80], world_t* world, struct Point p);
void generateTerrain(char gameBoard[21][80]);
void printPoint(struct Point p);
bool validatePoint(struct Point p);
void spawnNPC(Map *m, NPC *npc);
int terrainCostPC(char c);
int gateCheck(int r, int c);
void printNpcLocation(int pcrow, int pccol, int row, int col);
void fightInterface(NPC *npc, playerChar *pc);
Map * findMapPCMovingInto(Map *map, world_t *world, int row, int col);
void pokemonEncounter(Map *map);
LivePokemon * generatePokemon(Map *map);
void printLivePokemon(LivePokemon *lp, int y, int x);
void levelUp(LivePokemon *lp);
void giveNPCPokemon(NPC *npc, Map *map);
void trainerBattle(NPC *npc, playerChar *pc);
int computeDamage(Move m, LivePokemon attacker, LivePokemon defender);

std::vector<Pokemon> p;
std::vector<Move> m;
std::vector<Pokemon_Move> pm;
std::vector<Pokemon_Species> ps;
std::vector<Experience> e;
std::vector<Type_Name> tn;
std::vector<Pokemon_Stat> pst;
std::vector<Stat> s;
std::vector<Pokemon_Type> pt;




// struct Point {

//     int x;
//     int y;

// };

typedef struct node{

    int row;
    int col;

    int distance;
    int tCost;

}node_t;

void initPoint(struct Point* p, int x, int y){

    p->x = x;
    p->y = y;

}

// typedef struct playerChar{

//     int row;
//     int col;

// }playerChar_t;

void placePlayerChar(playerChar *PC, Map *map){

    
    while (true){
        int col = (rand() % 78 + 1); // [1, 78]

        if (col != map->nG && col != map->sG){


            int i;
            for (i = 1; i < 20; i++){

                

                if (map->screen[i][col] == '#'){

                    map->screen[i][col] = '@';
                    PC->col = col;
                    PC->row = i;
                    map->pc = PC;

                    NPC *player;
                    player = (NPC *) malloc(sizeof(NPC));
                    player->type = '@';
                    map->characterTracker[col][i] = player;

                    return;


                }

            }

        }

    }

   
}


//array entry
typedef struct arrE {

    int row;
    int col;

}arrE_t;

void initArrE(arrE_t* a, struct Point p){

    a->row = 200 - p.y;
    a->col = p.x + 200;

}

// typedef struct Map {

//     int eG;
//     int wG;
//     int nG;
//     int sG;
//     struct Point worldCoordinates;
//     char screen[21][80];


// }map_t;

void printMap(Map map){

    // printf("eG: %d\n", map.eG);
    // printf("wG: %d\n", map.wG);
    // printf("nG: %d\n", map.nG);
    // printf("sG: %d\n", map.sG);

    clear();
    printw("World coordinates:");
    printPoint(map.worldCoordinates);
    printw("\n");
    printw("\n");
    dumpBoard(map.screen);
    // printf("World coordinates:");
    // printPoint(map.worldCoordinates);
    // printf("NPCs: ");
    // printf("%d", map.npcCount);
    // printf("\n");


}

void initScreen(Map *map){

    
    // map->screen = 
    // char gameBoard[21][80] = map->screen;
    //fill with Ns
    int i, j;
    for (i = 0; i < 21; i++){
        for (j = 0; j < 80; j++){

            map->screen[i][j] = '.';

        }
    }


    //construct the frame of the board
    
    //top and bottom rows
    for (j = 0; j < 80; j++){

        map->screen[0][j] = '%';
        map->screen[20][j] = '%';

    }

    //now do the columns
    for (i = 0; i < 21; i++){

        map->screen[i][0] = '%';
        map->screen[i][79] = '%';

        
    }

}

// typedef struct World {

//     map_t *worldMaps[401][401];

// }world_t;

// void initWorld


void initMap(world_t *world, struct Point h){


    arrE_t h1;
    initArrE(&h1, h);
    world->worldMaps[h1.row][h1.col]->turnQ = NULL;
    world->worldMaps[h1.row][h1.col]->worldCoordinates = h;
    world->worldMaps[h1.row][h1.col]->npcCount = 0;
    initScreen(world->worldMaps[h1.row][h1.col]);
    generateTerrain(world->worldMaps[h1.row][h1.col]->screen);

    generatePaths(world->worldMaps[h1.row][h1.col]->screen, world, h);
    int i, j;
    for (i = 0; i < 21; i++){
        for (j = 0; j < 80; j++){

            world->worldMaps[h1.row][h1.col]->terrainOnly[i][j] = world->worldMaps[h1.row][h1.col]->screen[i][j];
            world->worldMaps[h1.row][h1.col]->characterTracker[i][j] = NULL;
            // world->worldMaps[h1.row][h1.col]->rivalHeatMap[i][j] = 4000;
            // world->worldMaps[h1.row][h1.col]->hikerHeatMap[i][j] = 4000;

        }
    }


}

void printPoint(struct Point p){

    printw("(");
    printw("%d", p.x);
    printw(", ");
    printw("%d", p.y);
    printw(")");
    

}

// typedef struct NPC{


//     int row;
//     int col;

//     int prow;
//     int pcol;

//     char type;

//     int Path;
//     int PMart;
//     int PCntr;
//     int TGras;
//     int SGras;
//     int Mtn;
//     int Forest;


// }NPC_t;

void NPCtoHiker(NPC *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 15;
    npc->SGras = 10;
    npc->Mtn = 4000;
    npc->Forest = 15;


    npc->type = 'h';


}

void NPCtoRival(NPC *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 20;
    npc->SGras = 10;
    npc->Mtn = 4000;
    npc->Forest = 4000;

    npc->type = 'r';

}

void NPCtoPacer(NPC *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 20;
    npc->SGras = 10;
    npc->Mtn = 4000;
    npc->Forest = 4000;

    npc->type = 'p';

}

void NPCtoWanderer(NPC *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 20;
    npc->SGras = 10;
    npc->Mtn = 4000;
    npc->Forest = 4000;

    npc->type = 'w';


}

void NPCtoExplorer(NPC *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 20;
    npc->SGras = 10;
    npc->Mtn = 4000;
    npc->Forest = 4000;

    npc->type = 'e';


}

void printHeatMap(node_t *heatMap[21][80]){

    int i, j;
    for (i = 0; i < 21; i++){

        for (j = 0; j < 80; j++){

            if (heatMap[i][j]->tCost == 4000){

                printf("  ");

            }
            else{

            int dMod = heatMap[i][j]->distance % 100;
            if (dMod == 0 || dMod == 5){

                printf("%d", 0);
                printf("%d", dMod);


            }else{
                printf("%d", dMod);
            }
            }
            if(j != 79){
            printf(" ");
            }

        }
        printf("\n");

    }

    printf("\n");


}


static int32_t path_cmp(const void *key, const void *with) {
  return ((node_t *) key)->distance - ((node_t *) with)->distance;
}



void initNode(node_t *n, int row, int col, int distance, int tCost){

    n->row = row;
    n->col = col;
    n->distance = distance;
    n->tCost = tCost;

}

int terrainCost(NPC npc, char c){

    if (c == '#'){

        return npc.Path;

    }
    else if (c == 'M'){

        return npc.PMart;

    }
    else if (c == 'C'){

        return npc.PCntr;

    }
    else if (c == ':'){

        return npc.TGras;

    }
    else if (c == '.'){

        return npc.SGras;

    }
    else if (c == '^'){

        return npc.Forest;

    }
    else if (c == '%'){

        return npc.Mtn;

    }
    else if (c == '~'){

        //TEMP
        return 4000;

    }
    else if(c == '@'){

        return 10;

    }

    return -1;


}

void addNPC(Map *map, char c){

    NPC *npc = new NPC();
    

    switch (c){

        case 'h':
            NPCtoHiker(npc);
            break;


        case 'r':
            NPCtoRival(npc);
            break;
        case 'p':
            NPCtoPacer(npc);
            break;
        case 'e':
            NPCtoExplorer(npc);
            break;
        case 'w':
            NPCtoWanderer(npc);
            break;
    }

    spawnNPC(map, npc);

    giveNPCPokemon(npc, map);


}

void spawnNPC(Map *m, NPC *npc){

    //just choose a random x and random y, check if that terrain is valid 
    while (true){
        int y1 = (rand() % 19) + 1; // [1, 19]
        int x1 = (rand() % 78 + 1); // [1, 78]

        if ((m->characterTracker[y1][x1] == NULL) && terrainCost(*npc, m->terrainOnly[y1][x1]) < 4000){

            m->characterTracker[y1][x1] = npc;
            m->screen[y1][x1] = npc->type;
            npc->row = y1;
            npc->col = x1;
            npc->prow = y1+1;
            npc->pcol = x1;
            break;

        }

    }
    npc->foughtPC = 0;
    m->npcList[m->npcCount] = npc;
    m->npcCount++;



}

//need some method that takes the map, the pc, the npc, . . ., in as input, and generates the next move 
//of the npc for us to re add to the queue. For now, returns a direction determined by:
//
//      0  1  2
//      7 npc 3
//      6  5  4
//
void generateMove(Map *m, playerChar *p, NPC *npc, arrE_t *arr){

    //Here we don't need to check if the heatMaps are updated, that is something we can do
    //every time we process a move from the character
    arrE_t next;
    int minDistance;
    int rowDifference;
    int colDifference;
    int nextRow;
    int nextCol;
    switch (npc->type){

        case 'h':


        // printHeatMap(m->hikerHeatMap);
        //simple, just check heatMap for hiker
        //just want to find the current location of the hiker, then look at all surrounding 
        //squares on the heatmap, then choose the smallest of those squares.
        minDistance = 4000;

            if (m->screen[npc->row - 1][npc->col] == '@' || m->screen[npc->row - 1][npc->col + 1] == '@' || m->screen[npc->row][npc->col + 1] == '@' || m->screen[npc->row + 1][npc->col + 1] == '@'
                || m->screen[npc->row + 1][npc->col] == '@' || m->screen[npc->row + 1][npc->col - 1] == '@' || m->screen[npc->row][npc->col - 1] == '@' || m->screen[npc->row - 1][npc->col - 1] == '@'){

                    fightInterface(npc, m->pc);
                    return;

            }
            
        
            //north
            if (m->screen[npc->row - 1][npc->col] != '@' && terrainCost(*npc, m->terrainOnly[npc->row - 1][npc->col]) < 4000 && m->hikerHeatMap[npc->row - 1][npc->col] < minDistance && (m->characterTracker[npc->row - 1][npc->col] == NULL)){

                minDistance = m->hikerHeatMap[npc->row - 1][npc->col];
                next.row = npc->row - 1;
                next.col = npc->col;

            }
            //northeast
            if (m->screen[npc->row - 1][npc->col + 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row - 1][npc->col + 1]) < 4000 && m->hikerHeatMap[npc->row - 1][npc->col + 1] < minDistance && (m->characterTracker[npc->row - 1][npc->col + 1] == NULL)){

                minDistance = m->hikerHeatMap[npc->row - 1][npc->col + 1];
                next.row = npc->row - 1;
                next.col = npc->col + 1;

            }
            //east
            if (m->screen[npc->row][npc->col + 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row][npc->col + 1]) < 4000 && m->hikerHeatMap[npc->row][npc->col + 1] < minDistance && (m->characterTracker[npc->row][npc->col + 1] == NULL)){

                minDistance = m->hikerHeatMap[npc->row][npc->col + 1];
                next.row = npc->row;
                next.col = npc->col + 1;


            }
            //southeast
            if (m->screen[npc->row + 1][npc->col + 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row + 1][npc->col + 1]) < 4000 && m->hikerHeatMap[npc->row + 1][npc->col + 1] < minDistance && (m->characterTracker[npc->row + 1][npc->col + 1] == NULL)){

                minDistance = m->hikerHeatMap[npc->row + 1][npc->col + 1];
                next.row = npc->row + 1;
                next.col = npc->col + 1;

            };
            //south
            if (m->screen[npc->row + 1][npc->col] != '@' && terrainCost(*npc, m->terrainOnly[npc->row + 1][npc->col]) < 4000 && m->hikerHeatMap[npc->row + 1][npc->col] < minDistance && (m->characterTracker[npc->row + 1][npc->col] == NULL)){

                minDistance = m->hikerHeatMap[npc->row + 1][npc->col];
                next.row = npc->row + 1;
                next.col = npc->col;

            }
            //southwest
            if (m->screen[npc->row + 1][npc->col - 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row + 1][npc->col - 1]) < 4000 && m->hikerHeatMap[npc->row + 1][npc->col - 1] < minDistance && (m->characterTracker[npc->row + 1][npc->col - 1] == NULL)){

                minDistance = m->hikerHeatMap[npc->row + 1][npc->col - 1];
                next.row = npc->row + 1;
                next.col = npc->col - 1;

            };
            //west
            if (m->screen[npc->row][npc->col - 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row][npc->col - 1]) < 4000 && m->hikerHeatMap[npc->row][npc->col - 1] < minDistance && (m->characterTracker[npc->row][npc->col - 1] == NULL)){

                minDistance = m->hikerHeatMap[npc->row][npc->col - 1];
                next.row = npc->row;
                next.col = npc->col - 1;

            }
            //northwest
            if (m->screen[npc->row - 1][npc->col - 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row - 1][npc->col - 1]) < 4000 && m->hikerHeatMap[npc->row - 1][npc->col - 1] < minDistance && (m->characterTracker[npc->row - 1][npc->col - 1] == NULL)){

                minDistance = m->hikerHeatMap[npc->row - 1][npc->col - 1];
                next.row = npc->row - 1;
                next.col = npc->col - 1;

            }

            break;





        case 'r':

            //simple, just check heatMap for Rival

            // printHeatMap(m->rivalHeatMap);

            minDistance = 4000;

            if (m->screen[npc->row - 1][npc->col] == '@' || m->screen[npc->row - 1][npc->col + 1] == '@' || m->screen[npc->row][npc->col + 1] == '@' || m->screen[npc->row + 1][npc->col + 1] == '@'
                || m->screen[npc->row + 1][npc->col] == '@' || m->screen[npc->row + 1][npc->col - 1] == '@' || m->screen[npc->row][npc->col - 1] == '@' || m->screen[npc->row - 1][npc->col - 1] == '@'){

                fightInterface(npc, m->pc);
                return;

            }
            
            //north
            if (m->screen[npc->row - 1][npc->col] != '@' && terrainCost(*npc, m->terrainOnly[npc->row - 1][npc->col]) < 4000 && m->rivalHeatMap[npc->row - 1][npc->col] < minDistance && (m->characterTracker[npc->row - 1][npc->col] == NULL)){

                minDistance = m->rivalHeatMap[npc->row - 1][npc->col];
                next.row = npc->row - 1;
                next.col = npc->col;

            }
            //northeast
            if (m->screen[npc->row - 1][npc->col + 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row - 1][npc->col + 1]) < 4000 && m->rivalHeatMap[npc->row - 1][npc->col + 1] < minDistance && (m->characterTracker[npc->row - 1][npc->col + 1] == NULL)){

                minDistance = m->rivalHeatMap[npc->row - 1][npc->col + 1];
                next.row = npc->row - 1;
                next.col = npc->col + 1;

            }
            //east
            if (m->screen[npc->row][npc->col + 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row][npc->col + 1]) < 4000 && m->rivalHeatMap[npc->row][npc->col + 1] < minDistance && (m->characterTracker[npc->row][npc->col + 1] == NULL)){

                minDistance = m->rivalHeatMap[npc->row][npc->col + 1];
                next.row = npc->row;
                next.col = npc->col + 1;


            }
            //southeast
            if (m->screen[npc->row + 1][npc->col + 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row + 1][npc->col + 1]) < 4000 && m->rivalHeatMap[npc->row + 1][npc->col + 1] < minDistance && (m->characterTracker[npc->row + 1][npc->col + 1] == NULL)){

                minDistance = m->rivalHeatMap[npc->row + 1][npc->col + 1];
                next.row = npc->row + 1;
                next.col = npc->col + 1;

            };
            //south
            if (m->screen[npc->row + 1][npc->col] != '@' && terrainCost(*npc, m->terrainOnly[npc->row + 1][npc->col]) < 4000 && m->rivalHeatMap[npc->row + 1][npc->col] < minDistance && (m->characterTracker[npc->row + 1][npc->col] == NULL)){

                minDistance = m->rivalHeatMap[npc->row + 1][npc->col];
                next.row = npc->row + 1;
                next.col = npc->col;

            }
            //southwest
            if (m->screen[npc->row + 1][npc->col - 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row + 1][npc->col - 1]) < 4000 && m->rivalHeatMap[npc->row + 1][npc->col - 1] < minDistance && (m->characterTracker[npc->row + 1][npc->col - 1] == NULL)){

                minDistance = m->rivalHeatMap[npc->row + 1][npc->col - 1];
                next.row = npc->row + 1;
                next.col = npc->col - 1;

            };
            //west
            if (m->screen[npc->row][npc->col - 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row][npc->col - 1]) < 4000 && m->rivalHeatMap[npc->row][npc->col - 1] < minDistance && (m->characterTracker[npc->row][npc->col - 1] == NULL)){

                minDistance = m->rivalHeatMap[npc->row][npc->col - 1];
                next.row = npc->row;
                next.col = npc->col - 1;

            }
            //northwest
            if (m->screen[npc->row - 1][npc->col - 1] != '@' && terrainCost(*npc, m->terrainOnly[npc->row - 1][npc->col - 1]) < 4000 && m->rivalHeatMap[npc->row - 1][npc->col - 1] < minDistance && (m->characterTracker[npc->row - 1][npc->col - 1] == NULL)){

                minDistance = m->rivalHeatMap[npc->row - 1][npc->col - 1];
                next.row = npc->row - 1;
                next.col = npc->col - 1;

            }
            break;



        case 'p':

        //check where this npc is on the nap

        //need to find the direction we were moving
        

        rowDifference = (npc->row - npc->prow);
        colDifference = (npc->col - npc->pcol);

        nextRow = rowDifference + npc->row;
        nextCol = colDifference + npc->col;

        if (m->screen[nextRow][nextCol] == '@' && !npc->foughtPC){

            fightInterface(npc, m->pc);
            return;

        }

        if (m->screen[nextRow][nextCol] != '@' && terrainCost(*npc, m->terrainOnly[nextRow][nextCol]) < 4000 && m->characterTracker[nextRow][nextCol] == NULL && m->terrainOnly[nextRow][nextCol] == m->terrainOnly[npc->row][npc->col]){

            
            next.col = nextCol;
            next.row = nextRow;


        }
        else{

            //go backwards
            next.col = npc->pcol;
            next.row = npc->prow;


        }


            break;
        case 'w':

        //check which terrain we are currently in, then check which way we are moving
        //if the block in that direction is the same terrain we are in continue that way
        //else, gen a random number between 1 and 7 to choose between the remaining 7 directions.
        //make sure to check that this direction is indeed legitimate

        rowDifference = (npc->row - npc->prow);
        colDifference = (npc->col - npc->pcol);

        nextRow = rowDifference + npc->row;
        nextCol = colDifference + npc->col;

        if (m->screen[nextRow][nextCol] == '@' && !npc->foughtPC){

            fightInterface(npc, m->pc);
            return;

        }

        if (m->screen[nextRow][nextCol] != '@' && m->terrainOnly[npc->row][npc->col] == m->terrainOnly[nextRow][nextCol] && m->characterTracker[nextRow][nextCol] == NULL){

            
            next.col = nextCol;
            next.row = nextRow;


        }
        else{

            //for now, just turn aorund, eventually add the randomizer in here

            next.row = npc->prow;
            next.col = npc->pcol;
            
            int ranNum = (rand() % 7); // [0, 7]

            //north
            if (m->screen[npc->row - 1][npc->col] != '@' && ranNum == 0 && m->terrainOnly[npc->row - 1][npc->col] == m->terrainOnly[npc->row][npc->col]){

                next.col = npc->col;
                next.row = npc->row - 1;

            }
            //northeast
            if (m->screen[npc->row - 1][npc->col + 1] != '@' && ranNum == 1 && m->terrainOnly[npc->row - 1][npc->col + 1] == m->terrainOnly[npc->row][npc->col]){

                next.col = npc->col + 1;
                next.row = npc->row - 1;

            }
            //east
            if (m->screen[npc->row][npc->col + 1] != '@' && ranNum == 2 && m->terrainOnly[npc->row][npc->col + 1] == m->terrainOnly[npc->row][npc->col]){

                next.col = npc->col + 1;
                next.row = npc->row;

            }
            //southeast
            if (m->screen[npc->row + 1][npc->col + 1] != '@' && ranNum == 3 && m->terrainOnly[npc->row + 1][npc->col + 1] == m->terrainOnly[npc->row][npc->col]){

                next.col = npc->col + 1;
                next.row = npc->row + 1;

            }
            //south
            if (m->screen[npc->row + 1][npc->col] != '@' && ranNum == 4 && m->terrainOnly[npc->row + 1][npc->col] == m->terrainOnly[npc->row][npc->col]){

                next.col = npc->col;
                next.row = npc->row + 1;

            }
            //southwest
            if (m->screen[npc->row + 1][npc->col - 1] != '@' && ranNum == 5 && m->terrainOnly[npc->row + 1][npc->col - 1] == m->terrainOnly[npc->row][npc->col]){

                next.col = npc->col - 1;
                next.row = npc->row + 1;

            }
            //west
            if (m->screen[npc->row][npc->col - 1] != '@' && ranNum == 6 && m->terrainOnly[npc->row][npc->col - 1] == m->terrainOnly[npc->row][npc->col]){

                next.col = npc->col - 1;
                next.row = npc->row;

            }
            //northwest
            if (m->screen[npc->row - 1][npc->col - 1] != '@' && ranNum == 7 && m->terrainOnly[npc->row - 1][npc->col - 1] == m->terrainOnly[npc->row][npc->col]){

                next.col = npc->col - 1;
                next.row = npc->row - 1;

            }

            


        }


            break;



        case 's':
            //dont move
            //error if this is in the queue tbh 
            break;



        case 'e':
            //
            
        rowDifference = (npc->row - npc->prow);
        colDifference = (npc->col - npc->pcol);

        nextRow = rowDifference + npc->row;
        nextCol = colDifference + npc->col;

        if (m->screen[nextRow][nextCol] == '@' && !npc->foughtPC){

            fightInterface(npc, m->pc);
            return;

        }

        if (m->screen[nextRow][nextCol] != '@' && terrainCost(*npc, m->terrainOnly[nextRow][nextCol]) < 4000 && m->characterTracker[nextRow][nextCol] == NULL){

            
            next.col = nextCol;
            next.row = nextRow;


        }
        else{

            //for now, just turn aorund, eventually add the randomizer in here
            next.col = npc->pcol;
            next.row = npc->prow;

            int ranNum = (rand() % 7); // [0, 7]

            //north
            if (m->screen[npc->row - 1][npc->col] != '@' && ranNum == 0 && terrainCost(*npc, m->terrainOnly[npc->row - 1][npc->col]) < 4000){

                next.col = npc->col;
                next.row = npc->row - 1;

            }
            //northeast
            if (m->screen[npc->row - 1][npc->col + 1] != '@' && ranNum == 1 && terrainCost(*npc, m->terrainOnly[npc->row - 1][npc->col + 1]) < 4000){

                next.col = npc->col + 1;
                next.row = npc->row - 1;

            }
            //east
            if (m->screen[npc->row][npc->col + 1] != '@' && ranNum == 2 && terrainCost(*npc, m->terrainOnly[npc->row][npc->col + 1]) < 4000){

                next.col = npc->col + 1;
                next.row = npc->row;

            }
            //southeast
            if (m->screen[npc->row + 1][npc->col + 1] != '@' && ranNum == 3 && terrainCost(*npc, m->terrainOnly[npc->row + 1][npc->col + 1]) < 4000){

                next.col = npc->col + 1;
                next.row = npc->row + 1;

            }
            //south
            if (m->screen[npc->row + 1][npc->col] != '@' && ranNum == 4 && terrainCost(*npc, m->terrainOnly[npc->row + 1][npc->col]) < 4000){

                next.col = npc->col;
                next.row = npc->row + 1;

            }
            //southwest
            if (m->screen[npc->row + 1][npc->col - 1] != '@' && ranNum == 5 && terrainCost(*npc, m->terrainOnly[npc->row + 1][npc->col - 1]) < 4000){

                next.col = npc->col - 1;
                next.row = npc->row + 1;

            }
            //west
            if (m->screen[npc->row][npc->col - 1] != '@' && ranNum == 6 && terrainCost(*npc, m->terrainOnly[npc->row][npc->col - 1]) < 4000){

                next.col = npc->col - 1;
                next.row = npc->row;

            }
            //northwest
            if (m->screen[npc->row - 1][npc->col - 1] != '@' && ranNum == 7 && terrainCost(*npc, m->terrainOnly[npc->row - 1][npc->col - 1]) < 4000){

                next.col = npc->col - 1;
                next.row = npc->row - 1;

            }


        }
            break;
        default:
            printf("unknown npc type");
            break;
    }

    arr->row = next.row;
    arr->col = next.col;


}

void findShortestPaths(playerChar *p, Map *map, NPC *npc, Map *m){

    node_t *heatMap[21][80];
    int isInHeap[21][80];
    heap_node_t *heapNodeMap[21][80];

    heap_t h;
    heap_init(&h, path_cmp, NULL);

    
    //init all nodes to inf
    int i, j;
    for (i = 0; i < 21; i++){

        for (j = 0; j < 80; j++){

            isInHeap[i][j] = 1;

            node_t *newNode;
            newNode = (node_t *) malloc(sizeof(node_t));

            if (i == p->row && j == p->col){

                int tc; 
                tc = terrainCost(*npc, map->terrainOnly[i][j]);
                initNode(newNode, i, j, 0, tc);

            }
            else{


                int tc = terrainCost(*npc, map->terrainOnly[i][j]);
                

                initNode(newNode, i, j, 4000, tc);



                


            }
            
                heapNodeMap[i][j] = heap_insert(&h, newNode);
                heatMap[i][j] = newNode;

        }

    }


    // printHeatMap(heatMap);

    //we put all nodes into heap;
    node_t *curNode;

    while ((curNode = (node_t *) heap_remove_min(&h))){

        // printHeatMap(heatMap);
        // usleep(250000);
        // printf("%d\n", h.size);

        if (curNode->tCost == 4000){

            continue;

        }
    

        //mark as visited/remove from heap
        isInHeap[curNode->row][curNode->col] = 0;

        int r = curNode->row;
        int c = curNode->col;

        //need to find all neighbors and check if they are legit

        //logically north
        if ((r - 1 < 21 && r - 1 > -1 && c > -1 && c < 80) && (isInHeap[r - 1][c])){
            int alt = heatMap[r][c]->distance + terrainCost(*npc, map->terrainOnly[r][c]);
            if (alt < heatMap[r-1][c]->distance){

                heatMap[r-1][c]->distance = alt;
                // heap_decrease_key(&h, heapNodeMap[r-1][c], heatMap[r-1][c]);
                heap_decrease_key_no_replace(&h, heapNodeMap[r-1][c]);

            }

        }

        
        //logically northeast    
        if ((r - 1 < 21 && r - 1 > -1 && c + 1 > -1 && c + 1< 80) && (isInHeap[r - 1][c + 1])){

            int alt = heatMap[r][c]->distance + terrainCost(*npc, map->terrainOnly[r][c]);
            if (alt < heatMap[r-1][c+1]->distance){

                heatMap[r-1][c+1]->distance = alt;
                // heap_decrease_key(&h, heapNodeMap[r-1][c+1], heatMap[r-1][c+1]);
                heap_decrease_key_no_replace(&h, heapNodeMap[r-1][c+1]);


            }

        }

        //logically northwest
        if ((r - 1 < 21 && r - 1 > -1 && c - 1 > -1 && c - 1 < 80) && (isInHeap[r - 1][c - 1])){

            int alt = heatMap[r][c]->distance + terrainCost(*npc, map->terrainOnly[r][c]);
            if (alt < heatMap[r-1][c-1]->distance){

                heatMap[r-1][c-1]->distance = alt;
                // heap_decrease_key(&h, heapNodeMap[r-1][c-1], heatMap[r-1][c-1]);
                heap_decrease_key_no_replace(&h, heapNodeMap[r-1][c-1]);


            }



        }
        //logically south
        if ((r + 1 < 21 && r + 1 > -1 && c > -1 && c < 80) && (isInHeap[r + 1][c])){


            int alt = heatMap[r][c]->distance + terrainCost(*npc, map->terrainOnly[r][c]);
            if (alt < heatMap[r+1][c]->distance){

                heatMap[r+1][c]->distance = alt;
                // heap_decrease_key(&h, heapNodeMap[r+1][c], heatMap[r+1][c]);
                heap_decrease_key_no_replace(&h, heapNodeMap[r+1][c]);

            }


        }

        //logically southeast
        if ((r + 1 < 21 && r + 1 > -1 && c + 1 > -1 && c + 1 < 80) && (isInHeap[r + 1][c + 1])){

            int alt = heatMap[r][c]->distance + terrainCost(*npc, map->terrainOnly[r][c]);
            if (alt < heatMap[r+1][c+1]->distance){

                heatMap[r+1][c+1]->distance = alt;
                // heap_decrease_key(&h, heapNodeMap[r+1][c+1], heatMap[r+1][c+1]);
                heap_decrease_key_no_replace(&h, heapNodeMap[r+1][c+1]);

            }

        }
        //logically southwest
        if ((r + 1 < 21 && r + 1 > -1 && c - 1 > -1 && c - 1 < 80) && (isInHeap[r + 1][c - 1])){

            int alt = heatMap[r][c]->distance + terrainCost(*npc, map->terrainOnly[r][c]);
            if (alt < heatMap[r+1][c-1]->distance){

                heatMap[r+1][c-1]->distance = alt;
                // heap_decrease_key(&h, heapNodeMap[r+1][c-1], heatMap[r+1][c-1]);
                heap_decrease_key_no_replace(&h, heapNodeMap[r+1][c-1]);

            }

        }

        //logically east
        if ((r < 21 && r > -1 && c + 1 > -1 && c + 1 < 80) && (isInHeap[r][c + 1])){

            int alt = heatMap[r][c]->distance + terrainCost(*npc, map->terrainOnly[r][c]);
            if (alt < heatMap[r][c+1]->distance){

                heatMap[r][c+1]->distance = alt;
                // heap_decrease_key(&h, heapNodeMap[r][c+1], heatMap[r][c+1]);
                heap_decrease_key_no_replace(&h, heapNodeMap[r][c+1]);

            }

        }

        //logically west
        if ((r < 21 && r > -1 && c - 1 > -1 && c - 1 < 80) && (isInHeap[r][c - 1])){

            int alt = heatMap[r][c]->distance + terrainCost(*npc, map->terrainOnly[r][c]);
            if (alt < heatMap[r][c-1]->distance){

                heatMap[r][c-1]->distance = alt;
                // heap_decrease_key(&h, heapNodeMap[r][c-1], heatMap[r][c-1]);
                heap_decrease_key_no_replace(&h, heapNodeMap[r][c-1]);

            }

        }





            // for (i = 0; i < 21; i++){

            //         for (j = 0; j < 80; j++){

            //             printf("%d ", isInHeap[i][j]);
                        

            //         }
            //         printf("\n");

            //     }



    }
    if (npc->type == 'r'){
        for (i = 0; i < 21; i++){

            for (j = 0; j < 80; j++){

                m->rivalHeatMap[i][j] = heatMap[i][j]->distance;

            }

        }
    }
    else{

        for (i = 0; i < 21; i++){

            for (j = 0; j < 80; j++){

                m->hikerHeatMap[i][j] = heatMap[i][j]->distance;

            }

        }




    }
    // printHeatMap(heatMap);





}

void computeHeatMaps(Map *map){





}

void generateTerrain(char gameBoard[21][80]){



    //Water

    struct Point w1, w2;

    w1.x = (rand() % 77) + 1; //[1, 78]
    w1.y = (rand() % 17) + 1; //[1, 18]
    w2.x = (rand() % 77) + 1; //[1, 78]
    w2.y = (rand() % 17) + 1; //[1, 18]

    fillRectangle(w1, w2, gameBoard, '~');

    // dumpBoard(gameBoard);

    //Forest

    struct Point f1, f2;
    
    f1.x = (rand() % 77) + 1; //[1, 78]
    f1.y = (rand() % 17) + 1; //[1, 18]
    f2.x = (rand() % 77) + 1; //[1, 78]
    f2.y = (rand() % 17) + 1; //[1, 18]

    fillRectangle(f1, f2, gameBoard, '^');

    // dumpBoard(gameBoard);

   //find two disjoint regions of tall grass
   //generate two points
   struct Point p, q;

    p.x = (rand() % 20) + 30; //[30, 50]
    p.y = (rand() % 6) + 7; //[7, 13]
    q.x = (rand() % 20) + 30;
    q.y = (rand() % 6) + 7;

    //figure out where the points are relative to each other in order to ensure
    //that when we generate their rectangle they are disjoint

    struct Point p1 = findPair(p, q);
    struct Point q1 = findPair(q, p);

    fillRectangle(p, p1, gameBoard, ':');
    fillRectangle(q, q1, gameBoard, ':');

    // dumpBoard(gameBoard);
    // generatePaths(gameBoard);

}



//this may be a confusing name, but the pc will get one of these too!
// typedef struct npcNode{

//     int costOfNextMove;

//     NPC_t *npc;

//     int sequenceNumber;

// }npcNode_t;

void initNpcNode(npcNode_t *node, int cost, NPC *guy, int seqNum){

    node->costOfNextMove = cost;
    node->npc = guy;
    node->sequenceNumber = seqNum;



}

static int32_t costCompare(const void *key, const void *with) {
  return ((npcNode_t *) key)->costOfNextMove - ((npcNode_t *) with)->costOfNextMove + (((npcNode_t *) key))->sequenceNumber - ((npcNode_t *) with)->sequenceNumber;
}

void moveGuy(Map *map, NPC *npc, arrE_t dest){

    arrE_t curLoc;
    curLoc.row = npc->row;
    curLoc.col = npc->col;

    npc->row = dest.row;
    npc->col = dest.col;

    map->characterTracker[curLoc.row][curLoc.col] = NULL;
    map->characterTracker[dest.row][dest.col] = npc;
    map->screen[curLoc.row][curLoc.col] = map->terrainOnly[curLoc.row][curLoc.col];
    map->screen[dest.row][dest.col] = npc->type;

    npc->pcol = curLoc.col;
    npc->prow = curLoc.row;





}

void printNPC(NPC *npc){

    if (npc == NULL){

        printw("null pointer");

    }
    else{
        printw("row %d\n", npc->row);
        printw("col %d\n", npc->col);
        printw("type %c\n\n", npc->type);
    }

    printw("done printing this npc\n");

}

void printNpcNode(npcNode_t *node){

    if (node->npc == NULL){

        printf("this node is the pc!!!\n");

    }
    else{
        printf("npc:\n");
        printNPC(node->npc);
        printf("sequence number %d\n", node->sequenceNumber);
    }

    printf("done printing this npcnode\n");

}

void movePC(int row, int col, Map *map){

    int oldrow = map->pc->row;
    int oldcol = map->pc->col;

    map->pc->col = col;
    map->pc->row = row;
    map->screen[oldrow][oldcol] = map->terrainOnly[oldrow][oldcol];
    map->screen[row][col] = '@';
    NPC *player = map->characterTracker[oldrow][oldcol];
    map->characterTracker[oldrow][oldcol] = NULL;
    map->characterTracker[row][col] = player;

};

Map * simulateGame(Map *map, world_t *world){

    //Need to make some sort of player object that will act as our vector to use the heap
    //Need to make sure that players are init into the characterMap

    // NPC_t *dummyHiker;
    // NPC_t *dummyRival;
    // dummyHiker = malloc(sizeof(NPC_t));
    // dummyRival = malloc(sizeof(NPC_t));
    // NPCtoHiker(dummyHiker);
    // NPCtoRival(dummyRival);
    

    NPC dummyHiker;
    NPC dummyRival;
    NPCtoHiker(&dummyHiker);
    NPCtoRival(&dummyRival);


    findShortestPaths(map->pc, map, &dummyHiker, map);
    findShortestPaths(map->pc, map, &dummyRival, map);

    heap_t h;
    int currentCost;

    if (map->turnQ == NULL){
        //create heap and assign it to the current map
        map->turnQ = &h;
        heap_init(&h, costCompare, NULL);

        //first create a heap item for the pc
        npcNode_t *playerChar;
        playerChar = (npcNode_t *) malloc(sizeof(npcNode_t));
        //(for now, just init next move to ten, later this will be variable obviously)
        initNpcNode(playerChar, 0, NULL, 0);
        map->heapNodePC = playerChar;
        heap_insert(&h, playerChar);


        

        
        int i;
        for (i = 0; i < map->npcCount; i++){

            npcNode_t *currentNPC;
            currentNPC = (npcNode_t *) malloc(sizeof(npcNode_t));
            // printNPC(map->npcList[i]);
            initNpcNode(currentNPC, 0, map->npcList[i], i + 1);
            map->heapNodeList[i] = currentNPC;

            // printNpcNode(currentNPC);

            heap_insert(&h, currentNPC);

    }
    }
    else{

        clear();
        h = *(map->turnQ);

        //if (h.size == 0){
        // printMap(*map);
        // int curMin = ((npcNode_t *) heap_peek_min(&h))->costOfNextMove;
        int i;
        for (i = h.size; i < map->npcCount; i++){

            heap_insert(&h, map->heapNodeList[i]);

        }
        npcNode_t *newPc;
        newPc = (npcNode_t *) malloc(sizeof(npcNode_t));
        // initNpcNode(newPc, ((npcNode_t *) heap_peek_min(&h))->costOfNextMove + 10, NULL, 0);
        map->heapNodePC->costOfNextMove = map->heapNodePC->costOfNextMove + 10;
        heap_insert(&h, map->heapNodePC);
        printMap(*map);
        //}
        
    }

    
    // //first create a heap item for the pc
    // npcNode_t *playerChar;
    // playerChar = (npcNode_t *) malloc(sizeof(npcNode_t));
    // //(for now, just init next move to ten, later this will be variable obviously)
    // initNpcNode(playerChar, 0, NULL, 0);
    // heap_insert(&h, playerChar);


    

    // int currentCost;
    // int i;
    // for (i = 0; i < map->npcCount; i++){

    //     npcNode_t *currentNPC;
    //     currentNPC = (npcNode_t *) malloc(sizeof(npcNode_t));
    //     // printNPC(map->npcList[i]);
    //     initNpcNode(currentNPC, 0, map->npcList[i], i + 1);

    //     // printNpcNode(currentNPC);

    //     heap_insert(&h, currentNPC);

    // }

    

    npcNode_t *current;
    while ((current = (npcNode_t *) heap_remove_min(&h))){
        
        
        // printNpcNode(current);

        //this if check if the current turn is the players or an npcs
        //the player has a NULL npc pointer
        // printNPC(current->npc);
        // if (current->npc == NULL){

        //     //gen a new pc node with + 10 cost just so it takes a turn;

        //     //IF THE PC MOVES, THEN WE REGEN THE MAPS

            

        //     current->costOfNextMove = current->costOfNextMove + 10;
        //     heap_insert(&h, current);
        //     printf("right before printMap\n");
        //     printMap(*map);
        //     printf("right after printMap\n");

            

            

        // }
        // else{

        //     printf("entered else statement");
            
        //     arrE_t currentNextSpot;

        //     // if (*(map->pc) != NULL && *(map->pc) && *(current->npc)){



        //     // }
            
        //     // *map;

        //     // *(map->pc);

        //     // *(current->npc);
        //     // dummyFunction(map, map->pc, current->npc);

        //     generateMove(*map, *(map->pc), *(current->npc), &currentNextSpot);
        //     currentCost = terrainCost(*(current->npc), map->terrainOnly[currentNextSpot.row][currentNextSpot.col]);

        //     //actually move there, like update the visuals and map stuff
        //     move(map, current->npc, currentNextSpot);

        //     current->costOfNextMove = current->costOfNextMove + currentCost;
        //     heap_insert(&h, current);
            


        // }
        // printf("right above if");
        if (current->npc != NULL){

            // printf("entered else statement");
            
            arrE_t *currentNextSpot;
            currentNextSpot = (arrE_t *) malloc(sizeof(arrE_t));
            currentNextSpot->row = -1;
            currentNextSpot->col = -1;
            

            if (!current->npc->foughtPC || current->npc->type == 'w' || current->npc->type == 'p' || current->npc->type == 'e'){

                generateMove(map, map->pc, current->npc, currentNextSpot);

                if (!current->npc->foughtPC || currentNextSpot->row != -1){

                    currentCost = terrainCost(*(current->npc), map->terrainOnly[currentNextSpot->row][currentNextSpot->col]);

                    //actually move there, like update the visuals and map stuff
                    moveGuy(map, current->npc, *currentNextSpot);

                    current->costOfNextMove = current->costOfNextMove + currentCost;
                    heap_insert(&h, current);

                }
                else{current->costOfNextMove = current->costOfNextMove + 10; heap_insert(&h, current);}

            }

            // printMap(*map);

        }
        else{

            clear();
            printMap(*map);
            //gen a new pc node with + 10 cost just so it takes a turn;
            //IF THE PC MOVES, THEN WE REGEN THE MAPS

            //take input to figure out where we want to move
            int row;
            int col;
            int invalid = 1;
            int moveCost;
            while (invalid){
                int32_t curMove = getch();
                switch (curMove){

                    case 'f':
                    {
                        clear();
                        printw("must hit enter after each entry\n");
                        printw("input the x coordinate of the desired map\n");
                        int x, y;
                        scanw("%d", &x);
                        printw("input the y coordinate of the desired map\n");
                        scanw("%d", &y);
                        printw("(%d, %d)", x, y);
                        struct Point *p = (struct Point *) malloc(sizeof(struct Point));
                        p->x = x;
                        p->y = y; 
                        arrE_t *mvIn = (arrE_t *) malloc(sizeof(arrE_t));
                        initArrE(mvIn, *p);
                        Map *movingInto;
                        

                        if (world->worldMaps[mvIn->row][mvIn->col] == NULL){

                            world->worldMaps[mvIn->row][mvIn->col] = (Map *) malloc(sizeof(Map));
                            initMap(world, *p);
                            addNPC(world->worldMaps[mvIn->row][mvIn->col], 'e');

                        }
                        // else if (movingInto->turnQ == NULL){

                        //     printw("turnQ is null");

                        // }
                        // else{

                        //     printw("%d", movingInto->turnQ->size);

                        // }

                        clear();
                        movingInto = world->worldMaps[mvIn->row][mvIn->col];
                        map->screen[map->pc->row][map->pc->col] = map->terrainOnly[map->pc->row][map->pc->col];
                        map->characterTracker[map->pc->row][map->pc->col] = NULL;
                        placePlayerChar(map->pc, movingInto);
                        return movingInto;

            


                        break;
                    }
                    case 'l':
                    {
                        clear();
                        printw("%d\n", h.size);
                        break;
                    }    
                    case '7':
                    {
                        //upper left - Northwest
                        row = map->pc->row - 1;
                        col = map->pc->col - 1;

                        if (map->characterTracker[row][col] && !map->characterTracker[row][col]->foughtPC){

                            fightInterface(map->characterTracker[row][col], map->pc);
                            clear();
                            printMap(*map);
                        }

                        if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                            movePC(row, col, map);
                            invalid = 0;

                        }
                        
                        if (!gateCheck(row, col) && ((row == map->eG && col == 79) || (row == map->wG && col == 0) || (col == map->nG && row == 0) || (col == map->sG && row == 20))){

                            Map *s = findMapPCMovingInto(map, world, row, col);
                            // simulateGame(s, world);
                            // return;
                            return s;
                        }
                        else{

                            printw("%d", terrainCostPC(map->terrainOnly[row][col]));

                        }
                        moveCost = terrainCostPC(map->terrainOnly[row][col]);


                        if (map->terrainOnly[row][col] == ':'){
                            pokemonEncounter(map);
                        }
                        
                        break;
                    }
                    case '8':
                        //up - North
                        row = map->pc->row - 1;
                        col = map->pc->col;

                        if (map->characterTracker[row][col] && !map->characterTracker[row][col]->foughtPC){

                            fightInterface(map->characterTracker[row][col], map->pc);
                            clear();
                            printMap(*map);
                        }

                        if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                            movePC(row, col, map);
                            invalid = 0;
                        }
                        
                        if (!gateCheck(row, col) && ((row == map->eG && col == 79) || (row == map->wG && col == 0) || (col == map->nG && row == 0) || (col == map->sG && row == 20))){

                            Map *s = findMapPCMovingInto(map, world, row, col);
                            // simulateGame(s, world);
                            // return;

                            return s;
                        }
                        else{

                            printw("%d", terrainCostPC(map->terrainOnly[row][col]));

                        }

                        moveCost = terrainCostPC(map->terrainOnly[row][col]);


                        if (map->terrainOnly[row][col] == ':'){
                            pokemonEncounter(map);
                        }


                        break;
                    case '9':
                        //upper right - Northeast
                        row = map->pc->row - 1;
                        col = map->pc->col + 1;

                        if (map->characterTracker[row][col] && !map->characterTracker[row][col]->foughtPC){

                            fightInterface(map->characterTracker[row][col], map->pc);
                            clear();
                            printMap(*map);
                        }

                        if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                            movePC(row, col, map);
                            invalid = 0;
                        }
                        
                        if (!gateCheck(row, col) && ((row == map->eG && col == 79) || (row == map->wG && col == 0) || (col == map->nG && row == 0) || (col == map->sG && row == 20))){

                            Map *s = findMapPCMovingInto(map, world, row, col);
                            // simulateGame(s, world);
                            // return;
                            return s;
                        }
                        else{

                            printw("%d", terrainCostPC(map->terrainOnly[row][col]));

                        }

                        moveCost = terrainCostPC(map->terrainOnly[row][col]);


                        if (map->terrainOnly[row][col] == ':'){
                            pokemonEncounter(map);
                        }


                        break;
                    case '6':
                        //right - East
                        row = map->pc->row;
                        col = map->pc->col + 1;

                        if (map->characterTracker[row][col] && !map->characterTracker[row][col]->foughtPC){

                            fightInterface(map->characterTracker[row][col], map->pc);
                            clear();
                            printMap(*map);
                        }

                        if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                            movePC(row, col, map);
                            invalid = 0;
                        }
                        
                        if (!gateCheck(row, col) && ((row == map->eG && col == 79) || (row == map->wG && col == 0) || (col == map->nG && row == 0) || (col == map->sG && row == 20))){

                            Map *s = findMapPCMovingInto(map, world, row, col);
                            // simulateGame(s, world);
                            // return;
                            // current->costOfNextMove = current->costOfNextMove + 10;
                            // heap_insert(&h, current);

                            map->turnQ = &h;
                            return s;
                        }
                        else{

                            printw("%d", terrainCostPC(map->terrainOnly[row][col]));

                        }

                        moveCost = terrainCostPC(map->terrainOnly[row][col]);


                        if (map->terrainOnly[row][col] == ':'){
                            pokemonEncounter(map);
                        }



                        break;
                    case '3':
                        //lower right - Southeast
                        row = map->pc->row + 1;
                        col = map->pc->col + 1;

                        if (map->characterTracker[row][col] && !map->characterTracker[row][col]->foughtPC){

                            fightInterface(map->characterTracker[row][col], map->pc);
                            clear();
                            printMap(*map);
                        }

                        if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                            movePC(row, col, map);
                            invalid = 0;
                        }
                        
                        if (!gateCheck(row, col) && ((row == map->eG && col == 79) || (row == map->wG && col == 0) || (col == map->nG && row == 0) || (col == map->sG && row == 20))){

                            Map *s = findMapPCMovingInto(map, world, row, col);
                            // simulateGame(s, world);
                            // return;
                            return s;
                        }
                        else{

                            printw("%d", terrainCostPC(map->terrainOnly[row][col]));

                        }

                        moveCost = terrainCostPC(map->terrainOnly[row][col]);


                        if (map->terrainOnly[row][col] == ':'){
                            pokemonEncounter(map);
                        }


                        break;
                    case '2':
                        //down - South
                        row = map->pc->row + 1;
                        col = map->pc->col;

                        if (map->characterTracker[row][col] && !map->characterTracker[row][col]->foughtPC){

                            fightInterface(map->characterTracker[row][col], map->pc);
                            clear();
                            printMap(*map);
                        }

                        if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                            movePC(row, col, map);
                            invalid = 0;
                        }
                        
                        if (!gateCheck(row, col) && ((row == map->eG && col == 79) || (row == map->wG && col == 0) || (col == map->nG && row == 0) || (col == map->sG && row == 20))){
                            
                            
                            Map *s = findMapPCMovingInto(map, world, row, col);
                            // simulateGame(s, world);
                            // return;

                            return s;
                        }
                        else{

                            printw("%d", terrainCostPC(map->terrainOnly[row][col]));

                        }

                        moveCost = terrainCostPC(map->terrainOnly[row][col]);


                        if (map->terrainOnly[row][col] == ':'){
                            pokemonEncounter(map);
                        }

                        break;
                    case '1':
                        //lower left - Southwest
                        row = map->pc->row + 1;
                        col = map->pc->col - 1;

                        if (map->characterTracker[row][col] && !map->characterTracker[row][col]->foughtPC){

                            fightInterface(map->characterTracker[row][col], map->pc);
                            clear();
                            printMap(*map);
                        }

                        if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                            movePC(row, col, map);
                            invalid = 0;
                        }
                        
                        if (!gateCheck(row, col) && ((row == map->eG && col == 79) || (row == map->wG && col == 0) || (col == map->nG && row == 0) || (col == map->sG && row == 20))){

                            Map *s = findMapPCMovingInto(map, world, row, col);
                            // simulateGame(s, world);
                            // return;
                            // clear();
                            // while (true){


                            //     printw("%d", h.size);
                            //     char g = getch();
                            //     if (g == 'g'){

                            //         break;
                            //     }

                            // }
                            map->turnQ = &h;
                            return s;
                        }
                        else{

                            printw("%d", terrainCostPC(map->terrainOnly[row][col]));

                        }

                        moveCost = terrainCostPC(map->terrainOnly[row][col]);


                        if (map->terrainOnly[row][col] == ':'){
                            pokemonEncounter(map);
                        }

                        break;
                    case '4':
                        //left - West
                        row = map->pc->row;
                        col = map->pc->col - 1;

                        if (map->characterTracker[row][col] && !map->characterTracker[row][col]->foughtPC){

                            fightInterface(map->characterTracker[row][col], map->pc);
                            clear();
                            printMap(*map);

                        }

                        //need to check some stuff
                        //-can we travel into this terrain type?
                        //-is there a player there?

                        if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                            movePC(row, col, map);
                            invalid = 0;
                        }
                        
                        if (!gateCheck(row, col) && ((row == map->eG && col == 79) || (row == map->wG && col == 0) || (col == map->nG && row == 0) || (col == map->sG && row == 20))){

                            Map *s = findMapPCMovingInto(map, world, row, col);
                            // simulateGame(s, world);
                            // return;
                            return s;
                        }   
                        else{

                            printw("%d", terrainCostPC(map->terrainOnly[row][col]));

                        }

                        moveCost = terrainCostPC(map->terrainOnly[row][col]);


                        if (map->terrainOnly[row][col] == ':'){
                            pokemonEncounter(map);
                        }

                        break;
                    case '>':
                        {
                        if (map->terrainOnly[map->pc->row][map->pc->col] == 'M'){

                            clear();
                            printw("Placeholder for PokeMart\n");

                            char mc;

                            while (true){

                                mc = getch();

                                if (mc != '<'){

                                    printw("Hit '<' to leave the PokeMart\n");

                                }
                                else{

                                    clear();
                                    printMap(*map);
                                    break;

                                }

                            }

                        }
                        else if (map->terrainOnly[map->pc->row][map->pc->col] == 'C'){

                            clear();
                            printw("Placeholder for PokeCenter\n");

                            char cc;

                            while (true){

                                cc = getch();

                                if (cc != '<'){

                                    printw("Hit '<' to leave the PokeCenter\n");

                                }
                                else{

                                    clear();
                                    printMap(*map);
                                    break;


                                }

                            }

                        }
                        else{printw("It doesn't appear you are at a PokeCenter or PokeMart...\n");}

                        break;
                        }
                    case '5':
                        {
                        moveCost = 10;
                        invalid = 0;
                        break;
                        }
                    case 't':
                        {
                            clear();
                            printw("NPC Menu:\n");
                            int pcrow, pccol, nrow, ncol;
                            pcrow = map->pc->row;
                            pccol = map->pc->col;
                            int i;
                            for (i = 0; i < min(map->npcCount, 20); i++){

                                nrow = map->npcList[i]->row;
                                ncol = map->npcList[i]->col;

                                addch(map->npcList[i]->type);
                                printw(" | ");
                                printNpcLocation(pcrow, pccol, nrow, ncol);

                            }
                            int32_t cur;
                            int scroll = 0;
                            int maxscroll = (map->npcCount / 20);
                            while (true){

                                
                                cur = getch();
                                if (cur != 27 && cur != 'w' && cur != 's' && cur != KEY_DOWN && cur != KEY_UP){

                                    printw("Enter escape to return to the game window");

                                }
                                else if (cur == 'w' || cur == KEY_UP){


                                    if (scroll + 1 <= maxscroll){

                                        clear();

                                        int upperbound;
                                        scroll++;
                                        if (scroll == maxscroll){

                                            upperbound = map->npcCount % 20;

                                        }
                                        else{

                                            upperbound = 20;

                                        }


                                        for (i = scroll * 20; i < upperbound + scroll * 20; i++){

                                            nrow = map->npcList[i]->row;
                                            ncol = map->npcList[i]->col;

                                            addch(map->npcList[i]->type);
                                            printw(" | ");
                                            printNpcLocation(pcrow, pccol, nrow, ncol);

                                        }
                                    }
                                    else{

                                        printw("beginning of list\n");

                                    }

                                }
                                else if (cur == 's' || cur == KEY_DOWN){

                                    if (scroll - 1 >= 0){

                                        clear();

                                        scroll--;

                                        for (i = scroll * 20; i < scroll * 20 + 20; i++){

                                                nrow = map->npcList[i]->row;
                                                ncol = map->npcList[i]->col;

                                                addch(map->npcList[i]->type);
                                                printw(" | ");
                                                printNpcLocation(pcrow, pccol, nrow, ncol);

                                        }

                                    }
                                    else{


                                        printw("end of list\n");


                                    }

                                    

                                }
                                else{clear(); printMap(*map);break;}

                            }
                            break;
                        }
                    case KEY_UP:
                        {
                            printw("up");
                            break;
                        }
                    case KEY_DOWN:
                        {
                            printw("down");
                            break;
                        }
                    case 27:
                        {
                            printw("escape hit");
                            break;
                        }
                    case 'Q':
                        return NULL;
                        break;


                    default:
                        printw("entry not recognized\n");
                        addch(curMove);
                        break;
                }
            }
            
            // current->costOfNextMove = current->costOfNextMove + terrainCostPC(map->terrainOnly[row][col]);
            current->costOfNextMove = current->costOfNextMove + moveCost;
            heap_insert(&h, current);
            //NEW ADDITION
            findShortestPaths(map->pc, map, &dummyHiker, map);
            findShortestPaths(map->pc, map, &dummyRival, map);
            //NEW ADDITION
            clear();
            printMap(*map);
            // usleep(250000);
        }
    }

    return NULL;
    
}

void pokemonEncounter(Map *map){

    // //determine whether or not we encounter a pokemon
    int r = (rand() % 10) + 1;
    if (r != 1){
        return;
    }
    clear();
    printLivePokemon(generatePokemon(map), -1, -1);
    printw("Press 'e' to escape");
    char c;
    while (c != 'e'){

        c = getch();
    }


}

LivePokemon * generatePokemon(Map *map){

    // clear();

    



    

    //choose which pokemon we will encounter (x ~ uniform);
    int pk = (rand() % 1092) + 1;
    
    //we will have an array of length 1092 that has all of the pokemon, just take arr[x]
    //to get the pokemon

    //choosing the level of the pokemon
    //we must compute the manhattan distance from the origin
    int l;
    int mdistance = abs(map->worldCoordinates.x) + abs(map->worldCoordinates.y);

    if (mdistance == 0 || mdistance == 1){
        l = 1;
    }
    else{

        if (mdistance <= 200){
            l = (rand() % (mdistance / 2)) + 1;
        }
        else{
            l = (rand() % 100) + ((mdistance - 200) / 2);
        }

    }

    //gen additive values to be added to IVS, x ~ [0, 15]
    int hp = rand() % 15;
    int attack = rand() % 15;
    int defense = rand() % 15;
    int speed = rand() % 15;
    int specialAttack = rand() % 15;
    int specialDefense = rand() % 15;

    int i = 0;
    while (p[pk].id != pst[i].pokemon_id){
        i++;
    }

    int baseHP;
    int baseAttack;
    int baseDefense;
    int baseSpeed;
    int baseSpecialAttack;
    int baseSpecialDefense;


    // hp += pst[i].base_stat;
    baseHP = pst[i].base_stat;
    i++;
    //attack += pst[i].base_stat;
    baseAttack = pst[i].base_stat;
    i++;
    //defense += pst[i].base_stat;
    baseDefense = pst[i].base_stat;
    i++;
    //speed += pst[i].base_stat;
    baseSpeed = pst[i].base_stat;
    i++;
    //specialAttack += pst[i].base_stat;
    baseSpecialAttack = pst[i].base_stat;
    i++;
    //specialDefense += pst[i].base_stat;
    baseSpecialDefense = pst[i].base_stat;



    const char *s = p[pk].identifier.c_str();
    bool gender;
    if (rand() % 2 == 0){
        gender = true;

    }
    bool isShiny;
    if (rand() % 8192 == 0){
        isShiny = true;

    }

    LivePokemon *lp = new LivePokemon(pk, p[pk].identifier, l, hp, attack, defense, speed, specialAttack, specialDefense, gender, isShiny, baseHP, baseAttack, baseDefense, baseSpeed, baseSpecialAttack, baseSpecialDefense);


    //now to find the possible moves
    std::vector<Pokemon_Move> possibleMoves;
    std::vector<Pokemon_Move> movesWithLevel;
    std::unordered_set<int> seenMoves;
    //we need to find our pokemon specie id, then go through the list of pokemon_moves and find
    //pokemon_moves that have their pokemon_id equal to our pokemon species id
    int speciesId = p[pk].species_id;

    for (int i = 0; i < pm.size(); i++){

        if (pm[i].pokemon_id == speciesId && pm[i].pokemon_move_method_id == 1){

            if (seenMoves.find(pm[i].move_id) == seenMoves.end()){

                possibleMoves.push_back(pm[i]);
                seenMoves.insert(pm[i].move_id);

            }
            

        }


    }

    for (int i = 0; i < possibleMoves.size(); i++){

        if (possibleMoves[i].level < lp->level){
            movesWithLevel.push_back(possibleMoves[i]);
        }

    }

    int move1;
    int move2;

    while (movesWithLevel.size() < 1){
        levelUp(lp);
        for (int i = 0; i < possibleMoves.size(); i++){
            if (possibleMoves[i].level < lp->level){
                movesWithLevel.push_back(possibleMoves[i]);
            }
        }
    }

    if (movesWithLevel.size() > 1){

        move1 = rand() % movesWithLevel.size();
        move2 = move1;

        while (move1 == move2){
            move2 = rand() % movesWithLevel.size();
        }


    }
    else{
        move1 = 0;
        move2 = NULL;

    }

    // if (movesWithLevel.size() > 1){
    //     move1 = (rand() % movesWithLevel.size());
    //     while (move2 != move1){

    //         move2 = (rand() % movesWithLevel.size());

    //     }
    // }
    // else if (movesWithLevel.size() == 1){
    //     move1 = 0;
    //     move2 = NULL;
    // }
    // else{
    //     while (movesWithLevel.size() < 1){
    //         l++;
    //         for (int i = 0; i < possibleMoves.size(); i++){
    //             if (possibleMoves[i].level < l){
    //                 movesWithLevel.push_back(possibleMoves[i]);
    //             }
    //         }
    //     }
    // }

    // Move m1;
    // Move m2;
    // if (move1 != NULL){
    //     int i = 0;
    //     while (m[i].id != move1)

    // }

    //gen ivs, x ~ [0, 15]
    // int hp = rand() % 15;
    // int attack = rand() % 15;
    // int defense = rand() % 15;
    // int speed = rand() % 15;
    // int specialAttack = rand() % 15;
    // int specialDefense = rand() % 15;

    // int i = 0;
    // while (p[pk].id != pst[i].pokemon_id){
    //     i++;
    // }

    // int baseHP;
    // int baseAttack;
    // int baseDefense;
    // int baseSpeed;
    // int baseSpecialAttack;
    // int baseSpecialDefense;


    // // hp += pst[i].base_stat;
    // baseHP = pst[i].base_stat;
    // i++;
    // //attack += pst[i].base_stat;
    // baseAttack = pst[i].base_stat;
    // i++;
    // //defense += pst[i].base_stat;
    // baseDefense = pst[i].base_stat;
    // i++;
    // //speed += pst[i].base_stat;
    // baseSpeed = pst[i].base_stat;
    // i++;
    // //specialAttack += pst[i].base_stat;
    // baseSpecialAttack = pst[i].base_stat;
    // i++;
    // //specialDefense += pst[i].base_stat;
    // baseSpecialDefense = pst[i].base_stat;



    // const char *s = p[pk].identifier.c_str();
    // bool gender;
    // if (rand() % 2 == 0){
    //     gender = true;

    // }
    // bool isShiny;
    // if (rand() % 8192 == 0){
    //     isShiny = true;

    // }



    // LivePokemon *lp = new LivePokemon(pk, p[pk].identifier, l, hp, attack, defense, speed, specialAttack, specialDefense, gender, isShiny, baseHP, baseAttack, baseDefense, baseSpeed, baseSpecialAttack, baseSpecialDefense);

    // for (int i = 0; i < movesWithLevel.size(); i++){

    //     int j = 0;

    //     while (movesWithLevel[i].move_id != m[j].id){
    //         j++;
    //     }
    //     // printw(m[j].identifier.c_str());
    //     // printw(" %d", m[j].id);
    //     lp->moves.push_back(m[j]);
    //     // printw("\n");

    // }
    int j = 0;
    while (movesWithLevel[move1].move_id != m[j].id){
        j++;
    }
    lp->moves.push_back(m[j]);
    j = 0;
    if (move2 != NULL){
        while (movesWithLevel[move2].move_id != m[j].id){
        j++;
        }
        lp->moves.push_back(m[j]);

    }
    

    return lp;


    clear();
    printw("Current Encounter:\n");
    printw("Pokemon: %d = ", pk);
    printw(s);
    printw("\n");
    printw("Pokemon Level: %d\n", l);
    printw("Manhattan Distance: %d\n", mdistance);
    // for (int i = 0; i < possibleMoves.size(); i++){

    //     printw("%d\n", possibleMoves[i].move_id);

    // }
    printw("possibleMove.size() = %d\n", possibleMoves.size());
    printw("movesWithLevel.size() = %d\n", movesWithLevel.size());

    for (int i = 0; i < movesWithLevel.size(); i++){

        int j = 0;

        while (movesWithLevel[i].move_id != m[j].id){
            j++;
        }
        // printw(m[j].identifier.c_str());
        // printw(" %d", m[j].id);
        lp->moves.push_back(m[j]);
        // printw("\n");

    }
    printw("Moves: \n");
    if (move1 != NULL){
        

    }



    char c;
    while (c != 'c'){

        c = getch();

    }



}

void levelUp(LivePokemon *lp){

    lp->level++;

    lp->hp = floor((((lp->baseHP + lp->hpIV) * 2) * lp->level) / 100) + lp->level + 10;

    lp->attack = floor((((lp->baseAttack + lp->attackIV) * 2) * lp->level) / 100) + 5;

    lp->defense = floor((((lp->baseDefense + lp->defenseIV) * 2) * lp->level) / 100) + 5;

    lp->speed = floor((((lp->baseSpeed + lp->speedIV) * 2) * lp->level) / 100) + 5;

    lp->specialAttack = floor((((lp->baseSpecialAttack + lp->specialAttackIV) * 2) * lp->level) / 100) + 5;

    lp->specialDefense = floor((((lp->baseSpecialDefense + lp->specialDefenseIV) * 2) * lp->level) / 100) + 5;

    

}

void pickPokemon(Map *map){

    clear();
    char ch;
    //gen 3 pokemon, print them out
    // printw("Note: If one of the pokemon is above level 1, this is because in order to give the pokemon a move, we had to level it up\n");
    printw("Choice 1:\n");
    LivePokemon *c1 = generatePokemon(map);
    printLivePokemon(c1, -1, -1);
    

    // while (ch != 'l'){

    //     ch = getch();

    // }
    
    mvprintw(0, 27, "Choice 2:\n");
    LivePokemon *c2 = generatePokemon(map);
    printLivePokemon(c2, 1, 27);
    

    // while (ch != 'm'){

    //     ch = getch();

    // }
    
    mvprintw(0, 54, "Choice 3:\n");
    LivePokemon *c3 = generatePokemon(map);
    printLivePokemon(c3, 1, 54);
    
    mvprintw(17, 0, "Input 1, 2, or 3\n");
    printw("Note: Some of the pokemon may be level 2 because we had to level them up in order for them to have two moves.");


    
    while (ch != '1' && ch != '2' && ch != '3'){

        ch = getch();

    }

    // map->pc->pokemon = new std::vector<LivePokemon>;

    if (ch == '1'){
        map->pc->pokemon.push_back(*c1);
    }
    else if (ch == '2'){
        map->pc->pokemon.push_back(*c2);
    }
    else{
        map->pc->pokemon.push_back(*c3);
    }

    printw("here");



}

void giveNPCPokemon(NPC *npc, Map *map){

    npc->pokemon.push_back(*generatePokemon(map));

    int count = 1;
    while (count <= 6){

        if (rand() % 10 <= 5){

            npc->pokemon.push_back(*generatePokemon(map));

        }
        else{
            return;
        }

    }

}

void printLivePokemon(LivePokemon *lp, int y, int x){

    if (y == -1 || x == -1){

        printw("Name: %s\n", lp->identifier.c_str());
        printw("Level: %d\n\n", lp->level);
        printw("Stats: \n");
        printw("HP: %d\n", lp->hp);
        printw("Attack: %d\n", lp->attack);
        printw("Defense: %d\n", lp->defense);
        printw("Speed: %d\n", lp->speed);
        printw("Special Attack: %d\n", lp->specialAttack);
        printw("Special Defense: %d\n\n", lp->specialDefense);
        for (int i = 0; i < lp->moves.size(); i++){

            printw("Move %d: %s\n", i + 1, (lp->moves[i].identifier).c_str());

        }

        return;

    }
    else{
        
        move(y, x);
        printw("Name: %s\n", lp->identifier.c_str());
        y++;
        move(y, x);
        printw("Level: %d\n\n", lp->level);
        y++;
        y++;
        move(y, x);
        printw("Stats: \n");
        y++;
        move(y, x);
        printw("HP: %d\n", lp->hp);
        y++;
        move(y, x);
        printw("Attack: %d\n", lp->attack);
        y++;
        move(y, x);
        printw("Defense: %d\n", lp->defense);
        y++;
        move(y, x);
        printw("Speed: %d\n", lp->speed);
        y++;
        move(y, x);
        printw("Special Attack: %d\n", lp->specialAttack);
        y++;
        move(y, x);
        printw("Special Defense: %d\n\n", lp->specialDefense);
        y++;
        y++;
        for (int i = 0; i < lp->moves.size(); i++){

            move(y, x);
            printw("Move %d: %s\n", i + 1, (lp->moves[i].identifier).c_str());
            y++;
        }


    }



}

Map * findMapPCMovingInto(Map *map, world_t *world, int row, int col){

    //remove pc from current map;

    map->screen[map->pc->row][map->pc->col] = '#';
    NPC *player = map->characterTracker[map->pc->row][map->pc->col];
    map->characterTracker[map->pc->row][map->pc->col] = NULL;

    //four cases: n, e, s, w
    arrE_t *currentLoc = (arrE_t *) malloc(sizeof(arrE_t));
    initArrE(currentLoc, map->worldCoordinates);
    Map *curMap;

    //north
    if (row == 0){

        //make new map point to pc
        if (world->worldMaps[currentLoc->row - 1][currentLoc->col] == NULL){
            curMap = world->worldMaps[currentLoc->row - 1][currentLoc->col] = (Map *) malloc(sizeof(Map));
            struct Point p;
            p.x = map->worldCoordinates.x;
            p.y = map->worldCoordinates.y + 1;
            initMap(world, p);
            addNPC(curMap, 'e');
        }
        else{
            curMap = world->worldMaps[currentLoc->row - 1][currentLoc->col];
        }
        curMap->pc = map->pc;
        map->pc = NULL;
        curMap->pc->row = 19;
        curMap->pc->col = curMap->sG;
        curMap->characterTracker[curMap->pc->row][curMap->pc->col] = player;
        curMap->screen[curMap->pc->row][curMap->pc->col] = '@';
        // addNPC(curMap, 'e');
        return curMap;
    }
    //south
    else if (row == 20){

        if (world->worldMaps[currentLoc->row + 1][currentLoc->col] == NULL){

            curMap = world->worldMaps[currentLoc->row + 1][currentLoc->col] = (Map *) malloc(sizeof(Map));
            struct Point p;
            p.x = map->worldCoordinates.x;
            p.y = map->worldCoordinates.y - 1;
            initMap(world, p);
            addNPC(curMap, 'e');

        }
        else{

            curMap = world->worldMaps[currentLoc->row + 1][currentLoc->col];

        }
            curMap->pc = map->pc;
            map->pc = NULL;
            curMap->pc->row = 1;
            curMap->pc->col = curMap->nG;
            curMap->characterTracker[curMap->pc->row][curMap->pc->col] = player;
            curMap->screen[curMap->pc->row][curMap->pc->col] = '@';
            // addNPC(curMap, 'e');
            return curMap;
    }
    //east
    else if (col == 79){

        if (world->worldMaps[currentLoc->row][currentLoc->col + 1] == NULL){

            curMap = world->worldMaps[currentLoc->row][currentLoc->col + 1] = (Map *) malloc(sizeof(Map));
            struct Point p;
            p.x = map->worldCoordinates.x + 1;
            p.y = map->worldCoordinates.y;
            initMap(world, p);
            addNPC(curMap, 'e');
        }
        else{
            curMap = world->worldMaps[currentLoc->row][currentLoc->col + 1];
        }
        
        curMap->pc = map->pc;
        map->pc = NULL;
        curMap->pc->row = curMap->wG;
        curMap->pc->col = 1;
        curMap->characterTracker[curMap->pc->row][curMap->pc->col] = player;
        curMap->screen[curMap->pc->row][curMap->pc->col] = '@';
        // addNPC(curMap, 'e');
        return curMap;
    }
    //west
    else if (col == 0){

        clear();

        if (world->worldMaps[currentLoc->row][currentLoc->col - 1] == NULL){

            curMap = world->worldMaps[currentLoc->row][currentLoc->col - 1] = (Map *) malloc(sizeof(Map));
            struct Point p;
            p.x = map->worldCoordinates.x - 1;
            p.y = map->worldCoordinates.y;
            initMap(world, p);
            addNPC(curMap, 'e');

        }
        else{
            curMap = world->worldMaps[currentLoc->row][currentLoc->col - 1];
        }

        
        curMap->pc = map->pc;
        map->pc = NULL;
        curMap->pc->row = curMap->eG;
        curMap->pc->col = 78;
        curMap->characterTracker[curMap->pc->row][curMap->pc->col] = player;
        curMap->screen[curMap->pc->row][curMap->pc->col] = '@';
        // addNPC(curMap, 'e');
        return curMap;

    }

    return NULL;
    

}

void fightInterface(NPC *npc, playerChar *pc){

    clear();

    trainerBattle(npc, pc);

    // printw("placeholder for pokemon battle. press 'esc' to exit\n");
    // printNPC(npc);

    // printw("Number of pokemon (npc): %d\n", npc->pokemon.size());
    // printw("Names and levels of pokemon:\n");
    // for (int i = 0; i < npc->pokemon.size(); i++){
    //     printw(npc->pokemon[i].identifier.c_str());
    //     printw(" (%d)\n", npc->pokemon[i].level);
    // }
    // printw("\n");

    // printw("Print the entire first pokemon. All of the pokemon are generated from the same function call in a loop, so if this pokemon is generated correctly, all of them are.\n\n");

    // printLivePokemon(&(npc->pokemon[0]), -1, -1);


    char fc;

    while (true){

        fc = getch();

        if (fc != 27){

            printw("please hit 'esc' to exit\n");

        }
        else{

            break;

        }


    }

    npc->foughtPC = 1;

}

void trainerBattle(NPC *npc, playerChar *pc){

    clear();

    int turn = 0;
    char c;
    bool b = true;
    int npcPokemonIndex = 0;
    int pcPokemonIndex = 0;
    bool pcUsingMove = false;
    int choice = -1;

    while (b){

        printw("Options: \n");
        printw("'f' to Fight\n");
        printw("'b' to use an item from your Bag\n");
        printw("'r' to flee the battle\n");
        printw("'p' to switch to a different active Pokemon\n");

        //get pcs action

        c = getch();

        switch (c){

            case 'f':{
                // printw("Fight\n");
                //Let user select move from active pokemon
                
                // LivePokemon pcLP = pc->pokemon[pcPokemonIndex];

                printw("Choose your move (type the corresponding number): \n");
                
                printLivePokemon(&(pc->pokemon[pcPokemonIndex]), -1, -1);

                char moveChoice;
                
                while (true){

                    moveChoice = getch();

                    if (moveChoice == '2' && pc->pokemon[pcPokemonIndex].moves.size() == 2){

                        // pcM = pcLP.moves[1];
                        choice = 1;
                        break;

                    }
                    else if (moveChoice == '1'){
                        // pcM = pcLP.moves[0];
                        choice = 0;
                        break;

                    }

                    

                }
                // Move pcM = pcLP.moves[choice];

                LivePokemon pcLP = pc->pokemon[pcPokemonIndex];
                Move pcM = pcLP.moves[choice];

                //get npcs action
                //find the npcs current active pokemon
                LivePokemon npcLP = npc->pokemon[npcPokemonIndex];
                //choose a random move from this pokemon
                Move npcM = npcLP.moves[rand() % npcLP.moves.size()];

                if (npcM.priority > pcM.priority){

                    printw("NPC uses %s\n", npcM.identifier.c_str());
                    if (rand() % 100 > npcM.accuracy){

                        printw("The move misses!\n");

                    }
                    else{
                        int damage = computeDamage(npcM, npcLP, pcLP);
                        printw("It does %d damage to %s\n", damage, pcLP.identifier.c_str());
                        pcLP.hp -= damage;

                        printw("%s has %d HP left!\n", pcLP.identifier.c_str(), pcLP.hp);

                        //CHECKPOINT
                        printw("Hit 'c' to continue\n");
                        while (true){
                            if (getch() == 'c'){
                            break;
                            }
                        }
                        //CHECKPOINT
                        if (pcLP.hp <= 0){

                            printw("%s has fainted!\n", pcLP.identifier.c_str());
                            pcPokemonIndex++;
                            if (pcPokemonIndex >= pc->pokemon.size()){

                                printw("You ran out of Pokemon and lost the fight!\n");
                                b = false;

                            }

                        }
                        else{

                            printw("PC uses %s\n", pcM.identifier.c_str());
                            if (rand() % 100 > npcM.accuracy){

                                printw("The move misses!\n");

                            }
                            else{
                                int damage = computeDamage(pcM, pcLP, npcLP);
                                printw("It does %d damge to %s\n", damage, npcLP.identifier.c_str());
                                npcLP.hp -= damage;

                                printw("%s has %d HP left!\n", npcLP.identifier.c_str(), npcLP.hp);

                                if (npcLP.hp <= 0){

                                    printw("%s has fainted!\n", npcLP.identifier.c_str());
                                    npcPokemonIndex++;
                                    if (npcPokemonIndex >= npc->pokemon.size()){

                                        printw("The NPC ran out of Pokemon and lost the fight!\n");
                                        b = false;

                                    }
                                }

                            }

                        }
                    }
                }
                else{

                    printw("PC uses %s\n", pcM.identifier.c_str());
                    if (rand() % 100 > pcM.accuracy){

                        printw("The move misses!\n");

                    }
                    else{
                        int damage = computeDamage(pcM, pcLP, npcLP);
                        printw("It does %d damge to %s\n", damage, npcLP.identifier.c_str());
                        npcLP.hp -= damage;

                        printw("%s has %d HP left!\n", npcLP.identifier.c_str(), npcLP.hp);

                        if (npcLP.hp <= 0){

                            printw("%s has fainted!\n", npcLP.identifier.c_str());
                            npcPokemonIndex++;
                            if (npcPokemonIndex >= npc->pokemon.size()){

                                printw("The NPC ran out of Pokemon and lost the fight!\n");
                                b = false;

                            }
                        }
                        else{

                            printw("NPC uses %s\n", npcM.identifier.c_str());
                            if (rand() % 100 > npcM.accuracy){
                                printw("The move misses\n");
                            }
                            else{

                                int damage = computeDamage(npcM, npcLP, pcLP);
                                printw("It does %d damage to %s\n", damage, pcLP.identifier.c_str());
                                pcLP.hp -= damage;

                                printw("%s has %d HP left!\n", pcLP.identifier.c_str(), pcLP.hp);

                                if (pcLP.hp <= 0){
                                    printw("%s has fainted\n", pcLP.identifier.c_str());
                                    pcPokemonIndex++;
                                    if (pcPokemonIndex >= pc->pokemon.size()){
                                        printw("The PC ran out of Pokemon and lost the fight!\n");
                                        b = false;
                                    }
                                }

                            }

                        }

                    }
                    

                }

                



                
                break;
            }
            case 'b':{
            printw("Look in Bag\n");
            printw("Printing the contents of the bag:\n");
            printw("Revives: %d\n", pc->revives);
            printw("Potions: %d\n", pc->potions);
            printw("Pokeballs: %d\n", pc->pokeballs);

            char bagChoice;
            printw("Choose 'r' for revive, 'p' for potion, 'b' for pokeball, or 'e' to exit the bag.\n");
            while (true){

                bagChoice = getch();
                if (bagChoice == 'r'){

                    //Revive knocked out pokemon and restore it to half its max HP

                    break;
                }
                else if (bagChoice == 'p'){

                    //Restore 20 HP of an alive pokemon, never going above its max HP

                    break;
                }
                else if (bagChoice == 'b'){

                    printw("You cannot capture the NPC's Pokemon! Try again.\n");

                }
                else if (bagChoice == 'e'){

                    break;

                }
                else{

                    printw("Try again.\n");

                }

            }
            break;
            }
            case 'r':
            printw("Run from fight\n");
            break;
            case 'p':
            printw("Here is a list of your Pokemon followed by their HP:\n");
            for (int i = 0; i < pc->pokemon.size(); i++){

                printw("%d: %s | %d\n", i, pc->pokemon[i].identifier.c_str(), pc->pokemon[i].hp);

            }
            printw("Type the number of the Pokemon you would like to select\n");
            char p;
            while (true){

                p = getch();
                int i = p - '0';
                if (pc->pokemon[i].hp <= 0){

                    printw("%d\n", (int) p);

                    printw("This pokemon has no HP. Choose another.");

                }
                else{

                    pcPokemonIndex = p;
                    break;

                }

            }
            break;
            case 'e':
            b = false;
            break;
            default:
            printw("Unkown input\n");
            break;


        }

        // LivePokemon pcLP = pc->pokemon[pcPokemonIndex];
        // Move pcM = pcLP.moves[choice];

        // //get npcs action
        // //find the npcs current active pokemon
        // LivePokemon npcLP = npc->pokemon[npcPokemonIndex];
        // //choose a random move from this pokemon
        // Move npcM = npcLP.moves[rand() % npcLP.moves.size()];

        // if (npcM.priority > pcM.priority){

        //     printw("NPC uses %s\n", npcM.identifier.c_str());
        //     if (rand() % 100 > npcM.accuracy){

        //         printw("The move misses!\n");

        //     }
        //     else{
        //         int damage = computeDamage(npcM, npcLP, pcLP);
        //         printw("It does %d damage to %s\n", damage, pcLP.identifier.c_str());
        //         pcLP.hp -= damage;
        //         //CHECKPOINT
        //         printw("Hit 'c' to continue\n");
        //         while (true){
        //             if (getch() == 'c'){
        //             break;
        //             }
        //         }
        //         //CHECKPOINT
        //         if (pcLP.hp <= 0){

        //             printw("%s has fainted!\n", pcLP.identifier.c_str());
        //             pcPokemonIndex++;
        //             if (pcPokemonIndex >= pc->pokemon.size()){

        //                 printw("You ran out of Pokemon and lost the fight!\n");
        //                 b = false;

        //             }

        //         }
        //         else{

        //             printw("PC uses %s\n", pcM.identifier.c_str());
        //             if (rand() % 100 > npcM.accuracy){

        //                 printw("The move misses!\n");

        //             }
        //             else{
        //                 int damage = computeDamage(pcM, pcLP, npcLP);
        //                 printw("It does %d damge to %s\n", damage, npcLP.identifier.c_str());
        //                 npcLP.hp -= damage;
        //                 if (npcLP.hp <= 0){

        //                     printw("%s has fainted!\n", npcLP.identifier.c_str());
        //                     npcPokemonIndex++;
        //                     if (npcPokemonIndex >= npc->pokemon.size()){

        //                         printw("The NPC ran out of Pokemon and lost the fight!\n");
        //                         b = false;

        //                     }
        //                 }

        //             }

        //         }
        //     }
        // }
        // else{

        //     printw("PC uses %s\n", pcM.identifier.c_str());
        //     if (rand() % 100 > pcM.accuracy){

        //         printw("The move misses!\n");

        //     }
        //     else{
        //         int damage = computeDamage(pcM, pcLP, npcLP);
        //         printw("It does %d damge to %s\n", damage, npcLP.identifier.c_str());
        //         npcLP.hp -= damage;
        //         if (npcLP.hp <= 0){

        //             printw("%s has fainted!\n", npcLP.identifier.c_str());
        //             npcPokemonIndex++;
        //             if (npcPokemonIndex >= npc->pokemon.size()){

        //                 printw("The NPC ran out of Pokemon and lost the fight!\n");
        //                 b = false;

        //             }
        //         }
        //         else{

        //             printw("NPC uses %s\n", npcM.identifier.c_str());
        //             if (rand() % 100 > npcM.accuracy){
        //                 printw("The move misses\n");
        //             }
        //             else{

        //                 int damage = computeDamage(npcM, npcLP, pcLP);
        //                 printw("It does %d damage to %s\n", damage, pcLP.identifier.c_str());
        //                 pcLP.hp -= damage;
        //                 if (pcLP.hp <= 0){
        //                     printw("%s has fainted\n", pcLP.identifier.c_str());
        //                     pcPokemonIndex++;
        //                     if (pcPokemonIndex >= pc->pokemon.size()){
        //                         printw("The PC ran out of Pokemon and lost the fight!\n");
        //                         b = false;
        //                     }
        //                 }

        //             }

        //         }

        //     }
            

        // }
        

        
        printw("Hit 'c' to continue\n");
        while (true){

            if (getch() == 'c'){

                break;
            }

        }

        clear();

    }
    

}



void wildBattle(playerChar *pc, LivePokemon *lp){



}

int computeDamage(Move m, LivePokemon attacker, LivePokemon defender){

    double numerator = (((2 * attacker.level) / 5) + 2) * m.power * (attacker.attack / defender.defense);
    double denominator = 50;
    double damage = numerator / denominator;
    damage+=2;
    printw("%f\n", damage);
    //Critical
    if (rand() % 256 < (attacker.baseSpeed / 2)){
        damage *= 1.5;
    }
    //random
    damage *= ((rand() % 16) + 85) / 100;
    printw("%f\n", damage);
    //STAB
    //Have to find attacker's type
    int attackerType = -1;
    for (int i = 0; i < pt.size(); i++){

        if (pt[i].pokemon_id == attacker.id){
            attackerType = pt[i].type_id;
            break;
        }

    }
    if (attackerType == m.type_id){
        damage *= 1.5;
    }
    //Not implementing Type, just use 1
    printw("%f\n", damage);

    return (int) damage;



}

void printNpcLocation(int pcrow, int pccol, int row, int col){

    int ns, ew;
    ns = pcrow - row;
    ew = pccol - col;

    if (ns <= 0 && ew <= 0){printw("%d south and %d east\n", abs(ns), abs(ew));}
    else if (ns <= 0 && ew >= 0){printw("%d south and %d west\n", abs(ns), abs(ew));}
    else if (ns >= 0 && ew <= 0){printw("%d north and %d east\n", abs(ns), abs(ew));}
    else if (ns >= 0 && ew >= 0){printw("%d north and %d west\n", abs(ns), abs(ew));}



}

void io_init_terminal(void){

    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);


}

int gateCheck(int r, int c){

    if (c == 0 || c == 79 || r == 0 || r == 20){

        return 0;

    }

    return 1;


}

int terrainCostPC(char c){

    if (c == '#'){

        return 10;

    }
    else if (c == 'P'){

        return 10;

    }
    else if (c == 'C'){

        return 10;

    }
    else if (c == 'M'){

        return 10;

    }
    else if (c == ':'){

        return 20;

    }
    else if (c == '.'){

        return 10;

    }
    else{

        return 4000;

    }

}

void dummyFunction(Map *map, playerChar *player, NPC *npc){

    printf("hello world");
    return;

}

void parsePokemon(std::vector<Pokemon>& pokemonList){

    // std::vector<Pokemon> pokemonList;

    std::ifstream myfile;
    std::stringstream currLine;
    myfile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon.csv");
    //check failure
    if (myfile.fail()){

        std::string home = std::getenv("HOME");
        std::string path = "/.poke327/pokedex/pokedex/data/csv/pokemon.csv";
        // std::string path = "/testT.txt";
        std::cout << home + path;
        myfile.open(home + path);

    }

    std::string myline;

    //read first line of file into 'myline'
    std::getline(myfile, myline);
    //put the contents of 'myline' into curLine
    currLine << myline;

    //print the contents of curLine to stdout
    // std::cout << currLine.str() << std::endl;

    std::string currentField;

    //put the first 
    std::getline(currLine, currentField, ',');
    // std::cout << currentField << std::endl;
    currLine.str(std::string());

    int id;
    std::string identifier;
    int species_id;
    int height;
    int weight;
    int base_experience;
    int order;
    int is_default;

    while (myfile.peek() != EOF){

        std::stringstream curLine;


        std::getline(myfile, myline);
        curLine << myline;

        // std::cout << myline << std::endl;
        // std::cout << "curline: " << curLine.str() << std::endl;

        // std::cout << curLine.str() << std::endl;

        // std::cout << "\n" << std::endl;

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        id = stoi(currentField);
        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        identifier = currentField;
        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        species_id = stoi(currentField);
        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        height = stoi(currentField);
        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        weight = stoi(currentField);
        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        base_experience = stoi(currentField);
        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        order = stoi(currentField);
        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        is_default = stoi(currentField);

        
        
        Pokemon *p = new Pokemon(id, identifier, species_id, height, weight, base_experience, order, is_default);
        pokemonList.push_back(*p);
    
        
    }

    // int n = pokemonList.size();
    // for (int i = 0; i < n; i++){

    //     std::cout << pokemonList[i].identifier << std::endl;

    // }

    return;

}

void parseMoves(std::vector<Move>& movesList){

    // std::vector<Pokemon> pokemonList;

    std::ifstream myfile;
    std::stringstream currLine;
    myfile.open("/share/cs327/pokedex/pokedex/data/csv/moves.csv");
    //check failure
    if (myfile.fail()){

        
        std::string home = std::getenv("HOME");
        std::string path = "/.poke327/pokedex/pokedex/data/csv/moves.csv";
        // std::string path = "/testT.txt";
        std::cout << home + path;
        myfile.open(home + path);

    }


    std::string myline;
    std::string currentField;
    //read first line of file into 'myline'
    std::getline(myfile, myline);
    //put the contents of 'myline' into curLine
    currLine << myline;

    //print the contents of curLine to stdout
    // std::cout << currLine.str() << std::endl;

    //put the first 
    std::getline(currLine, currentField, ',');
    // std::cout << currentField << std::endl;
    currLine.str(std::string());

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

    while(myfile.peek() != EOF){

        std::stringstream curLine;
        std::getline(myfile, myline);
        curLine << myline;

        // std::cout << myline << std::endl;
        // std::cout << "curline: " << curLine.str() << std::endl;

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        id = stoi(currentField);

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        identifier = currentField;
        

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            generation_id = INT_MAX;
        }
        else{
            generation_id = stoi(currentField);
        }
        

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            type_id = INT_MAX;
        }
        else{
            type_id = stoi(currentField);
        }
        

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            power = INT_MAX;
        }
        else{
            power = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            pp = INT_MAX;
        }
        else{
            pp = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            accuracy = INT_MAX;
        }
        else{
            accuracy = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            priority = INT_MAX;
        }
        else{
            priority = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            target_id = INT_MAX;
        }
        else{
            target_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            damage_class_id = INT_MAX;
        }
        else{
            damage_class_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            effect_id = INT_MAX;
        }
        else{
            effect_id = stoi(currentField);
        }

        

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            effect_chance = INT_MAX;
        }
        else{
            effect_chance = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            contest_type_id = INT_MAX;
        }
        else{
            contest_type_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            contest_effect_id = INT_MAX;
        }
        else{
            contest_effect_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            super_contest_effect_id = INT_MAX;
        }
        else{
            super_contest_effect_id = stoi(currentField);
        }

        
        Move *m = new Move(id, identifier, generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_contest_effect_id);
        movesList.push_back(*m);


    }


}

void parsePokemonMoves(std::vector<Pokemon_Move>& pokemonMovesList){

    std::ifstream myfile;
    std::stringstream currLine;
    myfile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon_moves.csv");

    //check failure
    if (myfile.fail()){

        
        std::string home = std::getenv("HOME");
        std::string path = "/.poke327/pokedex/pokedex/data/csv/pokemon_moves.csv";
        // std::string path = "/testT.txt";
        std::cout << home + path;
        myfile.open(home + path);

    }

    std::string myline;
    std::string currentField;
    //read first line of file into 'myline'
    std::getline(myfile, myline);
    //put the contents of 'myline' into curLine
    currLine << myline;

    //print the contents of curLine to stdout
    // std::cout << currLine.str() << std::endl;

    //put the first 
    std::getline(currLine, currentField, ',');
    // std::cout << currentField << std::endl;
    currLine.str(std::string());

    int pokemon_id;
    int version_group_id;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;

    while(myfile.peek() != EOF){

        std::stringstream curLine;
        std::getline(myfile, myline);
        curLine << myline;

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            pokemon_id = INT_MAX;
        }
        else{
            pokemon_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            version_group_id = INT_MAX;
        }
        else{
            version_group_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            move_id = INT_MAX;
        }
        else{
            move_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            pokemon_move_method_id = INT_MAX;
        }
        else{
            pokemon_move_method_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            level = INT_MAX;
        }
        else{
            level = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            order = INT_MAX;
        }
        else{
            order = stoi(currentField);
        }

        Pokemon_Move *pm = new Pokemon_Move(pokemon_id, version_group_id, move_id, pokemon_move_method_id, level, order);
        pokemonMovesList.push_back(*pm);

    }

}

void parsePokemonSpecies(std::vector<Pokemon_Species>& pokemonSpeciesList){

    std::ifstream myfile;
    std::stringstream currLine;
    myfile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon_species.csv");

    //check failure
    if (myfile.fail()){

        
        std::string home = std::getenv("HOME");
        std::string path = "/.poke327/pokedex/pokedex/data/csv/pokemon_species.csv";
        // std::string path = "/testT.txt";
        std::cout << home + path;
        myfile.open(home + path);

    }

    
    std::string myline;
    std::string currentField;
    //read first line of file into 'myline'
    std::getline(myfile, myline);
    //put the contents of 'myline' into curLine
    currLine << myline;

    //print the contents of curLine to stdout
    // std::cout << currLine.str() << std::endl;

    //put the first 
    std::getline(currLine, currentField, ',');
    // std::cout << currentField << std::endl;
    currLine.str(std::string());

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

    while(myfile.peek() != EOF){

        std::stringstream curLine;
        std::getline(myfile, myline);
        curLine << myline;

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            id = INT_MAX;
        }
        else{
            id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            identifier = INT_MAX;
        }
        else{
            identifier = currentField;
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            generation_id = INT_MAX;
        }
        else{
            generation_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            evolves_from_species_id = INT_MAX;
        }
        else{
            evolves_from_species_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            evolution_chain_id = INT_MAX;
        }
        else{
            evolution_chain_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            color_id = INT_MAX;
        }
        else{
            color_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            shape_id = INT_MAX;
        }
        else{
            shape_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            habitat_id = INT_MAX;
        }
        else{
            habitat_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            gender_rate = INT_MAX;
        }
        else{
            gender_rate = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            capture_rate = INT_MAX;
        }
        else{
            capture_rate = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            base_happiness = INT_MAX;
        }
        else{
            base_happiness = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            is_baby = INT_MAX;
        }
        else{
            is_baby = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            hatch_counter = INT_MAX;
        }
        else{
            hatch_counter = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            has_gender_differences = INT_MAX;
        }
        else{
            has_gender_differences = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            growth_rate_id = INT_MAX;
        }
        else{
            growth_rate_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            forms_switchable = INT_MAX;
        }
        else{
            forms_switchable = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            is_legendary = INT_MAX;
        }
        else{
            is_legendary = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            is_mythical = INT_MAX;
        }
        else{
            is_mythical = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            order = INT_MAX;
        }
        else{
            order = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            conquest_order = INT_MAX;
        }
        else{
            conquest_order = stoi(currentField);
        }

        Pokemon_Species *ps = new Pokemon_Species(id, identifier, generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order);
        pokemonSpeciesList.push_back(*ps);
    }

}

void parseExperience(std::vector<Experience>& experienceList){

    std::ifstream myfile;
    std::stringstream currLine;
    myfile.open("/share/cs327/pokedex/pokedex/data/csv/experience.csv");

    //check failure
    if (myfile.fail()){

        
        std::string home = std::getenv("HOME");
        std::string path = "/.poke327/pokedex/pokedex/data/csv/experience.csv";
        // std::string path = "/testT.txt";
        std::cout << home + path;
        myfile.open(home + path);

    }
    
    std::string myline;
    std::string currentField;
    //read first line of file into 'myline'
    std::getline(myfile, myline);
    //put the contents of 'myline' into curLine
    currLine << myline;

    //print the contents of curLine to stdout
    // std::cout << currLine.str() << std::endl;

    //put the first 
    std::getline(currLine, currentField, ',');
    // std::cout << currentField << std::endl;
    currLine.str(std::string());

    int growth_rate_id;
    int level;
    int experience;

    while(myfile.peek() != EOF){

        std::stringstream curLine;
        std::getline(myfile, myline);
        curLine << myline;

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            growth_rate_id = INT_MAX;
        }
        else{
            growth_rate_id = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            level = INT_MAX;
        }
        else{
            level = stoi(currentField);
        }

        std::getline(curLine, currentField, ',');
        // std::cout << currentField << std::endl;
        if (currentField == ""){
            experience = INT_MAX;
        }
        else{
            experience = stoi(currentField);
        }

        Experience *e = new Experience(growth_rate_id, level, experience);
        experienceList.push_back(*e);
    }

}

void parseTypeNames(std::vector<Type_Name>& typeNameList){

            std::ifstream myfile;
            std::stringstream currLine;
            myfile.open("/share/cs327/pokedex/pokedex/data/csv/type_names.csv");

            //check failure
            if (myfile.fail()){

        
                std::string home = std::getenv("HOME");
                std::string path = "/.poke327/pokedex/pokedex/data/csv/type_names.csv";
                // std::string path = "/testT.txt";
                std::cout << home + path;
                myfile.open(home + path);

            }
            
            std::string myline;
            std::string currentField;
            //read first line of file into 'myline'
            std::getline(myfile, myline);
            //put the contents of 'myline' into curLine
            currLine << myline;

            //print the contents of curLine to stdout
            // std::cout << currLine.str() << std::endl;

            //put the first 
            std::getline(currLine, currentField, ',');
            // std::cout << currentField << std::endl;
            currLine.str(std::string());

            int type_id;
            int local_language_id;
            std::string name;

            while(myfile.peek() != EOF){

                std::stringstream curLine;
                std::getline(myfile, myline);
                curLine << myline;

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    type_id = INT_MAX;
                }
                else{
                    type_id = stoi(currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    local_language_id = INT_MAX;
                }
                else{
                    local_language_id = stoi(currentField);
                }

                if (local_language_id == 9){

                    std::getline(curLine, currentField, ',');
                    // std::cout << currentField << std::endl;
                    if (currentField == ""){
                        name = INT_MAX;
                    }
                    else{
                        name = currentField;
                    }

                    Type_Name *tn = new Type_Name(type_id, local_language_id, name);
                    typeNameList.push_back(*tn);

                }

            }


}

void parsePokemonStats(std::vector<Pokemon_Stat>& pokemonStatsList){

            std::ifstream myfile;
            std::stringstream currLine;
            myfile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon_stats.csv");

            //check failure
            if (myfile.fail()){

        
                std::string home = std::getenv("HOME");
                std::string path = "/.poke327/pokedex/pokedex/data/csv/pokemon_stats.csv";
                // std::string path = "/testT.txt";
                std::cout << home + path;
                myfile.open(home + path);

            }
            
            std::string myline;
            std::string currentField;
            //read first line of file into 'myline'
            std::getline(myfile, myline);
            //put the contents of 'myline' into curLine
            currLine << myline;

            //print the contents of curLine to stdout
            // std::cout << currLine.str() << std::endl;

            //put the first 
            std::getline(currLine, currentField, ',');
            // std::cout << currentField << std::endl;
            currLine.str(std::string());

            int pokemon_id;
            int stat_id;
            int base_stat;
            int effort;

             while(myfile.peek() != EOF){

                std::stringstream curLine;
                std::getline(myfile, myline);
                curLine << myline;

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    pokemon_id = INT_MAX;
                }
                else{
                    pokemon_id = stoi(currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    stat_id = INT_MAX;
                }
                else{
                    stat_id = stoi(currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    base_stat = INT_MAX;
                }
                else{
                    base_stat = stoi(currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    effort = INT_MAX;
                }
                else{
                    effort = stoi(currentField);
                }


                Pokemon_Stat *ps = new Pokemon_Stat(pokemon_id, stat_id, base_stat, effort);
                pokemonStatsList.push_back(*ps);

             }

}

void parseStats(std::vector<Stat>& statList){

            std::ifstream myfile;
            std::stringstream currLine;
            myfile.open("/share/cs327/pokedex/pokedex/data/csv/stats.csv");

            //check failure
            if (myfile.fail()){

        
                std::string home = std::getenv("HOME");
                std::string path = "/.poke327/pokedex/pokedex/data/csv/stats.csv";
                // std::string path = "/testT.txt";
                std::cout << home + path;
                myfile.open(home + path);

            }
            
            std::string myline;
            std::string currentField;
            //read first line of file into 'myline'
            std::getline(myfile, myline);
            //put the contents of 'myline' into curLine
            currLine << myline;

            //print the contents of curLine to stdout
            // std::cout << currLine.str() << std::endl;

            //put the first 
            std::getline(currLine, currentField, ',');
            // std::cout << currentField << std::endl;
            currLine.str(std::string());

            int id;
            int damage_class_id;
            std::string identifier;
            int is_battle_only;
            int game_index;

            while(myfile.peek() != EOF){

                std::stringstream curLine;
                std::getline(myfile, myline);
                curLine << myline;

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    id = INT_MAX;
                }
                else{
                    id = stoi(currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    damage_class_id = INT_MAX;
                }
                else{
                    damage_class_id = stoi(currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    identifier = INT_MAX;
                }
                else{
                    identifier = (currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    is_battle_only = INT_MAX;
                }
                else{
                    is_battle_only = stoi(currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    game_index = INT_MAX;
                }
                else{
                    game_index = stoi(currentField);
                }

                Stat *s = new Stat(id, damage_class_id, identifier, is_battle_only, game_index);
                statList.push_back(*s);

            }

}

void parsePokemonTypes(std::vector<Pokemon_Type>& pokemonTypes){

            std::ifstream myfile;
            std::stringstream currLine;
            myfile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon_types.csv");

            //check failure
            if (myfile.fail()){

        
                std::string home = std::getenv("HOME");
                std::string path = "/.poke327/pokedex/pokedex/data/csv/pokemon_types.csv";
                // std::string path = "/testT.txt";
                std::cout << home + path;
                myfile.open(home + path);

            }
            
            std::string myline;
            std::string currentField;
            //read first line of file into 'myline'
            std::getline(myfile, myline);
            //put the contents of 'myline' into curLine
            currLine << myline;

            //print the contents of curLine to stdout
            // std::cout << currLine.str() << std::endl;

            //put the first 
            std::getline(currLine, currentField, ',');
            // std::cout << currentField << std::endl;
            currLine.str(std::string());

            int pokemon_id;
            int type_id;
            int slot;

            while(myfile.peek() != EOF){

                std::stringstream curLine;
                std::getline(myfile, myline);
                curLine << myline;

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    pokemon_id = INT_MAX;
                }
                else{
                    pokemon_id = stoi(currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    type_id = INT_MAX;
                }
                else{
                    type_id = stoi(currentField);
                }

                std::getline(curLine, currentField, ',');
                // std::cout << currentField << std::endl;
                if (currentField == ""){
                    slot = INT_MAX;
                }
                else{
                    slot = stoi(currentField);
                }

                Pokemon_Type *pt = new Pokemon_Type(pokemon_id, type_id, slot);
                pokemonTypes.push_back(*pt);

            }




}

void maxCheck(int i){

    if (i == INT_MAX){

        std::cout << "";

    }
    else{

        std::cout << i;

    }

}

int main(int argc, char *argv[]){

    parsePokemon(p);
    parseMoves(m);
    parsePokemonMoves(pm);
    parsePokemonSpecies(ps);
    parseExperience(e);
    parseTypeNames(tn);
    parsePokemonStats(pst);
    parseStats(s);
    parsePokemonTypes(pt);


    // if (argc >= 1){
    //     std::string input = argv[1];
    
    // std::cout << input << std::endl;
    
    

    //     if (input == "pokemon"){
    //         std::vector<Pokemon> p;
    //         parsePokemon(p);
    //         for (int i = 0; i < p.size(); i++){

    //             std::cout << p[i].id << "," << p[i].identifier <<"," << p[i].species_id <<"," << p[i].height <<"," << p[i].weight <<"," << p[i].base_experience <<"," << p[i].order <<"," << p[i].is_default <<std::endl;

    //         }
            
    //     }
    //     else if (input == "moves"){
    
    //         std::vector<Move> m;
    //         parseMoves(m);
    //         for (int i = 0; i < m.size(); i++){

    //             // std::cout << m[i].id;
    //             maxCheck(m[i].id);
    //             std::cout << ",";
    //             std::cout<< m[i].identifier;
    //             std::cout<< ",";
    //             // std::cout<< m[i].generation_id;
    //             maxCheck(m[i].generation_id);
    //             std::cout<< ","; 
    //             // std::cout<< m[i].type_id;
    //             maxCheck(m[i].type_id);
    //             std::cout<< ","; 
    //             // std::cout<< m[i].power;
    //             maxCheck(m[i].power);
    //             std::cout<< ","; 
    //             // std::cout<< m[i].pp;
    //             maxCheck(m[i].pp);
    //             std::cout<< ","; 
    //             // std::cout<< m[i].accuracy;
    //             maxCheck(m[i].accuracy);
    //             std::cout<< "," ;
    //             // std::cout<< m[i].priority;
    //             maxCheck(m[i].priority);
    //             std::cout<< "," ;
    //             // std::cout<< m[i].target_id;
    //             maxCheck(m[i].target_id);
    //             std::cout<< "," ;
    //             // std::cout<< m[i].damage_class_id;
    //             maxCheck(m[i].damage_class_id);
    //             std::cout<< "," ;
    //             // std::cout<< m[i].effect_id;
    //             maxCheck(m[i].effect_id);
    //             std::cout<< "," ;
    //             // std::cout<< m[i].effect_chance;
    //             maxCheck(m[i].effect_chance);
    //             std::cout<< "," ;
    //             // std::cout<< m[i].contest_type_id;
    //             maxCheck(m[i].contest_type_id);
    //             std::cout<< "," ;
    //             maxCheck(m[i].contest_effect_id);
    //             std::cout<< "," ;
    //             // std::cout<< m[i].super_contest_effect_id << std::endl;
    //             maxCheck(m[i].super_contest_effect_id);
    //             std::cout << std::endl;
        
    //         }
            
    //     }
    //     else if (input == "pokemon_moves") {
        
    //         std::vector<Pokemon_Move> pm;
    //         parsePokemonMoves(pm);
    //         for (int i = 0; i < pm.size(); i++){

    //             // std::cout << pm[i].pokemon_id;
    //             maxCheck(pm[i].pokemon_id);
    //             std::cout << ",";
    //             // std::cout << pm[i].version_group_id;
    //             maxCheck(pm[i].version_group_id);
    //             std::cout << ",";
    //             // std::cout << pm[i].move_id;
    //             maxCheck(pm[i].move_id);
    //             std::cout << ",";
    //             // std::cout << pm[i].pokemon_move_method_id;
    //             maxCheck(pm[i].pokemon_move_method_id);
    //             std::cout << ",";
    //             // std::cout << pm[i].level;
    //             maxCheck(pm[i].level);
    //             std::cout << ",";
    //             // std::cout << pm[i].order;
    //             maxCheck(pm[i].order);
    //             std::cout << std::endl;

    //         }
            
    //     }
    //     else if (input == "pokemon_species") {
        
    //         std::vector<Pokemon_Species> ps;
    //         parsePokemonSpecies(ps);
    //         std::cout << ps.size() << std::endl;
    //         for (int i = 0 ; i < ps.size(); i++){

    //             if (ps[i].id == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].id;
    //             }
    //             // std::cout << ps[i].id;

    //             std::cout << ",";
    //             std::cout << ps[i].identifier;
    //             std::cout << ",";
    //             if (ps[i].generation_id == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].generation_id;
    //             }
    //             // std::cout << ps[i].generation_id;

    //             std::cout << ",";
    //             if (ps[i].evolves_from_species_id == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].evolves_from_species_id;
    //             }
    //             // std::cout << ps[i].evolves_from_species_id;
    //             std::cout << ",";
    //             if (ps[i].evolution_chain_id == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].evolution_chain_id;
    //             }
    //             // std::cout << ps[i].evolution_chain_id;

    //             std::cout << ",";
    //             if (ps[i].color_id == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].color_id;
    //             }
    //             // std::cout << ps[i].color_id;
    //             std::cout << ",";

    //             if (ps[i].shape_id == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].shape_id;
    //             }
    //             // std::cout << ps[i].shape_id;
    //             std::cout << ",";

    //             if (ps[i].habitat_id == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].habitat_id;
    //             }
    //             // std::cout << ps[i].habitat_id;
    //             std::cout << ",";

    //             if (ps[i].gender_rate == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].gender_rate;
    //             }
    //             // std::cout << ps[i].gender_rate;
    //             std::cout << ",";

    //             if (ps[i].capture_rate == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].capture_rate;
    //             }
    //             // std::cout << ps[i].capture_rate;
    //             std::cout << ",";

    //             if (ps[i].base_happiness == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].base_happiness;
    //             }
    //             // std::cout << ps[i].base_happiness;
    //             std::cout << ",";

    //             if (ps[i].is_baby == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].is_baby;
    //             }
    //             // std::cout << ps[i].is_baby;
    //             std::cout << ",";

    //             if (ps[i].hatch_counter == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].hatch_counter;
    //             }
    //             // std::cout << ps[i].hatch_counter;
    //             std::cout << ",";

    //             if (ps[i].has_gender_differences == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].has_gender_differences;
    //             }
    //             // std::cout << ps[i].has_gender_differences;
    //             std::cout << ",";

    //             if (ps[i].growth_rate_id == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].growth_rate_id;
    //             }
    //             // std::cout << ps[i].growth_rate_id;
    //             std::cout << ",";

    //             if (ps[i].forms_switchable == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].forms_switchable;
    //             }
    //             // std::cout << ps[i].forms_switchable;
    //             std::cout << ",";

    //             if (ps[i].is_legendary == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].is_legendary;
    //             }
    //             // std::cout << ps[i].is_legendary;
    //             std::cout << ",";

    //             if (ps[i].is_mythical == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].is_mythical;
    //             }
    //             // std::cout << ps[i].is_mythical;
    //             std::cout << ",";

    //             if (ps[i].order == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].order;
    //             }
    //             // std::cout << ps[i].order;
    //             std::cout << ",";

    //             if (ps[i].conquest_order == INT_MAX){
    //                 std::cout << "";
    //             }
    //             else{
    //                 std::cout << ps[i].conquest_order;
    //             }
    //             // std::cout << ps[i].conquest_order;
    //             std::cout << std::endl;

    //         }
            
    //     }
    //     else if (input == "experience") {
        
    //         std::vector<Experience> e;
    //         parseExperience(e);
    //         for (int i = 0; i < e.size(); i++){

    //             std::cout << e[i].growth_rate_id;
    //             std::cout << ",";
    //             std::cout << e[i].level;
    //             std::cout << ",";
    //             std::cout << e[i].experience;
    //             std::cout << std::endl;

    //         }
            
    //     }
    //     else if (input == "type_names") {
        
    //         std::vector<Type_Name> tn;
    //         parseTypeNames(tn);
    //         for (int i = 0; i < tn.size(); i++){

    //             std::cout << tn[i].type_id;
    //             std::cout << ",";
    //             std::cout << tn[i].local_language_id;
    //             std::cout << ",";
    //             std::cout << tn[i].name;
    //             std::cout << std::endl;


    //         }
            
    //     }
    //     else if (input == "pokemon_stats") {
        
    //         std::vector<Pokemon_Stat> ps;
    //         parsePokemonStats(ps);
    //         for (int i  = 0; i < ps.size(); i++){

    //             // std::cout << ps[i].pokemon_id;
    //             maxCheck(ps[i].pokemon_id);
    //             std::cout << ",";
    //             // std::cout << ps[i].stat_id;
    //             maxCheck(ps[i].stat_id);
    //             std::cout << ",";
    //             // std::cout << ps[i].base_stat;
    //             maxCheck(ps[i].base_stat);
    //             std::cout << ",";
    //             // std::cout << ps[i].effort;
    //             maxCheck(ps[i].effort);
    //             std::cout << std::endl;

    //         }
            
    //     }
    //     else if (input == "stats") {
        
    //         std::vector<Stat> s;
    //         parseStats(s);
    //         for (int i = 0; i < s.size(); i++){

    //             // std::cout << s[i].id;
    //             maxCheck(s[i].id);
    //             std::cout << ",";
    //             // std::cout << s[i].damage_class_id;
    //             maxCheck(s[i].damage_class_id);
    //             std::cout << ",";
    //             std::cout << s[i].identifier;
                
    //             std::cout << ",";
    //             // std::cout << s[i].is_battle_only;
    //             maxCheck(s[i].is_battle_only);
    //             std::cout << ",";
    //             // std::cout << s[i].game_index;
    //             maxCheck(s[i].game_index);
    //             std::cout << std::endl;

    //         }
            
    //     }
    //     else if (input == "pokemon_types") {
        
    //         std::vector<Pokemon_Type> pt;
    //         parsePokemonTypes(pt);
    //         for (int i = 0; i < pt.size(); i++){

    //             std::cout << pt[i].pokemon_id;
    //             std::cout << ",";
    //             std::cout << pt[i].type_id;
    //             std::cout << ",";
    //             std::cout << pt[i].slot;
    //             std::cout << std::endl;

    //         }
            
            
    //     }
    
    // }
    

    // std::vector<Pokemon> poke;
    // parsePokemon(poke);
    // for (int i = 0; i < poke.size(); i++){

    //     std::cout << (poke.at(i)).id << " " << (poke.at(i)).identifier << std::endl; 

    // }

    // std::vector<Move> move;
    // parseMoves(move);

    

    // std::ifstream myfile;
    // std::stringstream curLine;
    // myfile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon.csv");

    // // std::vector<Pokemon> pokemonList;

    // std::string myline;

    // //read first line of file into 'myline'
    // std::getline(myfile, myline);
    // //put the contents of 'myline' into curLine
    // curLine << myline;

    // //print the contents of curLine to stdout
    // std::cout << curLine.str() << std::endl;

    // std::string currentField;

    // //put the first 
    // std::getline(curLine, currentField, ',');
    // std::cout << currentField << std::endl;

    // while (myfile.peek() != EOF){

    //     std::getline(myfile, myline);
    //     curLine << myline;

    //     std::cout << curLine.str() << std::endl;

    //     std::cout << "\n" << std::endl;

    //     curLine.str(std::string());
    // }

    // // while (myfile.peek() != EOF){

    // //     std::getline(myfile, myline, ',');
    // //     if (myline[0] == ('\n')){

    // //         std::cout << "newline" << std::endl;
    // //         std::cout << myline << std::endl;
    // //         return 1;

    // //     }
    // //     std::cout << myline << std::endl;
        
    // // }


    // return 1;

    io_init_terminal();

    int x;
    x=0;
    // x = 8;
    // if (strcmp(argv[1], "--numtrainers")){

    //     x = atoi(argv[2]);

    // }

    // printf("parameter 1 is: %d\n", x);

    // printf("%d\n", argc);

    // printf("%s\n", argv[1]);

    char str1[] = "--numtrainers";

    if (argc == 3 && strcmp(str1, argv[1]) == 0){

        printf("Used switch\n");
        x = atoi(argv[2]);

    }
    else{

        printf("Switch not used\n");

    }

    
    
    

    //init RNG
    srand(time(NULL));



    world_t world;
    struct Point worldStart;
    initPoint(&worldStart, 0, 0);
    world.currentCoordinates = worldStart;
    world.worldMaps[0][0] = NULL;
    int i, j;
    for (i = 0; i < 401; i++){

        for (j = 0; j < 401; j++){

            world.worldMaps[i][j] = NULL;

        }

    }
    

    struct Point p;
    initPoint(&p, 0, 0);
    arrE_t p1;
    initArrE(&p1, p);
    world.worldMaps[p1.row][p1.col] = (Map *) malloc(sizeof(Map));
    Map *start = &*world.worldMaps[p1.row][p1.col];
    initMap(&world, p);
    printMap(*start);


    playerChar *pc;
    // pc = (playerChar_t *) malloc(sizeof(pc));
    pc = new playerChar();
    placePlayerChar(pc, start);
    printMap(*start);

    NPC NPC1;
    NPCtoHiker(&NPC1);
    
    // addNPC(start, 'h');

    NPC NPC2;
    NPCtoRival(&NPC2);

    // addNPC(start, 'r');

    findShortestPaths(pc, start, &NPC1, start);
    findShortestPaths(pc, start, &NPC2, start);
    printMap(*start);


    // spawnNPC(start, &NPC1);
    // spawnNPC(start, &NPC2);
    printMap(*start);

    // NPC_t pacer;
    // NPC_t wanderer;
    NPC explorer;
    // NPCtoPacer(&pacer);
    if (x == 0){
        addNPC(start, 'h');
        addNPC(start, 'r');
        addNPC(start, 'e');
        addNPC(start, 'p');
        addNPC(start, 'w');
        addNPC(start, 'e');
        addNPC(start, 'w');
        addNPC(start, 'p');
    }
    else{

        addNPC(start, 'h');
        addNPC(start, 'r');

        int i, j;
        for (i = 0; i < x - 2; i++){

            if (i % 3 == 1){

                addNPC(start, 'w');

            }
            else if(i % 3 == 2){

                addNPC(start, 'e');

            }
            else{

                addNPC(start, 'p');

            }

        }


    }
    // NPCtoWanderer(&wanderer);
    // spawnNPC(start, &pacer);
    // spawnNPC(start, &wanderer);
    // spawnNPC(start, explorer);
    pickPokemon(start);
    printMap(*start);
    printw("-------------------------------------------\n");

    // arrE_t move;
    // generateMove(*start, pc, NPC1, &move);
    // printf("%d", move.row);

    Map * currentMap = start;
    while (true){

        currentMap = simulateGame(currentMap, &world);
        

        if (currentMap == NULL){

            break;

        }

    }

    // simulateGame(start, &world);

    endwin();


    // findShortestPaths(pc, *start, NPC2);
    


    // char c;
    // char trash;q
    // int w = 1;
    // arrE_t currentEntry;
    // while (w != 0){

    //     // printf("Enter command:\n");
        

    //     scanf("%c", &c);



        
    //     switch (c)
    //     {
    //     case 'n':
    //         //move to map north of current position
    //         struct Point north;
    //         initPoint(&north, world.currentCoordinates.x, world.currentCoordinates.y + 1);


    //         if (!validatePoint(north)){

    //             break;

    //         }


    //         arrE_t north1;
    //         initArrE(&north1, north);
    //         if (world.worldMaps[north1.row][north1.col] == NULL){

    //             world.worldMaps[north1.row][north1.col] = malloc(sizeof(map_t));
    //             initMap(&world, north);

    //         }
    //         world.currentCoordinates = north;

    //         currentEntry;
    //         initArrE(&currentEntry, world.currentCoordinates);
    //         printMap(*world.worldMaps[currentEntry.row][currentEntry.col]);

    //     break;




    //     case 'e':
    //         //move to map east of current position
    //         struct Point east;
    //         initPoint(&east, world.currentCoordinates.x + 1, world.currentCoordinates.y);


    //         if (!validatePoint(east)){

    //             break;

    //         }


    //         arrE_t east1;
    //         initArrE(&east1, east);
    //         if (world.worldMaps[east1.row][east1.col] == NULL){

    //             world.worldMaps[east1.row][east1.col] = malloc(sizeof(map_t));
    //             initMap(&world, east);

    //         }
    //         world.currentCoordinates = east;

    //         currentEntry;
    //         initArrE(&currentEntry, world.currentCoordinates);
    //         printMap(*world.worldMaps[currentEntry.row][currentEntry.col]);

    //         break;

    //     case 's':
    //         //move to map south
    //         struct Point south;
    //         initPoint(&south, world.currentCoordinates.x, world.currentCoordinates.y - 1);

    //         if (!validatePoint(south)){

    //             break;

    //         }


    //         arrE_t south1;
    //         initArrE(&south1, south);
    //         if (world.worldMaps[south1.row][south1.col] == NULL){

    //             world.worldMaps[south1.row][south1.col] = malloc(sizeof(map_t));
    //             initMap(&world, south);

    //         }
    //         world.currentCoordinates = south;

    //         currentEntry;
    //         initArrE(&currentEntry, world.currentCoordinates);
    //         printMap(*world.worldMaps[currentEntry.row][currentEntry.col]);

    //         break;
    //     case 'w':
    //         //move to map west
    //         struct Point west;
    //         initPoint(&west, world.currentCoordinates.x - 1, world.currentCoordinates.y);

    //         if (!validatePoint(west)){

    //             break;

    //         }


    //         arrE_t west1;
    //         initArrE(&west1, west);
    //         if (world.worldMaps[west1.row][west1.col] == NULL){

    //             world.worldMaps[west1.row][west1.col] = malloc(sizeof(map_t));
    //             initMap(&world, west);

    //         }
    //         world.currentCoordinates = west;

    //         currentEntry;
    //         initArrE(&currentEntry, world.currentCoordinates);
    //         printMap(*world.worldMaps[currentEntry.row][currentEntry.col]);


    //         break;
    //     case 'f':
    //         //move to map (x, y) of the world 
    //         int x, y;
    //         scanf("%d %d", &x, &y);
    //         printf("%d %d\n", x, y);
    //         struct Point desired;
    //         initPoint(&desired, x, y);
    //         if (!validatePoint(desired)){

    //             break;

    //         }


    //         arrE_t desired1;
    //         initArrE(&desired1, desired);
    //         if (world.worldMaps[desired1.row][desired1.col] == NULL){

    //             world.worldMaps[desired1.row][desired1.col] = malloc(sizeof(map_t));
    //             initMap(&world, desired);

    //         }
    //         world.currentCoordinates = desired;

    //         currentEntry;
    //         initArrE(&currentEntry, world.currentCoordinates);
    //         printMap(*world.worldMaps[currentEntry.row][currentEntry.col]);


           
    //         break;
    //     case 'q':
    //         w = 0;
    //     break;
    //     default:
    //         // arrE_t currentEntry;
    //         // initArrE(&currentEntry, world.currentCoordinates);
    //         // printMap(*world.worldMaps[currentEntry.row][currentEntry.col]);
    //         break;
            
    //     }

        


        


    // }


    return 0;
}

void printPc(playerChar *p){



}

bool validatePoint(struct Point p){

    if (p.x < -200 || p.x > 200 || p.y < -200 || p.y  > 200){
        printf("Coordinates out of bounds. Enter coordinates within [-200, 200]\n");
        return false;
    }

    return true;

}

void dumpBoard(char gameBoard[21][80]){

    int i, j;
    for (i = 0; i < 21; i++){
        for (j = 0; j < 80; j++){

            //printf("%c", gameBoard[i][j]);
            addch(gameBoard[i][j]);

        }
        addch('\n');
    }

    addch('\n');


}

struct Point findPair(struct Point p1, struct Point p2){

    struct Point result;

    if (p1.x == p2.x && p1.y == p2.y){

        //just pick top right corner
        result.x = 79;
        result.y = 1;
        return result;

    }

    if (p1.y == p2.y && max(p1.y, p2.y) < 19){

        if (p1.y > p2.y){

            p1.y++;

        }
        else{

            p2.y++;

        }

    }

    if (p1.x == p2.x && max(p1.x, p2.x) < 79){

        if (p1.x > p2.x){

            p1.x++;

        }
        else{

            p2.x++;

        }

    }

    //goal here is to return a point s such that the rectangle defined by p1 and s is disjoint by the 
    //rectangle that will eventually be defined by p2 and s' (s' being a point return from this function)

    if (p1.x > p2.x && p1.y > p2.y){

        result.x = (rand() % (77 - p1.x)) + p1.x; //[p1.x, 77]
        result.y = (rand() % (18 - p1.y)) + p1.y; //[p1.y, 18]
        // printf(">>");

    }
    else if (p1.x < p2.x && p1.y < p2.y){

        result.x = (rand() % (p1.x - 1)) + 1; //[1, p1.x]
        result.y = (rand() % (p1.y - 1)) + 1; //[1, p1.y]
        // printf("<<");

    }
    else if (p1.x < p2.x && p1.y > p2.y){

        result.x = (rand() % (p1.x - 1)) + 1; //[1, p1.x]
        result.y = (rand() % (18 - p1.y)) + p1.y; //[p1.y, 18]
        // printf("<>");

    }
    else if (p1.x > p2.x && p1.y < p2.y){

        result.x = (rand() % (77 - p1.x)) + p1.x; //[p1.x, 77]
        result.y = (rand() % (p1.y - 1)) + 1; //[1, p1.y]
        // printf("><");

    }
    else{

        printf("ERROR IN findPair\n");
        result.x = 1;
        result.y = 1;

    }

    return result;


}

void fillRectangle(struct Point p1, struct Point p2, char gameBoard[21][80], char c){

    //maybe do bound checking . . . come back to it

    int i, j;
    for (j = min(p1.y, p2.y); j < max(p1.y, p2.y); j++){
        for (i = min(p1.x, p2.x); i < max(p1.x, p2.x); i++){

            gameBoard[j][i] = c;

        }
    }


}

int min(int a, int b){

    if (a > b){

        return b;

    }
    else{

        return a;

    }

    if (a == b){

        return a;

    }

}

int max(int a, int b){

    if (a > b){

        return a;

    }
    else{

        return b;

    }

    if (a == b){

        return a;

    }

}
//need to add parameter world_t world
void generatePaths(char gameBoard[21][80], world_t* world, struct Point p){

    char c = '#';

    //first do E-W path

    //west gate y coord
    int y1 = (rand() % 19) + 1; // [1, 19]
    //east gate y coord
    int y2 = (rand() % 19) + 1; // [1, 19]

    //generate x value
    int x1 = (rand() % 78 + 1); // [1, 78]


    struct Point e;
    initPoint(&e, p.x + 1, p.y);
    struct Point w;
    initPoint(&w, p.x - 1, p.y);
    arrE_t e1, w1;
    initArrE(&e1, e);
    initArrE(&w1, w);
    // int eGate, wGate;
    if (world->worldMaps[e1.row][e1.col] != NULL && validatePoint(e)){

        //switched eGate to y1
        y2 = world->worldMaps[e1.row][e1.col]->wG;

    }

    if (world->worldMaps[w1.row][w1.col] != NULL && validatePoint(w)){

        //switched wGate to y2
        y1 = world->worldMaps[w1.row][w1.col]->eG;

    }

    arrE_t p1;
    initArrE(&p1, p);
    world->worldMaps[p1.row][p1.col]->wG = y1;
    world->worldMaps[p1.row][p1.col]->eG = y2;














    //draw straight line from (1, y1) to (x1, y1)
    int i;
    for (i = 0; i < x1 + 1; i++){

        gameBoard[y1][i] = c;

    }

    //draw straight line from (x1, y2) to (78, y2)
    for (i = x1; i < 80; i++){

        gameBoard[y2][i] = c;

    }

    //draw straight line up/down from (x1, y2) to (x1 + 1, y2)
    for (i = min(y1, y2); i < max(y1, y2); i++){

        gameBoard[i][x1] = c;

    }

    // if (y2 < y1){

    //     gameBoard[y1][x1] = c;

    // }

    //now for N-S path

    
    //north gate
    int a1 = (rand() % 78 + 1); // [1, 78]
    //southgate
    int a2 = (rand() % 78 + 1); // [1, 78]

    struct Point n;
    initPoint(&n, p.x, p.y + 1);
    struct Point s;
    initPoint(&s, p.x, p.y - 1);
    arrE_t n1, s1;
    initArrE(&n1, n);
    initArrE(&s1, s);
    // int nGate, sGate;
    if (world->worldMaps[n1.row][n1.col] != NULL && validatePoint(n)){

        a1 = world->worldMaps[n1.row][n1.col]->sG;

    }

    if (world->worldMaps[s1.row][s1.col] != NULL && validatePoint(s)){

        a2 = world->worldMaps[s1.row][s1.col]->nG;

    }

    //gen a y value
    int b1 = (rand() % 19) + 1; // [1, 19]

    for (i = 0; i < b1 + 1; i++){

        gameBoard[i][a1] = c;

    }

    for (i = b1; i < 21; i++){

        gameBoard[i][a2] = c;

    }

    for(i = min(a1, a2); i < max(a1, a2); i++){

        gameBoard[b1][i] = c;

    }

    world->worldMaps[p1.row][p1.col]->sG = a2;
    world->worldMaps[p1.row][p1.col]->nG = a1;






    int j;

    if (p.x == 200){
        for (i = 0; i < 21; i++){

            world->worldMaps[p1.row][p1.col]->screen[i][79] = '%';

            
        }
    }
    else if (p.x == -200){

        for (i = 0; i < 21; i++){

            world->worldMaps[p1.row][p1.col]->screen[i][0] = '%';

            
        }

    }

    if (p.y == 200){

        for (j = 0; j < 80; j++){

            world->worldMaps[p1.row][p1.col]->screen[0][j] = '%';


        }

    }
    else if (p.y == -200){

        for (j = 0; j < 80; j++){

            world->worldMaps[p1.row][p1.col]->screen[20][j] = '%';


        }

    }


    //want to add element of randomness to building spawn
    //first must calculate the probability that we want a given building to spawn
    float x = (min(200, (p.x + p.y)) * -45);
    x = x / 200;
    x = x + 50;
    x = x / 100;
    

    float buildProb = x * 100;
    // printf("%f", buildProb);
    int randM = (rand() % 99); // [0, 99]
    int randC = (rand() % 99); // [0, 99]

    int count;
    count = 0;

    
    while (true){

        if (count == 100){

            break;

        }

        int s = ((rand() % 76) + 1); // [1, 77]

        int currentY = 0;

        while (true){

            if (gameBoard[currentY][s] == '#'){

                break;

            }

            currentY++;

        }

        //look if we can fit buildings
        if (currentY > 1 && currentY < 18){

            if ((p.x == 0 && p.y == 0) || randM < buildProb){
                gameBoard[currentY - 1][s] = 'M';
            }

            if ((p.x == 0 && p.y == 0) || randC < buildProb){
                gameBoard[currentY + 1][s] = 'C';
            }

            break;

        }

        count++;


    }


}

