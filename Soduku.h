//
// Created by Achra on 1/19/2024.
//

#ifndef BACKTRACKING_SODUKU_H
#define BACKTRACKING_SODUKU_H
#define N 9
#include <iostream>
#include <random>
using namespace std;

class PuzzleSoduku
{
public:
    PuzzleSoduku();
    PuzzleSoduku(int grille[N][N]);
    void afficher();
    bool isOnRow(int row,int num);
    bool isOnCol(int col,int num);
    bool isOnBox(int startRow,int startCol,int num);
    bool isNotUsed(int num,int row,int col);
    bool solve(int grid[N][N],int row,int col);
    bool solve();
    void fillBox(int indexStartRow,int indexStartCol);
    void fillDiagonal();
    void copy(int dest[N][N],int src[N][N]);
    bool startBoard();
    void caseVide(int &indexRow,int &indexCol);
    void countSolution(int &number);
    void generateBoard();
    void initialiserGame();
    int board[N][N];
    int solution[N][N];
private:
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
};

/**
 * cette fonction initialise le puzzle soduku
 */
PuzzleSoduku::PuzzleSoduku()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
            board[i][j] = solution[i][j] = 0;
    }
}

/**
 * cette fonction initialise le puzzle soduku avec une matrice fournit en argument
 */
PuzzleSoduku::PuzzleSoduku(int (*grille)[9])
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
            board[i][j] = grille[i][j];
    }
}

/**
 * cette fonction permet de savoir si le numero est déjà utilisé dans une ligne
 * @param row : la ligne
 * @param num : le numero
 * @return    : true si déjà utilisée, false sinon
 */
bool PuzzleSoduku::isOnRow(int row, int num)
{
    for(int j=0; j<N; j++)
        if(board[row][j] == num)
            return true;
    return false;
}

/**
 * cette fonction permet de savoir si le numero est déjà utilisé dans une colonne
 * @param col : la colonne
 * @param num : le numero
 * @return    : true si déjà utilisée, false sinon
 */
bool PuzzleSoduku::isOnCol(int col, int num)
{
    for(int i=0; i<N; i++)
        if(board[i][col] == num)
            return true;
    return false;
}

/**
 * cette fonction permet de savoir si le numero est déjà utilisé dans un box 3x3
 * @param startRow : ligne début du ligne du box
 * @param startCol : ligne début de colonne du box
 * @param num      : lee numéro
 * @return         : true si déjà utilisée, false sinon
 */
bool PuzzleSoduku::isOnBox(int startRow, int startCol, int num)
{
    for(int i=startRow; i<startRow+3 ;i++)
    {
        for(int j=startCol; j<startCol+3; j++)
            if(board[i][j] == num) return true;
    }
    return false;
}

/**
 * cette fonction permet de savoir si le numero n'est pas utilisée en fonction de la ligne,la colonne et le box où il se situe
 * @param num : le numéro
 * @param row : la ligne
 * @param col : la colonne
 * @return    : true si déjà utilisée, false sinon
 */
bool PuzzleSoduku::isNotUsed(int num,int row,int col)
{
    if(!isOnRow(row,num)
       && !isOnCol(col,num)
       && !isOnBox(row - (row%3),col- (col%3) ,num))
        return true;
    return false;
}

/**
 * cette fonction permet d'afficher le puzzle du sodoku
 */
void PuzzleSoduku::afficher()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
            cout<<board[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;
}

bool PuzzleSoduku::solve(int (*grid)[9], int row, int col)
{
    if(row==N-1 && col==N)
        return true;
    if(col==N)
    {
        col=0;
        row++;
    }
    if(solution[row][col] != 0)
        return solve(solution,row,col+1);


    for(int num=1; num<=9; num++)
    {
        if(isNotUsed(num,row,col))
        {
            solution[row][col] = num;
            if(solve(solution,row,col+1))
                return true;
            solution[row][col] = 0;
        }
    }
    return false;

}

