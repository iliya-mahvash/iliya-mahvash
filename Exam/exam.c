#include <stdio.h>
#include <string.h>
#include "stdbool.h"
#include <stdlib.h>

typedef struct
{
    char name [20];
    int disk;
}player;

player player1;
player player2;
int r, c, size;
char board [20][21];
char turn = 'b';
char alph[21] = "ABCDEFGHIJKLMNOPQRST";
bool s = true;

void start ();
void show ();
void play();
int carry_on();
bool check (int r, int c);
bool check_hor_r (int r, int c);
bool check_hor_l (int r, int c);
bool check_ver_u (int r, int c);
bool check_ver_d (int r, int c);
bool check_cross_ur (int r, int c);
bool check_cross_ul (int r, int c);
bool check_cross_dr (int r, int c);
bool check_cross_dl (int r, int c);
char change ();
void rotate();
void rotate_hor(int x);
void rotate_ver(int x);
void rotate_cross(int x, int y);
void count();
void winner ();
void save();
bool load();
int main ()
{
    start();
    show();
    for (int i = 0; i < size*size; i++)
    {
        if (carry_on() == 1)    
        {    
            play();
            if (!s)
            {
                printf("if you want to save your game, press s. otherwise, press any button: ");
                if (getchar() == 's' || getchar() == 'S') save();
                break;
            }
            rotate();
            count();
            show();
            turn = change();
        }
        else break;
    }
    if (!s) return 0;
    winner();
    FILE *score = fopen("scoreboard.txt", "a");
    if (score == NULL) printf("Not Found!");
    fprintf (score, "%s:%i\t%s:%i\n", player1.name, player1.disk, player2.name, player2.disk);
    fclose (score);
    return 0;
}

void start ()
{
    printf("Welcome to Othello!\nHope you have a fun game\nJust remember: ");
    printf("when you want to play, first enter the letter and then enter the number\n");
    printf("if you have a saved game and you want to continue, enter 1. else, enter 0: ");
    if (getchar() == '1')
    {
        if (!load()) printf("No saved data\n");
        else return;
    }
    printf("Remember: if you wanted to close the program in middle of the game, enter 0\n");
    printf ("which size do you want your game board to be?");
    int i, j;
    do {
        printf ("(from 4*4 to 20*20 and even numbers only): ");
        scanf ("%i", &size);
    } while (size < 4 || size > 20 || (size % 2) != 0);
    getchar();
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++) board[i][j] = ' ';
    }
    board[size/2 - 1][size/2 - 1] = 'w';
    board[size/2][size/2] = 'w';
    board [size/2 - 1][size/2] = 'b';
    board[size/2][size/2 - 1] = 'b';
    printf("Name of the first player: ");
    fgets(player1.name, sizeof (player1.name), stdin);
    player1.name [strlen(player1.name) - 1] = '\0';
    i = 0;
    while (player1.name[i] == ' ') i ++;
    if (i == strlen (player1.name)) strcpy(player1.name, "Player 1");
    printf ("Name of the second player: ");
    fgets(player2.name, sizeof (player2.name), stdin);
    player2.name [strlen(player2.name) - 1] = '\0';
    i = 0;
    while (player2.name[i] == ' ') i ++;
    if (i == strlen (player2.name)) strcpy(player2.name, "Player 2");
    printf ("%s plays with black and %s plays with white\n\n", player1.name, player2.name);
    player1.disk = 2;
    player2.disk = 2;
}

void show ()
{
    static int x = 0;
    int i, j;
    if (x != 0)
    {
        system("cls");
        system("cls");
    }
    printf ("\n");
    for (i = 0; i < (size/2); i++) printf ("     ");
    printf ("%s: %i \t\t %s: %i\n\n", player1.name, player1.disk, player2.name, player2.disk);
    printf ("    ");
    for (i = 0; i < size; i ++)
    {
        printf("   %c  ", alph[i]);
    }
    printf ("\n    ");
    for (i = 0; i < (size * 6) + 1; i++) printf("-");
    printf("\n");
    for (i = 0; i < size; i++)
    {
        if (i < 9) printf (" %i  ", i+1);
        else printf (" %i ", i + 1);
        for (j = 0; j < size; j++) printf ("|  %c  ", board[i][j]);
        printf ("|\n");
        printf ("   ");
        for (j = 0; j < (size*6) + 1; j++) printf ("-");
        printf ("\n");
    }
    x++;    
}

int carry_on()
{
    int i,j;
    if (!start) return 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (board[i][j] == ' ' && check (i,j))
            {
                return 1;
            }
        }
    }
    turn = change ();
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (board[i][j] == ' ' && check (i,j))
            {
                return 1;
            }
        }
    }
    return 0;
}

