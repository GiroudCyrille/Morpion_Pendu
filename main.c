#include <stdlib.h>
// Déclaration des variables nécéssaires au morpion
int winnerId, started, game, actualPlayer = 1;
char restart;
int player1CasesSelected[5] = { 0,0,0,0,0 };
int player2CasesSelected[5] = { 0,0,0,0, -1 };
int winPossibilty[8][3] =
{
    { 7, 8, 9},
    { 1, 2, 3},
    { 4, 5, 6},
    { 1, 4, 7},
    { 2, 5, 8},
    { 3, 6, 9},
    { 1, 5, 9},
    { 3, 5, 7}
};

// Déclaration des variables nécéssaires au pendu
int die, win, attemptLeft = 10;
char letter;

char* secretWord;
const char* wordToFound;
const char* words[8] = {
    "CANAPE",
    "VOITURE",
    "MOTO",
    "SYSTEME",
    "VALEUR",
    "RESPECT",
    "MOYENNE",
    "PORTABLE"
};

int isGood(char);
char getCharByIndex(int);
const char* getWord();

int main()
{
    game = printMenu();
    while(!started)
    {
        switch(game)
        {
            case 1:
                return startMorpion();
                break;
            case 2:
                return startPendu();
                break;
            default:
                printf("Votre choix est incorrect. \n");
                game = printMenu();
        }
    }
}

int printMenu()
{
    int result;
    printf("+---------------------+ \n");
    printf("| Selectionner le jeu | \n");
    printf("| 1 : morpion         | \n");
    printf("| 2 : pendu           | \n");
    printf("+---------------------+ \n");
    scanf("%d", &result);
    return result;
}
// Code concernant le pendu
int startPendu()
{
    srand( time( NULL ) );
    showRules();
    wordToFound = getWord();
    while(!die && !win)
    {
        printf("Mot a trouver : %s \n", secretWord);
        printf("Entrez une lettre \n");
        getLetter();
        if(!isGood(letter))
        {
            attemptLeft--;
            printf("Erreur, %d tentatives restante. \n", attemptLeft);
        }
        checkWin();
    }
    return 0;
}

const char* getWord()
{
    int r = rand() % 6;
    const char* word = words[r];
    secretWord = malloc(strlen(word));
    for(int i = 0; i < strlen(word); i++)
        secretWord[i] = '*';

    return word;
}

void getLetter()
{
    getchar();
    scanf("%c", &letter);
    letter = toupper(letter);
}

int isGood(char letterTry)
{
    int found = 0;
    for(int i = 0; i <= strlen(wordToFound); i++)
    {
        if(wordToFound[i] == letterTry)
        {
            secretWord[i] = letterTry;
            found = 1;
        }
    }

    return found;
}
// Code concernant le morpion
int startMorpion()
{
    showRules();
    int choice = 0;
    while (hasWinner() == 0)
    {
        printf("Voici la grille du jeu \n");
        showGrid();

        printf("C'est au tour du joueur %d \n", actualPlayer);
        printf("Selectionnez la case que vous voulez jouer. \n");
        scanf_s("%d", &choice);

        while (checkChoice(choice, actualPlayer) == 0)
        {
            printf("Impossible de jouer cet case ! Selectionnez en une autre \n");
            scanf_s("%d", &choice);
        }
    }

    showGrid();

    printf("\nVoulez vous refaire une partie (y = oui)? \n");
    getchar();
    scanf("%c", &restart);

    if(restart == 'y')
        restartMorpion();

    return 0;
}

void restartMorpion()
{
    winnerId = 0;
    actualPlayer = 1;
    for(int i = 0; i < 5; i++)
    {
        player1CasesSelected[i] = 0;

        if(i != 4)
            player2CasesSelected[i] = 0;
        else
            player2CasesSelected[i] = -1;
    }

    startMorpion();
}

