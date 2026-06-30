#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#define SIZE 9
#define BOX_SIZE 3

#define COLOR_RESET   7
#define COLOR_GREEN   10
#define COLOR_RED     12
#define COLOR_BLUE    9
#define COLOR_YELLOW  14
#define COLOR_CYAN    11

void set_color(int color);
void print_puzzle(int puzzle[SIZE][SIZE], bool given[SIZE][SIZE]);
bool is_valid(int puzzle[SIZE][SIZE], int row, int col, int num);
bool is_in_row(int puzzle[SIZE][SIZE], int row, int num);
bool is_in_col(int puzzle[SIZE][SIZE], int col, int num);
bool is_in_box(int puzzle[SIZE][SIZE], int box_start_row, int box_start_col, int num);
bool is_board_full(int puzzle[SIZE][SIZE]);
void mark_given_cells(int puzzle[SIZE][SIZE], bool given[SIZE][SIZE]);

int main()
{
    int puzzle[SIZE][SIZE] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    bool given[SIZE][SIZE]; 
    mark_given_cells(puzzle, given);

    int mistakes = 0;

    set_color(COLOR_BLUE);
    printf("=========================================\n");
    printf("|          Play Sudoku in C!           |\n");
    printf("=========================================\n");
    set_color(COLOR_RESET);
    printf("Enter row, column and number (1-9) to play.\n");
    printf("Example: 1 3 5  -> places 5 at row 1, column 3\n\n");

    while (!is_board_full(puzzle))
    {
        print_puzzle(puzzle, given);

        printf("\nMistakes so far: %d\n", mistakes);
        printf("Enter row col number: ");

        int row, col, num;
        if (scanf("%d %d %d", &row, &col, &num) != 3)
        {
            while (getchar() != '\n');
            set_color(COLOR_RED);
            printf("\nInvalid input! Please enter three numbers.\n");
            set_color(COLOR_RESET);
            continue;
        }

        row--;
        col--;

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || num < 1 || num > 9)
        {
            set_color(COLOR_RED);
            printf("\nOut of range! Row, column and number must be between 1 and 9.\n");
            set_color(COLOR_RESET);
            continue;
        }

        if (given[row][col])
        {
            set_color(COLOR_RED);
            printf("\nThat cell is locked and cannot be changed!\n");
            set_color(COLOR_RESET);
            continue;
        }

        if (!is_valid(puzzle, row, col, num))
        {
            mistakes++;
            set_color(COLOR_RED);
            printf("\nInvalid move! %d already exists in that row, column or box.\n", num);
            set_color(COLOR_RESET);
            continue;
        }

        puzzle[row][col] = num;
        set_color(COLOR_GREEN);
        printf("\nGood move!\n");
        set_color(COLOR_RESET);
    }

    print_puzzle(puzzle, given);
    set_color(COLOR_YELLOW);
    printf("\nCongratulations! You solved the puzzle with %d mistake(s).\n", mistakes);
    set_color(COLOR_RESET);

    return 0;
}

void set_color(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void mark_given_cells(int puzzle[SIZE][SIZE], bool given[SIZE][SIZE])
{
    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            given[row][col] = (puzzle[row][col] != 0);
        }
    }
}

void print_puzzle(int puzzle[SIZE][SIZE], bool given[SIZE][SIZE])
{
    printf("\n     1 2 3   4 5 6   7 8 9\n");
    printf("   +-------+-------+-------+\n");

    for (int row = 0; row < SIZE; row++)
    {
        printf(" %d | ", row + 1);
        for (int col = 0; col < SIZE; col++)
        {
            if (puzzle[row][col] == 0)
            {
                printf(". ");
            }
            else if (given[row][col])
            {
                set_color(COLOR_CYAN); 
                printf("%d ", puzzle[row][col]);
                set_color(COLOR_RESET);
            }
            else
            {
                set_color(COLOR_GREEN); 
                printf("%d ", puzzle[row][col]);
                set_color(COLOR_RESET);
            }

            if ((col + 1) % BOX_SIZE == 0)
                printf("| ");
        }
        printf("\n");

        if ((row + 1) % BOX_SIZE == 0)
            printf("   +-------+-------+-------+\n");
    }
}

bool is_in_row(int puzzle[SIZE][SIZE], int row, int num)
{
    for (int col = 0; col < SIZE; col++)
    {
        if (puzzle[row][col] == num)
            return true;
    }
    return false;
}

bool is_in_col(int puzzle[SIZE][SIZE], int col, int num)
{
    for (int row = 0; row < SIZE; row++)
    {
        if (puzzle[row][col] == num)
            return true;
    }
    return false;
}

bool is_in_box(int puzzle[SIZE][SIZE], int box_start_row, int box_start_col, int num)
{
    for (int row = 0; row < BOX_SIZE; row++)
    {
        for (int col = 0; col < BOX_SIZE; col++)
        {
            if (puzzle[box_start_row + row][box_start_col + col] == num)
                return true;
        }
    }
    return false;
}

bool is_valid(int puzzle[SIZE][SIZE], int row, int col, int num)
{
    int box_start_row = row - (row % BOX_SIZE);
    int box_start_col = col - (col % BOX_SIZE);

    return !is_in_row(puzzle, row, num) &&
           !is_in_col(puzzle, col, num) &&
           !is_in_box(puzzle, box_start_row, box_start_col, num);
}

bool is_board_full(int puzzle[SIZE][SIZE])
{
    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            if (puzzle[row][col] == 0)
                return false;
        }
    }
    return true;
}