#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include<string.h>
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
void spawnNPC(map_t *m, NPC_t *npc);
int terrainCostPC(char c);
int gateCheck(int r, int c);


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

void placePlayerChar(playerChar_t *PC, map_t *map){

    
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

                    NPC_t *player;
                    player = malloc(sizeof(NPC_t));
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

void printMap(map_t map){

    // printf("eG: %d\n", map.eG);
    // printf("wG: %d\n", map.wG);
    // printf("nG: %d\n", map.nG);
    // printf("sG: %d\n", map.sG);

    clear();

    dumpBoard(map.screen);
    printf("World coordinates:");
    printPoint(map.worldCoordinates);
    printf("NPCs: ");
    printf("%d", map.npcCount);
    printf("\n");


}

void initScreen(map_t *map){

    
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

    printf("(");
    printf("%d", p.x);
    printf(", ");
    printf("%d", p.y);
    printf(")\n");
    

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

void NPCtoHiker(NPC_t *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 15;
    npc->SGras = 10;
    npc->Mtn = 4000;
    npc->Forest = 15;


    npc->type = 'h';


}

void NPCtoRival(NPC_t *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 20;
    npc->SGras = 10;
    npc->Mtn = 4000;
    npc->Forest = 4000;

    npc->type = 'r';

}

void NPCtoPacer(NPC_t *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 20;
    npc->SGras = 10;
    npc->Mtn = 4000;
    npc->Forest = 4000;

    npc->type = 'p';

}

void NPCtoWanderer(NPC_t *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 20;
    npc->SGras = 10;
    npc->Mtn = 4000;
    npc->Forest = 4000;

    npc->type = 'w';


}

void NPCtoExplorer(NPC_t *npc){

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

int terrainCost(NPC_t npc, char c){

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

void addNPC(map_t *map, char c){

    NPC_t *npc;
    npc = malloc(sizeof(NPC_t));

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


}

void spawnNPC(map_t *m, NPC_t *npc){

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
void generateMove(map_t *m, playerChar_t *p, NPC_t *npc, arrE_t *arr){

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

void findShortestPaths(playerChar_t *p, map_t *map, NPC_t *npc, map_t *m){

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
            newNode = malloc(sizeof(node_t));

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


    printHeatMap(heatMap);

    //we put all nodes into heap;
    node_t *curNode;

    while ((curNode = heap_remove_min(&h))){

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
    printHeatMap(heatMap);





}

void computeHeatMaps(map_t *map){





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
typedef struct npcNode{

    int costOfNextMove;

    NPC_t *npc;

    int sequenceNumber;

}npcNode_t;

void initNpcNode(npcNode_t *node, int cost, NPC_t *guy, int seqNum){

    node->costOfNextMove = cost;
    node->npc = guy;
    node->sequenceNumber = seqNum;



}

static int32_t costCompare(const void *key, const void *with) {
  return ((npcNode_t *) key)->costOfNextMove - ((npcNode_t *) with)->costOfNextMove + (((npcNode_t *) key))->sequenceNumber - ((npcNode_t *) with)->sequenceNumber;
}

void moveGuy(map_t *map, NPC_t *npc, arrE_t dest){

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

void printNPC(NPC_t *npc){

    if (npc == NULL){

        printf("null pointer");

    }
    else{
        printf("row %d\n", npc->row);
        printf("col %d\n", npc->col);
        printf("type %c\n\n", npc->type);
    }

    printf("done printing this npc\n");

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

void movePC(int row, int col, map_t *map){

    int oldrow = row;
    int oldcol = col;

    map->pc->col = col;
    map->pc->row = row;
    map->screen[oldrow][oldcol] = map->terrainOnly[oldrow][oldcol];
    map->screen[row][col] = '@';
    NPC_t *player = map->characterTracker[oldrow][oldcol];
    map->characterTracker[oldrow][oldcol] = NULL;
    map->characterTracker[row][col] = player;

};

void simulateGame(map_t *map){

    //Need to make some sort of player object that will act as our vector to use the heap
    //Need to make sure that players are init into the characterMap

    // NPC_t *dummyHiker;
    // NPC_t *dummyRival;
    // dummyHiker = malloc(sizeof(NPC_t));
    // dummyRival = malloc(sizeof(NPC_t));
    // NPCtoHiker(dummyHiker);
    // NPCtoRival(dummyRival);
    

    NPC_t dummyHiker;
    NPC_t dummyRival;
    NPCtoHiker(&dummyHiker);
    NPCtoRival(&dummyRival);


    findShortestPaths(map->pc, map, &dummyHiker, map);
    findShortestPaths(map->pc, map, &dummyRival, map);

    //create heap
    heap_t h;
    heap_init(&h, costCompare, NULL);

    
    //first create a heap item for the pc
    npcNode_t *playerChar;
    playerChar = malloc(sizeof(npcNode_t));
    //(for now, just init next move to ten, later this will be variable obviously)
    initNpcNode(playerChar, 0, NULL, 0);
    heap_insert(&h, playerChar);


    

    int currentCost;
    int i;
    for (i = 0; i < map->npcCount; i++){

        npcNode_t *currentNPC;
        currentNPC = malloc(sizeof(npcNode_t));
        // printNPC(map->npcList[i]);
        initNpcNode(currentNPC, 0, map->npcList[i], i + 1);

        // printNpcNode(currentNPC);

        heap_insert(&h, currentNPC);

    }

    

    npcNode_t *current;
    while ((current = heap_remove_min(&h))){
        
        printNpcNode(current);

        //this if check if the current turn is the players or an npcs
        //the player has a NULL npc pointer
        printNPC(current->npc);
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
        printf("right above if");
        if (current->npc != NULL){

            printf("entered else statement");
            
            arrE_t *currentNextSpot;
            currentNextSpot = malloc(sizeof(arrE_t));

            generateMove(map, map->pc, current->npc, currentNextSpot);
            currentCost = terrainCost(*(current->npc), map->terrainOnly[currentNextSpot->row][currentNextSpot->col]);

            //actually move there, like update the visuals and map stuff
            moveGuy(map, current->npc, *currentNextSpot);

            current->costOfNextMove = current->costOfNextMove + currentCost;
            heap_insert(&h, current);

            // printMap(*map);

        }
        else{
            //gen a new pc node with + 10 cost just so it takes a turn;
            //IF THE PC MOVES, THEN WE REGEN THE MAPS

            //take input to figure out where we want to move
            int row;
            int col;
            char curMove = getch();
            switch (curMove){

                case '7':
                    //upper left - Northwest
                    row = map->pc->row - 1;
                    col = map->pc->col - 1;

                    if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                        movePC(row, col, map);

                    }
                    
                    break;
                case '8':
                    //up - North
                    row = map->pc->row - 1;
                    col = map->pc->col;

                    if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                        movePC(row, col, map);

                    }
                    break;
                case '9':
                    //upper right - Northeast
                    row = map->pc->row - 1;
                    col = map->pc->col + 1;

                    if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                        movePC(row, col, map);

                    }
                    break;
                case '6':
                    //right - East
                    row = map->pc->row;
                    col = map->pc->col + 1;

                    if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                        movePC(row, col, map);

                    }
                    break;
                case '3':
                    //lower right - Southeast
                    row = map->pc->row + 1;
                    col = map->pc->col + 1;

                    if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                        movePC(row, col, map);

                    }
                    break;
                case '2':
                    //down - South
                    row = map->pc->row + 1;
                    col = map->pc->col;

                    if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                        movePC(row, col, map);

                    }
                    break;
                case '1':
                    //lower left - Southwest
                    row = map->pc->row + 1;
                    col = map->pc->col - 1;

                    if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                        movePC(row, col, map);

                    }
                    break;
                case '4':
                    //left - West
                    row = map->pc->row;
                    col = map->pc->col - 1;

                    //need to check some stuff
                    //-can we travel into this terrain type?
                    //-is there a player there?

                    if (terrainCostPC(map->terrainOnly[row][col]) < 4000 && map->characterTracker[row][col] == NULL && gateCheck(row, col)){

                        movePC(row, col, map);

                    }
                    break;
                case '>':
                    break;
                case '5':
                    break;
                case 't':
                    break;
                case KEY_UP:
                    break;
                case KEY_DOWN:
                    break;
                case 27:
                    break;
                case 'Q':
                    break;


                default:
                    printf("entry not recognized");
                    break;
            }
            current->costOfNextMove = current->costOfNextMove + terrainCostPC(map->terrainOnly[row][col]);
            heap_insert(&h, current);
            printMap(*map);
            usleep(250000);
        }
    }


    
}

void io_init_terminal(void){

    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);


}

int gateCheck(int r, int c){

    if (r == 0 || r == 79 || c == 0 || c == 20){

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

void dummyFunction(map_t *map, playerChar_t *player, NPC_t *npc){

    printf("hello world");
    return;

}


int main(int argc, char *argv[]){

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
    world.worldMaps[p1.row][p1.col] = malloc(sizeof(map_t));
    map_t *start = &*world.worldMaps[p1.row][p1.col];
    initMap(&world, p);
    printMap(*start);


    playerChar_t *pc;
    pc = malloc(sizeof(pc));
    placePlayerChar(pc, start);
    printMap(*start);

    NPC_t NPC1;
    NPCtoHiker(&NPC1);
    
    // addNPC(start, 'h');

    NPC_t NPC2;
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
    NPC_t explorer;
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
    printMap(*start);
    printw("-------------------------------------------\n");

    // arrE_t move;
    // generateMove(*start, pc, NPC1, &move);
    // printf("%d", move.row);

    simulateGame(start);




    // findShortestPaths(pc, *start, NPC2);
    


    // char c;
    // char trash;
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

void printPc(playerChar_t *p){



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

