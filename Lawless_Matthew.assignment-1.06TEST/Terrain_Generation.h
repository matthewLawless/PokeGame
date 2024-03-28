#include "heap.h"



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

