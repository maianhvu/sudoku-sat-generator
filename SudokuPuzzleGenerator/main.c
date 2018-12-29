//
//  main.c
//  SudokuPuzzleGenerator
//
//  Created by Mai Anh Vu on 29/12/18.
//  Copyright © 2018 Anh Vu Mai. All rights reserved.
//

#include <stdio.h>

#define x(i, j, k) ((i) * 81 + (j) * 9 + (k) + 1)

int main(int argc, const char * argv[]) {
    if (argc < 3) { printf("Usage: ./SudokuPuzzleGenerator <input puzzle file> <output cnf file>"); return 1; }
    
    FILE *f = fopen(argv[1], "r");
    char *line;
    size_t line_buffer;
    ssize_t read_size = getline(&line, &line_buffer, f);
    if (read_size < 82) { printf("Invalid puzzle.\n"); fclose(f); return 1; }
    
    FILE *of = fopen(argv[2], "w");
    
    int defined_values_count = 0;
    int defined_values[81];
    
    for (int i = 0; i < 81; ++i) {
        char c = line[i];
        if (c == '.') { continue; }
        int number = (int) c - '0';
        defined_values[defined_values_count++] = x(i / 9, i % 9, number - 1);
    }
    
    fprintf(of, "p cnf %d %d\n", x(8, 8, 8), 6156 + defined_values_count);
    
    for (int i = 0; i < defined_values_count; ++i) {
        fprintf(of, "%d\n", defined_values[i]);
    }
    
    // Uniqueness constraints
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            // First or clause
            for (int k = 0; k < 9; ++k) {
                fprintf(of, "%d", x(i, j, k));
                if (k + 1 != 9) { fprintf(of, " "); }
                else { fprintf(of, "\n"); }
            }
            // Distinction clause
            for (int k = 0; k < 9; ++k) {
                for (int l = 0; l < 9; ++l) {
                    if (k == l) { continue; }
                    fprintf(of, "%d %d\n", -x(i, j, k), -x(i, j, l));
                }
            }
        }
    }
    
    // Sudoku constraints
    
    // Rows
    for (int digit = 0; digit < 9; ++digit) {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                fprintf(of, "%d", x(row, col, digit));
                if (col + 1 != 9) { fprintf(of, " "); }
                else { fprintf(of, "\n"); }
            }
        }
    }
    
    // Columns
    for (int digit = 0; digit < 9; ++digit) {
        for (int col = 0; col < 9; ++col) {
            for (int row = 0; row < 9; ++row) {
                fprintf(of, "%d", x(row, col, digit));
                if (row + 1 != 9) { fprintf(of, " "); }
                else { fprintf(of, "\n"); }
            }
        }
    }
    
    // Squares
    for (int digit = 0; digit < 9; ++digit) {
        for (int square_row = 0; square_row < 3; ++square_row) {
            for (int square_col = 0; square_col < 3; ++square_col) {
                for (int row = square_row * 3; row < (square_row + 1) * 3; ++row) {
                    for (int col = square_col * 3; col < (square_col + 1) * 3; ++col) {
                        fprintf(of, "%d", x(row, col, digit));
                        if ((row + 1 == (square_row + 1) * 3) &&
                            (col + 1 == (square_col + 1) * 3)) {
                            fprintf(of, "\n");
                        }
                        else { fprintf(of, " "); }
                    }
                }
            }
        }
    }
    
    fclose(f);
    fclose(of);
    
    return 0;
}
