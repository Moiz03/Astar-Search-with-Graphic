#include <iostream>
#include <utility>
#include <set>
#include <cstring>
#include <float.h>
#include <cmath>
#include <stack>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <sstream>
#define ROW 60
#define COL 60
using namespace std;
using namespace sf;

typedef pair<int, int> Pair;
typedef pair<double, Pair> Dpair;

struct box
{
    int predecessor_i, predecessor_j;
    double f, g, h;
};

class Astar
{
private:
    Pair source;
    Pair destination;
    int area[ROW][COL];
    bool closedList[ROW][COL];
    set<Dpair> openList;
    box boxGrid[ROW][COL];
    vector<Pair> Path;
    int grid[ROW][COL];

public:
    Astar()
    {
        memset(closedList, false, sizeof(closedList));
        //memset(area, 1, sizeof(area));

        //

        source = make_pair(10, 20);
        destination = make_pair(50, 40);

        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                boxGrid[i][j].f = boxGrid[i][j].g = boxGrid[i][j].h = FLT_MAX;
                boxGrid[i][j].predecessor_i = boxGrid[i][j].predecessor_j = -1;
                area[i][j] = 1;
            }
        }

        for (int i = 0; i < 60; i++)
            for (int j = 0; j < 60; j++)
            {
                if (i == 0 || i == 59 || j == 0 || j == 59) //walls
                    grid[i][j] = 0;
                else
                    grid[i][j] = 1;
            }
        randomeBlockage();
        // printthis();
    }

    void AstarSearch()
    {
        int x = source.first;
        int y = source.second;
        boxGrid[x][y].f = boxGrid[x][y].g = boxGrid[x][y].h = 0.0;
        boxGrid[x][y].predecessor_i = source.first;
        boxGrid[x][y].predecessor_j = source.second;

        openList.insert(make_pair(0.0, make_pair(x, y)));

        while (!openList.empty())
        {
            Dpair p = *openList.begin();
            openList.erase(openList.begin());

            int row = p.second.first;
            int col = p.second.second;
            closedList[row][col] = true;

            if (AstarHelper(row - 1, col, row, col, 1.0))
                return;
            else if (AstarHelper(row + 1, col, row, col, 1.0))
                return;
            else if (AstarHelper(row, col - 1, row, col, 1.0))
                return;
            else if (AstarHelper(row, col + 1, row, col, 1.0))
                return;
            else if (AstarHelper(row - 1, col - 1, row, col, 1.414))
                return;
            else if (AstarHelper(row + 1, col + 1, row, col, 1.414))
                return;
            else if (AstarHelper(row + 1, col - 1, row, col, 1.414))
                return;
            else if (AstarHelper(row - 1, col + 1, row, col, 1.414))
                return;
        }

        cout << "Destination Not Found !!! \n";
        return;
    }

