//Basak Amasya

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>

using namespace std;

struct Cell //struct for cells of the maze
{
	Cell() {};
	Cell(int xval, int yval, int lval, int rvar, int dvar, int uvar)
	{
		visited = false;
		x = xval;
		y = yval;
		l = lval;
		r = rvar;
		d = dvar;
		u = uvar;
	};
	bool visited;
	int x;
	int y;
	int l;
	int r;
	int d;
	int u;
};

//templated stack class, taken from the lecture slides and modified
template <class T>
class Stack
{
public:
	Stack();
	bool isEmpty();
	void pop();
	void push(T & x);
	T gettop();

private:
	struct Node
	{
		T elem;
		Node *next;

		Node(T & info, Node * node = nullptr)
			: elem(info), next(node) { }
	};
	Node * top;
};

template <class T>
Stack<T>::Stack()
{
	top = nullptr;
}


template <class T>
bool Stack<T>::isEmpty()
{
	return (top == nullptr);
}

template <class T>
void Stack<T>::pop()
{
	if (!isEmpty())
	{
		Node * temp = top;
		top = top->next;
		delete temp;
	}
}

template <class T>
void Stack<T>::push(T & x)
{
	top = new Node(x, top);
}

template <class T>
T Stack<T>::gettop()
{
	if (!isEmpty())	return top->elem;
}

//the function checks whether that cell of the matrix is reachable, meaning is not yet visited and not out of scope
bool isReachable(Cell cell, vector<vector<Cell>> mazematrix)
{
	if (cell.y < 0 || cell.y > mazematrix.size() - 1 ) //check if it's negative or greater than the size of the matrix
	{
		return false;
	}
	if (cell.x < 0 || cell.x > mazematrix[(cell.y)].size() - 1 )
	{
		return false;
	}
	return true;
}

//the function that checks whether all of the cells of the matrix are visited or not
bool AllVisited(vector<vector<Cell>> mazematrix)
{
	for (int x = 0; x < mazematrix.size(); x++)
	{
		for (int y = 0; y < mazematrix[0].size(); y++)
		{
			if (!mazematrix[x][y].visited) //if the cell is not visited
			{
				return false;
			}
		}
	}
	return true;
}

//function that writes created maze into corresponding files
void WriteMazeFile(vector<vector<Cell>> mazematrix, int M, int N, int mazeno)
{
	ofstream file;
	string filename = "maze_" + to_string(mazeno) + ".txt";
	file.open(filename);
	file << M << " "<< N << "\n"; 

	for (int x = 0; x < mazematrix.size(); x++) //traversing the matrix with 2 for loops, writing each cell one by one
	{
		for (int y = 0; y < mazematrix[0].size(); y++)
		{
			file << "x=" << mazematrix[x][y].x << " " << "y=" << mazematrix[x][y].y << " " //the coordinates
				<< "l=" << mazematrix[x][y].l << " " << "r=" << mazematrix[x][y].r << " " //values of the walls
				<< "u=" << mazematrix[x][y].u << " " << "d=" << mazematrix[x][y].d << "\n";
		}
	}
	file.close();
}

//the functions checks whether there is a place to go from that cell when creating the maze
bool NoWhereToGo(vector<vector<Cell>> mazematrix, Cell c)
{
	Cell newcell(c.x, c.y + 1, 0, 0, 0, 0); //checks each neighbor cell
	if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited)
		return false; 
	newcell.x += 1;
	newcell.y -= 1;
	if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited)
		return false;
	newcell.x -= 1;
	newcell.y -= 1;
	if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited)
		return false;
	newcell.x -= 1;
	newcell.y += 1;
	if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited)
		return false;
	return true;
}

