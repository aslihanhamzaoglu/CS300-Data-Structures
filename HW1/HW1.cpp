#include <iostream>
#include "Stack.h"
#include "Stack.cpp"
#include <vector>
#include <string>
#include <fstream>
#include <random>

using namespace std;

struct cell { //struct to keep wall info of maze's in the given text format
    int x;
    int y;
    bool l;
    bool r;
    bool u;
    bool d;

    bool visited;
    bool path;
};

void Break(const int & random, vector<vector<cell>> & maze, Stack<cell> & currentPath ) {

    //break the current cell's and next cell's walls according to given direction
    //update the next cell as visited
    //pop the current cell and push the two cells to keep the updated values in stack

    cell top = currentPath.top();
    if (random % 4 == 0) {  // direction: up

        maze[top.y][top.x].u = false;
        maze[top.y + 1][top.x].d = false;

        maze[top.y + 1][top.x].visited = true;

        currentPath.pop();
        currentPath.push(maze[top.y][top.x]);
        currentPath.push(maze[top.y + 1][top.x]);

    }
    else if (random % 4 == 1) { // direction: down

        maze[top.y][top.x].d = false;
        maze[top.y - 1][top.x].u = false;

        maze[top.y - 1][top.x].visited = true;

        currentPath.pop();
        currentPath.push(maze[top.y][top.x]);
        currentPath.push(maze[top.y - 1][top.x]);
    }
    else if (random % 4 == 2) { // direction: right

        maze[top.y][top.x].r = false;
        maze[top.y][top.x + 1].l = false;

        maze[top.y][top.x + 1].visited = true;

        currentPath.pop();
        currentPath.push(maze[top.y][top.x]);
        currentPath.push(maze[top.y][top.x + 1]);
    }
    else {  // direction: left

        maze[top.y][top.x].l = false;
        maze[top.y][top.x - 1].r = false;

        maze[top.y][top.x - 1].visited = true;

        currentPath.pop();
        currentPath.push(maze[top.y][top.x]);
        currentPath.push(maze[top.y][top.x - 1]);

    }
}

bool isBreakable (const cell & top, const int & random, const vector<vector<cell>> & maze, const int & row, const int & col) {

    //check if the current cell is in the edge or given the direction if next cell is visited
    //if both conditions based on direction are satisfied return true
    //take the mods in case direction is incremented (ex: 4+1 = 5 % 4 -> direction is down (1) )

    if (random % 4 == 0 && top.y != row-1 && !maze[(top.y)+1][top.x].visited ) {   // direction: up
        return true;
    }
    else if(random % 4 == 1 && top.y != 0 && !maze[(top.y)-1][top.x].visited ) {   //direction: down
        return true;
    }
    else if(random % 4 == 2 && top.x != col-1 && !maze[top.y][top.x + 1].visited ) {   //direction: right
        return true;
    }
    else if ( random % 4 == 3 && top.x != 0 && !maze[top.y][top.x - 1].visited ) {  //direction: left
        return true;
    }
    return false;
}

vector<vector<cell>> mazeGenerator(int row, int col, int k) { //creates random maze with given m and n values

    //create 2D matrix for keeping cell info
    //all walls exist in the beginning, none of the cells are visited in the beginning
    vector<vector<cell>> maze(row, vector<cell> (col));

    for (int i = 0; i < row; i++) {     //initialize indexes, visited, path and wall values. (all the walls exist and all the cells' are not visited in the beginning.)
        for (int k = 0; k < col; k++) {

            maze[i][k].x = k;
            maze[i][k].y = i;
            maze[i][k].r = true;
            maze[i][k].l = true;
            maze[i][k].u = true;
            maze[i][k].d = true;
            maze[i][k].visited = false;
            maze[i][k].path = false;
        }
    }

    Stack<cell> currentPath; //currently followed broken path

    maze[0][0].visited = true;
    currentPath.push(maze[0][0]); //push cell 0,0 as the first element

    //break total cell num -1  number of walls
    for (int i = 0; i < ( row*col )-1; i++) {

        bool broken = false;

        while (!broken) {   //repeat the process till a wall is broken.

            //generate a random num for 4 possible directions
            random_device rd;
            mt19937 gen(rd()); //seed
            uniform_int_distribution<int> distribution(0, 3);
            int random = distribution(gen);

            // 0-> up, 1->down, 2->right, 3->left

            for(int count = 0; count<4 && !broken; count++) { //while wall is not broken check for all possible (4) directions to break a wall

                if ( isBreakable(currentPath.top(), random, maze, row, col) ) { //check if cell in the given direction is breakable

                    Break(random, maze,currentPath); //break the cell in the given direction
                    broken = true;
                }
                random++; //change the direction in case wall in the given direction is not breakable
            }

            if ( !broken) { //if the current cell has no breakable neighbour cell, pop the top cell and repeat the check process for previous cell with while loop

                    currentPath.pop();
            }
        }
    }

    string file = "maze_" + to_string(k) + ".txt";
    ofstream output;
    output.open(file );
    output << row << " " << col << endl;

    for (int i = 0; i<row; i++) {
        for (int l = 0; l<col; l++) {

            output <<"x="<< maze[i][l].x<<" y="<<maze[i][l].y<<" l="<<maze[i][l].l<<" r="<<maze[i][l].r<<" u="<<maze[i][l].u<<" d="<<maze[i][l].d<<endl;
        }
    }
    output.close();

    return maze;
}