void showRules()
{
    switch(game)
    {
        case 1:
            printf("Voici la facon de jouer. Chaque case est assigne a un chiffre, entrez le chiffre correspondant a la case ou vous voulez jouer. \n");
            printf("+-----------+ \n");
            printf("| 1 | 2 | 3 | \n");
            printf("+-----------+ \n");
            printf("| 4 | 5 | 6 | \n");
            printf("+-----------+ \n");
            printf("| 7 | 8 | 9 | \n");
            printf("+-----------+ \n");
            break;
        case 2:
            printf("Un mot est genere aleatoirement, vous devez proposer une lettre. \nCi celle-ci est presente dans le mot, alors la lettre s'affichera.");
            printf("\nSinon, vous perdrez 1 chance. Vous avez 10 chances. \n");
            break;
    }

}

void showGrid()
{
    printf("+-----------+ \n");
    printf("| %c | %c | %c | \n", getCharByIndex(1), getCharByIndex(2), getCharByIndex(3));
    printf("+-----------+ \n");
    printf("| %c | %c | %c | \n",  getCharByIndex(4), getCharByIndex(5), getCharByIndex(6));
    printf("+-----------+ \n");
    printf("| %c | %c | %c | \n", getCharByIndex(7), getCharByIndex(8), getCharByIndex(9));
    printf("+-----------+ \n");
}

int hasWinner()
{
    if (winnerId == 0)
        return 0;
    else
        return 1;
}

int checkChoice(int choiced, int player)
{
    if (choiced >= 1 && choiced <= 9)
    {
        if(isInArray(player1CasesSelected, choiced, 4) || isInArray(player2CasesSelected, choiced, 4))
            return 0;

        setChoice(choiced);
        checkWin();
        changePlayerTurn();

        return 1;
    }
    else
        return 0;
}

char getCharByIndex(int index)
{
    if(isInArray(player1CasesSelected, index, 4))
        return 'X';

    if(isInArray(player2CasesSelected, index, 4))
        return 'O';

    return ' ';
}

void setChoice(int choiced)
{
    for(int i = 0; i < 5; i++)
    {
        if(actualPlayer == 1)
        {
            if (player1CasesSelected[i] == 0)
            {
                player1CasesSelected[i] = choiced;
                return;
            }
        }
        else
        {
            if (player2CasesSelected[i] == 0)
            {
                player2CasesSelected[i] = choiced;
                return;
            }
        }
    }
}

int isInArray(int* number, int value, int length)
{
    for (int i = 0; i <= length; i++)
    {
        if (number[i] == value)
            return 1;
    }

    return 0;
}

void checkWin()
{
    switch(game)
    {
        case 1:
            for (int i = 0; i <= 7; i++)
            {
                if (isInArray(player1CasesSelected, winPossibilty[i][0], 4) && isInArray(player1CasesSelected, winPossibilty[i][1], 4) && isInArray(player1CasesSelected, winPossibilty[i][2], 4))
                {
                    winnerId = 1;
                    printf("\nLe jeu est fini ! Le joueur %d a gagner\n", winnerId);
                    return;
                }
                else if (isInArray(player2CasesSelected, winPossibilty[i][0], 4) && isInArray(player2CasesSelected, winPossibilty[i][1], 4) && isInArray(player2CasesSelected, winPossibilty[i][2], 4))
                {
                    winnerId = 2;
                    printf("\nLe jeu est fini ! Le joueur %d a gagner\n", winnerId);
                    return;
                }
                else if (i == 7 && !isInArray(player1CasesSelected, 0, 4) && !isInArray(player2CasesSelected, 0, 4))
                {
                    winnerId = -1;
                    printf("Match null \n");
                    return;
                }
            }
            break;
        case 2:
            if(attemptLeft <= 0)
            {
                printf("Vous avez perdu, vous n'avez pas trouver le mot !");
                die = 1;
            }
            if(strstr(secretWord,wordToFound))
            {
                printf("Vous avez gagner ! Le mot etait %s", secretWord);
                win = 1;
            }
    }
}

void changePlayerTurn()
{
    if(actualPlayer == 1)
        actualPlayer = 2;
    else
        actualPlayer = 1;
}