//the functions checks whether there is a place to go from that cell while solving the maze
bool NoWhereToGoSolving(vector<vector<Cell>> mazematrix, Cell c)
{
	Cell newcell(c.x, c.y + 1, 0, 0, 0, 0); //checks each neighbor cell
	if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited && c.u == 0)
		return false;
	newcell.x += 1;
	newcell.y -= 1;
	if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited && c.r == 0)
		return false;
	newcell.x -= 1;
	newcell.y -= 1;
	if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited && c.d == 0)
		return false;
	newcell.x -= 1;
	newcell.y += 1;
	if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited && c.l == 0)
		return false;
	return true;
}

//the function that turns the matrix into a maze
void CreateMaze(vector<vector<Cell>> mazematrix, int mazeno, vector<vector<vector<Cell>>> & allmazes)
{
	Stack<Cell> mazestack;
	mazematrix[0][0].visited = true; //start from (0,0)
	mazestack.push(mazematrix[0][0]);
	while (!AllVisited(mazematrix)) //until all of the cells are reachable from another cell(s)
	{
		Cell currentCell = mazestack.gettop();
		while (!mazestack.isEmpty() && NoWhereToGo(mazematrix,currentCell)) //if there's nowhere to go from that cell
		{
			mazestack.pop();
			currentCell = mazestack.gettop(); //backtrack the cell
		}
		Cell newcell = currentCell;
		newcell.visited = false;
		int dir = rand() % 4; //choose a random direction
		if (dir == 0) //check each direction whether they are suitable to go or not
		{
			newcell.y += 1;
			if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited) //if it's ok to go
			{
				mazematrix[currentCell.y][currentCell.x].u = 0; //break the wall betwenn the two cells
				mazematrix[newcell.y][newcell.x].d = 0;
				mazematrix[newcell.y][newcell.x].visited = true; //mark it as visited
				mazestack.push(mazematrix[newcell.y][newcell.x]); //put the cell into the stack
			}
		}
		else if (dir == 1)
		{
			newcell.x += 1;
			if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited)
			{
				mazematrix[currentCell.y][currentCell.x].r = 0;
				mazematrix[newcell.y][newcell.x].l = 0;
				mazematrix[newcell.y][newcell.x].visited = true;
				mazestack.push(mazematrix[newcell.y][newcell.x]);
			}
		}
		else if (dir == 2)
		{
			newcell.y -= 1;
			if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited)
			{
				mazematrix[currentCell.y][currentCell.x].d = 0;
				mazematrix[newcell.y][newcell.x].u = 0;
				mazematrix[newcell.y][newcell.x].visited = true;
				mazestack.push(mazematrix[newcell.y][newcell.x]);
			}
		}
		else if (dir == 3)
		{
			newcell.x -= 1;
			if (isReachable(newcell, mazematrix) && !mazematrix[newcell.y][newcell.x].visited)
			{
				mazematrix[currentCell.y][currentCell.x].l = 0;
				mazematrix[newcell.y][newcell.x].r = 0;
				mazematrix[newcell.y][newcell.x].visited = true;
				mazestack.push(mazematrix[newcell.y][newcell.x]);
			}
		}
	}
	allmazes.push_back(mazematrix); //put the maze into the all mazes matrix
	WriteMazeFile(mazematrix, mazematrix.size(), mazematrix[0].size(), mazeno); //write it into the corresponding file
}

//the function that creates the initial matrix for the maze
void CreateMatrix(int mazeno, int rowno, int colno, vector<vector<vector<Cell>>> & allmazes)
{
	for (int i = 0; i < mazeno; i++)
	{
		vector<vector<Cell>> mazematrix(rowno, vector<Cell> (colno)); //initializing the matrix
		for (int j = 0; j < rowno; j++)
		{
			for (int k = 0; k < colno; k++)
			{
				Cell newcell(k,j,1,1,1,1); //initializing each cell with 4 walls
				mazematrix[j][k] = newcell;
			}
		}
		CreateMaze(mazematrix, i+1, allmazes); //turning the matrix into a maze
	}
}

