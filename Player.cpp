#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;    
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.
//typedef AwfulPlayer HumanPlayer;
class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game &g);
    virtual bool isHuman() const { return true; }
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    Point convertStringToPoint(string thePoint);
    Point convertStringToPointTwo(string thePoint);

private:
    Point m_lastCellAttacked;
private:
};


HumanPlayer::HumanPlayer(string nm, const Game &g)
:Player(nm, g)
{
    
}

bool HumanPlayer::placeShips(Board &b)
{
    cout << name() << " must place " << game().nShips() << " ships." << endl;
    for(int i=0; i<game().nShips(); i++)
    {
        bool correctDirection = false;
        Direction theRealDirection = HORIZONTAL;
        while(correctDirection == false)
        {
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            string theDirection = "";
            getline(cin, theDirection);
            if((theDirection != "h") && (theDirection != "v"))
            {
                cout << "Direction must be h or v" << endl;
            }
            else
            {
                correctDirection = true;
                if(theDirection == "v")
                {
                    theRealDirection = VERTICAL;
                }
            }
        }
        
        bool correctPoint = false;
        while(correctPoint == false)
        {
            string thePoint = "";
            cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
            getline(cin, thePoint);
            Point p = convertStringToPoint(thePoint);
            if(b.placeShip(p, i, theRealDirection))
            {
                correctPoint = true;
            }
            else
            {
                cout << "The ship can not be placed there." << endl;
            }
            
        }
        
        b.display(!true);
        
    }
    
    
    
    return true;
}


Point HumanPlayer::recommendAttack()
{
    string thePoint;
    cout << "Enter the row and column to attack: (e.g, 3 5): ";
    getline(cin, thePoint);
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
                                    int firstPoint = stoi(charfirstPoint);
                                    int lastPoint = stoi(charsecondPoint);
                                    Point p(firstPoint, lastPoint);
                                    allSetToConvertToPoints = true;
                                    return p;
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
    
    
    
    
    
    
    
    
    
    
//    bool correctPoint = false;
    
 //   while(correctPoint == false)
 //   {
 //       string thePoint = "";
 //       cout << "Enter the row and column to attack (e.g, 3 5): ";
 //       getline(cin, thePoint);
  //      Point p = convertStringToPoint(thePoint);
  //      return p;
  //  }
  //  Point FailUre;
  //  return FailUre;
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                bool shipDestroyed, int shipId)
{
    
}

void HumanPlayer::recordAttackByOpponent(Point p)
{
    cerr << "lol dont need to do this" << endl;
}


Point HumanPlayer::convertStringToPoint(string thePoint)
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
                                    int firstPoint = stoi(charfirstPoint);
                                    int lastPoint = stoi(charsecondPoint);
                                    Point p(firstPoint, lastPoint);
                                    if(game().isValid(p))
                                    {
                                        cerr << "Woohoo" << endl;
                                        allSetToConvertToPoints = true;
                                        return p;
                                        break;
                                    }
                                    else
                                    {
                                        cout << "The ship can not be placed there." << endl;
                                        mustRestartAgain = true;
                                    }
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


//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game &g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    int placeShipsRecursion(Board &b, int ships, int& counter, vector<Point> theShipCoords);
    bool allShipsPlaced(Board &b);
private:
    vector<Point> ShipCoords;
    vector<Point> allShipPoints;
    vector<Direction> ShipDirections;
    vector<Point> usedPoints;
    vector<int> destroyedShips;
    vector<Point> pointsToHit;
    bool inStateOne;
};

MediocrePlayer::MediocrePlayer(string nm, const Game &g)
:Player(nm,g)
{
    inStateOne = true;
}


bool MediocrePlayer::allShipsPlaced(Board &b)
{
    game().nShips();
    return true;
}


