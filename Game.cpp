#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

//*************************************************************************
//                              SHIP CLASS
//*************************************************************************
class Ship
{
public:
    Ship(int length, char symbol, string name, int iD);
    int length() const;
    string name() const;
    char symbol() const;
    int iD() const;
    vector<Point> points(); //Returns the vector of points
    bool addPoints(Point p); //Adds points to vector, returns false if point is already in the vector
private:
    int m_length;
    string m_name;
    char m_symbol;
    int m_iD;
    vector<Point> m_points;
};

Ship::Ship(int length, char symbol, string name, int iD)
{
    m_length = length;
    m_name = name;
    m_symbol = symbol;
    m_iD = iD;
}

vector<Point> Ship::points()
{
    return m_points;
}

bool Ship::addPoints(Point p)
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

int Ship::length() const
{
    return m_length;
}

string Ship::name() const
{
    return m_name;
}

int Ship::iD() const
{
    return m_iD;
}

char Ship::symbol() const
{
    return m_symbol;
}

//*************************************************************************
//*************************************************************************


class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    ~GameImpl();
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
    vector<Ship*> ships() const;
    bool addPointToShip(Point p, int shipID);
    Point convertStringToPoint(string thePoint);
  private:
    vector<Ship*> theShips;
    int numOfRows;
    int numOfCols;
    int numOfShips;
};



void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    numOfRows = nRows;
    numOfCols = nCols;
    numOfShips = 0;
}

int GameImpl::rows() const
{
    return numOfRows;
}

int GameImpl::cols() const
{
    return numOfCols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    
    //Creating a new ship
    Ship* newShip = new Ship(length, symbol, name, numOfShips);
    numOfShips++;
    theShips.push_back(newShip);
    return true;
}

GameImpl::~GameImpl()
{
    //Deleting the individual ships inside the vector
    for(int i = 0; i<theShips.size(); i++)
        delete theShips[i];
}


int GameImpl::nShips() const
{
    return numOfShips;  // This compiles but may not be correct
}

int GameImpl::shipLength(int shipId) const
{
    if(shipId >= theShips.size())
    {
        cerr << "invalid ship ID" << endl;
        return 0;
    }
    return theShips[shipId]->length();  // This compiles but may not be correct
}

char GameImpl::shipSymbol(int shipId) const
{
    if(shipId >= theShips.size())
    {
        cerr << "invalid ship ID" << endl;
        return 0;
    }
    return theShips[shipId]->symbol();   // This compiles but may not be correct
}

string GameImpl::shipName(int shipId) const
{
    if(shipId >= theShips.size())
    {
        cerr << "invalid ship ID" << endl;
        return 0;
    }
    return theShips[shipId]->name(); 
}

vector<Ship*> GameImpl::ships() const
{
    return theShips;
}

bool GameImpl::addPointToShip(Point p, int shipID)
{
    return theShips[shipID]->addPoints(p);
}