//the function that writes the path into a file
void WritePathFile(int mazeID, int entryx, int entryy, int exitx, int exity, Stack<Cell> path)
{
	ofstream file;
	string filename = "maze_" + to_string(mazeID) + "_" + "path_" + to_string(entryx) + "_" + to_string(entryy) 
		+ "_" + to_string(exitx)+ "_" + to_string(exity) + ".txt";
	file.open(filename);
	Stack<Cell> reverse;
	while (!path.isEmpty()) //reversing the stack by popping and pushing each item to another cell
	{
		Cell c = path.gettop();
		reverse.push(c);
		path.pop();
	}
	while (!reverse.isEmpty())
	{
		Cell coordinates = reverse.gettop(); //get each cell one by one
		reverse.pop();
		file << coordinates.x << " " << coordinates.y << "\n";
	}
	file.close();
}

//converting the cells from visited to unvisited to prepare it for solving
void PrepareMazeToSolve(vector<vector<Cell>> & mazematrix)
{
	for (int j = 0; j < mazematrix.size(); j++)
	{
		for (int k = 0; k < mazematrix[0].size(); k++)
		{
			mazematrix[j][k].visited = false;
		}
	}
}

//the function finds a path from given entry points to given exit points
void FindPath(int mazeID, int entryx, int entryy, int exitx, int exity, vector<vector<vector<Cell>>> allmazes)
{
	Stack<Cell> path;
	vector<vector<Cell>> maze = allmazes[mazeID - 1]; //choose the maze to be solved
	PrepareMazeToSolve(maze); 
	maze[entryy][entryx].visited = true;
	Cell currentCell = maze[entryy][entryx]; //start from the entry cell
	path.push(currentCell);
	while (currentCell.x != exitx || currentCell.y != exity) //until the exit is found
	{
		while (!path.isEmpty() && NoWhereToGoSolving(maze, currentCell)) //while there is nowhere to go from that cell
		{
			path.pop();
			if (!path.isEmpty())
			{
				currentCell = path.gettop(); //backtrack the cell
			}
			else
			{
				maze[entryy][entryx].visited = true;
				currentCell = maze[entryy][entryx]; //start from the entry cell
				path.push(currentCell);
			}
		}
		Cell newcell = currentCell;
		if (currentCell.u == 0 && !maze[currentCell.y + 1][currentCell.x].visited) //check all directions one by one
		{
			newcell.y += 1;
			maze[newcell.y][newcell.x].visited = true;
			path.push(maze[newcell.y][newcell.x]);
		}
		else if (currentCell.r == 0 && !maze[currentCell.y][currentCell.x + 1].visited)
		{
			newcell.x += 1;
			maze[newcell.y][newcell.x].visited = true;
			path.push(maze[newcell.y][newcell.x]);
		}
		else if (currentCell.d == 0 && !maze[currentCell.y - 1][currentCell.x].visited)
		{
			newcell.y -= 1;
			maze[newcell.y][newcell.x].visited = true;
			path.push(maze[newcell.y][newcell.x]);
		}
		else if (currentCell.l == 0 && !maze[currentCell.y][currentCell.x - 1].visited)
		{
			newcell.x -= 1;
			maze[newcell.y][newcell.x].visited = true;
			path.push(maze[newcell.y][newcell.x]);
		}
		currentCell = path.gettop();
	}
	WritePathFile(mazeID, entryx, entryy, exitx, exity, path); //write the found path to the file
}

int main()
{
	vector<vector<vector<Cell>>> allmazes; //to keep all mazes
	int K, M, N, choosenmaze, entryx, entryy, exitx, exity;
	cout << "Enter the number of mazes: ";
	cin >> K;
	cout << "Enter the number of rows and columns (M and N): ";
	cin >> M >> N;
	CreateMatrix(K, M, N, allmazes); //create the matrix with given inputs
	cout << "All mazes are generated." << endl;
	cout << endl;

	cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path: ";
	cin >> choosenmaze;
	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin >> entryx >> entryy;
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> exitx >> exity;
	FindPath(choosenmaze, entryx, entryy, exitx, exity, allmazes); //find the path in the maze with given inputs

	system("pause");
	return 0;
}