int MediocrePlayer::placeShipsRecursion(Board &b, int shipID, int& counter, vector<Point> topOrLeftCoords)
{
    if(shipID == -1)
        return counter+100;
    if(shipID >= game().nShips())
        return counter;
    if(counter >= 50)
        return counter;
    
    
    //Just added part from above
    
    
    Point p;
    int row = 0;
    int col = 0;
    Direction dir = HORIZONTAL;
    bool couldntPlaceShip = false;
    
    //Ships have been placed yet
    if((shipID == 0) && (topOrLeftCoords.empty() == false))
    {
        //Remove item
        b.unplaceShip(topOrLeftCoords[shipID], shipID, HORIZONTAL);
        counter++;
    }
    else if((shipID != 0) && (topOrLeftCoords.size() > shipID))
    {
        dir = ShipDirections[shipID];
        if((dir == HORIZONTAL) && (col != game().cols()-1))
        {
            //do something
            p.c = topOrLeftCoords[shipID].c+1;
            p.r = topOrLeftCoords[shipID].r;
        }
        else if((dir == HORIZONTAL) && (col == game().cols()-1))
        {
            p.r++;
            p.c = 0;
        }
        if((dir == VERTICAL) && (row != game().rows()-1))
        {
            //do something
            p.r = topOrLeftCoords[shipID].r+1;
            p.c = topOrLeftCoords[shipID].c;
        }
        else if((dir == VERTICAL) && (row == game().rows()-1))
        {
            p.c++;
            p.r = 0;
        }
    
        topOrLeftCoords.erase(topOrLeftCoords.begin()+shipID-1);
        b.unplaceShip(topOrLeftCoords[shipID], shipID-1, HORIZONTAL);
    }
    
    ///Possibly places everything in every spot
    
    while((b.placeShip(p, shipID, dir) == false) && (couldntPlaceShip == false))//((p.r == game().rows()-1) && (p.c == game().cols()-1) && (dir == VERTICAL)))
    {
        if(row != game().rows() - 1)
        {
            row++;
            p.r = row;
            p.c = col;
        }
        else if((row == game().rows() - 1) && (col != game().cols()-1))
        {
            col++;
            row = 0;
            p.c = col;
            p.r = row;
        }
        else if((row == game().rows() - 1) && (col == game().cols()-1) && (dir == HORIZONTAL))
        {
            col = 0;
            row = 0;
            dir = VERTICAL;
            p.c = col;
            p.r = row;
        }
        else if((row == game().rows() - 1) && (col == game().cols()-1) && (dir == VERTICAL))
        {
            couldntPlaceShip = true;
        }
        
    }
    
    //Adding the ship to the vector
    
    int theReturn = 0;
    if(couldntPlaceShip == true)
    {
        theReturn += placeShipsRecursion(b, shipID-1, counter, topOrLeftCoords);
        //Have to go back up a level
    }
    else
    {
        topOrLeftCoords.push_back(p);
        ShipDirections.push_back(dir);
        theReturn += placeShipsRecursion(b, shipID+1, counter, topOrLeftCoords);
    }
    
   
    return theReturn;
}

bool MediocrePlayer::placeShips(Board &b)
{
    b.block();
    int couldPlace = 0;
    int counter = 0;
    couldPlace = placeShipsRecursion(b, 0, counter, ShipCoords);
    b.unblock();
    b.display(!true);
    if(couldPlace >= 50)
        return false;
    return true;
}

