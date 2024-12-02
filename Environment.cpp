#include "Environment.h"

// Constructor
Environment::Environment() : currentPlayer(0), direction(1) {}
    
// Friend Function being implemented
void displayDecks(const Environment& env){
    cout << "Draw Pile: " << endl;
    for (const auto& card : env.drawPile){
        cout << card.color << " " << card.number << endl;
    }
    
    cout << "Discard Pile: " << endl;
    for (const auto& card : env.discardPile){
        cout << card.color << " " << card.number << endl;
    }
}

/***************************************************************************
* drawCard
* __________________________________________________________________________
*
* This function will handle the draw card occurrence in Uno, essentially
* taking the top card of the Draw Pile.
*  - Card drawnCard // stores the last element or the "top card"
*  - env.drawPile.pop_back // will remove the drawn card from the Draw Pile
***************************************************************************/
Card Environment::drawCard() {
    if (drawPile.empty()) {
        throw runtime_error("The draw pile is empty!");
    }
    Card drawnCard = drawPile.back();
    drawPile.pop_back();
    return drawnCard;
}

/***************************************************************************
* addToDiscardPile
* ___________________________________________________________________________
*
* This function will allow the chosen card to be added to the discardPile
* vector.
***************************************************************************/
void Environment::addToDiscardPile(const Card& card) {
    discardPile.push_back(card);
}

/***************************************************************************
* addToDiscardPile
* ___________________________________________________________________________
*
* This function will allow access to the top card of the discardPile
* vector.
***************************************************************************/
Card& Environment::getTopDiscard() {
    if (discardPile.empty()) {
        throw runtime_error("The discard pile is empty!");
    }
    return discardPile.back();
}

/***************************************************************************
* initializeDeck
* __________________________________________________________________________
*
* This function will initialize the deck of cards that the player's will be
* utilizing. It will initialize the colors and numbers of each card.
*  - colors[] // holds colors
*  - Card card // temp object
*  - deck.push_back(card) // card added to vector
***************************************************************************/
void Environment::initializeDeck() {
    const int elements = 4;  // Number of colors
    const int maxNum = 9;    // Cards can only go up to 9
    string colors[elements] = {"Red", "Yellow", "Green", "Blue"};

    for (int i = 0; i < elements; ++i) {
        for (int j = 0; j <= maxNum; ++j) {
            Card card;
            card.color = colors[i];
            card.number = to_string(j);
            drawPile.push_back(card);
        }
        for (int k = 0; k < 2; ++k) {
            drawPile.push_back(Card{colors[i], "Draw Two"});
            drawPile.push_back(Card{colors[i], "Skip"});
            drawPile.push_back(Card{colors[i], "Reverse"});
        }
    }
    for (int i = 0; i < 4; ++i) {
        drawPile.push_back(Card{"Wild", "Card"});
        drawPile.push_back(Card{"Wild", "Draw Four"});
    }
}
    
/***************************************************************************
* shuffle
* __________________________________________________________________________
*
* This function will shuffle the elements in the initialize deck array to
* make sure the cards are truly random.
***************************************************************************/
void Environment::shuffleDeck() {
    for (int i = drawPile.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(drawPile[i], drawPile[j]);
    }
}

/***************************************************************************
* advancePlayer
* ___________________________________________________________________________
*
* This function will allow the movement to the next element in the
* numPlayers.
***************************************************************************/
void Environment::advancePlayer(int numPlayers) {
    currentPlayer = (currentPlayer + direction + numPlayers) % numPlayers;
}

/***************************************************************************
* reverseDirection
* ___________________________________________________________________________
*
* This function will allow the reverse card to be used effectively and will
* cause the flow of the game to go backwards in the vector.
***************************************************************************/
void Environment::reverseDirection() {
    direction *= -1;
}

/***************************************************************************
* getCurrentPlayerIndex
* ___________________________________________________________________________
*
* This function allows the ability to access the element correspondence
* with any current player.
***************************************************************************/
int Environment::getCurrentPlayerIndex() const {
    return currentPlayer;
}