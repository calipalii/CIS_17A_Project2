// Calista Ruiz	                   CSC17A                              Project 1    
/*******************************************************************************
 * UNO
 * ___________________________________________________________________________
 *
 * RULES: The game Uno allows 2-10 players starting with 7 cards each. The rest
 * of the cards are placed in a Draw Pile. There should also be a Discard Pile.
 * One card will be put in the Discard Pile and players will play based off that
 * card. The first player is whoever is designated as number one, players 
 * following chronologically. A match can only be made if a player has a card 
 * with either: the same color or the same number. If the player 
 * has no matches, they must pick up a card from the Draw Pile. (For purposes of 
 * this project it will not allow the picked up card to be played). There will
 * be five special cards circulated through the game, including a Wild Draw 4
 * card, a Wild card, a Draw Two card, a Skip card, and a Reverse card.
 * _____________________________________________________________________________
 ******************************************************************************/
#include "libraries.h"

using namespace std;

// Card Structure
struct Card {
    string color;
    string number;
};

// Player Structure
struct Player {
    vector<Card> hand;
    string name;
};

// Environment Class
class Environment {
private:
    vector<Card> drawPile;
    vector<Card> discardPile;
    int currentPlayer;
    int direction;

public:
    // Constructor
    Environment() : currentPlayer(0), direction(1) {}

    /***************************************************************************
    * drawCard
    * __________________________________________________________________________
    *
    * This function will handle the draw card occurrence in Uno, essentially
    * taking the top card of the Draw Pile.
    *  - Card drawnCard // stores the last element or the "top card"
    *  - env.drawPile.pop_back // will remove the drawn card from the Draw Pile
    ***************************************************************************/
    Card drawCard() {
        if (drawPile.empty()) {
            throw runtime_error("The draw pile is empty!");
        }
        Card drawnCard = drawPile.back();
        drawPile.pop_back();
        return drawnCard;
    }

    // Add a card to the discard pile
    void addToDiscardPile(const Card& card) {
        discardPile.push_back(card);
    }

    // Get the top card of the discard pile
    Card& getTopDiscard() {
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
    void initializeDeck() {
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
    void shuffleDeck() {
        for (int i = drawPile.size() - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            swap(drawPile[i], drawPile[j]);
        }
    }

    // Advance to the next player
    void advancePlayer(int numPlayers) {
        currentPlayer = (currentPlayer + direction + numPlayers) % numPlayers;
    }

    // Reverse the direction of play
    void reverseDirection() {
        direction *= -1;
    }

    // Get the current player index
    int getCurrentPlayerIndex() const {
        return currentPlayer;
    }
}; 

// Function Prototypes
//void initializeDeck(vector<Card> &deck);
//Card drawCard(Environment &env);
void dealer(Environment &env, vector<Player> &players, int numCards);
void displayHand(const vector<Card> &hand);
bool playTurn(Player &player, Environment &env);
//void shuffle(vector<Card> &deck);

// Main Function
int main() {
    srand(static_cast<unsigned int>(time(0)));

    Environment env;
    vector<Player> players;
    int numPlayers;
    int numCards = 7;  // Each player starts with 7 cards

    // Number of players
    cout << "Number of players (2-10): ";
    cin >> numPlayers;
    while (numPlayers < 2 || numPlayers > 10) {
        cout << "INVALID! Must be between 2 and 10: ";
        cin >> numPlayers;
    }

    // CALL initializeDeck
    env.initializeDeck();
    
    // CALL shuffleDeck
    env.shuffleDeck();

    // Initialize players and deal cards
    for (int i = 0; i < numPlayers; ++i) {
        Player player;
        cout << "Enter name for player " << i + 1 << ": ";
        cin >> player.name;
        players.push_back(player);
    }
    dealer(env, players, numCards);

    // Set starting card for discard pile
    env.addToDiscardPile(env.drawCard());

    // Game loop
    bool gameOver = false;

    while (!gameOver) {
        Player& currentPlayer = players[env.getCurrentPlayerIndex()];
        Card& topCard = env.getTopDiscard();

        // Display current player, top card, and hand
        cout << endl << currentPlayer.name << "'s turn!" << endl;
        cout << "Top Card: " << topCard.color << " " << topCard.number << endl;
        displayHand(currentPlayer.hand);

        if (playTurn(currentPlayer, env)) {
            cout << currentPlayer.name << " played a card!" << endl;
        } else {
            cout << currentPlayer.name << " has to draw a card." << endl;
            currentPlayer.hand.push_back(env.drawCard());
        }

        // Check for winner
        if (currentPlayer.hand.empty()) {
            cout << currentPlayer.name << " wins!!!" << endl;
            gameOver = true;
        }

        // Move to next player
        env.advancePlayer(players.size());
    }

    return 0;
}

/*******************************************************************************
 * dealer
 * ___________________________________________________________________________
 *
 * This function will deal seven cards to each of the participating players.
 *  - Player &player : players // accesses the ve
 ******************************************************************************/
void dealer(Environment &env, vector<Player> &players, int numCards) {
    for (int i = 0; i < numCards; ++i) {
        // Traditional for loop to iterate through players
        for (int j = 0; j < players.size(); ++j) {
            players[j].hand.push_back(env.drawCard());
        }
    }
}


/*******************************************************************************
 * displayHand
 * ___________________________________________________________________________
 *
 * This function will display the current player's hand so they can choose what
 * card they would like to play.
 *  - hand[i].color  // displays the current color element in the loop
 *  - hand[i].number // displays the current number in the loop
 ******************************************************************************/
// FUNCTION - Displays the player's hand
void displayHand(const vector<Card> &hand) {
    for (int i = 0; i < hand.size(); ++i) {
        cout << i + 1 << ". " << hand[i].color << " " << hand[i].number << endl;
    }
    cout << endl;
}

/*******************************************************************************
 * playTurn
 * __________________________________________________________________________
 * 
 * This function will allow the player to choose what card they will like to
 * play. It will make sure that the chosen card matches with the top card,
 * otherwise an error message will appear.  
 ******************************************************************************/
bool playTurn(Player& player, Environment& env) {
    Card& topCard = env.getTopDiscard();
    int choice;

    // Card selection prompt
    cout << "Select a card to play or enter 0 to draw a card: ";
    cin >> choice;

    if (choice == 0) {
        return false;
    } else if (choice > 0 && choice <= player.hand.size()) {
        Card selectedCard = player.hand[choice - 1];

        // If the card is a Wild Card
        if (selectedCard.number == "Card") {
            string newColor;
            cout << "You played a Wild Card! Choose a new color ";
            cout << "(Red, Yellow, Green, Blue): ";
            cin >> newColor;

            // Ensure the chosen color is valid
            while (newColor != "Red" && newColor != "Yellow" 
                   && newColor != "Green" && newColor != "Blue") {
                cout << "Invalid color. Choose a new color ";
                cout << "(Red, Yellow, Green, Blue): ";
                cin >> newColor;
            }

            // Set the new color for the Wild Card and add to discard pile
            selectedCard.color = newColor;
            env.addToDiscardPile(selectedCard);

            // Remove the card from the player's hand
            player.hand.erase(player.hand.begin() + (choice - 1));
            return true;
        }
        
        // Test to see if chosen card matches top card in any form
        if (selectedCard.color == topCard.color 
            || selectedCard.number == topCard.number) {
            env.addToDiscardPile(selectedCard);
            player.hand.erase(player.hand.begin() + (choice - 1));
            return true;
        } else {
            // Error prompt
            cout << "Invalid match! Try Again." << endl;
            return playTurn(player, env);
        }
    }
}