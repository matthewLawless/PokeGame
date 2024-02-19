#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "Terrain_Generation.h"


void dumpBoard(char gameBoard[21][80]);
int max(int a, int b);
int min(int a, int b);
struct Point findPair(struct Point p1, struct Point p2);
void fillRectangle(struct Point p1, struct Point p2, char gameBoard[21][80], char c);
void generatePaths(char gameBoard[21][80], world_t* world, struct Point p);
void generateTerrain(char gameBoard[21][80]);
void printPoint(struct Point p);
bool validatePoint(struct Point p);


// struct Point {

//     int x;
//     int y;

// };

typedef struct node{

    int row;
    int col;

    int distance;

}node_t;

void initPoint(struct Point* p, int x, int y){

    p->x = x;
    p->y = y;

}

typedef struct playerChar{

    int row;
    int col;

}playerChar_t;

void placePlayerChar(playerChar_t *PC, map_t *map){


    while (true){
        int row = (rand() % 19) + 1; // [1, 19]
        int col = (rand() % 78 + 1); // [1, 78]

        if (map->screen[row][col] != '#'){

            map->screen[row][col] = '@';
            PC->col = col;
            PC->row = row;
            break;

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



    dumpBoard(map.screen);
    printf("World coordinates:");
    printPoint(map.worldCoordinates);


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
    initScreen(world->worldMaps[h1.row][h1.col]);
    generateTerrain(world->worldMaps[h1.row][h1.col]->screen);

    generatePaths(world->worldMaps[h1.row][h1.col]->screen, world, h);

}

void printPoint(struct Point p){

    printf("(");
    printf("%d", p.x);
    printf(", ");
    printf("%d", p.y);
    printf(")\n");
    

}

typedef struct NPC{

    int row;
    int col;

    int Path;
    int PMart;
    int PCntr;
    int TGras;
    int SGras;
    int Mtn;
    int Forest; 

}NPC_t;

void NPCtoHiker(NPC_t *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 15;
    npc->SGras = 10;
    npc->Mtn = 15;
    npc->Forest = 15;


}

void NPCtoRival(NPC_t *npc){

    npc->Path = 10;
    npc->PMart = 50;
    npc->PCntr = 50;
    npc->TGras = 20;
    npc->SGras = 10;

}

void printHeatMap(node_t *heatMap[21][80]){

    int i, j;
    for (i = 0; i < 21; i++){

        for (j = 0; j < 80; j++){

            int dMod = heatMap[i][j]->distance % 100;
            if (dMod == 0){

                printf("%d", 0);
                printf("%d", 0);


            }else{
                printf("%d", dMod);
            }
            if(j != 79){
            printf(" ");
            }

        }
        printf("\n");

    }


}


static int32_t path_cmp(const void *key, const void *with) {
  return ((node_t *) key)->distance - ((node_t *) with)->distance;
}



void initNode(node_t *n, int row, int col, int distance){

    n->row = row;
    n->col = col;
    n->distance = distance;

}

int findShortestPaths(playerChar_t p, map_t map, NPC_t npc){

    node_t *heatMap[21][80];

    heap_t h;
    heap_init(&h, path_cmp, NULL);

    
    //init all nodes to inf
    int i, j;
    for (i = 0; i < 21; i++){

        for (j = 0; j < 80; j++){

            node_t *newNode;
            newNode = malloc(sizeof(node_t));

            if (i == p.row && j == p.col){

                initNode(newNode, i, j, 0);

            }
            else{
                initNode(newNode, i, j, 4000);
            }
            heap_insert(&h, newNode);
            heatMap[i][j] = newNode;

        }

    }


    
    


    arrE_t currentSpot;
    currentSpot.row = p.row;
    currentSpot.col = p.col;

    printHeatMap(heatMap);

    //we put all nodes into heap;
    node_t *curNode;

    while (curNode = heap_remove_min(&h)){

        printf("%d", curNode->distance);
        break;

    }
    




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



int main(int argc, char *argv[]){

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


    playerChar_t pc;
    placePlayerChar(&pc, start);
    printMap(*start);

    NPC_t NPC;
    NPCtoHiker(&NPC);

    findShortestPaths(pc, *start, NPC);
    


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

            printf("%c", gameBoard[i][j]);

        }
        printf("\n");
    }

    printf("\n");


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