bool PuzzleSoduku::solve()
{
    return solve(solution,0,0);
}

void PuzzleSoduku::copy(int (*dest)[9], int (*src)[9])
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            dest[i][j] = src[i][j];
        }
    }
}
void PuzzleSoduku::fillBox(int indexStartRow,int indexStartCol)
{
    srand(static_cast<unsigned>(time(NULL)));
    int boucle = 3;
    int i=indexStartRow;
    int j=indexStartCol;
    int random;
    int cpy[N][N];

    while (boucle != 0)
    {
        do
        {
            random = 1 + (rand() % 9);
        } while (!isNotUsed(random, i, j));
        this->board[i][j] = random;
        copy(cpy,board);
        if (!solve(cpy, 0, 0))
        {
            this->board[i][j] = 0;
            continue;
        }
        i++;
        j++;
        boucle--;
    }
}

void PuzzleSoduku::fillDiagonal()
{
    for (int i = 0; i < N; i = i+3)
    {
        for (int j = 0; j < N; j = j+3)
        {
            fillBox(i,j);
        }
    }

}

void shuffle(std::vector<int>& array)
{
    srand(static_cast<unsigned>(time(NULL)));
    int j ;
    for (int i = array.size() - 1; i > 0; i--)
    {
        //assurer que chaque fois l'indice est différent que du précédent
        j = (rand() * (i+1) ) % array.size() ;
        std::swap(array[i], array[j]);
    }
}

void PuzzleSoduku::caseVide(int &indexRow,int &indexCol)
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            if(board[i][j]!=0)
                continue;
            indexRow = i;
            indexCol = j;
            return;
        }
    }
}

bool PuzzleSoduku::startBoard()
{
    int i=-1, j=-1;
    int counter = 0;

    caseVide(i,j);
    if(i==-1 && j==-1)
        return true;

    shuffle(nums);      //melanger la liste des nombres
    for (const auto& num : nums)
    {
        counter++;
        //pour forcer l'arret de la fonction si on a fait plusieurs essai 20000000 c'est équivalent à 1.3 sec
        if ( counter > 20000000)
        {
            cerr<<"Temps dépassé pour générer le startBord."<<endl;
            exit(-1);
        }
        //si c'est possible de placer le numéro
        if(isNotUsed(num,i,j))
        {
            board[i][j] = num;
            if(startBoard())
                return true;
            //si le numéro est faux, on réinitialise à 0 et on ressai avec le prochain
            board[i][j] = 0;
        }
    }
    //impossible de placer un numéro (1-9)
    return false;
}

void PuzzleSoduku::countSolution(int &number)
{
    int row=-1, col=-1;
    caseVide(row,col);
    if(row==-1 && col==-1)
    {
        number++;
        return;
    }

    shuffle(nums);
    for (int i=0;i<9 && number<2;i++)
    {
        if(isNotUsed(nums[i],row,col))
        {
            this->board[row][col] = nums[i];
            countSolution(number);
        }
        this->board[row][col] = 0;
    }

}


void PuzzleSoduku::generateBoard()
{
    vector<int> gridPos;
    gridPos.resize(81);
    for(int i=0;i<81;i++)
    {
        gridPos[i] = i;
    }

    shuffle(gridPos);
    for(int i=0;i<81;i++)
    {
        int x = (gridPos[i])/9;
        int y = (gridPos[i])%9;
        int temp = this->board[x][y];
        this->board[x][y] = 0;

        // If now more than 1 solution , replace the removed cell back.
        int check=0;
        countSolution(check);
        if(check!=1)
        {
            this->board[x][y] = temp;
        }
    }
}

void PuzzleSoduku::initialiserGame()
{
    this->startBoard();
    this->generateBoard();
    copy(solution,board);
    if(!this->solve())
        cerr<<"Probléme lors de résolution de soduku !"<<endl ;
}


#endif //BACKTRACKING_SODUKU_H
