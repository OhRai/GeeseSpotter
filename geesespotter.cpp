#include <iostream>
#include "geesespotter_lib.h"
/* 

Reference: 

0x00 (0b00000000) - no adjacent geese 
0x01 (0b00000001) - one adjacent goose
0x02 (0b00000010) - two adjacent geese
0x03 (0b00000011) - three adjacent geese

0x09 (0b00001001) - field with a goose

0x10 (0b00010000) - marked field
0x20 (0b00100000) - hidden field 
0x0F (0b00001111) - used to easily remove marked and hidden bits from a field value

Examples:
0x03 (0b00000011) - field is revealed; not marked; three adjacent geese
0x29 (0b00101001) - field is not revealed; not marked; goose is in the field (0x20 + 0x09)
0x39 (0b00111001) - field is not revealed; marked; goose is in the field

*/

char *createBoard(std::size_t xdim, std::size_t ydim) {
    int a_cap;
    a_cap = xdim * ydim;

    char *p_array{ new char[a_cap]{0} };

    return p_array;
}

void cleanBoard(char *board) {
    
    delete[] board;
    board = nullptr;
}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim) {

    // Makes every field hidden in the array
    for (int i{0}; i < (xdim * ydim); ++i) {
        board[i] |= 0x20;
    }
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim){

    for (int i{0}; i < (xdim * ydim); ++i) { 

            // Marked Field
            if ((board[i] & markedBit()) == 0x10) {
                std::cout << 'M';
            }

            // Hidden Field
            else if ((board[i] & hiddenBit()) == 0x20) {
                std::cout << '*';
            }

            // Goose / Adjacent field
            else {
                std::cout << (board[i] & valueMask());
            } 

            if ((i+1) % xdim == 0) {
                std::cout << std::endl;
            }
    }
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim) {

    for (int k{0}; k < (xdim * ydim); ++k) {
        int goose{0};
    
        if ((board[k] != 9)) {
            // Top left corner
            if ((k == 0)) {
                if ((board[k + 1] == 9)) { ++goose; }
                if ((board[k + xdim] == 9)) { ++goose; }
                if ((board[k + xdim + 1] == 9)) { ++goose; }
            }

            // Top right corner
            else if (k == (xdim - 1)) {
                if ((board[k - 1] == 9)) { ++goose; }
                if ((board[k + xdim] == 9)) { ++goose; }
                if ((board[k + xdim - 1] == 9)) { ++goose; }
            }

            // Bottom left corner
            else if (k == (xdim*(ydim - 1))) {
                if ((board[k - xdim] == 9)) { ++goose; }
                if ((board[k - xdim + 1] == 9)) { ++goose; }
                if ((board[k + 1] == 9)) { ++goose; }
            }

            // Bottom right corner
            else if (k == (xdim*ydim - 1)) {
                if ((board[k - xdim] == 9)) { ++goose; }
                if ((board[k - xdim - 1] == 9)) { ++goose; }
                if ((board[k - 1] == 9)) { ++goose; }
            }

            // Top side
            else if (k < xdim) {
                if ((board[k - 1] == 9)) { ++goose; }
                if ((board[k + xdim] == 9)) { ++goose; }
                if ((board[k + 1] == 9)) { ++goose; }
                if ((board[k + xdim + 1] == 9)) { ++goose; }
                if ((board[k + xdim - 1] == 9)) { ++goose; }
            }

            // Bottom side
            else if (k > (xdim*ydim - xdim)) {
                if ((board[k - 1] == 9)) { ++goose; }
                if ((board[k + 1] == 9)) { ++goose; }
                if ((board[k - xdim] == 9)) { ++goose; }
                if ((board[k - xdim - 1] == 9)) { ++goose; }
                if ((board[k - xdim + 1] == 9)) { ++goose; }
            }

            // Left side
            else if ((k % xdim) == 0) {
                if ((board[k + 1] == 9)) { ++goose; }
                if ((board[k + xdim] == 9)) { ++goose; }
                if ((board[k - xdim] == 9)) { ++goose; }
                if ((board[k - xdim + 1] == 9)) { ++goose; }
                if ((board[k + xdim + 1] == 9)) { ++goose; }
            }

            // Right side
            else if (((k + 1) % xdim ) == 0) {
                if ((board[k - 1] == 9)) { ++goose; }
                if ((board[k + xdim] == 9)) { ++goose; }
                if ((board[k - xdim] == 9)) { ++goose; }
                if ((board[k - xdim - 1] == 9)) { ++goose; }
                if ((board[k + xdim - 1] == 9)) { ++goose; }
            }

            // Everything else
            else {
                if ((board[k + 1] == 9)) { ++goose; }
                if ((board[k - 1] == 9)) { ++goose; }
                if ((board[k + xdim] == 9)) { ++goose; }
                if ((board[k + xdim + 1] == 9)) { ++goose; }
                if ((board[k + xdim - 1] == 9)) { ++goose; }
                if ((board[k - xdim] == 9)) { ++goose; }
                if ((board[k - xdim + 1] == 9)) { ++goose; }
                if ((board[k - xdim - 1] == 9)) { ++goose; }
            }

            board[k] = goose;
        }


        // int x;
        // x = board[k];

        // std::cout << x << std::endl;
    }
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
std::size_t yloc){ 

    int k = ((yloc * xdim) + xloc);

    // If marked
    if ((board[k] & markedBit()) == 0x10) {
        return 1;
    }

    // If revealed
    else if ((board[k] & hiddenBit()) != 0x20) {
        return 2;
    }

    // If goos
    else if ((board[k] & valueMask()) == 0b00001001){
        board[k] &= ~hiddenBit();
        return 9;
    }

    //if zero goos
    else if (((board[k] & valueMask()) == 0)) {

        board[k] &= ~hiddenBit();

        // Top left corner
            if ((k == 0)) {
                board[k + 1] &= ~hiddenBit();
                board[k + xdim] &= ~hiddenBit();
                board[k + xdim + 1] &= ~hiddenBit();
            }

            // Top right corner
            else if (k == (xdim - 1)) {
                board[k - 1] &= ~hiddenBit();
                board[k + xdim] &= ~hiddenBit();
                board[k + xdim - 1] &= ~hiddenBit();
            }

            // Bottom left corner
            else if (k == (xdim*(ydim - 1))) {
                board[k - xdim] &= ~hiddenBit();
                board[k - xdim + 1] &= ~hiddenBit();
                board[k + 1] &= ~hiddenBit();
            }

            // Bottom right corner
            else if (k == (xdim*ydim - 1)) {
                board[k - xdim] &= ~hiddenBit();
                board[k - xdim - 1] &= ~hiddenBit();
                board[k - 1] &= ~hiddenBit();
            }

            // Top side
            else if (k < xdim) {
                board[k - 1] &= ~hiddenBit();
                board[k + xdim] &= ~hiddenBit();
                board[k + 1] &= ~hiddenBit();
                board[k + xdim + 1] &= ~hiddenBit();
                board[k + xdim - 1] &= ~hiddenBit();
            }

            // Bottom side
            else if (k > (xdim*ydim - xdim)) {
                board[k - 1] &= ~hiddenBit();
                board[k + 1] &= ~hiddenBit();
                board[k - xdim] &= ~hiddenBit();
                board[k - xdim - 1] &= ~hiddenBit();
                board[k - xdim + 1] &= ~hiddenBit();
            }

            // Left side
            else if ((k % xdim) == 0) {
                board[k + 1] &= ~hiddenBit();
                board[k + xdim] &= ~hiddenBit();
                board[k - xdim] &= ~hiddenBit();
                board[k - xdim + 1] &= ~hiddenBit();
                board[k + xdim + 1] &= ~hiddenBit();
            }

            // Right side
            else if (((k + 1) % xdim ) == 0) {
                board[k - 1] &= ~hiddenBit();
                board[k + xdim] &= ~hiddenBit();
                board[k - xdim] &= ~hiddenBit();
                board[k - xdim - 1] &= ~hiddenBit(); 
                board[k + xdim - 1] &= ~hiddenBit();
            }

            // Everything else
            else {
                board[k + 1] &= ~hiddenBit();
                board[k - 1] &= ~hiddenBit();
                board[k + xdim] &= ~hiddenBit();
                board[k + xdim + 1] &= ~hiddenBit();
                board[k + xdim - 1] &= ~hiddenBit();
                board[k - xdim] &= ~hiddenBit();
                board[k - xdim + 1] &= ~hiddenBit();
                board[k - xdim - 1] &= ~hiddenBit();
            }
    }

    board[k] = (board[k] & ~hiddenBit());
    return 0;
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
std::size_t yloc) {

    // If the field is hidden
    if ((board[(yloc * xdim) + xloc] & hiddenBit())) {
        board[(yloc * xdim) + xloc] ^= markedBit();
        return 0; 
    }

    return 2;  
}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim) { 
    int counter_g{0};
    int counter_h{0};

    for (int k{0}; k < (xdim * ydim); ++k) {

        if ((board[k] & valueMask()) == 9) {
            ++counter_g;
        }

        if ((board[k] & hiddenBit()) == 0) {
            ++counter_h;
        }

        if ((xdim*ydim) - counter_g == counter_h) {
            return true;
        }

    }

    return false;
}