bool visit(const int & direction, vector<vector<cell>> & maze, Stack<cell> & path, const int & row, const int & col) {

    //check if there exist a cell in the given direction
    //check if the next cell in the given direction is not visited and there are no walls blocking a path
    //if cell is visitable then add it to the path

    cell top = path.top();

    if (direction % 4 == 0 && top.y != row-1 && !maze[top.y + 1][top.x].path  && !maze[top.y + 1][top.x].d )  {  // direction: up

        maze[top.y + 1][top.x].path = true;
        path.push(maze[top.y + 1][top.x]);
        return true;
    }
    else if (direction % 4 == 1 && top.y != 0 && !maze[top.y - 1][top.x].path && !maze[top.y - 1][top.x].u) {  // direction: down

        maze[top.y - 1][top.x].path = true;
        path.push(maze[top.y - 1][top.x]);
        return true;
    }
    else if (direction % 4 == 2 && top.x != col-1 && !maze[top.y][top.x + 1].path && !maze[top.y][top.x + 1].l) {  // direction: right

        maze[top.y][top.x + 1].path = true;
        path.push(maze[top.y][top.x + 1]);
        return true;
    }
    else if (direction % 4 == 3 && top.x != 0 && !maze[top.y][top.x - 1].path && !maze[top.y][top.x - 1].r){  // direction: left

        maze[top.y][top.x -1 ].path = true;
        path.push(maze[top.y][top.x - 1]);
        return true;
    }
    return false;
}

void pathFinder(const int & enX, const int & enY, const int & exX, const int & exY, vector<vector<cell>> maze, const int & row, const int & col, const int & ID) {

    Stack<cell> path;

    maze[enY][enX].path = true;
    path.push(maze[enY][enX]); //push the entry point to stack
    int direction = 0;

    while( path.top().x != maze[exY][exX].x || path.top().y != maze[exY][exX].y) { //while exit cell is not added to the path

        bool found = false;

        for (int count = 0; count<4 && !found; count++) { //while a neighbour visitable cell is not found repeat the process for all directions

            if ( visit(direction, maze, path, row, col) ) { //check if cell in the given direction is breakable

                found = true;
            }
            direction++; //change the direction
        }
        if ( !found) { //if none of the neighbour cells are visitable remove the current cell and try again for the previously added cell

            path.pop();
        }
    }

    Stack<cell> reverse;
    cell c;

    while (!path.isEmpty()) {

        c = path.topAndPop();
        reverse.push(c);
    }

    string name = "maze_" + to_string(ID) +"_path_"+to_string(enX) + "_" + to_string(enY) + "_" + to_string(exX) + "_" + to_string(exY) + ".txt";
    ofstream out;
    out.open(name );

    while ( !reverse.isEmpty()) {

        out<< reverse.top().x << " "<< reverse.topAndPop().y <<endl;
    }
    out.close();
}

int main() {

    int k, row, col, ID,enX,enY,exX,exY;

    cout<<"Enter the number of mazes: ";
    cin>> k;

    cout<<endl<<"Enter the number of rows and columns (M and N): ";
    cin>>row>>col;
    cout<<endl;

    vector<vector<cell>> mazeList[k];

    for (int t = 0; t < k; t++) { //create random mazes in asked amount

        mazeList[t] = mazeGenerator(row,col, t + 1);
    }

    cout <<"All mazes are generated."<<endl<<endl;
    cout<<"Enter a maze ID between 1 to "<< k <<" inclusive to find a path: ";
    cin>> ID;
    cout<<endl<<"Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin>>enX>>enY;
    cout<<endl<<"Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin>>exX>>exY;

    pathFinder(enX, enY, exX, exY, mazeList[ID-1], row, col, ID); //find path for asked maze ID with entry and exit points

    return 0;
}
