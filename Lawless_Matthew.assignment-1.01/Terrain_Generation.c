#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void dumpBoard(char gameBoard[21][80]);
int max(int a, int b);
int min(int a, int b);
struct Point findPair(struct Point p1, struct Point p2);
void fillRectangle(struct Point p1, struct Point p2, char gameBoard[21][80], char c);
void generatePaths(char gameBoard[21][80]);


struct Point {

    int x;
    int y;

};

void printPoint(struct Point p){

    printf("(");
    printf("%d", p.x);
    printf(", ");
    printf("%d", p.y);
    printf(")\n");
    

}



int main(int argc, char *argv[]){

    //init RNG
    srand(time(NULL));

    char gameBoard[21][80];
    //fill with Ns
    int i, j;
    for (i = 0; i < 21; i++){
        for (j = 0; j < 80; j++){

            gameBoard[i][j] = '.';

        }
    }


    //construct the frame of the board
    
    //top and bottom rows
    for (j = 0; j < 80; j++){

        gameBoard[0][j] = '%';
        gameBoard[20][j] = '%';

    }

    //now do the columns
    for (i = 0; i < 21; i++){

        gameBoard[i][0] = '%';
        gameBoard[i][79] = '%';

        
    }

    //Water

    struct Point w1, w2;

    w1.x = (rand() % 77) + 1; //[1, 78]
    w1.y = (rand() % 17) + 1; //[1, 18]
    w2.x = (rand() % 77) + 1; //[1, 78]
    w2.y = (rand() % 17) + 1; //[1, 18]

    fillRectangle(w1, w2, gameBoard, '~');

    dumpBoard(gameBoard);

    //Forest

    struct Point f1, f2;
    
    f1.x = (rand() % 77) + 1; //[1, 78]
    f1.y = (rand() % 17) + 1; //[1, 18]
    f2.x = (rand() % 77) + 1; //[1, 78]
    f2.y = (rand() % 17) + 1; //[1, 18]

    fillRectangle(f1, f2, gameBoard, '^');

    dumpBoard(gameBoard);

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

    dumpBoard(gameBoard);

    generatePaths(gameBoard);

    dumpBoard(gameBoard);

    


    

    






    return 0;
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

void generatePaths(char gameBoard[21][80]){

    char c = '#';

    //first do E-W path

    //choose two y values
    int y1 = (rand() % 19) + 1; // [1, 19]
    int y2 = (rand() % 19) + 1; // [1, 19]

    //generate x value
    int x1 = (rand() % 78 + 1); // [1, 78]

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

    //choose two x values
    int a1 = (rand() % 78 + 1); // [1, 78]
    int a2 = (rand() % 78 + 1); // [1, 78]

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


    while (true){

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

            gameBoard[currentY - 1][s] = 'M';
            gameBoard[currentY + 1][s] = 'C';
            break;

        }


    }


}

