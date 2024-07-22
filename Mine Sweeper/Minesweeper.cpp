#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <conio.h>
#include <fstream>
using namespace std;

enum class CellStatus { Hidden, Revealed, Flagged };

class Cell 
{
private:
    bool isMine;
    int adjacentMines;
    CellStatus status;

public:
    Cell() : isMine(false), adjacentMines(0), status(CellStatus::Hidden) {}

    void setMine() 
	{
        isMine = true;
    }

    bool hasMine() const 
	{
        return isMine;
    }

    void setAdjacentMines(int count) 
	{
        adjacentMines = count;
    }

    int getAdjacentMines() const 
	{
        return adjacentMines;
    }

    void reveal() 
	{
        status = CellStatus::Revealed;
    }

    void toggleFlag() 
	{
        if (status == CellStatus::Hidden)
            status = CellStatus::Flagged;
        else if (status == CellStatus::Flagged)
            status = CellStatus::Hidden;
    }

    CellStatus getStatus() const 
	{
        return status;
    }
};
struct Player 
{
    string name;
    int time;
};

class Board 
{
private:
    int rows;
    int cols;
    int numMines;
    int numFlags;
    int numRevealed;
    Cell ** grid;
    time_t startTime;
    time_t finishTime;

    Player topPlayersEasy[5];
    Player topPlayersIntermediate[5];
    Player topPlayersAdvanced[5];

    
public:
    Board(int r, int c, int mines) : rows(r), cols(c), numMines(mines), numFlags(0), numRevealed(0) 
	{
        grid = new Cell * [r];
        for (int i = 0; i < r; i++)
            grid[i] = new Cell[c];
        placeMines();
        calculateAdjacentMines();
        startTime = time(NULL);
    }
    ~Board() 
	{
        for (int i = 0; i < rows; ++i)
            delete[] grid[i];
        delete[] grid;
    }
    void placeMines() 
	{
        srand(time(NULL));
        int minesPlaced = 0;
        while (minesPlaced < numMines) 
		{
            int row = rand() % rows;
            int col = rand() % cols;
            if (!grid[row][col].hasMine()) 
			{
                grid[row][col].setMine();
                minesPlaced++;
            }
        }
    }
	void startTimer() 
	{
        startTime = time(NULL);
    }
    void stopTimer() 
	{
        finishTime = time(NULL);
    }
    int elapsedTime() const 
	{
        return static_cast<int>(finishTime - startTime);
    }
    bool isGameWon() const 
	{
        int totalNonMineCells = rows * cols - numMines;
    	return numRevealed == totalNonMineCells;
    }
    void calculateAdjacentMines() 
	{
        for (int r = 0; r < rows; ++r) 
		{
            for (int c = 0; c < cols; ++c) 
			{
                int count = 0;
                for (int dr = -1; dr <= 1; ++dr) 
				{
                    for (int dc = -1; dc <= 1; ++dc) 
					{
                        int nr = r + dr;
                        int nc = c + dc;
                        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc].hasMine()) 
						{
                            ++count;
                        }
                    }
                }
                grid[r][c].setAdjacentMines(count);
            }
        }
    }

	void display() 
	{
    cout << "   ";
    for (int i = 0; i < cols; ++i) 
	{
        if (i < 10)
            cout << " " << i << " ";
        else
            cout << " " << i << "";
    }
    cout << endl;
    for (int r = 0; r < rows; ++r) 
	{
        if (r < 10)
            cout << r << "  "; // Print row numbers
        else
            cout << r<<" "; // Adjust spacing for two-digit row numbers

        for (int c = 0; c < cols; ++c) 
		{
            cout << " ";
            if (grid[r][c].getStatus() == CellStatus::Hidden) 
			{
                cout << "*"; // Hidden cells
            } 
			else if (grid[r][c].getStatus() == CellStatus::Flagged) 
			{
                cout << "F"; // Flagged cells
            } 
			else if (grid[r][c].hasMine()) 
			{
                cout << "M"; // Mine cells
            } 
			else
			 {
                int adjMines = grid[r][c].getAdjacentMines();
                if (adjMines == 0)
                    cout << "."; // Empty cells
                else
                    cout << adjMines; // Cells with adjacent mines
            }
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Number of Mines on Field: " << numMines - numFlags << endl ;
    cout << "Time elapsed: " << difftime(time(NULL), startTime) << " seconds" << endl;
}
    void revealCell(int row, int col) 
	{
        if (row < 0 || row >= rows || col < 0 || col >= cols)
            return;
        if (grid[row][col].getStatus() == CellStatus::Revealed || grid[row][col].getStatus() == CellStatus::Flagged)
            return;
        grid[row][col].reveal();
        numRevealed++;
        if (grid[row][col].hasMine()) 
		{
            cout << "\aGame over! You hit a mine." << endl;
            for (int r = 0; r < rows; ++r) 
			{
                for (int c = 0; c < cols; ++c) 
				{
                    if (grid[r][c].hasMine())
                        grid[r][c].reveal();
                }
            }
        }
        else if (grid[row][col].getAdjacentMines() == 0) 
		{

            for (int dr = -1; dr <= 1; ++dr) 
			{
                for (int dc = -1; dc <= 1; ++dc) 
				{
                    int nr = row + dr;
                    int nc = col + dc;
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) 
					{
                        revealCell(nr, nc);
                    }
                }
            }
        }
    }
    void flagCell(int row, int col) 
	{
        if (row < 0 || row >= rows || col < 0 || col >= cols)
            return;
        if (grid[row][col].getStatus() == CellStatus::Revealed)
            return;
        grid[row][col].toggleFlag();
        if (grid[row][col].getStatus() == CellStatus::Flagged)
            numFlags++;
        else
            numFlags--;
    }
	bool anyMineRevealed() const 
	{
    		for (int r = 0; r < rows; ++r) 
			{
        		for (int c = 0; c < cols; ++c) 
				{
           			 if (grid[r][c].hasMine() && grid[r][c].getStatus() == CellStatus::Revealed) 
					{
               		 return true;
         		   }
        		}
    		}
    	return false;
	}
    bool isGameOver() const 
	{
       return numRevealed == rows * cols - numMines || anyMineRevealed();
    }
    void updateTopPlayers(const string &name, int time) 
	{
        Player player = {name, time};
        switch (rows) 
		{
            case 8:
                updateTopPlayerList(topPlayersEasy, player);
                break;
            case 15:
                updateTopPlayerList(topPlayersIntermediate, player);
                break;
            case 20:
                updateTopPlayerList(topPlayersAdvanced, player);
                break;
        }
    }

    void updateTopPlayerList(Player (&topPlayers)[5], const Player &newPlayer) 
	{
        int index = -1;
        for (int i = 0; i < 5; ++i) 
		{
            if (index == -1 && topPlayers[i].time == 0) 
			{
                index = i;
            }
			 else if (topPlayers[i].time > newPlayer.time) 
			 {
                index = i;
                break;
            }
        }

        if (index != -1) 
		{
            for (int i = 4; i > index; --i) 
			{
                topPlayers[i] = topPlayers[i - 1];
            }
            topPlayers[index] = newPlayer;
        }
    }

    void saveTopPlayers() 
	{
        ofstream outFile("top_players.txt");
        if (!outFile.is_open()) 
		{
            cout << "Unable to open file for saving top players." << endl;
            return;
        }
        outFile << "Top Players - Easy" << endl;
        writeTopPlayerList(outFile, topPlayersEasy);
        outFile << "Top Players - Intermediate" << endl;
        writeTopPlayerList(outFile, topPlayersIntermediate);
        outFile << "Top Players - Advanced" << endl;
        writeTopPlayerList(outFile, topPlayersAdvanced);
        outFile.close();
    }

    void writeTopPlayerList(ofstream &outFile, const Player (&topPlayers)[5]) 
	{
        for (int i = 0; i < 5; ++i) 
		{
            if (topPlayers[i].time > 0) 
			{
                outFile << topPlayers[i].name << " " << topPlayers[i].time << endl;
            }
        }
    }


};
char selectDifficulty()
{
	char choice;
	do
	{
		
	cout << "Select your Difficulty \n1.(E)asy\n2.(I)ntermediate\n3.(A)dvance\n" << endl;
	choice = getch();
	}while(choice != 'E' && choice !='e' && choice !='I' && choice != 'i' && choice !='A' && choice !='a');
	return choice;
}
void setDifficulty(char ch, int & r , int & c , int & n)
{
	if(ch=='E' || ch=='e')
	{
		r = 8;
		c = 8;
		n = 10;
	}
	else if(ch == 'I' || ch == 'i')
	{
		r = 15;
		c = 15;
		n = 40;
	}
	else
	{
		r = 20;
		c = 20;
		n = 99;
	}
}

int main() 
{
    int rows ;
    int cols ;
    int numMines;
    char choice = selectDifficulty();
    setDifficulty(choice,rows,cols,numMines);
	Board board(rows, cols, numMines);
    board.display();
    while (!board.isGameOver()) 
	{
        cout << "Enter row and column to reveal or flag (e.g., 2 3 L): ";
        int row, col;
        char action;
        cin >> row >> col >> action;

        if (row < 0 || row >= rows || col < 0 || col >= cols) 
		{
            cout << "Invalid cell coordinates." << endl;
            continue;
        }

        if (action == 'L' || action == 'l') 
		{
            board.revealCell(row, col);
        } 
		else if (action == 'R' || action == 'r') 
		{
            board.flagCell(row, col);
        } 
		else 
		{
            cout << "Invalid action. Use 'L' to reveal or 'R' to flag." << endl;
            continue;
        }

        board.display();

        if (board.isGameOver()) 
		{
            if (board.isGameWon())
			{
                cout << "Congratulations! You won the game!" << endl;
            } 
			else 
			{
                cout << "Game over! You hit a mine.\a" << endl;
                break;
            }
        }
    }
    return 0;
}