void play()
{
    int i, j, counter = 0, row, timer = 30;
    char who[6];
    char entry[4];
    char entrynum[3];
    char possible[6];
    int poss[5];
    char small[21] = "abcdefghijklmnopqrst";
    for (i = 0; i < 5; i++)
    {
        possible[i] = '\0';
        poss[i] = 0;
    }
    if (turn == 'b') strcpy (who, "Black");
    else strcpy (who, "White");
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j ++)
        {
            if (check (i, j))
            {
                possible[counter] = alph[j];
                poss[counter] = i + 1;
                counter ++;
                if (counter == 5) break;
            }
        }
        if (counter == 5) break;
    }
    printf("%s's turn\nSuggestions: \n", who);
    for (i = 0; i < strlen(possible); i++) printf ("%c%i\t", possible[i], poss[i]);
    printf ("\n");
    do 
    {
        printf("Enter the square notation: ");
        fgets(entry, sizeof(entry), stdin);
        if (entry[0] == '0' &&entry[2] == '\0')
        {
            s = false;
            return;
        }
        for (i = 0; i < 2; i++) entrynum[i] = entry[i + 1];
        row = atoi (entrynum);
    } while (((entry[0] < 97 || entry[0] >= (97 + size)) && (entry[0] < 65 || entry [0] >= (97 + size))) \
    || (row < 1 || row > 20));
    r = row - 1;
    for (i = 0; i < size; i++)
    {
        if (entry[0] == alph[i] || entry[0] == small[i]) break;
    }
    c = i;
    if (check (r, c))
    {
        board [r][c] = turn;
    } 
    else
    {
        printf(" You Can't put your disk there!\n");
        play();
    }
}

bool check (int r, int c)
{
    if (board[r][c] != ' ') return false;
    if (check_hor_r (r, c))
    {
        return true;
    }
    if (check_hor_l (r, c))
    {
        return true;
    }   
    if (check_ver_u (r, c))
    {
        return true;
    }
    if (check_ver_d (r, c))
    {
        return true;
    }
    if (check_cross_ur (r, c))
    {
        return true;
    }
    if (check_cross_ul (r, c))
    {
        return true;
    }
    if (check_cross_dr (r, c))
    {
        return true;
    }
    if (check_cross_dl (r, c))
    {
        return true;
    }
    return false;
}
bool check_hor_r (int r, int c)
{
    //horizontal to right
    int i, j = 2;
    if (board[r][c+1] == change())
    {   for (i = c+2; i < size; i++)
        {
            if (board[r][i] == ' ') 
            {
                return false;
            }
            if (board[r][i] == turn) 
            {
                return true;
            }
        }
    }
    return false;
}
bool check_hor_l (int r, int c)
{
    //horizontal to left
    int i, j = 2;
    if (board[r][c-1] == change())
    {    for (i = c-2; i >= 0; i--)
        {
            if (board[r][i] == ' ') 
            {
                return false;
            }
            if (board[r][i] == turn) 
            {
                return true;
            }
        }
    }
    return false;
}
bool check_ver_d (int r, int c)
{
    int i, j = 2;
    //vertical to down
    if (board[r + 1][c] == change())
    {
        for (i = r+2; i < size; i++)
        {
            if (board[i][c] == ' ') 
            {
                return false;
            }
            if (board[i][c] == turn) 
            {
                return true;
            }
        }
    }
    return false;
}
bool check_ver_u (int r, int c)
{
    int i, j = 2;
    //vertical to up
    if (board[r - 1][c] == change())
    {
        for (i = r-2; i >= 0; i--)
        {
            if (board[i][c] == ' ') 
            {
                return false;
            }
            if (board[i][c] == turn) 
            {
                return true;
            }
        }
    }
    return false;
}
bool check_cross_dr (int r, int c)
{
    int i, j = 2;
    //cross down right
    if (board[r+1][c+1] == change())
    {
        while ((r+j) < size && (c+j) < size)
        {
            if (board[r+j][c+j] == ' ')
            {
                return false;
            }
            if (board[r+j][c+j] == turn)
            {
                return true;
            }
            j++;
        }
    }
    return false;
}
bool check_cross_dl (int r, int c)
{
    int i, j = 2;
    //cross down left
    j = 2;
    if (board[r+1][c-1] == change())
    {
        while ((r+j) < size && (c-j) >= 0)
        {
            if (board[r+j][c-j] == ' ')
            {
                return false;
            }
            if (board[r+j][c-j] == turn)
            {
                return true;
            }
            j++;
        }
    }
    return false;
}
bool check_cross_ur(int r, int c)
{
    int i, j = 2;
    //cross up right
    j = 2;
    if (board[r-1][c+1] == change())
    {
        while ((r-j) >= 0 && (c+j) < size)
        {
            if (board[r-j][c+j] == ' ')
            {
                return false;
            }
            if (board[r-j][c+j] == turn)
            {
                return true;
            }
            j++;
        }
    }
    return false;
}
bool check_cross_ul (int r, int c)
{
    int i, j = 2;
    //cross up left
    j = 2;
    if (board[r-1][c-1] == change())
    {
        while ((r-j) >= 0 && (c-j) >= 0)
        {
            if (board[r-j][c-j] == ' ')
            {
                return false;
            }
            if (board[r-j][c-j] == turn)
            {
                return true;
            }
            j++;
        }
    }
    return false;
}

