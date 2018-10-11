#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;





//*************************************************************************
//                              SHIP CLASS
//*************************************************************************
class BoardShip
{
public:
    BoardShip(int length, char symbol, string name, int iD);
    int length() const;
    int health() const;
    void hit(Point p);
    string name() const;
    char symbol() const;
    int iD() const;
    vector<Point> points(); //Returns the vector of points
    bool addPoints(Point p); //Adds points to vector, returns false if point is already in the vector
private:
    int m_health;
    int m_length;
    string m_name;
    char m_symbol;
    int m_iD;
    vector<Point> m_points;
};

BoardShip::BoardShip(int length, char symbol, string name, int iD)
{
    m_length = length;
    m_name = name;
    m_symbol = symbol;
    m_iD = iD;
    m_health = length;
}

vector<Point> BoardShip::points()
{
    return m_points;
}

int BoardShip::health() const
{
    return m_health;
}

void BoardShip::hit(Point q)
{
    for (vector<Point>::iterator p = m_points.begin(); p != m_points.end(); p++)
    {
        if((p->r == q.r) && (p->c == q.c))
        {
            m_points.erase(p); //deletes the point from list of points
            break;
        }

    }
    m_health--;
}

bool BoardShip::addPoints(Point p)
{
    for(int i=0; i<m_points.size(); i++)
    {
        if((p.r == m_points[i].r) && (p.c == m_points[i].c))
        {
            return false;
        }
    }
    m_points.push_back(p);
    return true;
    
}

int BoardShip::length() const
{
    return m_length;
}

string BoardShip::name() const
{
    return m_name;
}

int BoardShip::iD() const
{
    return m_iD;
}

char BoardShip::symbol() const
{
    return m_symbol;
}

//*************************************************************************
//*************************************************************************



class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    ~BoardImpl();
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

    
  private:
 
    
    //Checks if there is a ship at a certain point, returns true if the board contains a ship
    bool containsShip(Point p) const
    {
        for(int i=0; i<shipPoints.size(); i++)
        {
            if((shipPoints[i].c == p.c) && (shipPoints[i].r == p.r))
                return true;
        }
        return false;
    }
    
    bool blockedShip(Point p) const
    {
        for(int i=0; i<blockedPoints.size(); i++)
        {
            if((blockedPoints[i].c == p.c) && (blockedPoints[i].r == p.r))
                return true;
        }
        return false;
    }
    
    
    int rows;
    int cols;
    char theGrid[MAXROWS][MAXCOLS];
    vector<Point> shipPoints;
    vector<Point> blockedPoints;
    vector<int> usedIDs;
    vector<Point> shotPoints;
    vector<BoardShip*> theBoardShips;
    const Game& m_game;
};




BoardImpl::BoardImpl(const Game& g)
 : m_game(g), usedIDs(10, -3)
{
    rows = m_game.rows();
    cols = m_game.cols();
    //Initializes every element in theGrid;
    for(int i=0; i< MAXROWS; i++)
    {
        for(int j=0; j<MAXCOLS; j++)
        {
            theGrid[i][j] = '-';
        }
    }
    
    //Makes the points in the used coordinates . to "clear" the board
    clear();
    
}


BoardImpl::~BoardImpl()
{
    for(int i=0; i<theBoardShips.size(); i++)
    {
        delete theBoardShips[i];
    }
}

void BoardImpl::clear()
{
    for(int i=0; i< m_game.rows(); i++)
    {
        for(int j=0; j< m_game.cols(); j++)
        {
            theGrid[i][j] = '.';
        }
    }
    // This compiles, but may not be correct
}

void BoardImpl::block()
{
      // Block cells with 50% probability
    Point p;
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                p.r = r;
                p.c = c;
                if(blockedPoints.size() < ((m_game.rows()*m_game.cols())/2))
                    blockedPoints.push_back(p);
            }
}

