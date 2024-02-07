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
}map_t;

typedef struct World{
    map_t *worldMaps[401][401];
    struct Point currentCoordinates;
}world_t;

