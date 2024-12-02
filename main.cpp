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

// Environment Structure
struct Environment {
    vector<Card> drawPile;
    vector<Card> discardPile;
    int currentPlayer;
    int direction;
};

// Function Prototypes
void initializeDeck(vector<Card> &deck);
Card drawCard(Environment &env);
void dealer(Environment &env, vector<Player> &players, int numCards);
void displayHand(const vector<Card> &hand);
bool playTurn(Player &player, Environment &env);
void shuffle(vector<Card> &deck);

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
    initializeDeck(env.drawPile);
    
    // CALL shuffleDeck
    shuffle(env.drawPile);

    // Initialize players and deal cards
    for (int i = 0; i < numPlayers; ++i) {
        Player player;
        cout << "Enter name for player " << i + 1 << ": ";
        cin >> player.name;
        players.push_back(player);
    }
    dealer(env, players, numCards);

    // Set starting card for discard pile
    env.discardPile.push_back(drawCard(env));
    env.currentPlayer = 0;
    env.direction = 1;

    // Game loop
    bool gameOver = false;

    while (!gameOver) {
        Player& currentPlayer = players[env.currentPlayer];
        Card& topCard = env.discardPile.back();

        // Display current player, top card, and hand
        cout << endl << currentPlayer.name << "'s turn!" << endl;
        cout << "Top Card: " << topCard.color << " " << topCard.number << endl;
        displayHand(currentPlayer.hand);

        if (playTurn(currentPlayer, env)) {
            cout << currentPlayer.name << " played a card!" << endl;
        } else {
            cout << currentPlayer.name << " has to draw a card." << endl;
            currentPlayer.hand.push_back(drawCard(env));
        }

        // Check for winner
        if (currentPlayer.hand.empty()) {
            cout << currentPlayer.name << " wins!!!" << endl;
            gameOver = true;
        }

        // Move to next player
        env.currentPlayer = (env.currentPlayer + env.direction + players.size())
                             % players.size();
    }

    return 0;
}

/*******************************************************************************
 * initializeDeck
 * ___________________________________________________________________________
 *
 * This function will initialize the deck of cards that the player's will be
 * utilizing. It will initialize the colors and numbers of each card.
 *  - colors[] // holds colors
 *  - Card card // temp object
 *  - deck.push_back(card) // card added to vector
 ******************************************************************************/
void initializeDeck(vector<Card>& deck) {
    const int elements = 4; // number of colors
    const int maxNum = 9;   // Cards can only go to 9
    string colors[elements] = {"Red", "Yellow", "Green", "Blue"};
    
    // Loop to add cards
    for (int i = 0; i < elements; ++i) { 
        for (int j = 0; j <= maxNum; ++j) {  // Iterate over the numbers 0 to 9
            Card card;
            card.color = colors[i];  // Access the color using the index
            card.number = to_string(j);  // Convert the number to a string
            deck.push_back(card);
        }
        
        // For Draw Two, Kip, and Reverse Cards
        for (int k = 0; k < 2; ++k){
            Card drawTwo, skip, reverse;
        
            // Draw Two
            drawTwo.color = colors[i];
            drawTwo.number = "Draw Two";
            deck.push_back(drawTwo);
        
            // Skip
            drawTwo.color = colors[i];
            drawTwo.number = "Skip";
            deck.push_back(skip);
        
            // Reverse
            drawTwo.color = colors[i];
            drawTwo.number = "Reverse";
            deck.push_back(reverse);
        }
    }
    
    // For Wild card
    for(int i = 0; i < 4; ++i){
        Card wildCard;
        wildCard.color = "Wild";
        wildCard.number = "Card";
        deck.push_back(wildCard);
    }
    
    // For Wild Draw card
    for(int i = 0; i < 4; ++i){
        Card wildDrawFour;
        wildDrawFour.color = "Wild";
        wildDrawFour.number = "Draw Four";
        deck.push_back(wildDrawFour);
    }
}

/*******************************************************************************
 * drawCard
 * ___________________________________________________________________________
 *
 * This function will handle the draw card occurrence in Uno, essentially
 * taking the top card of the Draw Pile.
 *  - Card drawnCard // stores the last element or the "top card"
 *  - env.drawPile.pop_back // will remove the drawn card from the Draw Pile
 ******************************************************************************/
Card drawCard(Environment &env) {
    Card drawnCard = env.drawPile.back();
    env.drawPile.pop_back();
    return drawnCard;
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
            players[j].hand.push_back(drawCard(env));
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
    Card& topCard = env.discardPile.back();
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
            env.discardPile.push_back(selectedCard);

            // Remove the card from the player's hand
            player.hand.erase(player.hand.begin() + (choice - 1));
            return true;
        }
        
        // Test to see if chosen card matches top card in any form
        if (selectedCard.color == topCard.color 
            || selectedCard.number == topCard.number) {
            env.discardPile.push_back(selectedCard);
            player.hand.erase(player.hand.begin() + (choice - 1));
            return true;
        } else {
            // Error prompt
            cout << "Invalid match! Try Again." << endl;
            return playTurn(player, env);
        }
    }
}


/*******************************************************************************
 * shuffle
 * ___________________________________________________________________________
 *
 * This function will shuffle the elements in the initialize deck array to make
 * sure the cards are truly random.
 ******************************************************************************/
void shuffle(vector<Card> &deck){
    // Loop to swap cards
    for(int i = deck.size() - 1; i > 0; --i){
        int j = rand() % (i + 1); // Random number gen for rand index
        // Swaps elements
        swap(deck[i], deck[j]);
    }
}