void BoardImpl::unblock()
{
    Point q;
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            q.r = r;
            q.c = c;
            if(blockedShip(q))
            {
                for (vector<Point>::iterator p = blockedPoints.begin(); p != blockedPoints.end(); p++)
                {
                    if((p->r == q.r) && (p->c == q.c))
                    {
                        blockedPoints.erase(p); //deletes the point from list of points
                        break;
                    }
                    
                }

            }
            
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    //Check if valid shipID
    if((shipId >= m_game.nShips()) || shipId < 0)
        return false;
    
    int shipLength = m_game.shipLength(shipId);
    char shipSymbol = m_game.shipSymbol(shipId);
    string shipName = m_game.shipName(shipId);
    
    //Check if a valid point
    if(!m_game.isValid(topOrLeft))
        return false;
    
    //Check for if outside or inside of the board
    //If the length is greater than the rows and cols, then it will go outside the board
    if(dir == VERTICAL)
    {
        if((rows-topOrLeft.r)-shipLength < 0)
            return false;
    }
    if(dir == HORIZONTAL)
    {
        if((cols-topOrLeft.c)-shipLength < 0)
            return false;
    }
    
    //Getting points of where the ship will be
    vector<Point> potentialPoints;
    Point temp(topOrLeft.r,topOrLeft.c);
    if(dir == VERTICAL)
    {
        for(int i=0; i<shipLength; i++)
        {
            temp.r = topOrLeft.r+i;
            potentialPoints.push_back(temp);
        }
    }
    if(dir == HORIZONTAL)
    {
        for(int i=0; i<shipLength; i++)
        {
            temp.c = topOrLeft.c+i;
            potentialPoints.push_back(temp);
        }
    }
    
    
    //Check for if blocked or ship already there
    for(int i=0; i<potentialPoints.size(); i++)
    {
        for(int j=0; j<blockedPoints.size(); j++)
        {
            if((potentialPoints[i].c == blockedPoints[j].c) && (potentialPoints[i].r == blockedPoints[j].r))
                return false;
        }
      
        
        for(int k=0; k<theBoardShips.size(); k++)
        {
            for(int m = 0; m<theBoardShips[k]->points().size(); m++)
            {
                if((potentialPoints[i].c == theBoardShips[k]->points()[m].c) && (potentialPoints[i].r == theBoardShips[k]->points()[m].r))
                    return false;
            }
        }
    }
    
    //Check if id is already used
    for(int j=0; j<usedIDs.size(); j++)
    {
        if(shipId == usedIDs[j])
            return false;
    }
    
    
    //If code reaches here, that means it has passed all the checks for placing a ship down!
    
    int placingRow = topOrLeft.r;
    int placingCol = topOrLeft.c;
    usedIDs.insert(usedIDs.begin()+shipId, shipId);
 //   usedIDs.push_back(shipId);

    temp.r = topOrLeft.r;
    temp.c = topOrLeft.c;
    
    BoardShip* temporaryShip = new BoardShip(shipLength, shipSymbol, shipName, shipId);
    
    
    for(int i=0; i<shipLength; i++)
    {
        if(dir == VERTICAL)
        {
            theGrid[placingRow+i][placingCol] = shipSymbol;
            temp.r = placingRow+i;
            temporaryShip->addPoints(temp);
            shipPoints.push_back(temp);
        }
        else if(dir == HORIZONTAL)
        {
            theGrid[placingRow][placingCol+i] = shipSymbol;
            temp.c = placingCol+i;
            temporaryShip->addPoints(temp);
            shipPoints.push_back(temp);
        }
    }
    
    theBoardShips.push_back(temporaryShip);
    
    
    return true; // This compiles, but may not be correct
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    //How to use direction in unplaceship?
    if((shipId >= m_game.nShips()) || (shipId < 0))
        return false;
    if(containsShip(topOrLeft) == false)
        return false;
    delete theBoardShips[shipId];
    
    for (vector<BoardShip*>::iterator p = theBoardShips.begin(); p != theBoardShips.end(); p++)
    {
        if(((*p)->iD() == shipId))
        {
            theBoardShips.erase(p); //deletes the point from list of points
            break;
        }
        
    }
    
    for(vector<int>::iterator p = usedIDs.begin(); p!=usedIDs.end(); p++)
    {
        if((*p) == shipId)
        {
            usedIDs.erase(p);
            break;
        }
    }
    
    return true; // This compiles, but may not be correct
}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for(int i=0; i<cols; i++)
    {
        cout << i;
    }
    cout << endl;
    if(shotsOnly)
    {
        for(int i=0; i<rows; i++)
        {
            cout << i << " ";
            for(int j=0; j<cols; j++)
            {
                
                if((theGrid[i][j] != 'X') && (theGrid[i][j] != 'o'))
                    cout << ".";
                else if((theGrid[i][j] == 'X'))
                    cout << "X";
                else if((theGrid[i][j] == 'o'))
                    cout << "o";
                else
                    cout << ".";
            }
            cout << endl;
        }
    }
    else
    {
        for(int i=0; i<rows; i++)
        {
            cout << i << " ";
            for(int j=0; j<cols; j++)
            {
                cout << theGrid[i][j];
            }
            cout << endl;
        }
    }
        
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    //Check if a position has been shot before
    
    //If the attack function returns false, then cout:
    //(Player name) wasted a shot at (P).
    //Example:
    //Shuman the Human wasted a shot at (4,6).
    if(!m_game.isValid(p))
        return false;
    
    for(int i=0; i<shotPoints.size(); i++)
    {
        if((shotPoints[i].c == p.c) && (shotPoints[i].r == p.r))
        {
            return false;
        }
    }
    
    shotHit = false;
    
    
    
    for(int i=0; i<theBoardShips.size(); i++)
    {
        for(int j=0; j<theBoardShips[i]->points().size(); j++)
        {
            if((theBoardShips[i]->points()[j].c == p.c) && (theBoardShips[i]->points()[j].r == p.r))
            {
                //That means there is a hit!
                shotHit = true;
                theBoardShips[i]->hit(p);
                theGrid[p.r][p.c] = 'X';
                if(theBoardShips[i]->health() == 0)
                {
                    shipDestroyed = true;
                    shipId = theBoardShips[i]->iD(); //Set to dead ID
                    
                    //DO NOT WANT TO DELETE BECAUSE THE INFO IS STILL GETTING USED!
                    
                  /*  delete theBoardShips[i];
                    int counter = 0;
                    for (vector<BoardShip*>::iterator p = theBoardShips.begin(); p != theBoardShips.end(); p++)
                    {
                        if(i == counter)
                            theBoardShips.erase(p); //deletes the ship from list of points
                        else
                            counter++;
                    }
                   */
                }
                
                
                //Check if need to do anything with theBoardShips
            }
        }
    }
    
    if(shotHit == false)
    {
        theGrid[p.r][p.c] = 'o';
    }
    
    shotPoints.push_back(p);
    return true; // This compiles, but may not be correct
}

bool BoardImpl::allShipsDestroyed() const
{
    //Check when all items in the vector are erased so when it is empty
    bool finalReturn = true;
    for(int i=0; i<theBoardShips.size(); i++)
    {
        if(!theBoardShips[i]->points().empty())
        {
            finalReturn = false;
        }
        
    }
    return finalReturn;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
