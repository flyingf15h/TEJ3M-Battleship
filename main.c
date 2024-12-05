#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* 
    void   |   createBoard(char [10][10]);                          |   initialize the board the users are playing on
    int    |   showBoard(char [10][10], int);                       |   displays the board updated with the shots they've missed and hit
    void   |   placeShip(char [10][10], int, const char*);          |   let users place their ships on their board
    int    |   shipValidity(char [10][10], int, int, int, char);    |   error traps for if the ship placement is valid
    void   |   takeShot(char [10][10], char [10][10], int*);        |   lets the user take a shot at their opponents board
    void   |   convertInput(char*, int*, int*);                     |   converts input of their shot into indices
    void   |   playGame();                                          |   play the game
    int    |   numWins1, numWins2                                   |   the number of times user 1 and user 2 have won the game of battleship

*/
void createBoard(char [10][10]);
void showBoard(char [10][10], int);
void placeShip(char [10][10], int, const char*);
int shipValidity(char [10][10], int, int, int, char);
void takeShot(char [10][10], char [10][10], int*);
void convertInput(char*, int*, int*);
void playGame();
int numWins1, numWins2; 

/*
int choice - the users choice to display the instructions, play the game, or to exit the program
*/
int main() {
    int choice = 0;

    while (choice != 3) {
        printf("\n========== Battleship ==========\n");
        printf("1. Show instructions\n");
        printf("2. Play game\n");
        printf("3. Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            // If the user chose to see the instructions, it displays instructions and waits for user to enter a key to return to menu
            case 1:
                printf("\nBattleship Instructions:\n");
                printf("Battleship requires 2 players. Each user has x1 2-unit long ship, x2 3-unit long ships, x1 4-unit long ship, and x1 5-unit long ship.\n");
                printf("When the game starts, the players will place their ships horizontally or vertically on the 10x10 unit board. Note that ships cannot stack on top of each other.\n");
                printf("The console will clear between user turns to maintain privacy. Rows of the board are labelled with the letters A to J, and columns with numbers 1 to 10.\n");
                printf("Players take turns entering the unit they would like to shoot (e.g., 'B6'). If an enemy ship covers this unit, the ship gets damaged at that spot ('x').\n");
                printf("If there is no ship, it’s a miss ('o'). The first player to sink all enemy ships wins.\n");
                printf("\nEnter a key to return to the menu.\n");
                getchar();
                getchar(); // takes extra character to account for newline
                break;
            
            // runs code to play game if user chooses play game 
            case 2:
                playGame();
                break;
                
            // displays thank you, next cycle the while loop will not continue because choice is 3 
            case 3:
                printf("Player 1 has won %d times!\n", numWins1);
                printf("Player 2 has won %d times!\n", numWins2);
                printf("Thank you for playing Battleship! This program was made by a fish.\n");
                break;
                
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

// Uses other functions to play battleship 
// char shipPos1[10][10], shipPos2[10][10] - array contains positions of their ships on their boards
// char board1[10][10], board2[10][10] - contains their board with misses and hits marked 
// int shipsLeft1, shipsLeft2 - the number of units of ships remaining for each player 
void playGame() {
    char shipPos1[10][10], shipPos2[10][10];
    char board1[10][10], board2[10][10];
    int shipsLeft1 = 17, shipsLeft2 = 17;

    printf("Welcome to (console) Battleship!\n\n");

    createBoard(shipPos1);
    createBoard(shipPos2);
    createBoard(board1);
    createBoard(board2);

    // player 1 places ships
    printf("Player 1, place your ships on the board:\n");
    showBoard(shipPos1, 0);
    placeShip(shipPos1, 2, "Player 1");
    placeShip(shipPos1, 3, "Player 1");
    placeShip(shipPos1, 3, "Player 1");
    placeShip(shipPos1, 4, "Player 1");
    placeShip(shipPos1, 5, "Player 1");
    system("clear");
    
    // player 2 places ships
    printf("Player 2, place your ships on the board:\n");
    showBoard(shipPos2, 0);
    placeShip(shipPos2, 2, "Player 2");
    placeShip(shipPos2, 3, "Player 2");
    placeShip(shipPos2, 3, "Player 2");
    placeShip(shipPos2, 4, "Player 2");
    placeShip(shipPos2, 5, "Player 2");
    system("clear");
    
    // while both players have ships left, alternate turns showing the board with their hits and misses and asking them where they would like to shoot next
    while (shipsLeft1 > 0 && shipsLeft2 > 0) {
        printf("Player 1's Turn:\n");
        showBoard(board2, 1);
        takeShot(shipPos2, board2, &shipsLeft2);
        if (shipsLeft2 == 0) break;

        printf("Player 2's Turn:\n");
        showBoard(board1, 1);
        takeShot(shipPos1, board1, &shipsLeft1);
    }
    
    // whoever kills all enemy ships first wins 
    if (shipsLeft2 == 0) {
        printf("Player 1 wins!\n");
        numWins1++;
    } else {
        printf("Player 2 wins!\n");
        numWins2++;
    }

}

// Make a board 
void createBoard(char board[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            board[i][j] = '.';
        }
    }
}

// Show board 
void showBoard(char board[10][10], int hideShips) {
    printf("   1 2 3 4 5 6 7 8 9 10\n");
    for (int i = 0; i < 10; i++) {
        printf("%c  ", ('A' + i));
        for (int j = 0; j < 10; j++) {
            if (hideShips && board[i][j] == 'S') {
                printf(". ");
            } else {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// Place a ship
// char position[4], direction; - the position of each ship and whether it is vertical or horizontal
// int row, col - the row and column the ship is being placed in
// bool notPlaced - whether or not the ship has been placed in a valid location 
void placeShip(char board[10][10], int size, const char* player) {
    char position[4], direction;
    int row, col;
    bool notPlaced = true;
    while (notPlaced) {
        printf("%s, enter starting position for your %d-unit ship (e.g., A1): ", player, size);
        scanf("%s", position);
        printf("Enter direction (H for horizontal, V for vertical): ");
        scanf(" %c", &direction);
        
        // converts input to indices for placing ship, S stands for ship, H for horizontal 
        convertInput(position, &row, &col);
        if (shipValidity(board, row, col, size, direction)) {
            for (int i = 0; i < size; i++) {
                if (direction == 'H' || direction == 'h') {
                    board[row][col + i] = 'S';
                } else {
                    board[row + i][col] = 'S';
                }
            }
            showBoard(board, 0);
            notPlaced = false; 
        } else {
            printf("Invalid placement, Please try again.\n");
        }
    }
}

// Error trap ship position to make sure its in bounds
int shipValidity(char board[10][10], int row, int col, int size, char direction) {
    if (direction == 'H' || direction == 'h') {
        if (col + size > 10) return 0;
        for (int i = 0; i < size; i++) {
            if (board[row][col + i] != '.') return 0;
        }
    } else if (direction == 'V' || direction == 'v') {
        if (row + size > 10) return 0;
        for (int i = 0; i < size; i++) {
            if (board[row + i][col] != '.') return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

// Take a shot
// int row, col - the row and column the ship is being placed in
// bool notPlaced - whether or not the ship has been placed in a valid location 
void takeShot(char board[10][10], char displayBoard[10][10], int* shipsRemaining) {
    char position[4];
    int row, col;
    bool notPlaced = true; 
    while (notPlaced) {
        printf("Enter the unit you would like to shoot (e.g., B4): ");
        scanf("%s", position);
        convertInput(position, &row, &col);
        
        // If hit location is in bounds, checks if there's a ship there for a hit or if there isn't for a miss, and updates user board
        if (row >= 0 && row < 10 && col >= 0 && col < 10) {
            if (board[row][col] == 'S') {
                printf("Hit!\n");
                board[row][col] = 'x';
                displayBoard[row][col] = 'x';
                (*shipsRemaining)--;
                notPlaced = false; 
            } else if (board[row][col] == '.') {
                printf("Miss.\n");
                board[row][col] = 'o';
                displayBoard[row][col] = 'o';
                notPlaced = false;
            } else {
                printf("That position has been shot already, please try again.\n");
            }
        } else {
            printf("You can't shoot there! Please try again.\n");
        }
    }
}

// Convert input to row and column indices
void convertInput(char* position, int* row, int* col) {
    *row = position[0] - 'A';
    *col = atoi(&position[1]) - 1;
}
