#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
using namespace std;

// Card Structure
struct Card{
    string color;
    string number;
};

// Player Structure
template <typename T>
struct Player {
    vector<Card> hand;
    T name;
};

//Environment Class
class Environment{
private:
    vector<Card> drawPile;
    vector<Card> discardPile;
    int currentPlayer;
    int direction;

public:
    // Constructor
    Environment();
    
    // Friend Function
    friend void displayDecks(const Environment& env);
    
    // Functions
    Card drawCard();
    void addToDiscardPile(const Card& card);
    Card& getTopDiscard();
    void initializeDeck();
    void shuffleDeck();
    void advancePlayer(int numPlayers);
    void reverseDirection();
    int getCurrentPlayerIndex() const;
};