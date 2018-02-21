#include <iostream>
#include "graphics.h"
#include "random.h"

using namespace std;
using namespace mssm;

Random rnd;

enum class Cardinal {
    north,
    south,
    east,
    west
};

const vector<Cardinal> allDirections { Cardinal::north, Cardinal::south, Cardinal::east, Cardinal::west };

class Position {
public:
    int row;
    int col;
};

class Maze {
private:
    int numRows;
    int numCols;
    vector<vector<bool>> verticalWalls;
    vector<vector<bool>> horizontalWalls;
    vector<vector<bool>> markers;
    vector<vector<bool>> solutionMarkers;
    bool alternateDrawStyle;
public:
    Maze(int rows, int cols);
    void init(int rows, int cols);
    void draw(Graphics& g);
    void toggleDrawStyle() { alternateDrawStyle = !alternateDrawStyle; }

    void generate(Graphics& g);
    void regen(Graphics& g, Position pos);
    void solve(Graphics& g);
    bool recursolve(Graphics& g, Position pos);

    bool isValid(Position pos);
    bool inMap(Position pos); // is the specified cell on the map
    bool isExplored(Position pos);

    bool isValidSolve(Cardinal dir, Position pos);
    bool isWallThere(Cardinal dir, Position pos);

    Position adjacentPos(Cardinal dir, Position pos);
    void removeWall(Cardinal dir, Position pos);
};

void Maze::removeWall(Cardinal dir, Position pos)
{
    switch (dir) {
    case Cardinal::north:
        horizontalWalls[pos.row][pos.col] = false;
        break;
    case Cardinal::south:
        horizontalWalls[pos.row+1][pos.col] = false;
        break;
    case Cardinal::east:
        verticalWalls[pos.row][pos.col+1] = false;
        break;
    case Cardinal::west:
    default:
        verticalWalls[pos.row][pos.col] = false;
        break;
    }
}

bool Maze::isValid(Position pos)
{
    return inMap(pos) && !isExplored(pos);
}

bool Maze::isValidSolve(Cardinal dir, Position pos){
    return inMap(pos) && !isWallThere(dir, pos) && !isExplored(adjacentPos(dir, pos));
}

bool Maze::inMap(Position pos)  // is the specified cell on the map
{
    return pos.row >= 0 &&
           pos.row < numRows &&
           pos.col >= 0 &&
           pos.col < numCols;
}

bool Maze::isExplored(Position pos)
{
    return markers[pos.row][pos.col];
}

bool Maze::isWallThere(Cardinal dir, Position pos){
    switch(dir){
    case Cardinal::north:
        return horizontalWalls[pos.row][pos.col];
    case Cardinal::south:
        return horizontalWalls[pos.row+1][pos.col];
    case Cardinal::east:
        return verticalWalls[pos.row][pos.col+1];
    case Cardinal::west:
    default:
        return verticalWalls[pos.row][pos.col];
    }
}

Position Maze::adjacentPos(Cardinal dir, Position pos)
{
    switch (dir) {
    case Cardinal::north:
        return Position{pos.row-1, pos.col};
    case Cardinal::south:
        return Position{pos.row+1, pos.col};
    case Cardinal::east:
        return Position{pos.row, pos.col+1};
    case Cardinal::west:
    default:
        return Position{pos.row, pos.col-1};
    }
}

void Maze::regen(Graphics& g, Position pos)
{
    markers[pos.row][pos.col] = true;

    while (true) {
        vector<Cardinal> directions;

        for (Cardinal d : allDirections) {
            if (isValid(adjacentPos(d, pos))) {
                directions.push_back(d);
            }
        }

        if (directions.empty()) {
            return;
        }

        Cardinal validDirection = directions[g.randomInt(0,directions.size()-1)];

        // remove the wall in that direction...

        removeWall(validDirection, pos);


        Position validPos = adjacentPos(validDirection, pos);

        regen(g, validPos);
    }

}

void Maze::generate(Graphics& g)
{
    regen(g, Position{0,0});
}

Maze::Maze(int rows, int cols)
{
    init(rows, cols);
    alternateDrawStyle = false;
}

void Maze::init(int rows, int cols)
{
    numRows = rows;
    numCols = cols;
    verticalWalls = vector<vector<bool>>(rows, vector<bool>(cols+1, true));
    horizontalWalls = vector<vector<bool>>(rows+1, vector<bool>(cols, true));
    markers = vector<vector<bool>>(rows, vector<bool>(cols+1, false));
    solutionMarkers = vector<vector<bool>>(rows, vector<bool>(cols+1, false));
}

void drawGrid(Graphics& g, int numRows, int numCols, std::function<mssm::Color(int row, int col)> cellColor)
{
    constexpr int margin = 10;
    double sizeX = (g.width()-2*margin)/numCols;
    double sizeY = (g.height()-2*margin)/numRows;

    double blockSize = min(sizeX, sizeY);

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            double y = i * blockSize + margin;
            double x = j * blockSize + margin;
            Color color = cellColor(i,j);
            g.rect(x+1,y+1,blockSize-2,blockSize-2,color,color);
        }
    }
}

