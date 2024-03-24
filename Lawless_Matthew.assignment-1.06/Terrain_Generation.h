typedef struct NPC{


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

typedef struct playerChar{

    int row;
    int col;

}playerChar_t;

struct Point{
    int x;
    int y;
};

typedef struct Map{
    int eG;
    int wG;
    int nG;
    int sG;
    struct Point worldCoordinates;
    char screen[21][80];
    char terrainOnly[21][80];
    NPC_t *characterTracker[21][80];
    int rivalHeatMap[21][80];
    int hikerHeatMap[21][80];
    int npcCount;
    NPC_t *npcList[1000];
    playerChar_t *pc;

}map_t;

typedef struct World{
    map_t *worldMaps[401][401];
    struct Point currentCoordinates;
}world_t;