Point MediocrePlayer::recommendAttack()
{
    if(inStateOne)
    {
        bool hasARandomPoint = false;
        Point p(game().randomPoint());
        
        while(hasARandomPoint == false)
        {
            for(int i=0; i<usedPoints.size(); i++)
            {
                if((p.r == usedPoints[i].r) && (p.c == usedPoints[i].c))
                {
                    Point q(game().randomPoint());
                    p.r = q.r;
                    p.c = q.c;
                    i = -1;
                }
            }
            hasARandomPoint = true;
            
        }
        usedPoints.push_back(p);
        return p;
    }
    else
    {
        Point q;
        int i=0;
        bool needToChooseAnotherPoint = true;
        while((i<=pointsToHit.size()-1) && (needToChooseAnotherPoint == true))
        {
            q.r = pointsToHit[i].r;
            q.c = pointsToHit[i].c;
            
            for (vector<Point>::iterator ppp = pointsToHit.begin(); ppp != pointsToHit.end(); ppp++)
            {
                if((ppp->r == q.r) && (ppp->c == q.c))
                {
                    pointsToHit.erase(ppp);
                    break;
                }
                
            }
            
            bool poop = false;
            for(int j=0; j<usedPoints.size(); j++)
            {
                if((usedPoints[j].r == q.r) && (usedPoints[j].c == q.c))
                {
                    needToChooseAnotherPoint = true;
                    poop = true;
                }
            }
            
            if(poop == false)
            {
                needToChooseAnotherPoint = false;
            }
            i++;
            
            
            
        }
        
        usedPoints.push_back(q);
        return q;
    }
    
    //To get individual elements for each coord
    /*for(int i=0; i<ShipCoords.size(); i++)
     {
     for(int j=0; j<game().shipLength(i); j++)
     {
     Point temp;
     temp.r = ShipCoords[i].r+j;
     temp.c = ShipCoords[i].c+j;
     if((p.r == temp.r) && (p.c = temp.c))
     {
     hasARandomPoint = true;
     }
     }
     
     if((p.r == ShipCoords[i].r) && (p.c == ShipCoords[i].c))
     {
     p = game().randomPoint();
     }
     }*/
    // }
    // return p;
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    if(validShot == true)
    {
        bool hasThisPointAlready = false;
        for(int k=0; k<pointsToHit.size(); k++)
        {
            if((pointsToHit[k].r == p.r) && (pointsToHit[k].c == p.c))
            {
                hasThisPointAlready = true;
                break;
            }
        }
        for(int k=0; k<usedPoints.size(); k++)
        {
            if((usedPoints[k].r == p.r) && (usedPoints[k].c == p.c))
            {
                hasThisPointAlready = true;
                break;
            }
        }
        if((shotHit == false) && (hasThisPointAlready == false))
        {
            usedPoints.push_back(p);
        }
        
        if(shotHit == true)
        {
            for(int j= -1; j<2; j+=2)
            {
                for(int i=1; i<5; i++)
                {
                    Point q;
                    q.r = (p.r-(i*j));
                    q.c = p.c;
                    bool hasThisPointAlready = false;
                    for(int k=0; k<pointsToHit.size(); k++)
                    {
                        if((pointsToHit[k].r == q.r) && (pointsToHit[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    for(int k=0; k<usedPoints.size(); k++)
                    {
                        if((usedPoints[k].r == q.r) && (usedPoints[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    if((game().isValid(q)) && (hasThisPointAlready == false))
                        pointsToHit.push_back(q);
                }
            }
            for(int j= -1; j<2; j+=2)
            {
                for(int i=1; i<5; i++)
                {
                    Point q;
                    q.c = (p.c-(i*j));
                    q.r = p.r;
                    bool hasThisPointAlready = false;
                    for(int k=0; k<pointsToHit.size(); k++)
                    {
                        if((pointsToHit[k].r == q.r) && (pointsToHit[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                        }
                    }
                    for(int k=0; k<usedPoints.size(); k++)
                    {
                        if((usedPoints[k].r == q.r) && (usedPoints[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    if((game().isValid(q)) && (hasThisPointAlready == false))
                        pointsToHit.push_back(q);
                }
            }
            
            if((inStateOne == true) && (shipDestroyed == true))
            {
                inStateOne = true;
            }
            else
            {
                inStateOne = false;
                if(shipDestroyed == true)
                {
                    destroyedShips.push_back(shipId);
                    while(pointsToHit.empty() == false)
                    {
                        pointsToHit.pop_back();
                    }
                    inStateOne = true;
                }
            }
        }
    }
    
}

void MediocrePlayer::recordAttackByOpponent(Point p)
{
    cerr << "lol dont need to do this" << endl;
}


// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.
//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game &g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool placeShipsRecursion(Board &b, int shipID, int zeroeth, bool& submarinePlaced);
    Point theGivenPoint(int counter);
    vector<Point> combinedRowsAndCols(vector<int> rows, vector<int> cols);
    vector<int> orderRows();
    vector<int> orderCols();
private:
    vector<Point> ShipCoords;
    vector<Point> allShipPoints;
    vector<Direction> ShipDirections;
    vector<Point> usedPoints;
    vector<int> destroyedShips;
    vector<Point> pointsToHit;
    vector<Point> orderOfPoints;
    vector<Point> priority;
    vector<int> orderofRows;
    vector<int> orderofCols;
    bool inStateOne;
    int counter;
};

GoodPlayer::GoodPlayer(string nm, const Game &g)
:Player(nm,g)
{
    inStateOne = true;
    Point temp;
    counter = 0;
    orderofRows = orderRows();
    orderofCols = orderCols();
    orderOfPoints = combinedRowsAndCols(orderofRows, orderofCols);
    
}

vector<int> GoodPlayer::orderRows()
{
    vector<int> orderofRow;
    orderofRow.push_back(5);
    orderofRow.push_back(4);
    orderofRow.push_back(5);
    orderofRow.push_back(4);
    orderofRow.push_back(5);
    orderofRow.push_back(4);
    orderofRow.push_back(9);
    orderofRow.push_back(8);
    orderofRow.push_back(9);
    orderofRow.push_back(8);
    orderofRow.push_back(9);
    orderofRow.push_back(8);
    orderofRow.push_back(1);
    orderofRow.push_back(0);
    orderofRow.push_back(1);
    orderofRow.push_back(0);
    orderofRow.push_back(1);
    orderofRow.push_back(0);
    
    
    orderofRow.push_back(0);
    orderofRow.push_back(0);
    orderofRow.push_back(1);
    orderofRow.push_back(1);
    orderofRow.push_back(2);
    orderofRow.push_back(2);
    orderofRow.push_back(2);
    orderofRow.push_back(2);
    orderofRow.push_back(3);
    orderofRow.push_back(3);
    orderofRow.push_back(3);
    orderofRow.push_back(4);
    orderofRow.push_back(4);
    orderofRow.push_back(5);
    orderofRow.push_back(5);
    orderofRow.push_back(6);
    orderofRow.push_back(6);
    orderofRow.push_back(6);
    orderofRow.push_back(6);
    orderofRow.push_back(7);
    orderofRow.push_back(7);
    orderofRow.push_back(7);
    orderofRow.push_back(8);
    orderofRow.push_back(8);
    orderofRow.push_back(9);
    orderofRow.push_back(9);
    orderofRow.push_back(3);
    orderofRow.push_back(7);
    orderofRow.push_back(2);
    orderofRow.push_back(3);
    orderofRow.push_back(7);
    orderofRow.push_back(6);

    return orderofRow;
}

vector<int> GoodPlayer::orderCols()
{
    vector<int> orderofCol;
    orderofCol.push_back(4);
    orderofCol.push_back(5);
    orderofCol.push_back(8);
    orderofCol.push_back(1);
    orderofCol.push_back(0);
    orderofCol.push_back(9);
    orderofCol.push_back(4);
    orderofCol.push_back(5);
    orderofCol.push_back(8);
    orderofCol.push_back(1);
    orderofCol.push_back(0);
    orderofCol.push_back(9);
    orderofCol.push_back(4);
    orderofCol.push_back(5);
    orderofCol.push_back(0);
    orderofCol.push_back(1);
    orderofCol.push_back(8);
    orderofCol.push_back(9);
    orderofCol.push_back(3);
    orderofCol.push_back(7);
    orderofCol.push_back(2);
    orderofCol.push_back(6);
    orderofCol.push_back(1);
    orderofCol.push_back(3);
    orderofCol.push_back(5);
    orderofCol.push_back(7);
    orderofCol.push_back(2);
    orderofCol.push_back(6);
    orderofCol.push_back(8);
    orderofCol.push_back(3);
    orderofCol.push_back(7);
    orderofCol.push_back(2);
    orderofCol.push_back(6);
    orderofCol.push_back(1);
    orderofCol.push_back(3);
    orderofCol.push_back(5);
    orderofCol.push_back(7);
    orderofCol.push_back(2);
    orderofCol.push_back(6);
    orderofCol.push_back(8);
    orderofCol.push_back(3);
    orderofCol.push_back(7);
    orderofCol.push_back(2);
    orderofCol.push_back(6);
    orderofCol.push_back(4);
    orderofCol.push_back(4);
    orderofCol.push_back(9);
    orderofCol.push_back(0);
    orderofCol.push_back(0);
    orderofCol.push_back(9);

    return orderofCol;
}


bool GoodPlayer::placeShipsRecursion(Board &b, int shipID, int zeroeth, bool& submarinePlaced)
{
    bool theReturn;
    Point temp;
    if(zeroeth == 5)
    {
        temp.r = 4;
        temp.c = 4;
        b.placeShip(temp, shipID, VERTICAL);
    }
    else if(zeroeth == 4)
    {
        temp.r = 2;
        temp.c = 0;
        b.placeShip(temp, shipID, HORIZONTAL);
    }
    else if(zeroeth == 3)
    {
        if(submarinePlaced == false)
        {
            temp.r = 5;
            temp.c = 9;
            b.placeShip(temp, shipID, VERTICAL);
            submarinePlaced = true;
        }
        else
        {
            temp.r = 0;
            temp.c = 8;
            b.placeShip(temp, shipID, VERTICAL);
        }
    }
    else if(zeroeth == 2)
    {
        temp.r = 3;
        temp.c = 5;
        b.placeShip(temp, shipID, VERTICAL);
    }
    return submarinePlaced;
}

vector<Point> GoodPlayer::combinedRowsAndCols(vector<int> rows, vector<int> cols)
{
    Point temp;
    vector<Point> orderPoints;
    for(int i=0; i<50; i++)
    {
        temp.r = rows[i];
        temp.c = cols[i];
        orderPoints.push_back(temp);
    }
    return orderPoints;
}


bool GoodPlayer::placeShips(Board &b)
{
    //Optimal arrangement of ships
    int zeroeth = game().shipLength(0);
    int first = game().shipLength(1);
    int second = game().shipLength(2);
    int third = game().shipLength(3);
    int fourth = game().shipLength(4);
    bool submarinePlaced = false;
    
    placeShipsRecursion(b, 0, zeroeth, submarinePlaced);
    placeShipsRecursion(b, 1, first, submarinePlaced);
    placeShipsRecursion(b, 2, second, submarinePlaced);
    placeShipsRecursion(b, 3, third, submarinePlaced);
    placeShipsRecursion(b, 4, fourth, submarinePlaced);

    return true;
}

Point GoodPlayer::recommendAttack()
{
    if(inStateOne)
    {
        if(orderOfPoints.empty())
        {
            bool hasARandomPoint = false;
            Point p(game().randomPoint());
            
            while(hasARandomPoint == false)
            {
                for(int i=0; i<usedPoints.size(); i++)
                {
                    if((p.r == usedPoints[i].r) && (p.c == usedPoints[i].c))
                    {
                        Point q(game().randomPoint());
                        p.r = q.r;
                        p.c = q.c;
                        i = -1;
                    }
                }
                hasARandomPoint = true;
                
            }
            usedPoints.push_back(p);
            return p;
        }
        else
        {
        Point p(orderOfPoints[counter]);
        //Check for surrounding point
        for(int i=0; i<usedPoints.size(); i++)
        {
            if((p.r == usedPoints[i].r) && (p.c == usedPoints[i].c))
            {
                for (vector<Point>::iterator ppp = orderOfPoints.begin(); ppp != orderOfPoints.end(); ppp++)
                {
                    if((ppp->r == p.r) && (ppp->c == p.c))
                    {
                        orderOfPoints.erase(ppp);
                        break;
                    }
                    
                }
                Point q(orderOfPoints[counter]);
                p.r = q.r;
                p.c = q.c;
                i = -1;
            }
        }
        usedPoints.push_back(p);
        for (vector<Point>::iterator ppp = orderOfPoints.begin(); ppp != orderOfPoints.end(); ppp++)
        {
            if((ppp->r == p.r) && (ppp->c == p.c))
            {
                orderOfPoints.erase(ppp);
                break;
            }
            
        }
        return p;
        }
    }
    else
    {
        Point q;
        int i=0;
        bool needToChooseAnotherPoint = true;
        while((i<=pointsToHit.size()-1) && (needToChooseAnotherPoint == true))
        {
            q.r = pointsToHit[i].r;
            q.c = pointsToHit[i].c;
            
            for (vector<Point>::iterator ppp = pointsToHit.begin(); ppp != pointsToHit.end(); ppp++)
            {
                if((ppp->r == q.r) && (ppp->c == q.c))
                {
                    pointsToHit.erase(ppp);
                    break;
                }
                
            }
            
            bool poop = false;
            for(int j=0; j<usedPoints.size(); j++)
            {
                if((usedPoints[j].r == q.r) && (usedPoints[j].c == q.c))
                {
                    needToChooseAnotherPoint = true;
                    poop = true;
                }
            }
            
            if(poop == false)
            {
                needToChooseAnotherPoint = false;
            }
            i++;
            
            
            
        }
        
        usedPoints.push_back(q);
        return q;
    }
    

    
    
    /*
    if(inStateOne)
    {
        Point p(orderOfPoints[counter]);
        //Check for surrounding point
        usedPoints.push_back(p);
        for (vector<Point>::iterator ppp = orderOfPoints.begin(); ppp != orderOfPoints.end(); ppp++)
        {
            if((ppp->r == p.r) && (ppp->c == p.c))
            {
                orderOfPoints.erase(ppp);
                break;
            }
            
        }
        return p;
    }
    else
    {
        Point q;
        bool CHOOSEPOINTPLZ = true;
        while(CHOOSEPOINTPLZ == true)
        {
            CHOOSEPOINTPLZ = false;
        bool needToChooseAnotherPoint = true;
        if(priority.size() > 0)
        {
            q.r = priority[priority.size()-1].r;
            q.c = priority[priority.size()-1].c;
            priority.pop_back();
            needToChooseAnotherPoint = false;
        }
        while((pointsToHit.size() >0) && (needToChooseAnotherPoint == true))
        {
            q.r = pointsToHit[pointsToHit.size()-1].r;
            q.c = pointsToHit[pointsToHit.size()-1].c;
            pointsToHit.pop_back();
            needToChooseAnotherPoint = false;
        }
        
        
        
        
            
        for(int j=0; j<usedPoints.size(); j++)
        {
            if((usedPoints[j].r == q.r) && (usedPoints[j].c == q.c))
            {
                CHOOSEPOINTPLZ = true;
            }
        }
            
            
            
    }
        
        usedPoints.push_back(q);
        return q;
    }
    */
    //To get individual elements for each coord
    /*for(int i=0; i<ShipCoords.size(); i++)
     {
     for(int j=0; j<game().shipLength(i); j++)
     {
     Point temp;
     temp.r = ShipCoords[i].r+j;
     temp.c = ShipCoords[i].c+j;
     if((p.r == temp.r) && (p.c = temp.c))
     {
     hasARandomPoint = true;
     }
     }
     
     if((p.r == ShipCoords[i].r) && (p.c == ShipCoords[i].c))
     {
     p = game().randomPoint();
     }
     }*/
    // }
    // return p;
    
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    if(validShot == true)
    {
        bool hasThisPointAlready = false;
        for(int k=0; k<pointsToHit.size(); k++)
        {
            if((pointsToHit[k].r == p.r) && (pointsToHit[k].c == p.c))
            {
                hasThisPointAlready = true;
                break;
            }
        }
        for(int k=0; k<usedPoints.size(); k++)
        {
            if((usedPoints[k].r == p.r) && (usedPoints[k].c == p.c))
            {
                hasThisPointAlready = true;
                break;
            }
        }
        if((shotHit == false) && (hasThisPointAlready == false))
        {
            usedPoints.push_back(p);
        }
        
        if(shotHit == true)
        {
            for(int j= -1; j<2; j+=2)
            {
                for(int i=1; i<5; i++)
                {
                    Point q;
                    q.r = (p.r-(i*j));
                    q.c = p.c;
                    bool hasThisPointAlready = false;
                    for(int k=0; k<pointsToHit.size(); k++)
                    {
                        if((pointsToHit[k].r == q.r) && (pointsToHit[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    for(int k=0; k<usedPoints.size(); k++)
                    {
                        if((usedPoints[k].r == q.r) && (usedPoints[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    if((game().isValid(q)) && (hasThisPointAlready == false))
                        pointsToHit.push_back(q);
                }
            }
            for(int j= -1; j<2; j+=2)
            {
                for(int i=1; i<5; i++)
                {
                    Point q;
                    q.c = (p.c-(i*j));
                    q.r = p.r;
                    bool hasThisPointAlready = false;
                    for(int k=0; k<pointsToHit.size(); k++)
                    {
                        if((pointsToHit[k].r == q.r) && (pointsToHit[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                        }
                    }
                    for(int k=0; k<usedPoints.size(); k++)
                    {
                        if((usedPoints[k].r == q.r) && (usedPoints[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    if((game().isValid(q)) && (hasThisPointAlready == false))
                        pointsToHit.push_back(q);
                }
            }
            
            if((inStateOne == true) && (shipDestroyed == true))
            {
                inStateOne = true;
            }
            else
            {
                inStateOne = false;
                if(shipDestroyed == true)
                {
                    destroyedShips.push_back(shipId);
                    
                    inStateOne = true;
                }
            }
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
  /*  if(validShot == true)
    {
        bool hasThisPointAlready = false;
        for(int k=0; k<pointsToHit.size(); k++)
        {
            if((pointsToHit[k].r == p.r) && (pointsToHit[k].c == p.c))
            {
                hasThisPointAlready = true;
                break;
            }
        }
        for(int k=0; k<usedPoints.size(); k++)
        {
            if((usedPoints[k].r == p.r) && (usedPoints[k].c == p.c))
            {
                hasThisPointAlready = true;
                break;
            }
        }
        if((shotHit == false) && (hasThisPointAlready == false))
        {
            usedPoints.push_back(p);
        }
        
        if(shotHit == true)
        {
            if((inStateOne == true) && (shipDestroyed == true))
            {
                inStateOne = true;
            }
            else if(inStateOne)
            {
                Point q;
               
                q.r = (p.r+1);
                q.c = p.c;
                if(game().isValid(q))
                {
                    bool hasThisPointAlready = false;
                    for(int k=0; k<pointsToHit.size(); k++)
                    {
                        if((pointsToHit[k].r == q.r) && (pointsToHit[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    for(int k=0; k<usedPoints.size(); k++)
                    {
                        if((usedPoints[k].r == q.r) && (usedPoints[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    if(hasThisPointAlready == true)
                        priority.push_back(q);
                    if((game().isValid(q)) && (hasThisPointAlready == false))
                        pointsToHit.push_back(q);
                }
        
                q.r = (p.r-1);
                q.c = p.c;
                if(game().isValid(q))
                {
                    bool hasThisPointAlready = false;
                    for(int k=0; k<pointsToHit.size(); k++)
                    {
                        if((pointsToHit[k].r == q.r) && (pointsToHit[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    for(int k=0; k<usedPoints.size(); k++)
                    {
                        if((usedPoints[k].r == q.r) && (usedPoints[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    if(hasThisPointAlready == true)
                        priority.push_back(q);
                    if((game().isValid(q)) && (hasThisPointAlready == false))
                        pointsToHit.push_back(q);
                }
            
            
                q.r = (p.r-1);
                q.c = p.c-1;
                if(game().isValid(q))
                {
                    bool hasThisPointAlready = false;
                    for(int k=0; k<pointsToHit.size(); k++)
                    {
                        if((pointsToHit[k].r == q.r) && (pointsToHit[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    for(int k=0; k<usedPoints.size(); k++)
                    {
                        if((usedPoints[k].r == q.r) && (usedPoints[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    if(hasThisPointAlready == true)
                        priority.push_back(q);
                    if((game().isValid(q)) && (hasThisPointAlready == false))
                        pointsToHit.push_back(q);
                }
                
                q.r = (p.r);
                q.c = p.c+1;
                if(game().isValid(q))
                {
                    bool hasThisPointAlready = false;
                    for(int k=0; k<pointsToHit.size(); k++)
                    {
                        if((pointsToHit[k].r == q.r) && (pointsToHit[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    for(int k=0; k<usedPoints.size(); k++)
                    {
                        if((usedPoints[k].r == q.r) && (usedPoints[k].c == q.c))
                        {
                            hasThisPointAlready = true;
                            break;
                        }
                    }
                    if(hasThisPointAlready == true)
                        priority.push_back(q);
                    if((game().isValid(q)) && (hasThisPointAlready == false))
                        pointsToHit.push_back(q);
                }
                inStateOne =false;
            }
            else
            {
                inStateOne = false;
                if(shipDestroyed == true)
                {
                    destroyedShips.push_back(shipId);
                    while(pointsToHit.empty() == false)
                    {
                        pointsToHit.pop_back();
                    }
                    inStateOne = true;
                }
            }
        }
    }
    */
}

void GoodPlayer::recordAttackByOpponent(Point p)
{
    cerr << "lol dont need to do this" << endl;
}


//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}