private:
    void randomeBlockage()
    {
        srand(time(NULL));
        for (int k = 0; k < 500; k++)
        {
            int i = rand() % ROW;
            int j = rand() % COL;

            if (i != 0 && j != 0)
            {
                if (destinationFound(i, j) == false)
                {
                    area[i][j] = 0;
                    grid[i][j] = 0;
                }
            }
        }
    }

    bool AstarHelper(int i, int j, int row, int col, double length)
    {
        if (isExist(i, j))
        {
            if (destinationFound(i, j) == true)
            {
                boxGrid[i][j].predecessor_i = row;
                boxGrid[i][j].predecessor_j = col;
                printf("The destination cell is found\n");
                print();
                return true;
            }

            else if (closedList[i][j] == false && isAvaliable(i, j) == true)
            {
                double g = boxGrid[row][col].g + length;
                double h = findHuersitic(i, j);
                double f = g + h;

                if (boxGrid[i][j].f == FLT_MAX || boxGrid[i][j].f > f)
                {
                    openList.insert(make_pair(f, make_pair(i, j)));
                    boxGrid[i][j].f = f;
                    boxGrid[i][j].g = g;
                    boxGrid[i][j].h = h;
                    boxGrid[i][j].predecessor_i = row;
                    boxGrid[i][j].predecessor_j = col;
                }
            }
        }
        return false;
    }

    void print()
    {
        printf("\nThe Path is ");
        int row = destination.first;
        int col = destination.second;

        while (!(boxGrid[row][col].predecessor_i == row && boxGrid[row][col].predecessor_j == col))
        {
            Path.push_back(make_pair(row, col));
            int temp_row = boxGrid[row][col].predecessor_i;
            int temp_col = boxGrid[row][col].predecessor_j;
            row = temp_row;
            col = temp_col;
        }
        Path.push_back(make_pair(row, col));
        for (int i = Path.size()-1; i >= 0; i--)
        {
            pair<int, int> p = Path[i];
            printf("-> (%d,%d) ", p.first, p.second);
        }

        printf("\n");
        drawGrid();

        return;
    }

    bool isExist(int row, int col)
    {
        return ((row >= 0) && (row < ROW) && (col >= 0) && (col < COL));
    }

    bool destinationFound(int row, int col)
    {
        return ((row == destination.first) && (col == destination.second));
    }

    bool isAvaliable(int row, int col)
    {
        return ((area[row][col]) == 1);
    }

    double findHuersitic(int row, int col)
    {
        return ((double)sqrt((row - destination.first) * (row - destination.first) + (col - destination.second) * (col - destination.second)));
    }

    void printthis()
    {
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                cout << area[i][j] << " ";
            }
            cout << endl;
        }
    }

    void drawGrid()
    {
        int filled[ROW][COL];
        int source_x = source.first;
        int source_y = source.second;
        int dest_x = destination.first;
        int dest_y = destination.second;
        RenderWindow window(VideoMode(800, 600), "GRID");

        //map with obstacle

        for (int i = 0; i < ROW; i++)
            for (int j = 0; j < COL; j++)
            {
                //sptSet[i][j]=false;     //dijkstra all unexplored
                filled[i][j] = 0; //all uncolored
            }
        // Shapes

        RectangleShape rectangle(Vector2f(10, 10)); //default box :White
        rectangle.setFillColor(Color::White);
        RectangleShape brectangle(Vector2f(10, 10)); //Black box
        brectangle.setFillColor(Color::Black);
        RectangleShape grectangle(Vector2f(10, 10)); //Green
        grectangle.setFillColor(Color::Green);
        grectangle.setOutlineThickness(2);
        grectangle.setOutlineColor(Color::Red);
        RectangleShape mrectangle(Vector2f(10, 10)); //Magenta
        mrectangle.setFillColor(Color::Magenta);
        mrectangle.setOutlineThickness(2);
        mrectangle.setOutlineColor(Color::Red);
        RectangleShape blueRectangle(Vector2f(10, 10));
        blueRectangle.setFillColor(Color::Blue);
        blueRectangle.setOutlineThickness(2);
        blueRectangle.setOutlineColor(Color::Red);
        RectangleShape rrectangle(Vector2f(10, 10));
        rrectangle.setFillColor(Color::Red);
        rrectangle.setOutlineThickness(2);
        rrectangle.setOutlineColor(Color::Red);
        RectangleShape yrectangle(Vector2f(10, 10));
        yrectangle.setFillColor(Color::Yellow);

        //display
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
                    window.close();
            }
            window.clear();
            stringstream ss2; //int2string
            ss2 << Path.size();

            if (!Path.empty())
            {
                for (int i = 0; i < int(Path.size()); i++)
                {
                    mrectangle.setPosition(Path[i].second * 10, Path[i].first * 10); //Reversed notion of row & column
                    window.draw(mrectangle);                                         //final Path

                    filled[Path[i].first][Path[i].second] = 1;
                }
            }

            blueRectangle.setPosition(source_y * 10, source_x * 10);
            window.draw(blueRectangle); //source
            filled[source_x][source_y] = 1;
            grectangle.setPosition(dest_y * 10, dest_x * 10);
            window.draw(grectangle); //destination
            filled[dest_x][dest_y] = 1;
            for (int i = 0; i <= 590; i += 10)
                for (int j = 0; j <= 590; j += 10)
                {

                    if (grid[i / 10][j / 10] == 0)
                    {
                        brectangle.setOutlineThickness(2);
                        brectangle.setOutlineColor(Color::Red);
                        brectangle.setPosition(j, i);
                        window.draw(brectangle); //User's obstacle
                    }

                    if (closedList[i / 10][j / 10] == true && filled[i / 10][j / 10] == 0)
                    {
                        yrectangle.setOutlineThickness(2);
                        yrectangle.setOutlineColor(Color::Red);
                        yrectangle.setPosition(j, i);
                        window.draw(yrectangle); // Explored  Cells by A*
                    }
                    if (grid[i / 10][j / 10] == 1 && closedList[i / 10][j / 10] == false && filled[i / 10][j / 10] == 0)
                    { //not in dijkstra & A*
                        rectangle.setOutlineThickness(2);
                        rectangle.setOutlineColor(Color::Red);
                        rectangle.setPosition(j, i);
                        window.draw(rectangle); //default white cells
                    }
                }
            window.display();
        }
    }
};
int main()
{
    Astar *a = new Astar;
    a->AstarSearch();

    return 0;
}