char change ()
{
    if (turn == 'b')
    {
        return 'w';
    }
    else
    {
        return 'b';
    }
}

void rotate()
{
    if (check_hor_r (r, c)) rotate_hor(1);
    if (check_hor_l (r, c)) rotate_hor(-1);
    if (check_ver_u (r, c)) rotate_ver(-1);
    if (check_ver_d (r, c)) rotate_ver(1);
    if (check_cross_ur (r, c)) rotate_cross(-1,1);
    if (check_cross_ul (r, c)) rotate_cross(-1,-1);
    if (check_cross_dr (r, c)) rotate_cross(1,1);
    if (check_cross_dl (r, c)) rotate_cross(1,-1);
}
void rotate_hor(int x)
{
    int i = 2, j;
    while (board[r][c+(i*x)] != turn && (c + (i*x)) < size && (c + (i*x)) >= 0) i++;
    for (j = 1; j < i; j++) board[r][c+(j*x)] = turn;
}
void rotate_ver(int x)
{
    int i = 2, j;
    while (board[r+(i*x)][c]!= turn && (r + (i*x)) < size && (r + (i*x)) >= 0) i++;
    for (j = 1; j < i; j++) board[r+(j*x)][c] = turn;
}
void rotate_cross(int x, int y)
{
    int i = 2, j;
    while (board[r+(i*x)][c + (i*y)] != turn  && r+(i*x) < size && c + (i*y) < size && c + (i*y) >= 0 && r+(i*x) >= 0) i++;
    for (j = 1; j < i; j++) board[r+(j*x)][c+(j*y)] = turn;
}
void count ()
{
    int i, j;
    player1.disk = 0;
    player2.disk = 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (board[i][j] == 'b') player1.disk ++;
            if (board[i][j] == 'w') player2.disk ++;
        }
    }
}
void winner()
{
    if (player1.disk > player2.disk) printf (" %s is the winner!\n", player1.name);
    else if (player1.disk < player2.disk) printf(" %s is the winner!\n", player2.name);
    else printf (" It's a tie!\n");
    printf ("Press any Button to Exit\n");
    getchar();
}
void save()
{
    FILE *file = fopen ("resume.txt", "w");
    int digit1, digit2, digitdisk1 = 0, digitdisk2 = 0, lensize;
    if (strlen(player1.name) > 9) digit1 = 2;
    else digit1 = 1;
    if (strlen (player2.name) > 9)digit2 = 2;
    else digit2 = 1;
    int disk1 = player1.disk, disk2 = player2.disk;
    while (disk1 > 0)
    {
        digitdisk1 ++;
        disk1 /= 10;
    } 
    while (disk2 > 0)
    {
        digitdisk2 ++;
        disk2 /= 10;
    }
    if (size > 9) lensize = 2;
    else lensize = 1;
    fprintf (file, "%i%i%s%i%i%i%i%s%i%i%i%i", digit1, strlen(player1.name), player1.name, digitdisk1, player1.disk, \
    digit2, strlen(player2.name), player2.name, digitdisk2, player2.disk, lensize, size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fprintf(file, "%c", board[i][j]);
        }
    }
    fclose(file);
}
bool load()
{
    char len1, len2, digitlen1, digitlen2, digitdisk1, digitdisk2, digitsize;
    FILE *file = fopen ("resume.txt", "r");
    if (file == NULL)
    {
        return false;
    }
    fread(&digitlen1, sizeof(char), 1, file);
    if (digitlen1 == '\0' || digitlen1 == ' ')
    {
        return false;
    }
    fread (&len1, sizeof(char), digitlen1-48, file);
    printf("len 1 is %c\n", len1);
    fread(&player1.name, sizeof (char), len1-48, file);
    printf("name 1 is %s\n", player1.name);
    fread(&digitdisk1, sizeof(char), 1, file);
    fread (&player1.disk, sizeof(char), digitdisk1-48, file);
    player1.disk -= 48;
    printf("disk 1 is %i\n", player1.disk);
    fread(&digitlen2, sizeof(char), 1, file);
    fread (&len2, sizeof(char), digitlen2-48, file);
    printf("len 2 is %c\n", len2);
    fread(&player2.name, sizeof (char), len2-48, file);
    printf("name 2 is %s\n", player2.name);
    fread(&digitdisk2, sizeof(char), 1, file);
    fread (&player2.disk, sizeof(char), digitdisk2-48, file);
    player2.disk -= 48;
    printf("disk 2 is %i\n", player2.disk);
    fread (&digitsize, sizeof(char), 1, file);
    fread (&size, sizeof(char), digitsize - 48, file);
    size -= 48;
    printf("size is %i\n", size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fread(&board[i][j], sizeof(char), 1, file);
        }
    }
    fclose (file);
    return true;
}