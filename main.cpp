// Calista Ruiz	                   CSC17A                              Project 2    
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
#include "Environment.h"

// Function Prototypes
void dealer(Environment &env, vector<Player<string>> &players, int numCards);
void displayHand(const vector<Card> &hand);
bool playTurn(Player<string> &player, Environment &env);

// Main Function
int main() {
    srand(static_cast<unsigned int>(time(0)));

    Environment env;
    vector<Player<string>> players;
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
        Player<string> player;
        cout << "Enter name for player " << i + 1 << ": ";
        cin >> player.name;
        players.push_back(player);
    }
    
    dealer(env, players, numCards);

    // Ensures a wild card or a wild draw four will not be the first card
    Card initialCard;
    do{
        initialCard = env.drawCard();
    } while(initialCard.color == "Wild");
        
    // Set starting card for discard pile
    env.addToDiscardPile(env.drawCard());
    
    // Game loop
    bool gameOver = false;

    while (!gameOver) {
        Player<string>& currentPlayer = players[env.getCurrentPlayerIndex()];
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
void dealer(Environment &env, vector<Player<string>> &players, int numCards) {
    
    // Outer loop to iterate over the number of cards to be dealt
    for (int i = 0; i < numCards; ++i) {
        
        // Loop through each player to give them a card
        for (int j = 0; j < players.size(); ++j) {
            
            // Inform that the card is being dealt to the player
            cout << "Dealing card " << i + 1 << " to player ";
            cout << players[j].name << endl;
            
            // Draw a card from the environment and add it to the player's hand
            Card drawnCard = env.drawCard(); // Store the drawn card temporarily
            
            // Debugging: Show the card drawn
            cout << "Player " << players[j].name << " drew a card: "
                 << drawnCard.color << " " << drawnCard.number << endl;
            
            // Add the card to the player's hand
            players[j].hand.push_back(drawnCard);

            // Print the player's hand after the card has been dealt
            cout << "Player " << players[j].name << "'s hand now contains: ";
            
            // Loop through the player's hand to display the cards
            for (int k = 0; k < players[j].hand.size(); ++k) {
                cout << players[j].hand[k].color << " ";
                cout << players[j].hand[k].number << " ";
            }
            
            cout << endl;

        }
        
        cout << "Round " << i + 1 << " complete. Moving to the next round.";
        cout << endl;
        
    }
    
    cout << "All cards have been dealt!" << endl;

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
void displayHand(const vector<Card> &hand) {
    // Start a loop to iterate through each card in the hand
    for (int i = 0; i < hand.size(); ++i) {
        // Output the card's index, starting at 1
        cout << i + 1;
        
        // Print the period and space after the index
        cout << ". ";
        
        // Output the color of the current card
        cout << hand[i].color;
        
        // Print a space between the color and number
        cout << " ";
        
        // Output the number or special type of the current card
        cout << hand[i].number;
        
        // Move to the next line for the next card
        cout << endl;
    }

    // Print a blank line after displaying all cards in the hand
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
bool playTurn(Player<string>& player, Environment& env) {
    Card& topCard = env.getTopDiscard();
    int choice;

    // Card selection prompt
    cout << "Select a card to play or enter 0 to draw a card: ";
    cin >> choice;

    if (choice == 0) {
        return false;
    } else if (choice > 0 && choice <= player.hand.size()) {
        Card selectedCard = player.hand[choice - 1];

        // Convert top card to upper
        string topCardColor = topCard.color;
        for (char &c : topCardColor){
            c = toupper(c);
        }
        
        // If the card is a Wild Card
        if (selectedCard.number == "Draw Four" || 
            selectedCard.number == "Card") {
            cout << "You played a " << selectedCard.number;
            cout << ". Choose a new color! ";
            
            string newColor;
            cout << "(Red, Yellow, Green, Blue): ";
            cin >> newColor;
            
            // Convert input to uppercase
            for (char &c : newColor){
                c = toupper(c);
            }

            // Ensure the chosen color is valid
            while (newColor != "RED" && newColor != "YELLOW" 
                   && newColor != "GREEN" && newColor != "BLUE") {
                cout << "Invalid color. Choose a new color ";
                cout << "(Red, Yellow, Green, Blue): ";
                cin >> newColor;
                
                // Convert input to uppercase
                for (char &c : newColor){
                    c = toupper(c);
                }
            }
            
            selectedCard.color = newColor;
            env.addToDiscardPile(selectedCard);
            player.hand.erase(player.hand.begin() + (choice - 1));

            // Wild Draw Four adds 4 cards to the next player
            if (selectedCard.number == "Wild Draw Four") {
                cout << "Next player draws 4 cards!" << endl;
                env.advancePlayer(env.getCurrentPlayerIndex());
                Player<string>& nextPlayer = player; // Access next player
                for (int i = 0; i < 4; ++i) {
                    nextPlayer.hand.push_back(env.drawCard());
                }
            }

            return true;
        }

        // If the card is Draw Two
        if (selectedCard.number == "Draw Two" && 
           (selectedCard.color == topCard.color || 
            selectedCard.number == topCard.number)) {
            
            env.addToDiscardPile(selectedCard);
            player.hand.erase(player.hand.begin() + (choice - 1));

            cout << "Next player draws 2 cards!" << endl;
            env.advancePlayer(env.getCurrentPlayerIndex());
            Player<string>& nextPlayer = player; // Access next player
            for (int i = 0; i < 2; ++i) {
                nextPlayer.hand.push_back(env.drawCard());
            }

            return true;
        }

        // If the card is Skip
        if (selectedCard.number == "Skip" && 
           (selectedCard.color == topCard.color 
           || selectedCard.number == topCard.number)){
            
            env.addToDiscardPile(selectedCard);
            player.hand.erase(player.hand.begin() + (choice - 1));

            cout << "Next player is skipped!" << endl;
            env.advancePlayer(2); // Skip the next player

            return true;
        }

        // If the card is Reverse
        if (selectedCard.number == "Reverse" && 
           (selectedCard.color == topCard.color 
           || selectedCard.number == topCard.number)){
            
            env.addToDiscardPile(selectedCard);
            player.hand.erase(player.hand.begin() + (choice - 1));

            cout << "Play direction reversed!" << endl;
            env.reverseDirection();

            return true;
        }
        
        // If the card matches by color or number
        if (selectedCard.color == topCard.color 
           || selectedCard.number == topCard.number) {
            
            env.addToDiscardPile(selectedCard);
            player.hand.erase(player.hand.begin() + (choice - 1));
            return true;
        } else {
            // Error prompt
            cout << "Invalid match! Try again." << endl;
            return playTurn(player, env);
        }
    }
    
    return false;
}