void Maze::draw(Graphics& g)
{
    if (alternateDrawStyle) {
        drawGrid(g, numRows*2+1, numCols*2+1, [this](int r, int c) {
           constexpr Color wallColor = BLACK;
           constexpr Color floorColor = WHITE;

           bool rowOdd = r % 2 == 1;
           bool colOdd = c % 2 == 1;

           if (r == numRows*2) {
               // last row, just look at horizontal wall below
               if (colOdd) {
                   return horizontalWalls[r/2][c/2] ? wallColor : floorColor;
               }
               return wallColor;
           }
           if (c == numCols*2) {
               if (rowOdd) {
                   return verticalWalls[r/2][c/2] ? wallColor : floorColor;;
               }
               return wallColor;
           }

           if (rowOdd && colOdd) {
               if (markers[r/2][c/2]) {
                   return GREEN;
               }
               else {
                   return floorColor;
               }
           }
           else if (!rowOdd && !colOdd){
               return wallColor;
           }
           else if (rowOdd && !colOdd){
               if ((c/2) < numCols && verticalWalls[r/2][c/2]) {
                   return wallColor;
               }
               return floorColor;
           }
           else {
               if ((r/2) < numRows && horizontalWalls[r/2][c/2]) {
                   return wallColor;
               }
               return floorColor;
           }
        });
    }
    else {
        constexpr int margin = 10;
        double sizeX = (g.width()-2*margin)/numCols;
        double sizeY = (g.height()-2*margin)/numRows;
        double blockSize = min(sizeX, sizeY);

        // draw vertical walls
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j <= numCols; j++) {
                if (verticalWalls[i][j]) {
                    double x = j * blockSize + margin;
                    double y1 = i * blockSize + margin;
                    double y2 = (i+1) * blockSize + margin;
                    g.line(x,y1,x,y2,WHITE);
                }
            }
        }

        // draw horizontal walls
        for (int i = 0; i <= numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (horizontalWalls[i][j]) {
                    double y = i * blockSize + margin;
                    double x1 = j * blockSize + margin;
                    double x2 = (j+1) * blockSize + margin;
                    g.line(x1,y,x2,y,WHITE);
                }
            }
        }

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (markers[i][j]) {
                    double y = i * blockSize + margin;
                    double x = j * blockSize + margin;
                //    g.rect(x+2,y+2,blockSize-4,blockSize-4, GREEN, GREEN);
                    g.ellipse(x+blockSize/2-2, y+blockSize/2-2, 4, 4, GREEN, GREEN);
                }
            }
        }

        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (solutionMarkers[i][j]) {
                    double y = i * blockSize + margin;
                    double x = j * blockSize + margin;
                    g.ellipse(x+blockSize/2-2, y+blockSize/2-2, 4, 4, BLUE, BLUE);
                }
            }
        }
    }
}

bool Maze::recursolve(Graphics& g, Position pos){

    markers[pos.row][pos.col] = true;

    //g.clear();
    //draw(g);
    //g.draw();

    while(true){

        vector<Cardinal> directions;

        // if solved
        if(markers[numRows - 1][numCols - 1] == true){
            solutionMarkers[pos.row][pos.col] = true;
            return true;
        }

        for (Cardinal d : allDirections) {
            if (isValidSolve(d, pos)) {
                directions.push_back(d);
            }
        }

        if (directions.empty()) {
            break;
        }

        Cardinal validDirection = directions[g.randomInt(0,directions.size()-1)];

        Position validPos = adjacentPos(validDirection, pos);

        recursolve(g, validPos);
    }
}

void Maze::solve(Graphics& g){

    // reset all markers to false
    for(int i=0; i<numRows; i++){
        for(int j=0; j<numCols; j++){
            markers[i][j] = false;
        }
    }

    recursolve(g, Position{0,0});
}

void graphicsMain(Graphics &g)
{
    Maze maze(20,25);

    maze.draw(g);

    while (g.draw())
    {
        auto events = g.events();

        for (unsigned int i = 0; i < events.size(); ++i) {
            Event e = events[i];
            switch (e.evtType) {
            case EvtType::KeyPress:
                switch (e.arg) {
                case 'G':
                    maze.generate(g);
                    g.clear();
                    maze.draw(g);
                    break;
                case 'S':
                    maze.solve(g);
                    g.clear();
                    maze.draw(g);
                    break;
                case 'D':
                    maze.toggleDrawStyle();
                    g.clear();
                    maze.draw(g);
                    break;
                }
                break;
            case EvtType::KeyRelease:
                break;
            case EvtType::MouseMove:
                break;
            case EvtType::MousePress:
                g.clear();
                g.text(20,50,20,"D to toggle draw mode");
                g.text(20,80,20,"G to generate maze");
                g.text(20,110,20,"S to solve maze");
                g.text(20,110,20,"BLUE DOTS = SOLUTION");
                g.draw(1000);
                break;
            case EvtType::MouseRelease:
                break;
            case EvtType::MouseWheel:
                break;
            default:
                break;
            }
        }
    }
}

int main()
{
    Graphics g("Graphics App", 1000, 800, graphicsMain);
}
