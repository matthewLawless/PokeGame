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


}NPC_t;

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
    char characterTracker[21][80];
    char rivalHeatMap[21][80];
    char hikerHeatMap[21][80];
    int npcCount;
    NPC_t *npcList[20];
}map_t;

typedef struct World{
    map_t *worldMaps[401][401];
    struct Point currentCoordinates;
}world_t;