Point GameImpl::convertStringToPoint(string thePoint)
{
    string charfirstPoint = "";
    string charsecondPoint = "";
    bool spaceNeededNow = false;
    bool allSetToConvertToPoints = false;
    bool mustRestartAgain = false;
    while(allSetToConvertToPoints == false)
    {
        if(thePoint == "")
        {
            cout << "You must enter two integers." << endl;
            mustRestartAgain = true;
        }
        for(int i=0; i<thePoint.length(); i++)
        {
            if((!isdigit(thePoint[i])) && (mustRestartAgain == false) && (thePoint[i] != ' ') && (thePoint[i] != '\n'))
            {
                cout << "You must enter two integers." << endl;
                mustRestartAgain = true;
            }
            else
            {
                if(isdigit(thePoint[i]))
                {
                    //adds a digit before space. ex 32 3
                    if(spaceNeededNow == true)
                    {
                        charfirstPoint += thePoint[i];
                    }
                    
                    //if the first character is empty
                    if(charfirstPoint == "")
                    {
                        charfirstPoint += thePoint[i];
                        spaceNeededNow = true;
                    }
                    
                    //Now doing second point
                    if((charfirstPoint != "") && (spaceNeededNow == false))
                    {
                        charsecondPoint += thePoint[i];
                        if(i+1 == thePoint.length())
                        {
                            allSetToConvertToPoints = true;
                            break;
                        }
                        else
                        {
                            for(int j=i+1; j<thePoint.length(); j++)
                            {
                                if(isdigit(thePoint[j]))
                                {
                                    charsecondPoint += thePoint[j];
                                }
                                else
                                {
                                    cerr << "Woohoo" << endl;
                                    allSetToConvertToPoints = true;
                                    break;
                                }
                            }
                        }
                    }
                    
                }
                //Makes possible for transition onto second point
                if((spaceNeededNow == true) && (thePoint[i] == ' '))
                {
                    spaceNeededNow = false;
                }
                else if((spaceNeededNow == true) && (thePoint[i] == '\n'))
                {
                    spaceNeededNow = false;
                }
                
                
                
            }
        }
        
        if(mustRestartAgain)
        {
            cout << "Enter the row and column to attack: (e.g, 3 5): ";
            cin >> thePoint;
        }
    }
    
    

    int firstPoint = stoi(charfirstPoint);
    int lastPoint = stoi(charsecondPoint);
    Point p(firstPoint, lastPoint);
    return p;
}


Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    //Create 2 board objects
    
    bool is1Playable = p1->placeShips(b1);
    bool is2Playable = p2->placeShips(b2);
    
    if(is1Playable == false || is2Playable == false)
        return nullptr;
    
    //Go until one of the boards has all of the ships destroyed
    bool shotsOnly = true;
    bool shotHit = false;
    bool shipDestroyed = false;
    int shipID = 0;
    Point p;
    
    while((b1.allShipsDestroyed() == false) && (b2.allShipsDestroyed() == false))
    {
        
        //Display of board depending on if human playing or not
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        if(p1->isHuman())
        {
            b2.display(shotsOnly);
        }
        else
        {
            b2.display(!shotsOnly);
        }
        
        //NEED TO CHANGE:

   //     string thePoint = "";
   //     cout << "Enter the row and column to attack: (e.g, 3 5): ";
   //     getline(cin, thePoint);
        
   //     Point p = convertStringToPoint(thePoint);
        
        //Probably need to put recommended attack and stuff here :D
        
        
        Point p = p1->recommendAttack();
        
        
        bool correctAttack = b2.attack(p, shotHit, shipDestroyed, shipID);
        
        p1->recordAttackResult(p, correctAttack, shotHit, shipDestroyed, shipID);
        
        shotsOnly = p2->isHuman();
        
        if(correctAttack == false)
        {
            cout << p1->name() << " wasted a shot at (" << p.r << "," << p.c << ")." << endl;
        }
        else if(shipDestroyed == true)
        {
            cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and destroyed the " << ships()[shipID]->name() << " resulting in:" << endl;
            b2.display(shotsOnly);
        }
        else if(shotHit == true)
        {
            cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and hit something, resulting in:" << endl;
            b2.display(shotsOnly);
        }
        else if(shotHit == false)
        {
            cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and missed, resulting in:" << endl;
            b2.display(shotsOnly);
        }
        
        shotHit = false;
        shipDestroyed = false;
    
        
        //Player 2 is now attacking
        if(b2.allShipsDestroyed() == true)
            break;
        
        if(shouldPause)
            waitForEnter();
        
        if(p2->isHuman())
        {
            b1.display(shotsOnly);
        }
        else
        {
            b1.display(!shotsOnly);
        }
        
        
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
        
        
   //     thePoint = "";
   //     cout << "Enter the row and column to attack: (e.g, 3 5): ";
   //     getline(cin, thePoint);
   //
   //     Point q = convertStringToPoint(thePoint);
        
        //Probably need to put recommended attack and stuff here :D
        
        Point q = p2->recommendAttack();
        
        correctAttack = b1.attack(q, shotHit, shipDestroyed, shipID);
        
        p2->recordAttackResult(q, correctAttack, shotHit, shipDestroyed, shipID);
        
        shotsOnly = p1->isHuman();
        
        if(correctAttack == false)
        {
            cout << p2->name() << " wasted a shot at (" << q.r << "," << q.c << ")." << endl;
        }
        else if(shipDestroyed == true)
        {
            cout << p2->name() << " attacked (" << q.r << "," << q.c << ") and destroyed the " << ships()[shipID]->name() << " resulting in:" << endl;
            b1.display(shotsOnly);
        }
        else if(shotHit == true)
        {
            cout << p2->name() << " attacked (" << q.r << "," << q.c << ") and hit something, resulting in:" << endl;
            b1.display(shotsOnly);
        }
        else if(shotHit == false)
        {
            cout << p2->name() << " attacked (" << q.r << "," << q.c << ") and missed, resulting in:" << endl;
            b1.display(shotsOnly);
        }
        
        if((!b1.allShipsDestroyed()) && (shouldPause == true))
            waitForEnter();
        
        shotHit = false;
        shipDestroyed = false;
        
    }
    
    
    //Says the name of the person who won! Woohoo!
    if(b1.allShipsDestroyed() == true)
    {
        if(p1->isHuman())
            b2.display(!shotsOnly);
        cout << p2->name() << " wins!" << endl;
        return p2;
    }
    else if(b2.allShipsDestroyed() == true)
    {
        if(p2->isHuman())
            b1.display(!shotsOnly);
        cout << p1->name() << " wins!" << endl;
        return p1;
    }
        
    
    return nullptr;  // This compiles but may not be correct
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

