#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void dumpBoard(char gameBoard[21][80]);
int max(int a, int b);
int min(int a, int b);
struct Point findPair(struct Point p1, struct Point p2);
void fillRectangle(struct Point p1, struct Point p2, char gameBoard[21][80], char c);


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
    // for (i = 0; i < 21; i++){
    //     for (j = 0; j < 80; j++){

    //         gameBoard[i][j] = 'N';

    //     }
    // }

    struct Point f1, f2;
    
    f1.x = 0;
    f1.y = 0;

    f2.x = 80;
    f2.y = 21;

    fillRectangle(f1, f2, gameBoard, 'T');

    dumpBoard(gameBoard);


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

   //find two disjoint regions of tall grass
   //generate two points
   struct Point p, q;

    p.x = (rand() % 78) + 1;
    p.y = (rand() % 19) + 1;
    q.x = (rand() % 78) + 1;
    q.y = (rand() % 19) + 1;
    
    //figure out where the points are relative to each other in order to ensure
    //that when we generate their rectangle they are disjoint

    printPoint(p);

    printPoint(q);

    struct Point p1 = findPair(p, q);
    struct Point q1 = findPair(q, p);

    printPoint(p1);
    printPoint(q1);


    

    






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

    //goal here is to return a point s such that the rectangle defined by p1 and s is disjoint by the 
    //rectangle that will eventually be defined by p2 and s' (s' being a point return from this function)

    if (p1.x > p2.x && p1.y > p2.y){

        result.x = (rand() % 77) + p1.x; //[p1.x, 77]
        result.y = (rand() % 18) + p1.y; //[p1.y, 18]

    }
    else if (p1.x < p2.x && p1.y < p2.y){

        result.x = (rand() % p1.x) + 1; //[1, p1.x]
        result.y = (rand() % p1.y) + 1; //[1, p1.y]

    }
    else if (p1.x < p2.y && p1.y > p2.y){

        result.x = (rand() % p1.x) + 1; //[1, p1.x]
        result.y = (rand() % 18) + p1.y; //[p1.y, 18]

    }
    else if (p1.x > p2.x && p1.y < p2.y){

        result.x = (rand() % 77) + p1.x; //[p1.x, 77]
        result.y = (rand() % p1.y) + 1; //[1, p1.y]

    }
    else{

        printf("ERROR IN findPair\n");

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

