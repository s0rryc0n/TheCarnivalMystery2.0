//Ethan Nguyen, CS G179
//Carnival Mystery Game

//-----------------------+
//---------------------+ |
//SECTION 1: Libraries | |
//---------------------+ |
//-----------------------+

#include <chrono>                              
#include <cstdlib>                          
#include <ctime>                                 
#include <iomanip>                               
#include <iostream>                            
#include <set>                                      
#include <string>                                  
#include <thread>                                    
#include <vector>                                 
#include <algorithm>                             
#include <map>                                        
using namespace std;
using namespace std::chrono;

//-------------------------------+
//-----------------------------+ |
//SECTION 2: Classes / Structs | |
//-----------------------------+ |
//-------------------------------+

//2.1 - Base Class representing a general location at the carnival
class Location                                        
{
public:
    string name;                                      
    string description;                              

    Location() : name(""), description("") {}        
                                       
    Location(string aName, string aDescription)      
    {
        name = aName;
        description = aDescription;
    }
};

//2.2 - Subclass representing a game booth
class GameBooth : public Location                   
{
public:
    int tokensRewarded;                          
    int winCount = 0;                              

    GameBooth(string aName, string aDescription,     
        int reward) : Location(aName, aDescription)
    {
        tokensRewarded = reward;
    }
};

//2.3 - Subclass representing a food stand
class FoodStand : public Location                
{
public:
    FoodStand(string aName, string aDescription)     
        : Location(aName, aDescription) {}
};

//2.4 - Subclass representing a ride
class Ride : public Location                         
{
public:
    int cost;                                         

    Ride() : Location(), cost(0) {}                   

    Ride(string aName, string aDescription, int aCost)  
        : Location(aName, aDescription)
    {
        cost = aCost;
    }
};

//2.5 - Struct representing a player
struct Player 
{
    //name and passcode
    string name;
    string passcode;

    //time and resources
    int clock = 720;
    int tokenCount = 0;
    int scrollCount = 0;

    //win condition related
    bool actionCompleted = false;
    bool savedGus = false;

    //other variables not used much
    int winCount = 0;
    int ride1ClueCount = 0;
    int ride2ClueCount = 0;

    //stores what the player collects
    vector<string> foodClues;
    vector<string> rideClues;
    vector<string> premiumClues;
    vector<string> items;
    map<string, pair<string, string>> contacts;

    //checks if player has item
    bool hasBasicStopwatch = false;
    bool hasPreciseStopwatch = false;
    bool hasPlayfulStopwatch = false;
    bool hasGreedyStopwatch = false;
    bool hasCompetitiveStopwatch = false;
    bool hasShield = false;
    bool hasTicket = false;

    //checks if player has contact
    bool hasAbel = false;
    bool hasValerie = false;
    bool hasJacques = false;
    bool hasMaria = false;

    //checks if player has premium clue
    bool hasDigitReveal = false;
    bool hasPositionReveal = false;
    bool hasNeighborReveal = false;
    bool hasOrderReveal = false;
    bool hasSpaceReveal = false;

    //checks if player has food clue
    bool hasFoodClue1 = false;
    bool hasFoodClue2 = false;
    bool hasFoodClue3 = false;
    bool hasFoodClue4 = false;
    bool hasFoodClue5 = false;
    bool hasFoodClue6 = false;
    bool hasFoodClue7 = false;
    bool hasFoodClue8 = false;

    //checks if player solved food clue
    bool completedFoodClue1 = false;
    bool completedFoodClue2 = false;
    bool completedFoodClue3 = false;
    bool completedFoodClue4 = false;
    bool completedFoodClue5 = false;
    bool completedFoodClue6 = false;
    bool completedFoodClue7 = false;
    bool completedFoodClue8 = false;
    bool completed1WinEachGame = false;
    bool completed7WinsFor1Game = false;

    //checks if player has ride clue
    bool hasRideClue1 = false;
    bool hasRideClue2 = false;
    bool hasRideClue3 = false;
    bool hasRideClue4 = false;
    bool hasRideClue5 = false;
    bool hasRideClue6 = false;
    bool hasRideClue7 = false;
    bool hasRideClue8 = false;
    bool hasRideClue9 = false;
    bool hasRideClue10 = false;
    bool hasRideClue11 = false;
    bool hasRideClue12 = false;
    bool hasRideClue13 = false;
    bool hasRideClue14 = false;
    bool hasRideClue15 = false;
    bool hasRideClue16 = false;
};

//-------------------------------------+
//-----------------------------------+ |
//SECTION 3: Supplementary Functions | |
//-----------------------------------+ |
//-------------------------------------+

//3.1 - Validates user input given: prompt, min value, max value
int getValidInt(const string& prompt, int min, int max)
{
    int value;
    bool invalidInput = true;

    //loop happens until user input is valif
    while (invalidInput)
    {
        cout << prompt;
        cin >> value;

        //if user types letter, cin is cleared & ignored, and user is prompted to type a number
        if (cin.fail()) 
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input. Please enter a number.\n";
        }
        //if user types number of out range, user is prompted to type a number in range
        else if (value < min || value > max) 
        {
            cout << "Number is out of range. (" << min << "-" << max << ")\n";
        }
        //when the input is correct
        else 
        {
            invalidInput = false;
        }
    }
    cin.ignore();
    return value;
}

//3.2 - Prints shop items with varying colors based on if player bought it already
void printShopItem(const string& line, bool owned)
{
    if (owned)
        //prints gray color when bought
        cout << "\033[38;2;100;100;100m" << line << "\033[0m"; 
    else
        //prints white color when not bought
        cout << line; 
}

//-------------------+
//-----------------+ |
//SECTION 4: GAMES | |
//-----------------+ |
//-------------------+

//4.1 - Typing Game: player has a time limit to type out a given sentence
void playTypingGame(Player& player, GameBooth& typingGame, const string& foodClue6, int foodClue6Index, int foodClue7Index, const vector<string>& foodClueSet6)
{
    //pool of sentences
    vector<string> sentences =
    {
        "But at least you'll play this song when I'm gone", "Are you real, or are you an illusion?",
        "Figure in the corner I can't quite see", "Don't you let me down, if you let me drown",
        "Why won't you let me go? Why won't you let me leave?", "Ever since I was a kid, I been legit",
        "Long in my life, I wouldn't let love inside", "It's better when I'm by myself",
        "Just hold my heartbeat close to you", "Laisse tomber les filles", "They all feel the same",
        "Precious little diamond", "You did many things that I liked, that I liked",
        "Do you live in a pineapple under the sea or nah?", "So what do I do if I can't figure it out?",
        "You make it look like it's magic", "She asked me if I do this every day", "She's capable of anything, it's riveting",
        "She told me, \"Don't worry about it\"", "I'm so glad we're acquainted", "I only call you when it's half-past five",
        "House so empty, need a centerpiece", "Got up, thank the Lord for the day", "If it ain't XO, then it gotta go",
        "Even though we're goin' through it and it makes you feel alone", "You've been scared of love and what it did to you",
        "I claimed you so proud, and openly", "Once you put your pride aside, you can notify me",
        "What makes a grown man wanna cry?", "She pulled up to the studio", "Photoshoots, I'm a star now",
        "I feel everything from my body to my soul", "Sin City's cold and empty",
        "You lie, but I don't let it define you", "I saw you dancing in a crowded room",
        "I lied to you, I lied to you, I lied to you", "Nothing compares to the emptiness we both share",
        "I'm always ready for a war again", "I got one comin' over and one right now", "Cause he seems like he's good for you",
        "And I love it when you watch me sleep", "How do I make you love me?", "It's like a dream what she feels with me",
        "Now I can't keep you from loving him, you made up your mind", "Is there someone else or not?",
        "I don't wanna know, if you're playin' me, keep it on the low", "Never be free 'cause she popular", "Temperature risin'",
        "I'd rather spare you the details, my love", "Give me tough love", "It's like eviction number four now",
        "You're the only one I've got my sights on", "But you still hesitate, 'cause you choose loyalty",
        "All that money, the money is the motive", "Hot like risin' sun", "Cause they loving the crew", "It's indescribable",
        "This time I might not make it", "And the Paris girls, they sing my song with love", "Underrated is an understatement",
        "Your hesitation speaks to me louder than million words per minute"
    };

    //sets time limit
    int timeLimit = 15;

    //instructions
    cout << "\nGOAL: Type out the sentence that will appear below."
        << "\nTIME LIMIT: " << timeLimit << " seconds"
        << "\n\nPress ENTER to start...";

    //press enter when player is ready
    string input;
    getline(cin, input);

    //generates random sentence
    string sentence = sentences[rand() % sentences.size()];
    cout << "\nTYPE: " << sentence << "\n\n";

    //takes in user input while calculating how long player took
    auto start = steady_clock::now();
    string answer;
    getline(cin, answer);
    auto end = steady_clock::now();
    int duration = duration_cast<seconds>(end - start).count();

    //converts sentence to uppercase for secret
    string upperSentence = sentence;
    transform(upperSentence.begin(), upperSentence.end(), upperSentence.begin(), ::toupper);

    //if player solved a secret, they are rewarded accordingly
    if (player.hasFoodClue6 && (duration <= timeLimit) && (answer == foodClueSet6[foodClue6Index]))
    {
        if (!player.completedFoodClue6)
        {
            player.completedFoodClue6 = true;
            cout << "\nSECRET UNLOCKED: +5 Scrolls\n";
            player.scrollCount += 5;
        }
        else
        {
            cout << "\nYou already unlocked this secret.\n";
        }
    }
    //if player solved another secret, they are rewarded accordingly
    else if (player.hasFoodClue7 && foodClue7Index == 0 && (duration <= timeLimit) && (answer == upperSentence))
    {
        if (!player.completedFoodClue7)
        {
            player.completedFoodClue7 = true;
            cout << "\nSECRET UNLOCKED: +5 Scrolls\n";
            player.scrollCount += 5;
        }
        else
        {
            cout << "\nYou already unlocked this secret.\n";
        }
    }
    //if player took too long
    else if (duration > timeLimit)
    {
        //if player got it right
        if (answer == sentence)
        {
            cout << "\nCorrect typing, but you were too slow! You took " << duration << " seconds.\n";
        }
        //if player got it wrong
        else
        {
            cout << "\nIncorrect typing! And you were too slow! You took " << duration << " seconds.\n";
        }
    }
    //if player met time requirement
    else
    {
        //if player got it right
        if (answer == sentence)
        {
            cout << "\nPerfect! You typed it correctly in " << duration << " second"
                << (duration == 1 ? "" : "s") << ". +" << typingGame.tokensRewarded << " tokens\n";
            player.tokenCount += typingGame.tokensRewarded;
            ++typingGame.winCount;
        }
        //if player got it wrong
        else
        {
            cout << "\nIncorrect typing! You took " << duration << " second" << (duration == 1 ? "" : "s") << ".\n";
        }
    }
}

//4.2 - Counting Game: player has a time limit to count the number of symbols in a line
void playCountingGame(Player& player, GameBooth& countingGame, const vector<int>& foodClueSet3, int foodClue3Index) 
{
    //sets up game variables
    char symbols[] = {'$', '@', '#', '%', '&', '?'};
    char symbol = symbols[rand() % 6];
    int length = 15 + rand() % 76;
    int timeLimit = 10;
    int tolerance = 3;

    //instructions
    cout << "\nGOAL: Count how many '" << symbol << "' will appear below."
        << "\nTIME LIMIT: " << timeLimit << " seconds"
        << "\nROOM FOR ERROR: +/- " << tolerance
        << "\n\nPress ENTER to start...";

    //player presses enter to start
    string input;
    getline(cin, input);

    //ruler for help
    cout << "\nRULER: ----------------25-----------------------50-----------------------75----------------------100\n";
    cout << "\n" << string(length, symbol) << "\n";

    //times user
    auto start = steady_clock::now();
    int guess = getValidInt("\nEnter your guess: ", 0, 200);
    auto end = steady_clock::now();
    int duration = duration_cast<seconds>(end - start).count();

    //if player solves a secret, they are rewarded accordingly
    if (player.hasFoodClue3 && (duration <= timeLimit) && (guess == foodClueSet3[foodClue3Index]))
    {
        if (!player.completedFoodClue3)
        {
            player.completedFoodClue3 = true;
            cout << "\nSECRET UNLOCKED: +5 Scrolls\n";
            player.scrollCount += 5;
        }
        else
        {
            cout << "\nYou already unlocked this secret.\n";
        }
    }
    //if player was too slow
    else if (duration > timeLimit)
    {
        cout << "\nToo slow! You took " << duration << " seconds. There were " << length << " '" << symbol << "'.\n";
    }
    //if player was fast
    else
    {
        //if player was correct
        if (guess == length)
        {
            cout << "\nCorrect! There were " << length << " '" << symbol << "'. You took "
                << duration << " second" << (duration == 1 ? "" : "s") << ". +" << countingGame.tokensRewarded << " tokens\n";
            player.tokenCount += countingGame.tokensRewarded;
            ++countingGame.winCount;
        }
        //if player was close
        else if (abs(guess - length) <= tolerance)
        {
            cout << "\nClose enough! There were " << length << " '" << symbol << "'. You took "
                << duration << " second" << (duration == 1 ? "" : "s") << ". +" << countingGame.tokensRewarded << " tokens\n";
            player.tokenCount += countingGame.tokensRewarded;
            ++countingGame.winCount;
        }
        //if player was wrong
        else
        {
            cout << "\nWrong! There were " << length << " '" << symbol << "'. You took "
                << duration << " second" << (duration == 1 ? "" : "s") << ".\n";
        }
    }
}

//4.3 - Time Guessing Game: player has to test their internal clock
void playTimeGuessingGame(Player& player, GameBooth& timeGuessingGame)
{
    //set variables
    int targetTime = 3 + rand() % 13;
    int tolerance = 1;

    //instruction
    cout << "\nGOAL: Press ENTER to start the clock. Then, press ENTER again when you think " << targetTime << " seconds have passed."
        << "\nROOM FOR ERROR: " << tolerance << " second" << (tolerance == 1 ? "" : "s")
        << "\n\nPress ENTER to start...";

    //player press enter when ready
    string input;
    getline(cin, input);

    //start clock
    cout << "\nClock has started...";
    auto start = steady_clock::now();

    //player press enter when they think time has passed
    getline(cin, input);

    //timer stops
    auto end = steady_clock::now();
    int duration = duration_cast<seconds>(end - start).count();

    //calculates how accurate player was
    int difference = abs(duration - targetTime);

    //if player is correct
    if (difference <= tolerance)
    {
        cout << "\nGreat timing! (" << duration << " second" << (duration == 1 ? "" : "s") << ") +" << timeGuessingGame.tokensRewarded << " tokens\n";
        player.tokenCount += timeGuessingGame.tokensRewarded;
        ++timeGuessingGame.winCount;
    }
    //if player is wrong
    else
    {
        cout << "\nTry again! (" << duration << " second"
            << (duration == 1 ? "" : "s") << ")\n";

    }
}

//4.4 - Reaction Game: player has a time limit to react to the symbol
void playReactionGame(Player& player, GameBooth& reactionGame, int foodClue4Index)
{
    //set time limit
    int timeLimit = 250;

    //instruction
    cout << "\nGOAL: Wait for the signal (\"NOW!\"), then press ENTER as fast as possible."
        << "\nTIME LIMIT: " << timeLimit << " milliseconds"
        << "\n\nPress ENTER to start...";

    //player press enter when ready
    string input;
    getline(cin, input);

    //tells user to wait
    cout << "\nWait for it...\n";

    //sets up variables
    int waitTime = 1000 + rand() % 5000;
    bool inputReceived = false;

    //starts a separate task that tracks if player reacted too early
    thread inputThread([&]()
    {
        getline(cin, input);
        inputReceived = true;
    });

    //starts the wait period before player is supposed to react
    auto waitStart = steady_clock::now();
    while (duration_cast<milliseconds>(steady_clock::now() - waitStart).count() < waitTime)
    {
        //loop happens if player reacted too early
        if (inputReceived)
        {
            inputThread.join();

            //player unlocks secret if they reacted too early
            if (player.hasFoodClue4 && foodClue4Index == 0)
            {
                if (!player.completedFoodClue4)
                {
                    player.completedFoodClue4 = true;
                    cout << "\nSECRET UNLOCKED: +5 Scrolls\n";
                    player.scrollCount += 5;
                }
                else
                {
                    cout << "\nYou already unlocked this secret.\n";
                }
            }
            //player is told to not react early
            else
            {
                cout << "Too early! Try again.\n";
            }
            return;
        }
        //checks for user input every 1 ms
        this_thread::sleep_for(milliseconds(1));
    }

    //reaction signal
    cout << "\nNOW!";

    //starts timer and calculates how long it took for player to react
    auto start = steady_clock::now();
    inputThread.join();
    auto end = steady_clock::now();
    int reaction = duration_cast<milliseconds>(end - start).count();

    //player unlocks secret if they react for too long
    if (player.hasFoodClue4 && foodClue4Index == 1 && reaction >= 5000)
    {
        if (!player.completedFoodClue4)
        {
            player.completedFoodClue4 = true;
            cout << "\nSECRET UNLOCKED: +5 Scrolls\n";
            player.scrollCount += 5;
        }
        else
        {
            cout << "\nYou already unlocked this secret.\n";
        }
    }
    //if they reacted fast
    else if (reaction <= timeLimit)
    {
        cout << "\nReaction time: " << reaction << " ms\n";
        cout << "\nGood job! +" << reactionGame.tokensRewarded << " tokens\n";
        player.tokenCount += reactionGame.tokensRewarded;
        ++reactionGame.winCount;
    }
    //if they reacted slow
    else
    {
        cout << "\nReaction time: " << reaction << " ms\n";
        cout << "\nToo slow! Try again.\n";
    }
}

//4.5 - Color Game: player has a time limit to select the different color
void playColorGame(Player& player, GameBooth& colorGame)
{
    //time limit
    int timeLimit = 10;

    //instructions
    cout << "\nGOAL: Type the number that is a different color."
        << "\nTIME LIMIT: " << timeLimit << " seconds"
        << "\n\nPress ENTER to start...";

    //player presses enter when ready
    string input;
    getline(cin, input);

    //chooses which number will be different and which RGB value will be changed
    int differentNumber = rand() % 10;
    int differentHue = rand() % 3;

    //randomly assigns color for normal numbers
    int red = 10 + rand() % 246;
    int green = 10 + rand() % 246;
    int blue = 10 + rand() % 246;

    //initializes the different hue
    int differentRed = 0;
    int differentGreen = 0;
    int differentBlue = 0;

    //changes the number's color to be slightly different
    if (red <= 235)
        differentRed = red + 20;
    else
        differentRed = red - 20;

    if (green <= 235)
        differentGreen = green + 20;
    else
        differentGreen = green - 20;

    if (blue <= 235)
        differentBlue = blue + 20;
    else
        differentBlue = blue - 20;

    cout << "\n";

    //loops for all numbers
    for (int i = 0; i < 10; ++i)
    {
        //if it is the special number, outputs using a slightly changed color
        if (i == differentNumber)
        {
            if (differentHue == 0)
            {
                cout << "\033[38;2;" << differentRed << ";" << green << ";" << blue << "m" << i << " ";
            }
            else if (differentHue == 1)
            {
                cout << "\033[38;2;" << red << ";" << differentGreen << ";" << blue << "m" << i << " ";
            }
            else if (differentHue == 2)
            {
                cout << "\033[38;2;" << red << ";" << green << ";" << differentBlue << "m" << i << " ";
            }
        }
        //if not the special number, outputs using normal color
        else
        {
            cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m" << i << " ";
        }

    }

    //resets the color
    cout << "\033[0m\n";

    //starts timer and calculates player time
    auto start = steady_clock::now();
    int guess = getValidInt("\nEnter your guess: ", 0, 9);
    auto end = steady_clock::now();
    int duration = duration_cast<seconds>(end - start).count();

    //if player took too long
    if (duration > timeLimit)
    {
        //if player was correct
        if (guess == differentNumber)
        {
            cout << "\nCorrect, but you were too slow! You took " << duration << " seconds.\n";
        }
        //if player was wrong
        else
        {
            cout << "\nWrong! And you were too slow! The correct answer was " << differentNumber << ". You took " << duration << " seconds.\n";
        }
    }
    //if player was fast and correct
    else if (guess == differentNumber)
    {
        //Fast & Correct + Time
        cout << "\nCorrect! You took " << duration << " second" << (duration == 1 ? "" : "s") << ". +" << colorGame.tokensRewarded << " tokens\n";
        player.tokenCount += colorGame.tokensRewarded;
        ++colorGame.winCount;
    }
    //if player was fast but wrong
    else
    {
        cout << "\nWrong! The correct answer was " << differentNumber << ". You took " << duration << " second" << (duration == 1 ? "" : "s") << ".\n";
    }
}

//4.6 - Counting Game: player has a time limit to memorize a sequence of numbers
void playMemoryGame(Player& player, GameBooth& memoryGame, const vector<string>& foodClueSet1, int foodClue1Index)
{
    //set variables
    int memorizeTime = 3;
    int timeLimit = 10;

    //instructions
    cout << "\nGOAL: Memorize the sequence of numbers. When it disappears, type it back."
        << "\nTIME TO MEMORIZE: " << memorizeTime << " seconds"
        << "\nTIME TO TYPE: " << timeLimit << " seconds"
        << "\n\nPress ENTER to start...";

    //player presses enter when ready
    string input;
    getline(cin, input);

    //generate digit sequence
    int length = 9;
    string sequence = "";
    for (int i = 0; i < length; i++)
    {
        sequence += char('0' + rand() % 10);
    }

    //flash sequence
    cout << "\n" << sequence;
    this_thread::sleep_for(seconds(memorizeTime));

    //hide sequence
    cout << "\r" << string(sequence.length(), ' ') << "\r";

    //start timer after sequence disappears and calculate player time
    auto start = steady_clock::now();
    cout << "Enter the sequence: ";
    getline(cin, input);
    auto end = steady_clock::now();
    int duration = duration_cast<seconds>(end - start).count();

    //if player solves a secret, they are rewarded accordingly
    if (player.hasFoodClue1 && (duration <= timeLimit) && (input == foodClueSet1[foodClue1Index]))
    {
        if (!player.completedFoodClue1)
        {
            player.completedFoodClue1 = true;
            cout << "\nSECRET UNLOCKED: +5 Scrolls\n";
            player.scrollCount += 5;
        }
        else
        {
            cout << "\nYou already unlocked this secret.\n";
        }
    }
    //if player took too long
    else if (duration > timeLimit)
    {
        //if player was correct
        if (input == sequence)
        {
            cout << "\nCorrect, but you were too slow! You took " << duration << " seconds.\n";
        }
        //if player was wrong
        else
        {
            cout << "\nWrong! And you were too slow! The correct sequence was " << sequence << ". You took " << duration << " seconds.\n";
        }
    }
    //if player was fast
    else
    {
        //if player was correct
        if (input == sequence)
        {
            cout << "\nCorrect! You took " << duration << " second"
                << (duration == 1 ? "" : "s")
                << ". +" << memoryGame.tokensRewarded << " tokens\n";
            player.tokenCount += memoryGame.tokensRewarded;
            ++memoryGame.winCount;
        }
        //if player was wrong
        else
        {
            cout << "\nWrong! The correct sequence was " << sequence << ". You took " << duration << " second" << (duration == 1 ? "" : "s") << ".\n";
        }
    }
}

//4.7 - Archery Game: player has a time limit to stop a moving target
void playArcheryGame(Player& player, GameBooth& archeryGame, int foodClue2Index)
{
    //set time limit
    int timeLimit = 10;

    //instruction
    cout << "\nGOAL: Press ENTER to stop the dot in the green zone!"
        << "\nTIME LIMIT: " << timeLimit << " seconds"
        << "\n\nPress ENTER to start...";

    //player press enter when ready
    string input;
    getline(cin, input);

    //sets moving zone
    int width = 50;

    //target is at least 4 away from edges
    int target = 4 + rand() % (width - 8);

    //sets moving target variables
    int pos = 0;
    int direction = 1;

    //starts timer
    auto start = steady_clock::now();

    cout << "\nPress ENTER at the right moment...\n\n";

    //animation thread that keeps the target moving until player stops it
    bool running = true;
    thread t([&]()
    {
        //generates a line that the target is moving across
        while (running)
        {
            string line = "[";

            for (int i = 0; i < width; ++i)
            {
                //"0" is the moving target that is on a given position
                if (i == pos)
                {
                    line += "0";
                }
                //target zone is colored green, otherwise it is white
                else
                {
                    int distanceFromTarget = abs(i - target);

                    if (distanceFromTarget <= 1)
                    {
                        line += "\033[38;2;0;255;0m-\033[0m";
                    }
                    else
                    {
                        line += "-";
                    }
                }
            }

            line += "]";

            //rewrites itself so it's just one smooth line
            cout << "\r" << line;

            //move dot
            pos += direction;
            if (pos == width - 1 || pos == 0)
                direction *= -1;

            //animation rewrites every 10 ms
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    });

    //wait for enter
    getline(cin, input);

    //then the animation thread stops
    running = false;
    t.join();

    //calculates final position of the moving target
    int finalPos = pos;
    if (direction > 0)
    {
        --finalPos;
    }
    else
    {
        ++finalPos;
    }

    //calculates how long the player took
    auto end = steady_clock::now();
    int duration = duration_cast<seconds>(end - start).count();

    //calculates how close player is too target
    int distanceFromTarget = abs(finalPos - target);

    //calculates if player completed secret
    bool playerCompletedArcherySecret = false;
    if (foodClue2Index == 0 && finalPos == 0)
    {
        playerCompletedArcherySecret = true;
    }
    else if (foodClue2Index == 1 && finalPos == width - 1)
    {
        playerCompletedArcherySecret = true;
    }

    //if player solves secret, they are rewarded accordingly
    if (player.hasFoodClue2 && (duration <= timeLimit) && playerCompletedArcherySecret)
    {
        if (!player.completedFoodClue2)
        {
            player.completedFoodClue2 = true;
            cout << "\nSECRET UNLOCKED: +5 Scrolls\n";
            player.scrollCount += 5;
        }
        else
        {
            cout << "\nYou already unlocked this secret.\n";
        }
    }
    //if player took too long
    else if (duration > timeLimit)
    {
        cout << "\nToo slow! You took " << duration << " seconds.\n";
    }
    //if player was fast
    else
    {
        //if player landed in green zone
        if (distanceFromTarget <= 1)
        {
            cout << "\nPerfect! You took " << duration << " second" << (duration == 1 ? "" : "s") << ". +" << archeryGame.tokensRewarded << " tokens\n";
            player.tokenCount += archeryGame.tokensRewarded;
            ++archeryGame.winCount;
        }
        //if player did not land in green zone
        else
        {
            cout << "\nMiss! You were " << distanceFromTarget << " away. You took " << duration << " second" << (duration == 1 ? "" : "s") << ".\n";
        }
    }
}

//Function to play Game #8: Tracking Game
void playTrackingGame(Player& player, GameBooth& trackingGame)
{
    //set variable
    int options = 5;
    int greenIndex = rand() % options;

    //helper function that displays numbers with varying color
    auto render = [&](int greenIndex, string greenColor)
    {
        cout << "\r";
        for (int i = 0; i < options; i++)
        {
            if (i == greenIndex)
                cout << greenColor << "0\033[0m ";
            else
                cout << "0 ";
        }
    };

    //time limit
    int timeLimit = 10;

    //instruction
    cout << "\nGOAL: Pay attention to the green '0' as it gets blindly shuffled."
        << "\n      Shuffles will be indicated by two 0's being highlighted red and blue, in which they will swap."
        << "\n      In the end, pick the '0' you believe was green at the start."
        << "\nTIME LIMIT: " << timeLimit << " seconds"
        << "\n\nPress ENTER to start...";

    //player press enter when ready
    string input;
    getline(cin, input);

    cout << "\n";

    //show the green zero for 2 seconds
    render(greenIndex, "\033[32m");
    this_thread::sleep_for(seconds(2));

    //hide the green zero. Now all zeros are white 0.5 seconds
    render(-1, "");
    this_thread::sleep_for(milliseconds(500));

    //starts the sequence of shuffling the 0's
    int shuffles = 10;
    for (int s = 0; s < shuffles; s++) 
    {
        //generates two random positions that will switch numbers
        int a = rand() % options;
        int b;
        do { b = rand() % options; } while (b == a);

        //flashes the first number as blue, and the second number as red
        cout << "\r";
        for (int i = 0; i < options; i++) {
            if (i == a)      cout << "\033[34m0\033[0m "; //blue
            else if (i == b) cout << "\033[31m0\033[0m "; //red
            else             cout << "0 ";
        }
        //flashes for 150 ms
        this_thread::sleep_for(milliseconds(150));

        //swap the number that is "green"
        if (greenIndex == a) greenIndex = b;
        else if (greenIndex == b) greenIndex = a;

        //flash the numbers in reverse colors, indicating that the green has switched spots
        cout << "\r";
        for (int i = 0; i < options; i++) {
            if (i == a)      cout << "\033[31m0\033[0m "; //red
            else if (i == b) cout << "\033[34m0\033[0m "; //blue
            else             cout << "0 ";
        }
        //flashes for 150 ms
        this_thread::sleep_for(milliseconds(150));

        //when done shuffling, show all as white for 150 ms
        render(-1, "");
        this_thread::sleep_for(milliseconds(250));
    }

    //start timer
    auto start = steady_clock::now();

    //player is prompted to choose the 0 that started off as green
    cout << "\n\n";
    int guess = getValidInt("Which position was green? (1-5): ", 1, 5) - 1;

    //end timer
    auto end = steady_clock::now();
    int duration = duration_cast<seconds>(end - start).count();

    //if player was too slow
    if (duration > timeLimit)
    {
        //if player was right
        if (guess == greenIndex)
        {
            cout << "\nCorrect, but you were too slow! You took " << duration << " seconds.\n";
        }
        //if player was wrong
        else
        {
            cout << "\nWrong! And you were too slow! The correct answer was " << greenIndex + 1 << ". You took " << duration << " seconds.\n";
        }
    }
    //if player was fast
    else
    {
        //if player was right
        if (guess == greenIndex)
        {
            cout << "\nCorrect! You took " << duration << " second" << (duration == 1 ? "" : "s") << ". +" << trackingGame.tokensRewarded << " tokens\n";
            player.tokenCount += trackingGame.tokensRewarded;
            ++trackingGame.winCount;
        }
        //if player was wrong
        else
        {
            cout << "\nWrong! The correct answer was " << greenIndex + 1 << ". You took " << duration << " second" << (duration == 1 ? "" : "s") << ".\n";
        }
    }
}

//4.9 - Grid Game: player has a time limit to memorize a grid and replicate the marked positions
void playGridGame(Player& player, GameBooth& gridGame, int foodClue5Index)
{
    //set variables
    int rows = 5, cols = 5;
    int totalCells = rows * cols;
    int markedCount = 9;
    int flashTime = 3;
    int timeLimit = 15;

    //instructions
    cout << "\nGOAL: Memorize the marked positions (X) in the 5x5 grid. Then, type all marked positions back. (Order does not matter)"
        << "\nTIME TO MEMORIZE: " << flashTime << " seconds"
        << "\nTIME TO ANSWER: " << timeLimit << " seconds"
        << "\n\nPress ENTER to start...";

    //player press enter when ready
    string input;
    getline(cin, input);

    //randomly pick which positions on the grid will be marked, and add it to a set
    bool picked[26] = {};
    set<int> marked;
    while ((int)marked.size() < markedCount)
    {
        int pos = 1 + rand() % totalCells;
        if (!picked[pos])
        {
            picked[pos] = true;
            marked.insert(pos);
        }
    }

    //flash the grid with marked positions
    cout << "\n";
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int pos = r * cols + c + 1;
            if (marked.count(pos))
                cout << "\033[32mX\033[0m  ";
            else
                cout << ".  ";
        }
        cout << "\n";
    }
    this_thread::sleep_for(seconds(flashTime));

    //go back up 5 lines
    cout << "\033[5A";

    //print the reference key
    cout << "POSITION KEY:\n";
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int pos = r * cols + c + 1;
            cout << pos << (pos < 10 ? "   " : "  ");
        }
        cout << "\n";
    }

    //prompt player
    cout << "\nEnter marked positions (space-separated): ";

    //start timer and calculate duration
    auto start = steady_clock::now();
    getline(cin, input);
    auto end = steady_clock::now();
    int duration = duration_cast<seconds>(end - start).count();

    //if player took too long
    if (duration > timeLimit)
    {
        //Slow Regardless + Time + Answer
        cout << "\nToo slow! You took " << duration << " seconds.\n";
        cout << "Marked positions were: ";
        for (int m : marked) cout << m << " ";
        cout << "\n";
    }
    //if player was fast
    else
    {
        //parses the player's guesses
        set<int> guesses;
        stringstream ss(input);
        int num;
        bool invalidInput = false;

        //checks if player input anything invalid.
        //if it's valid, add it to set
        //if it's invalid change the flag
        while (ss >> num)
        {
            if (num < 1 || num > totalCells)
            {
                invalidInput = true;
                break;
            }
            guesses.insert(num);
        }

        //if player had invalid input
        if (invalidInput)
        {
            cout << "\nInvalid input! Numbers must be between 1 and 25.\n";
        }
        //if player had valid input
        else
        {
            //set up sets for the secrets
            set<int> blockG = { 1, 2, 3, 4, 5, 6, 11, 16, 21, 22, 23, 24, 25, 20, 15, 14, 13 };
            set<int> roundG = { 2, 3, 4, 6, 11, 16, 22, 23, 24, 20, 15, 14, 13 };
            set<int> heart = { 2, 8, 4, 6, 11, 10, 15, 17, 19, 23 };
            set<int> oddCheckerboard = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25 };
            set<int> evenCheckerboard = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 };

            //checks if player completed a secret for this game
            bool playerCompletedGridSecret = false;
            if (foodClue5Index == 0 && guesses == blockG || guesses == roundG)
            {
                playerCompletedGridSecret = true;
            }
            else if (foodClue5Index == 1 && guesses == heart)
            {
                playerCompletedGridSecret = true;
            }
            else if (foodClue5Index == 2 && guesses == oddCheckerboard || guesses == evenCheckerboard)
            {
                playerCompletedGridSecret = true;
            }

            //if player completed secret, they are rewarded accordingly
            if (player.hasFoodClue5 && playerCompletedGridSecret)
            {
                if (!player.completedFoodClue5)
                {
                    player.completedFoodClue5 = true;
                    cout << "\nSECRET UNLOCKED: +5 Scrolls\n";
                    player.scrollCount += 5;
                }
                else
                {
                    cout << "\nYou already unlocked this secret.\n";
                }
            }
            else
            {
                //calculates correct guesses, wrong guesses, final score
                int correct = 0;
                for (int g : guesses)
                    if (marked.count(g)) ++correct;
                int wrong = (int)guesses.size() - correct;
                int score = correct - wrong;

                //if player got everything correct
                if (score == markedCount)
                {
                    cout << "\nPerfect! All " << markedCount << " positions correct in "
                        << duration << " second" << (duration == 1 ? "" : "s")
                        << ". +" << gridGame.tokensRewarded << " tokens\n";
                    player.tokenCount += gridGame.tokensRewarded;
                    ++gridGame.winCount;
                }
                //if player got something wrong
                else
                {
                    cout << "\nWrong! You got " << correct << "/" << markedCount
                        << " correct with " << wrong << " wrong guess" << (wrong == 1 ? "" : "es") << ".\n";

                    cout << "Marked positions were: ";
                    for (int m : marked) cout << m << " ";
                    cout << "\n";

                    cout << "You took " << duration << " second" << (duration == 1 ? "" : "s") << ".\n";
                }
            }
        }
    }
}

//4.10 - Scanning Game: player has a time limit to find a near-black symbol in a sea of black
void playScanningGame(Player& player, GameBooth& scanningGame, int foodClue7Index)
{
    //time limit
    int timeLimit = 15;

    //instructions
    cout << "\nGOAL: Find the hidden character (it's really dark gray) inside the box and enter it."
        << "\nTIME LIMIT: " << timeLimit << " seconds"
        << "\n\nPress ENTER to start...";

    //player press enter when ready
    string input;
    getline(cin, input);

    //set variable
    int boxRows = 20;
    int boxCols = 100;

    //pick a random symbol
    string symbolPool = "@#$%&?0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char symbol = symbolPool[rand() % symbolPool.size()];

    //pick a random position inside the box
    int symbolRow = 1 + rand() % (boxRows - 2);
    int symbolCol = 1 + rand() % (boxCols - 2);

    //pick a second secret symbol for the secret
    char secretSymbol;
    do {
        secretSymbol = symbolPool[rand() % symbolPool.size()];
    } while (secretSymbol == symbol);

    //pick a second position for the secret
    int secretRow, secretCol;
    do {
        secretRow = 1 + rand() % (boxRows - 2);
        secretCol = 1 + rand() % (boxCols - 2);
    } while (secretRow == symbolRow && secretCol == symbolCol);

    cout << "\n";

    //generate the big box of black
    for (int r = 0; r < boxRows; ++r)
    {
        for (int c = 0; c < boxCols; ++c)
        {
            //sets more variables
            bool isBorder = (r == 0 || r == boxRows - 1 || c == 0 || c == boxCols - 1);
            bool isSymbol = (r == symbolRow && c == symbolCol);
            bool isSecret = (r == secretRow && c == secretCol);

            //prints border characters if on edge of box
            if (isBorder)
            {
                cout << "\033[48;2;0;0;0m";
                if (r == 0 || r == boxRows - 1)
                    cout << (c == 0 || c == boxCols - 1 ? "+" : "-");
                else
                    cout << "|";
                cout << "\033[0m";
            }
            //prints near-black symbol
            else if (isSymbol)
            {
                cout << "\033[48;2;0;0;0m\033[38;2;7;7;7m" << symbol << "\033[0m";
            }
            //prints secret symbol
            else if (isSecret)
            {
                //secret symbol is slightly revealed if clue is discovered
                if (player.hasFoodClue7 && foodClue7Index == 1 && !player.completedFoodClue7)
                {
                    cout << "\033[48;2;0;0;0m\033[38;2;10;0;0m" << secretSymbol << "\033[0m";
                }
                //or else it's black like everything else
                else
                {
                    cout << "\033[48;2;0;0;0m\033[38;2;0;0;0m" << secretSymbol << "\033[0m";
                }
            }
            //prints other characters black too, which will be distractions
            else
            {
                cout << "\033[48;2;0;0;0m\033[38;2;0;0;0m" << symbolPool[rand() % symbolPool.size()] << "\033[0m";
            }
        }
        cout << "\n";
    }

    //start timer and calculate duration
    auto start = steady_clock::now();
    cout << "\nEnter the hidden symbol: ";
    getline(cin, input);
    auto end = steady_clock::now();
    int duration = duration_cast<seconds>(end - start).count();

    //if player solved secret, they are rewarded accordingly
    if (player.hasFoodClue7 && (duration <= timeLimit) && (input.length() == 1 && input[0] == secretSymbol) && foodClue7Index  == 1)
    {
        if (!player.completedFoodClue7)
        {
            player.completedFoodClue7 = true;
            cout << "\nSECRET UNLOCKED: +5 Scrolls\n";
            player.scrollCount += 5;
        }
        else
        {
            cout << "\nYou already unlocked this secret.\n";
        }
    }
    //if player took too long
    else if (duration > timeLimit)
    {
        //if player got it right
        if (input.length() == 1 && input[0] == symbol)
        {
            cout << "\nCorrect, but you were too slow! You took " << duration << " seconds.\n";
        }
        //if player was wrong
        else
        {
            cout << "\nWrong! And you were too slow! The hidden symbol was '" << symbol << "'. You took " << duration << " seconds.\n";
        }
    }
    //if player was fast
    else
    {
        //if player was right
        if (input.length() == 1 && input[0] == symbol)
        {
            cout << "\nCorrect! You took " << duration << " second"
                << (duration == 1 ? "" : "s") << ". +" << scanningGame.tokensRewarded << " tokens\n";
            player.tokenCount += scanningGame.tokensRewarded;
            ++scanningGame.winCount;
        }
        //if player was wrong
        else
        {
            cout << "\nWrong! The hidden symbol was '" << symbol << "'. You took " << duration << " second" << (duration == 1 ? "" : "s") << ".\n";
        }
    }
}

//-------------------------+
//-----------------------+ |
//SECTION 5: Food Stands | |
//-----------------------+ |
//-------------------------+

//5.1 - Wing Stand: player can get clues here
void eatChickenWings(Player& player, const string& foodClue1, const string& foodClue2, const string& foodClue3, const string& foodClue4)
{
    //provides menu and price
    cout << "\nChicken Wings Menu                 Tokens: " << setw(2) << player.tokenCount << "\n";
    cout << "---------------------------------------------\n";
    printShopItem("1. Lemon Pepper                      3 tokens\n", player.hasFoodClue1);
    printShopItem("2. Garlic Parmesan                   3 tokens\n", player.hasFoodClue2);
    printShopItem("3. Mango Habanero                    3 tokens\n", player.hasFoodClue3);
    printShopItem("4. Spicy Gochujang                   3 tokens\n", player.hasFoodClue4);
    cout << "5. Back\n";

    //prompts player to order
    int choice = getValidInt("\nWhat would you like to order? (1-5): ", 1, 5);

    //matches player input with respective menu item
    if (choice == 1 && player.tokenCount >= 3) //also checks if player has enough tokens to pay for food
    {
        player.tokenCount -= 3; //subtracts respective token count from the player
        cout << "\nThank you for ordering!\n"; //informs if player received clue
        cout << "\nBuying Lemon Pepper Wings got you...";
        if (!player.hasFoodClue1)
        {
            player.hasFoodClue1 = true;
            player.foodClues.push_back(foodClue1); //add clue to vector
            cout << "\n - Clue: " << player.foodClues[player.foodClues.size() - 1] << "\n"; // display clue
        }
        else
        {
            cout << "\n - Nothing else!\n";
        }
    }
    else if (choice == 2 && player.tokenCount >= 3)
    {
        player.tokenCount -= 3;
        cout << "\nThank you for ordering!\n";
        cout << "\nBuying Garlic Parmesan Wings got you...";
        if (!player.hasFoodClue2)
        {
            player.hasFoodClue2 = true;
            player.foodClues.push_back(foodClue2);
            cout << "\n - Clue: " << player.foodClues[player.foodClues.size() - 1] << "\n";
        }
        else
        {
            cout << "\n - Nothing else!\n";
        }
    }
    else if (choice == 3 && player.tokenCount >= 3)
    {
        player.tokenCount -= 3;
        cout << "\nThank you for ordering!\n";
        cout << "\nBuying Mango Habanero Wings got you...";
        if (!player.hasFoodClue3)
        {
            player.hasFoodClue3 = true;
            player.foodClues.push_back(foodClue3);
            cout << "\n - Clue: " << player.foodClues[player.foodClues.size() - 1] << "\n";
        }
        else
        {
            cout << "\n - Nothing else!\n";
        }
    }
    else if (choice == 4 && player.tokenCount >= 3)
    {
        player.tokenCount -= 3;
        cout << "\nThank you for ordering!\n";
        cout << "\nBuying Spicy Gochujang Wings got you...";
        if (!player.hasFoodClue4)
        {
            player.hasFoodClue4 = true;
            player.foodClues.push_back(foodClue4);
            cout << "\n - Clue: " << player.foodClues[player.foodClues.size() - 1] << "\n";
        }
        else
        {
            cout << "\n - Nothing else!\n";
        }
    }
    else if (choice == 5)
    {
        cout << "\nYou exit the Chicken Wing Stand empty-handed.\n";
        player.actionCompleted = false;
    }
    else //case for when player does not have enough tokens
    {
        cout << "\nYou do not have enough tokens. Go win some games to earn tokens!\n";
    }
    
}

//5.2 - Boba Stand: player can get clues here
void eatBoba(Player& player, const string& foodClue5, const string& foodClue6, const string& foodClue7, const string& foodClue8)
{
    //provides menu and price
    cout << "\nBoba Menu                          Tokens: " << setw(2) << player.tokenCount << "\n";
    cout << "---------------------------------------------\n";
    printShopItem("1. Thai Milk Tea                     3 tokens\n", player.hasFoodClue5);
    printShopItem("2. Pandan Matcha                     3 tokens\n", player.hasFoodClue6);
    printShopItem("3. Passionfruit Tea                  3 tokens\n", player.hasFoodClue7);
    printShopItem("4. Strawberry Milk Tea               3 tokens\n", player.hasFoodClue8);
    cout << "5. Back\n";

    //prompts player to order
    int choice = getValidInt("\nWhat would you like to order? (1-5): ", 1, 5);

    //matches player input with respective menu item
    if (choice == 1 && player.tokenCount >= 3) //also checks if player has enough tokens to pay for food
    {
        player.tokenCount -= 3; //subtracts respective token count from the player
        cout << "\nThank you for ordering!\n"; //informs if player received clue
        cout << "\nBuying Thai Milk Tea got you...";
        if (!player.hasFoodClue5)
        {
            player.hasFoodClue5 = true;
            player.foodClues.push_back(foodClue5); //adds clue to vector
            cout << "\n - Clue: " << player.foodClues[player.foodClues.size() - 1] << "\n"; //displays clue
        }
        else
        {
            cout << "\n - Nothing else!\n";
        }
    }
    else if (choice == 2 && player.tokenCount >= 3)
    {
        player.tokenCount -= 3;
        cout << "\nThank you for ordering!\n";
        cout << "\nBuying Pandan Matcha got you...";
        if (!player.hasFoodClue6)
        {
            player.hasFoodClue6 = true;
            player.foodClues.push_back(foodClue6);
            cout << "\n - Clue: " << player.foodClues[player.foodClues.size() - 1] << "\n";
        }
        else
        {
            cout << "\n - Nothing else!\n";
        }
    }
    else if (choice == 3 && player.tokenCount >= 3)
    {
        player.tokenCount -= 3;
        cout << "\nThank you for ordering!\n";
        cout << "\nBuying Passionfruit Tea got you...";
        if (!player.hasFoodClue7)
        {
            player.hasFoodClue7 = true;
            player.foodClues.push_back(foodClue7);
            cout << "\n - Clue: " << player.foodClues[player.foodClues.size() - 1] << "\n";
        }
        else
        {
            cout << "\n - Nothing else!\n";
        }
    }
    else if (choice == 4 && player.tokenCount >= 3)
    {
        player.tokenCount -= 3;
        cout << "\nThank you for ordering!\n";
        cout << "\nBuying Strawberry Milk Tea got you...";
        if (!player.hasFoodClue8)
        {
            player.hasFoodClue8 = true;
            player.foodClues.push_back(foodClue8);
            cout << "\n - Clue: " << player.foodClues[player.foodClues.size() - 1] << "\n";
        }
        else
        {
            cout << "\n - Nothing else!\n";
        }
    }
    else if (choice == 5)
    {
        cout << "\nYou exit the Boba Stand empty-handed.\n";
        player.actionCompleted = false;
    }
    else //case for when player does not have enough tokens
    {
        cout << "\nYou do not have enough tokens. Go win some games to earn tokens!\n";
    }
    
}

//-------------------+
//-----------------+ |
//SECTION 6: Rides | |
//-----------------+ |
//-------------------+

//6.1 - House of Balloons: player can get clues here
void rideHouseOfBalloons(Player& player, Ride& houseOfBalloons, vector<string>& rideClueSet1)
{
    //instructions
    cout << "You have: " << player.tokenCount << " token" << (player.tokenCount == 1 ? "" : "s") << "\n";
    
    cout << "\nIn this ride, you have a:\n";
    cout << "- 30% chance of getting lost\n";
	cout << "- 100% chance of getting a clue (" << player.ride1ClueCount << "/7)\n";

    //prompt to continue or leave
    cout << "\nDo you want to enter?"
        << "\n1. Yes"
        << "\n2. No\n";

    int choice = getValidInt("\nEnter your choice (1-2): ", 1, 2);

    //if player leaves
    if (choice == 2)
    {
        cout << "\nYou step away from the House of Balloons.\n";
        player.actionCompleted = false;
    }
    //if player is poor
    else if (player.tokenCount < houseOfBalloons.cost)
    {
        cout << "\nYou don't have enough tokens.\n";
    }
    //start ride
    else
    {
        //take money
        player.tokenCount -= houseOfBalloons.cost;

        //story
        cout << "\nYou step inside the House of Balloons.";
        cout << "\nColorful balloons fill every corner, blocking your view.";
        cout << "\nYou push through the maze, popping balloons as you go.";

        //randomize the ride breaking down
        int roll = rand() % 10;
        bool brokeDown = (roll <= 2);

        //if it broke down
        if (brokeDown)
        {
            cout << "\n\nOh no! After many confusing turns, you find yourself lost in the maze!\n";

            bool resolved = false;
            
            //loops until break down is resolved
            while (!resolved)
            {
                //prompts user to call someone
                cout << "\nWhat do you do?";
                cout << "\n1. Call for help";
                cout << "\n2. Continue and try to find the exit (lose 90 minutes)\n";
                int breakChoice = getValidInt("\nEnter your choice (1-2): ", 1, 2);

                //if player wants to call
                if (breakChoice == 1)
                {
                    //if contact list is empty
                    if (player.contacts.empty())
                    {
                        cout << "\nYou have no contacts. Unlock them from the Gift Shop!\n";
                        continue;
                    }

                    bool calledSomeone = false;

                    //loops until player calls someone
                    while (!calledSomeone)
                    {
                        //shows contact list and prompts the user to choose
                        cout << "\n--- CONTACTS ---\n";
                        int i = 1;
                        vector<string> contactNames;
                        for (auto& c : player.contacts)
                        {
                            cout << i << ". " << c.first << "\n";
                            contactNames.push_back(c.first);
                            ++i;
                        }
                        cout << i << ". Go Back\n";

                        int callChoice = getValidInt("\nEnter your choice (1-" + to_string(i) + "): ", 1, i);

                        //go back if user wants to no longer call
                        if (callChoice == i)
                        {
                            break;
                        }

                        //gets contact name, ride, and work time
                        string chosenName = contactNames[callChoice - 1];
                        string contactRide = player.contacts[chosenName].second;
						string workTime = player.contacts[chosenName].first;

                        //calls contact
                        cout << "\nCalling " << chosenName;
                        for (int j = 0; j < 3; ++j)
                        {
                            this_thread::sleep_for(seconds(1));
                            cout << ".";
                        }
                        cout << "\n";

                        //makes sure the contact works here
                        if (contactRide == "House of Balloons")
                        {
                            //basic work time takes a certain time
                            if (workTime == "Basic")
                            {
                                cout << "\n" << chosenName << " answers the phone and starts making his way over to you.\n";
                                cout << "Once he arrives, he guides you out of the maze, taking 15 minutes to do so.\n";
                                player.clock += 15;
                            }
                            //precise work time takes a certain time
                            else if (workTime == "Precise")
                            {
                                int remainder = player.clock % 30;
                                int timeToNextHalf = (remainder == 0) ? 30 : (30 - remainder);
                                cout << "\n" << chosenName << " answers the phone and starts making her way over to you.\n";
                                cout << "Once she arrives, she guides you out of the maze, taking " << timeToNextHalf << (timeToNextHalf == 1 ? " minute" : " minutes") << " to do so.\n";
                                player.clock += timeToNextHalf;
							}

                            resolved = true;
                            calledSomeone = true;
                        }
                        //if player calls someone who doesn't work here
                        else
                        {
                            cout << "\n" << chosenName << " picks up, but they don't know this ride. Try someone else.\n";
                        }
                    }
                }
                //if player wants to leave by themselves, losing a lot of time
                else
                {
                    cout << "\nYou frantically push through the maze and eventually find the exit. That cost you 90 minutes!\n";
                    player.clock += 90;
                    resolved = true;
                }
            }
        }
        //if ride does not break down
        else
        {
            cout << "\nAfter twisting and turning, you finally find the exit.\n";
        }

        //give clue from rideClueSet1
        if (!rideClueSet1.empty())
        {
            //chooses random clue and pops it from vector
            int clueIndex = rand() % rideClueSet1.size();
            string clue = rideClueSet1[clueIndex];
            rideClueSet1.erase(rideClueSet1.begin() + clueIndex);

            cout << "\nA mysterious crate sits waiting at the exit!";
            cout << "\nYou pick it up.";

            //prompt user to open crate
            cout << "\n\nPress ENTER to open...";
            string enter;
            getline(cin, enter);
            cout << "\n";

            //animate opening the crate
            for (int i = 3; i >= 1; --i)
            {
                int r = 255;
                int g = 120 + (5 - i) * 25;
                int b = 0;

                cout << "Opening crate: ";
                cout << "\033[38;2;" << r << ";" << g << ";" << b << "m" << i << "\033[0m";

                this_thread::sleep_for(seconds(1));
                cout << "\r";
            }

            //opens the crate
            cout << "Opening crate: ";
            cout << "\033[38;2;0;255;0mDONE!\033[0m\n";

            //displays clue
            cout << "\nYou unlocked a CLUE: " << clue << "\n";

            //adds clue to vector
            player.rideClues.push_back(clue);
            ++player.ride1ClueCount;
        }
        //if all clues were found
        else
        {
            cout << "\nA crate sits at the exit, but it's empty. All clues have been found!\n";
        }
    }
}

//6.2 - Nightcrawler: player can get clues here
void rideNightcrawler(Player& player, Ride& nightcrawler, vector<string>& rideClueSet2)
{
    //instructions
    cout << "You have: " << player.tokenCount << " token" << (player.tokenCount == 1 ? "" : "s") << "\n";

    cout << "\nIn this ride, you have a:\n";
    cout << "- 30% chance of getting stuck on the roller coaster\n";
    cout << "- 100% chance of getting a clue (" << player.ride2ClueCount << "/7)\n";

    //prompt user
    cout << "\nDo you want to enter?"
        << "\n1. Yes"
        << "\n2. No\n";

    int choice = getValidInt("\nEnter your choice (1-2): ", 1, 2);

    //player leaves
    if (choice == 2)
    {
        cout << "\nYou step away from the Nightcrawler.\n";
        player.actionCompleted = false;
    }
    //player is poor
    else if (player.tokenCount < nightcrawler.cost)
    {
        cout << "\nYou don't have enough tokens.\n";
    }
    //player rides
    else
    {
        //takes tokens
        player.tokenCount -= nightcrawler.cost;

        //story
        cout << "\nYou step inside the Nightcrawler.";
        cout << "\nThe ride twists and turns, thrilling you with every movement.";
        cout << "\nYou hold on tight as the Nightcrawler takes you on a wild journey.";
        int roll = rand() % 10;
        bool brokeDown = (roll <= 2);

        //ride broke down
        if (brokeDown)
        {
            cout << "\n\nOh no! The Nightcrawler has come to a sudden stop mid-track!\n";

            bool resolved = false;

            //loop happens until break down is resolved
            while (!resolved)
            {
                //prompt user
                cout << "\nWhat do you do?";
                cout << "\n1. Call for help";
                cout << "\n2. Climb out yourself and navigate the exit (lose 90 minutes)\n";
                int breakChoice = getValidInt("\nEnter your choice (1-2): ", 1, 2);

                //player wants to call
                if (breakChoice == 1)
                {
                    //player has no contacts
                    if (player.contacts.empty())
                    {
                        cout << "\nYou have no contacts. Unlock them from the Gift Shop!\n";
                        continue;
                    }

                    bool calledSomeone = false;

                    //loops until player calls someone
                    while (!calledSomeone)
                    {
                        //shows list of contacts
                        cout << "\n--- CONTACTS ---\n";
                        int i = 1;
                        vector<string> contactNames;
                        for (auto& c : player.contacts)
                        {
                            cout << i << ". " << c.first << "\n";
                            contactNames.push_back(c.first);
                            ++i;
                        }
                        cout << i << ". Go Back\n";

                        int callChoice = getValidInt("\nEnter your choice (1-" + to_string(i) + "): ", 1, i);

                        //player wants to go back
                        if (callChoice == i)
                        {
                            break;
                        }

                        //show contacts name, ride, work time
                        string chosenName = contactNames[callChoice - 1];
                        string contactRide = player.contacts[chosenName].second;
                        string workTime = player.contacts[chosenName].first;
                            
                        //calling contact
                        cout << "\nCalling " << chosenName;
                        for (int j = 0; j < 3; ++j)
                        {
                            this_thread::sleep_for(seconds(1));
                            cout << ".";
                        }
                        cout << "\n";

                        //checks ride if contact works at this ride
                        if (contactRide == "Nightcrawler")
                        {
                            cout << "\n" << chosenName << " answers the phone and begins remotely restarting the ride.\n";

                            //greedy work time changes time to leave
                            if (workTime == "Greedy")
                            {
                                int timeCost = player.tokenCount;
                                cout << "It takes " << timeCost << (timeCost == 1 ? " minute" : " minutes") << " for " << chosenName << " to restart the Nightcrawler.\n";
                                player.clock += timeCost;
                            }
                            //odd work time changes time to leave
                            else if (workTime == "Odd")
                            {
                                int hour = (player.clock / 60) % 12;
                                if (hour == 0) hour = 12;
                                int timeCost = (hour % 2 == 1) ? 5 : 30;
                                cout << "It takes " << timeCost << (timeCost == 1 ? " minute" : " minutes") << " for " << chosenName << " to restart the Nightcrawler.\n";
                                player.clock += timeCost;
                            }

                            cout << "The roller coaster roars back to life and eventually reaches the end.\n";

                            resolved = true;
                            calledSomeone = true;
                        }
                        //if contact does not work here
                        else
                        {
                            cout << "\n" << chosenName << " picks up, but they don't know this ride. Try someone else.\n";
                        }
                    }
                }
                //if player leaves by themselves, costing them a lot of time
                else
                {
                    cout << "\nYou climb out of the roller coaster and carefully walk along the track.";
                    cout << "\nOnce it gets low enough, you jump off the track and eventually find the exit. That cost you 90 minutes!\n";
                    player.clock += 90;
                    resolved = true;
                }
            }
        }
        //if ride never broke down
        else
        {
            cout << "\nThe coaster roars back to life and screeches to the finish!\n";
        }

        //give clue from rideClueSet2
        if (!rideClueSet2.empty())
        {
            //randomly generate clue
            int clueIndex = rand() % rideClueSet2.size();
            string clue = rideClueSet2[clueIndex];
            rideClueSet2.erase(rideClueSet2.begin() + clueIndex);

            cout << "\nA mysterious crate sits waiting at the exit!";
            cout << "\nYou pick it up.";

            //press enter to open
            cout << "\n\nPress ENTER to open...";
            string enter;
            getline(cin, enter);
            cout << "\n";

            //animate crate opening
            for (int i = 3; i >= 1; --i)
            {
                int r = 255;
                int g = 120 + (5 - i) * 25;
                int b = 0;

                cout << "Opening crate: ";
                cout << "\033[38;2;" << r << ";" << g << ";" << b << "m" << i << "\033[0m";

                this_thread::sleep_for(seconds(1));
                cout << "\r";
            }

            //opening crate
            cout << "Opening crate: ";
            cout << "\033[38;2;0;255;0mDONE!\033[0m\n";

            //show cluw
            cout << "\nYou unlocked a CLUE: " << clue << "\n";

            //add clue to vector
            player.rideClues.push_back(clue);
            ++player.ride2ClueCount;
        }
        //if player gets all clues
        else
        {
            cout << "\nA crate sits at the exit, but it's empty. All clues have been found!\n";
        }
    }
}

//----------------------------------+
//--------------------------------+ | - I could've put these directly inside the gift shop, but a previous version of my game could buy items from multiple sources.
//SECTION 7a: Gift Shop Functions | | - So it made sense to have helper functions buy items. Now, items can only be bought from one place, so I don't need separate helper function.
//--------------------------------+ | - But, I'll just keep it like this anyway
//----------------------------------+

void buyBasicStopwatch(Player& player)
{
    //only buys if player doesn't have item yet
    if (!player.hasBasicStopwatch)
    {
        cout << "\nYou obtained BASIC STOPWATCH: ";
        player.hasBasicStopwatch = true;
        player.items.push_back("Basic Stopwatch"); //adds item to vector
        cout << "Travel 40 minutes back in time\n";
        player.clock -= 40; //item goes into effect
    }
    else
    {
        //if player already bought item, refunds their scrolls
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 2;
    }
}
void buyPreciseStopwatch(Player& player)
{
    if (!player.hasPreciseStopwatch)
    {
        cout << "\nYou obtained PRECISE STOPWATCH: ";
        player.hasPreciseStopwatch = true;
        player.items.push_back("Precise Stopwatch");
        cout << "Travel back to the nearest hour\n";
        player.clock = (player.clock / 60) * 60;
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 2;
    }
}
void buyPlayfulStopwatch(Player& player)
{
    //only buys if player doesn't have item yet
    if (!player.hasPlayfulStopwatch)
    {
        cout << "\nYou obtained PLAYFUL STOPWATCH: ";
        player.hasPlayfulStopwatch = true;
        player.items.push_back("Playful Stopwatch"); //adds item to vector

        //press enter to start minigame
        cout << "Press ENTER to play...";
        string input;
        getline(cin, input);

        //instruction
        cout << "\nGOAL: Press ENTER to stop the random number generator.";
        cout << "\n      The number you land on determines how many minutes you will travel back in time.";
        cout << "\nTIME LIMIT: 10 seconds - or else you don't travel back at all\n";

        //press enter to actually start
        cout << "Press ENTER to start...\n";
        getline(cin, input);

        //generate variables
        int timeLimit = 10;
        int currentNumber = 0;
        bool inputReceived = false;

        //thread to start a new task that keeps on generating random number
        bool running = true;
        thread t([&]()
        {
            srand(time(0));
            while (running)
            {
                currentNumber = rand() % 101;
                cout << "\r" << currentNumber << "   ";
                //generates new number every 100 ms
                this_thread::sleep_for(milliseconds(100));
            }
        });

        //starts timer
        auto start = steady_clock::now();

        //starts new task to wait for user input
        thread inputThread([&]()
        {
            getline(cin, input);
            inputReceived = true;
        });

        //constantly checks if user inputted an enter
        while (!inputReceived)
        {
            //if time exceeds the time limit, this check stops
            auto now = steady_clock::now();
            int elapsed = duration_cast<seconds>(now - start).count();
            if (elapsed >= timeLimit)
                break;
            this_thread::sleep_for(milliseconds(1));
        }

        running = false;
        t.join(); //waits for code to finish before continuing

        //if user pressed enter before the time limit
        if (inputReceived)
        {
            //stops the thread that waits for input
            inputThread.join();

            //see what number the player landed on and give them time accordingly
            int landed = currentNumber;
            cout << "\nYou landed on " << landed << "!";
            cout << "\nYou travel back " << landed << (landed == 1 ? " minute" : " minutes") << " in time.\n";
            player.clock -= landed;
        }
        //if player takes too long
        else
        {
            inputThread.detach(); //main code continues without waiting for thread to finish
            cout << "\n\nToo slow! You don't travel back at all.\n";
        }
    }
    //if player already bought this stopwatch
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 3;
    }
}
void buyGreedyStopwatch(Player& player)
{
    if (!player.hasGreedyStopwatch)
    {
        cout << "\nYou obtained GREEDY STOPWATCH: ";
        player.hasGreedyStopwatch = true;
        player.items.push_back("Greedy Stopwatch");
        cout << "Travel back " << (player.tokenCount * 3)
            << " minutes because you have " << player.tokenCount
            << " token" << (player.tokenCount == 1 ? "" : "s") << "\n";
        player.clock -= (player.tokenCount * 3);
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 3;
    }
}
void buyCompetitiveStopwatch(Player& player)
{
    if (!player.hasCompetitiveStopwatch)
    {
        cout << "\nYou obtained COMPETITIVE STOPWATCH: ";
        player.hasCompetitiveStopwatch = true;
        player.items.push_back("Competitive Stopwatch");
        cout << "Travel back " << (player.winCount * 3)
            << " minutes because you have " << player.winCount
            << " win" << (player.winCount == 1 ? "" : "s") << "\n";
        player.clock -= (player.winCount * 3);
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 3;
    }
}
void buyShield(Player& player)
{
    if (!player.hasShield)
    {
        cout << "\nYou obtained a SHIELD: ";
        player.hasShield = true;
        player.items.push_back("Shield");
        cout << "You can now defend yourself against a throat punch.\n";
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 3;
    }
}
void buyTicket(Player& player)
{
    if (!player.hasTicket)
    {
        cout << "\nYou obtained a TICKET: ";
        player.hasTicket = true;
        player.items.push_back("Ticket");
        cout << "You can now make someone happy if needed.\n";
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 3;
    }
}
void buyAbel(Player& player)
{
    //if player does not have contact yet
    if (!player.hasAbel)
    {
        cout << "\nYou obtained Abel's phone number: ";
        player.hasAbel = true;
        player.contacts["Abel"] = { "Basic", "House of Balloons" }; //add contact to player collection
        cout << "You can now call Abel for help if needed.\n";
    }
    else
    {
        //if player already has this item, refund the player
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 1;
    }
}
void buyValerie(Player& player)
{
    if (!player.hasValerie)
    {
        cout << "\nYou obtained Valerie's phone number: ";
        player.hasValerie = true;
        player.contacts["Valerie"] = { "Precise", "House of Balloons" };
        cout << "You can now call Valerie for help if needed.\n";
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 1;
    }
}
void buyJacques(Player& player)
{
    if (!player.hasJacques)
    {
        cout << "\nYou obtained Jacques's phone number: ";
        player.hasJacques = true;
        player.contacts["Jacques"] = { "Greedy", "Nightcrawler" };
        cout << "You can now call Jacques for help if needed.\n";
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 1;
    }
}
void buyMaria(Player& player)
{
    if (!player.hasMaria)
    {
        cout << "\nYou obtained Maria's phone number: ";
        player.hasMaria = true;
        player.contacts["Maria"] = { "Odd", "Nightcrawler" };
        cout << "You can now call Maria for help if needed.\n";
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 1;
    }
}
void buyDigitReveal(Player& player)
{
    //if player does not have item yet
    if (!player.hasDigitReveal)
    {
        cout << "\nYou obtained a DIGIT REVEAL";
        player.hasDigitReveal = true;
        cout << "!\n";

        //prompt the user the corresponding benefit
        int choice = getValidInt("\nEnter the digit whose position you want to reveal (0-9): ", 0, 9);
        int pos = player.passcode.find(to_string(choice));
        string clue = "The digit " + to_string(choice) + " is at position " + to_string(pos + 1) + ".";
        cout << "\nDigit Reveal: " << clue << "\n";

        player.premiumClues.push_back("Digit Reveal: " + clue); //add clue to vector
    }
    else
    {
        //if player already bought this, refund them
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 5;
    }
}
void buyPositionReveal(Player& player)
{
    if (!player.hasPositionReveal)
    {
        cout << "\nYou obtained a POSITION REVEAL";
        player.hasPositionReveal = true;
        cout << "!\n";

        int choice = getValidInt("\nEnter the position whose digit you want to reveal (1-10): ", 1, 10);
        int digit = player.passcode[choice - 1] - '0';
        string clue = "The digit at position " + to_string(choice) + " is " + to_string(digit) + ".";
        cout << "\nPosition Reveal: " << clue << "\n";

        player.premiumClues.push_back("Position Reveal: " + clue);
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 5;
    }
}
void buyNeighborReveal(Player& player)
{
    if (!player.hasNeighborReveal)
    {
        cout << "\nYou obtained a NEIGHBOR REVEAL";
        player.hasNeighborReveal = true;
        cout << "!\n";

        int choice = getValidInt("\nEnter the digit whose neighbors you want to reveal (0-9): ", 0, 9);
        int pos = player.passcode.find(to_string(choice));
        string left = (pos > 0) ? to_string(player.passcode[pos - 1] - '0') : "N/A";
        string right = (pos < 9) ? to_string(player.passcode[pos + 1] - '0') : "N/A";

        string clue;
        if (rand() % 2 == 0)
            clue = "The neighbors of " + to_string(choice) + " are " + left + " and " + right + ".";
        else
            clue = "The neighbors of " + to_string(choice) + " are " + right + " and " + left + ".";

        cout << "\nNeighbor Reveal: " << clue;
        cout << "\n(Note: The neighbors are shown in random order)\n";

        player.premiumClues.push_back("Neighbor Reveal: " + clue);
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 5;
    }
}
void buyOrderReveal(Player& player)
{
    if (!player.hasOrderReveal)
    {
        cout << "\nYou obtained an ORDER REVEAL";
        player.hasOrderReveal = true;
        cout << "!\n";

        int choiceA = getValidInt("\nEnter the first digit (0-9): ", 0, 9);
        int choiceB = getValidInt("Enter the second digit (0-9): ", 0, 9);
        int posA = player.passcode.find(to_string(choiceA));
        int posB = player.passcode.find(to_string(choiceB));
        string first = (posA < posB) ? to_string(choiceA) : to_string(choiceB);
        string second = (posA < posB) ? to_string(choiceB) : to_string(choiceA);
        string clue = "The digit " + first + " comes before the digit " + second + ".";
        cout << "\nOrder Reveal: " << clue << "\n";

        player.premiumClues.push_back("Order Reveal: " + clue);
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 5;
    }
}
void buySpaceReveal(Player& player)
{
    if (!player.hasSpaceReveal)
    {
        cout << "\nYou obtained a SPACE REVEAL";
        player.hasSpaceReveal = true;
        cout << "!\n";

        int choiceA = getValidInt("\nEnter the first digit (0-9): ", 0, 9);
        int choiceB = getValidInt("Enter the second digit (0-9): ", 0, 9);
        int posA = player.passcode.find(to_string(choiceA));
        int posB = player.passcode.find(to_string(choiceB));
        int space = abs((int)posA - (int)posB) - 1;
        string clue = "There " + string(space == 1 ? "is " : "are ") + to_string(space) + " digit" + (space == 1 ? "" : "s") + " between " + to_string(choiceA) + " and " + to_string(choiceB) + ".";
        cout << "\nSpace Reveal: " << clue << "\n";

        player.premiumClues.push_back("Space Reveal: " + clue);
    }
    else
    {
        cout << "\nYou already bought this item.\n";
        player.scrollCount += 5;
    }
}

//------------------------+
//----------------------+ |
//SECTION 7b: Gift Shop | |
//----------------------+ |
//------------------------+


//Gift Shop - player buys the important stuff here
void visitGiftShop(Player& player)
{
    //print the menu
    cout << "\n----------------- GIFT SHOP -----------------\n";
    cout << "Scrolls: " << player.scrollCount << "\n\n";

    cout << "ITEMS                      COST            DESCRIPTION\n";
    printShopItem("1. Basic Stopwatch         2 Scrolls       Takes you back in time by a flat 40 minutes\n", player.hasBasicStopwatch);
    printShopItem("2. Precise Stopwatch       2 Scrolls       Takes you back in time to the nearest hour\n", player.hasPreciseStopwatch);
    printShopItem("3. Playful Stopwatch       3 Scrolls       Takes you back in time (0-100 minutes) based on a minigame\n", player.hasPlayfulStopwatch);
    printShopItem("4. Greedy Stopwatch        3 Scrolls       Takes you back in time 3 minutes for every token you currently have\n", player.hasGreedyStopwatch);
    printShopItem("5. Competitive Stopwatch   3 Scrolls       Takes you back in time 3 minutes for every carnival win you have\n", player.hasCompetitiveStopwatch);
    printShopItem("6. Shield                  3 Scrolls       Can defend a punch in the throat\n", player.hasShield);
    printShopItem("7. Ticket                  3 Scrolls       Can make someone happy\n\n", player.hasTicket);

    cout << "CONTACTS                   COST            DESCRIPTION\n";
    printShopItem("8. Abel                    1 Scroll        House of Balloons employee; finishes helping you in 15 minutes\n", player.hasAbel);
    printShopItem("9. Valerie                 1 Scroll        House of Balloons employee; finishes helping you in the nearest half hour\n", player.hasValerie);
    printShopItem("10. Jacques                1 Scroll        Nightcrawler employee; takes 1 minute to help you for every token you have\n", player.hasJacques);
    printShopItem("11. Maria                  1 Scroll        Nightcrawler employee; takes 5 minutes on odd hours & 30 minutes on even hours\n\n", player.hasMaria);

    cout << "PREMIUM CLUES              COST            DESCRIPTION\n";
    printShopItem("12. Digit Reveal           5 Scrolls       Enter any digit to reveal its position\n", player.hasDigitReveal);
    printShopItem("13. Position Reveal        5 Scrolls       Enter any position to reveal the digit\n", player.hasPositionReveal);
    printShopItem("14. Neighbor Reveal        5 Scrolls       Enter any digit to reveal its neighboring digits\n", player.hasNeighborReveal);
    printShopItem("15. Order Reveal           5 Scrolls       Enter two digits to reveal which digit comes first\n", player.hasOrderReveal);
    printShopItem("16. Space Reveal           5 Scrolls       Enter two digits to reveal how much space is in between them\n\n", player.hasSpaceReveal);

    cout << "17. Back\n";

    cout << "\n* All items can only be bought once\n";

    //prompt user
    int choice = getValidInt("\nWhat would you like to get? (1-17): ", 1, 17);

    //buy items respectively
    if (choice == 1 && player.scrollCount >= 2)
    {
        player.scrollCount -= 2;
        buyBasicStopwatch(player);
    }
    else if (choice == 2 && player.scrollCount >= 2)
    {
        player.scrollCount -= 2;
        buyPreciseStopwatch(player);
    }
    else if (choice == 3 && player.scrollCount >= 3)
    {
        player.scrollCount -= 3;
        buyPlayfulStopwatch(player);
    }
    else if (choice == 4 && player.scrollCount >= 3)
    {
        player.scrollCount -= 3;
        buyGreedyStopwatch(player);
    }
    else if (choice == 5 && player.scrollCount >= 3)
    {
        player.scrollCount -= 3;
        buyCompetitiveStopwatch(player);
    }
    else if (choice == 6 && player.scrollCount >= 3)
    {
        player.scrollCount -= 3;
        buyShield(player);
    }
    else if (choice == 7 && player.scrollCount >= 3)
    {
        player.scrollCount -= 3;
        buyTicket(player);
    }
    else if (choice == 8 && player.scrollCount >= 1)
    {
        player.scrollCount -= 1;
        buyAbel(player);
    }
    else if (choice == 9 && player.scrollCount >= 1)
    {
        player.scrollCount -= 1;
        buyValerie(player);
    }
    else if (choice == 10 && player.scrollCount >= 1)
    {
        player.scrollCount -= 1;
        buyJacques(player);
    }
    else if (choice == 11 && player.scrollCount >= 1)
    {
        player.scrollCount -= 1;
        buyMaria(player);
    }
    else if (choice == 12 && player.scrollCount >= 5)
    {
        player.scrollCount -= 5;
        buyDigitReveal(player);
    }
    else if (choice == 13 && player.scrollCount >= 5)
    {
        player.scrollCount -= 5;
        buyPositionReveal(player);
    }
    else if (choice == 14 && player.scrollCount >= 5)
    {
        player.scrollCount -= 5;
        buyNeighborReveal(player);
    }
    else if (choice == 15 && player.scrollCount >= 5)
    {
        player.scrollCount -= 5;
        buyOrderReveal(player);
    }
    else if (choice == 16 && player.scrollCount >= 5)
    {
        player.scrollCount -= 5;
        buySpaceReveal(player);
    }
    else if (choice == 17)
    {
        cout << "\nYou exit the Gift Shop empty-handed.\n";
        player.actionCompleted = false;
    }
    else //case for when player does not have enough resources
    {
        cout << "\nYou do not have enough resources. Go buy some food to earn resources!\n";
    }
}


//---------------------+
//-------------------+ |
//SECTION 8: Special | |
//-------------------+ |
//---------------------+

//8.1 - Magic Tent: where magician is
bool visitMagicTent(Player& player)
{
    //tells user that they need to enter passcode
    cout << "\nThe tent is locked! You need to enter a 10-digit non-repeating passcode to get in!\n";
    cout << "1. Enter the passcode\n";
    cout << "2. Leave the tent\n";

    int choice = getValidInt("\nWhat do you do? (1-2): ", 1, 2);

    if (choice == 1)
    {
        //they enter passcode
        cout << "\nPASSCODE: ";
        string choice;
        cin >> choice;

        //if right, access granted
        if (choice == player.passcode)
        {
            cout << "\n\033[38;2;0;255;0mACCESS GRANTED\033[0m\n";
        }
        //if wrong, access denied
        else
        {
            cout << "\n\033[38;2;255;0;0mACCESS DENIED\033[0m\n";
            cin.ignore();
            return false;
        }
    }
    //when players choose to not enter passcode
    else
    {
        cout << "\nYou exit the tent area, hoping to find more clues.\n";
        player.actionCompleted = false;
        return false;
    }

    cout << "\nPress ENTER to continue...";
    string enter;
    getline(cin, enter);

    //continues the story if player got the passcode correct
    cout << "\n\nSCENE 1: CONFRONTATION";
    cout << "\nYou unlock the Magic Tent.";
    cout << "\nAnd an unknown person wearing all black immediately punches you in the throat.";

    //player is forced out of the tent if they do not have the shield
    if (!player.hasShield)
    {
        cout << "\nAnd you fly out of the tent!\n";
        return false;
    }

    //if the player has the shield, they can block the kidnapper's attack
    cout << "\nBut your shield blocks the attack!\n";

    cout << "\nPress ENTER to continue...";
    getline(cin, enter);

    //continues story if player used shield
    cout << "\nSCENE 2: NEGOTIATION";
    cout << "\nYOU: Why did you kidnap Gus the Great?";
    cout << "\nKIDNAPPER: I just wanted a ticket to see his show. Do you have a spare?";

    //player is forced out of tent if they don't have ticket
    if (!player.hasTicket)
    {
        cout << "\nYOU: No I don't.";
        cout << "\nAs a result, the kidnapper drop kicks you out of the tent!\n";
        return false;
    }

    //continues story if player used ticket
    cout << "\nYOU: Yes I do. Here it is.";
    cout << "\nYou give the kidnapper your ticket.";
    cout << "\nKIDNAPPER: Thank you so much! I am sorry, and I will let go of Gus now.\n";

    cout << "\nPress ENTER to continue...";
    getline(cin, enter);

    //player saves Gus
    cout << "\nSCENE 3: CONGRATULATIONS " << player.name << "!";
    cout << "\nYou saved Gus, and the show continues!";
    cout << "\nGus rewards you with VIP tickets!\n";
    player.savedGus = true;

    return true; //causes the game to be over now that Gus has been saved
}

//8.2 - Inventory: where player views their items
void viewInventory(Player& player)
{
    cout << "\n----------------- INVENTORY -----------------\n";

    //view food stand clues
    cout << "\nFOOD STAND CLUES (" << player.foodClues.size() << "/8)\n";
    if (player.foodClues.empty())
    {
        cout << "- You have not discovered any Food Stand Clues yet.\n";
    }
    else
    {
        for (int i = 0; i < player.foodClues.size(); ++i)
        {
            cout << i + 1 << ". " << player.foodClues[i] << "\n";
        }
    }

    //view ride clues
    cout << "\nRIDE CLUES (" << player.rideClues.size() << "/14)\n";
    if (player.rideClues.empty())
    {
        cout << "- You have not discovered any Ride Clues yet.\n";
    }
    else
    {
        for (int i = 0; i < player.rideClues.size(); ++i)
        {
            cout << i + 1 << ". " << player.rideClues[i] << "\n";
        }
    }

    //view premium clues
    cout << "\nPREMIUM CLUES (" << player.premiumClues.size() << "/5)\n";
    if (player.premiumClues.empty())
    {
        cout << "- You have not discovered any Premium Clues yet.\n";
    }
    else
    {
        for (int i = 0; i < player.premiumClues.size(); ++i)
        {
            cout << i + 1 << ". " << player.premiumClues[i] << "\n";
        }
    }

    //view items
    cout << "\nITEMS (" << player.items.size() << "/7)\n";
    if (player.items.empty())
    {
        cout << "- You have not unlocked any Items yet.\n";
    }
    else
    {
        for (int i = 0; i < player.items.size(); ++i)
        {
            cout << i + 1 << ". " << player.items[i] << "\n";
        }
    }

    //view contacts
    cout << "\nCONTACTS (" << player.contacts.size() << "/4)\n";
    if (player.contacts.empty())
    {
        cout << "- You have not unlocked any Contacts yet.\n";
    }
    else
    {
        int i = 1;
        for (auto& c : player.contacts)
        {
            cout << i << ". " << c.first << " (" << c.second.second << ")\n";
            ++i;
        }
    }

    //this is not a real move so player can check as much as they want
    player.actionCompleted = false;
}

//------------------+
//----------------+ |
//SECTION 9: Main | |
//----------------+ |
//------------------+

int main()
{
    //ensures that every generated number is truly random based on the time
    srand(time(0));

    //pool of food clues, not all will be in play during a full game
    vector<string> foodClueSet1 =
    {
        "90210", "3500"
    };
    vector<string> foodClueSet2 =
    {
        "Gus was so bad at archery, he would always aim at the left edge.",
        "Gus was so bad at archery, he would always aim at the right edge."
    };
    vector<int> foodClueSet3 =
    {
        3, 5, 7, 10
    };
    vector<string> foodClueSet4 =
    {
        "Gus was so impatient, he would react too early.",
        "Gus had a slow reaction speed of above 5 seconds."
    };
    vector<string> foodClueSet5 =
    {
        "Gus liked to draw out the letter \"G\" wherever he could.",
        "Gus liked to draw the outline of a heart wherever he could.",
        "Gus liked to draw a checkerboard pattern wherever he could."
    };
    vector<string> foodClueSet6 =
    {
        "I get those goosebumps every time",
        "Don't you open up that window",
        "Twelve more hours left for us in a day",
        "When the night crawls",
        "Four-by-four I tint my windows up",
        "Wait, I been up tourin' state",
        "For this life I cannot change",
        "Sundays in the city movin' slow",
        "In that 90210, 90210, looking for that alley",
        "To win the retreat, we all in too deep",
        "She saw my eyes, she know I'm gone",
        "The career's more at stake when you in your prime",
        "Ice water, turned Atlantic",
        "I let it slide when I really shoulda slid",
        "We pulled out the feathers for this type of weather",
        "Tell me, is you still up?",
        "We see the hype outside, right from the house",
        "Rollin', rollin', rollin', got me stargazin'",
        "No you can't say if I'm mad or not",
        "6'1\", on the money, 9'2\"",
        "Never will I cheat on you, never will I commit treason",
        "Serotonin, that's a cool type mix",
        "Oh my, what a wonderful time",
        "Always thought T was a rex",
        "Custom the things, custom the wings",
        "I don't want your apple pie, mama",
        "Let me tell you a lil' story 'bout this side",
        "I stick to the code, binary like I's and O's",
        "When the day gets brighter, the night gets nighter",
        "Chain on ice cream, can't melt",
        "I was just there at the function"
    };
    vector<string> foodClueSet7 =
    {
        "When singing songs, Gus always liked to shout his lyrics.",
        "Gus always believed that the scanning game hid something darker."
    };
    vector<string> foodClueSet8 =
    {
        "One day, Gus hopes to beat every carnival game at least once.",
        "One day, Gus hopes to beat any carnival game at least 7 times."
    };

    //establish random clues from the sets
    int foodClue1Index = rand() % foodClueSet1.size();
    int foodClue2Index = rand() % foodClueSet2.size();
    int foodClue3Index = rand() % foodClueSet3.size();
    int foodClue4Index = rand() % foodClueSet4.size();
    int foodClue5Index = rand() % foodClueSet5.size();
    int foodClue6Index = rand() % foodClueSet6.size();
    int foodClue7Index = rand() % foodClueSet7.size();
    int foodClue8Index = rand() % foodClueSet8.size();

    //finalize these clues into a string
    string foodClue1 = "Gus could only memorize the digits: " + foodClueSet1[foodClue1Index];
    string foodClue2 = foodClueSet2[foodClue2Index];
    string foodClue3 = "The highest number Gus could count up to was: " + to_string(foodClueSet3[foodClue3Index]);
    string foodClue4 = foodClueSet4[foodClue4Index];
    string foodClue5 = foodClueSet5[foodClue5Index];
    string foodClue6 = "Gus' favorite song lyric to sing was: " + foodClueSet6[foodClue6Index];
    string foodClue7 = foodClueSet7[foodClue7Index];
    string foodClue8 = foodClueSet8[foodClue8Index];

    //generate random password
    vector<int> digits = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (int i = 9; i > 0; --i)
    {
        int j = rand() % (i + 1);
        swap(digits[i], digits[j]);
    }
    string passcode;
    for (int d : digits)
        passcode += to_string(d);

    //establish ride clue values based on the password
    int rideClue1Digit = rand() % 10;
    string rideClue1Half = (passcode.find(to_string(rideClue1Digit)) < 5) ? "first" : "second";

    int rideClue2Digit;
    do
    {
        rideClue2Digit = rand() % 10;
    } while (rideClue2Digit == rideClue1Digit);
    string rideClue2Half = (passcode.find(to_string(rideClue2Digit)) < 5) ? "first" : "second";

    int rideClue3Count = 0;
    for (int i = 0; i < 5; ++i)
        if ((passcode[i] - '0') % 2 == 1)
            ++rideClue3Count;

    int rideClue4Count = 0;
    for (int i = 0; i < 5; ++i)
        if ((passcode[i] - '0') >= 5)
            ++rideClue4Count;

    int rideClue5Sum = 0;
    for (int i = 0; i < 5; ++i)
        rideClue5Sum += (passcode[i] - '0');

    int rideClue6Pos = rand() % 10;
    string rideClue6Parity = ((passcode[rideClue6Pos] - '0') % 2 == 1) ? "odd" : "even";

    int rideClue7Pos = rand() % 10;
    string rideClue7Range = ((passcode[rideClue7Pos] - '0') >= 5) ? "at least 5" : "no more than 4";

    int rideClue8Digit;
    do
    {
        rideClue8Digit = rand() % 10;
    } while (rideClue8Digit == rideClue1Digit || rideClue8Digit == rideClue2Digit);
    string rideClue8Half = (passcode.find(to_string(rideClue8Digit)) < 5) ? "first" : "second";

    int rideClue9DigitA, rideClue9DigitB;
    if (rand() % 2)
    {
        rideClue9DigitA = passcode[0] - '0';
        rideClue9DigitB = passcode[9] - '0';
    }
    else
    {
        rideClue9DigitA = passcode[9] - '0';
        rideClue9DigitB = passcode[0] - '0';
    }

    int rideClue10Count = 0;
    for (int i = 3; i <= 6; ++i)
        if ((passcode[i] - '0') % 2 == 1)
            ++rideClue10Count;

    int rideClue11Count = 0;
    for (int i = 3; i <= 6; ++i)
        if ((passcode[i] - '0') >= 5)
            ++rideClue11Count;

    int rideClue12Sum = 0;
    for (int i = 3; i <= 6; ++i)
        rideClue12Sum += (passcode[i] - '0');

    int rideClue13Pos = rand() % 9 + 1;
    string rideClue13Dir = ((passcode[rideClue13Pos] - '0') > (passcode[rideClue13Pos - 1] - '0')) ? "higher" : "lower";

    int rideClue14Pos;
    do
    {
        rideClue14Pos = rand() % 9; 
    } while (rideClue14Pos == rideClue13Pos);
    string rideClue14Dir = ((passcode[rideClue14Pos] - '0') > (passcode[rideClue14Pos + 1] - '0')) ? "higher" : "lower";

    //list of ride clues
    vector<string> rideClueSet1 =
    {
        "The digit " + to_string(rideClue1Digit) + " is in the " + rideClue1Half + " half of the sequence.",
        "The digit " + to_string(rideClue2Digit) + " is in the " + rideClue2Half + " half of the sequence.",
        rideClue3Count == 1 ? 
            "In the first half, only 1 digit is odd." : 
            "In the first half, only " + to_string(rideClue3Count) + " digits are odd.",
        rideClue4Count == 1 ? 
            "In the first half, only 1 digit has a value of at least 5." : 
            "In the first half, only " + to_string(rideClue4Count) + " digits have a value of at least 5.",
        "The sum of the digits in the sequence's first half is " + to_string(rideClue5Sum) + ".",
        "The digit at position " + to_string(rideClue6Pos + 1) + " is " + rideClue6Parity + ".",
        "The digit at position " + to_string(rideClue7Pos + 1) + " is " + rideClue7Range + "."
    };

    vector<string> rideClueSet2 =
    {
        "The digit " + to_string(rideClue8Digit) + " is in the " + rideClue8Half + " half of the sequence.",
        to_string(rideClue9DigitA) + " or " + to_string(rideClue9DigitB) + " is the first digit. The other is the last digit.",
        rideClue10Count == 1 ? 
            "In the middle four positions, only 1 digit is odd." :
            "In the middle four positions, only " + to_string(rideClue10Count) + " digits are odd.",
        rideClue11Count == 1 ? 
            "In the middle four positions, only 1 digit has a value of at least 5." :
            "In the middle four positions, only " + to_string(rideClue11Count) + " digits have a value of at least 5.",
        "The sum of the middle four digits is " + to_string(rideClue12Sum) + ".",
        "The digit at position " + to_string(rideClue13Pos + 1) + " is " + rideClue13Dir + " than the digit before it.",
        "The digit at position " + to_string(rideClue14Pos + 1) + " is " + rideClue14Dir + " than the digit after it."
    };

    //initialize game booths, food stands, rides, and special
    GameBooth typingGame("TYPING GAME", "A booth where you test your typing skills.\n", 3);
    GameBooth countingGame("COUNTING GAME", "A booth where you test your counting skills.\n", 3);
    GameBooth timeGuessingGame("TIME GUESSING GAME", "A booth where you test your timing skills.\n", 6);
    GameBooth reactionGame("REACTION GAME", "A booth where you test your reaction skills.\n", 18);
    GameBooth colorGame("COLOR GAME", "A booth where you test your color perception skills.\n", 6);
    GameBooth memoryGame("MEMORY GAME", "A booth where you test your memory skills.\n", 15);
    GameBooth archeryGame("ARCHERY GAME", "A booth where you test your archery skills.\n", 9);
    GameBooth trackingGame("TRACKING GAME", "A booth where you test your tracking skills.\n", 6);
    GameBooth gridGame("GRID GAME", "A booth where you test your spatial memory skills.\n", 15);
    GameBooth scanningGame("SCANNING GAME", "A booth where you test your scanning skills.\n", 12);
    FoodStand chickenWings("CHICKEN WING STAND", "A stand selling chicken wings.\n");
    FoodStand boba("BOBA STAND", "A stand selling boba.\n");
    Ride houseOfBalloons("HOUSE OF BALLOONS", "A maze filled with balloons.\n", 5);
    Ride nightcrawler("NIGHTCRAWLER", "A roller coaster that takes you up and down.\n", 5);
    Location giftShop("GIFT SHOP", "Cash out on big prizes!\n");
    Location magicTent("MAGIC TENT", "What's in here?\n");
    Location inventory("INVENTORY", "Let's see what I have...\n");

    //create the player and their location pointer
    Player player;
    cout << "Welcome to the Carnival Mystery!\n"
        << "Enter your name: ";
    getline(cin, player.name);
    Location* currentLocation = nullptr;
    player.passcode = passcode;

    //introduces the game to the player
    cout << "\n" << string(53, '-') << " INTRODUCTION " << string(53, '-') << "\n";
    cout << "Welcome " << player.name << "!\n\n";

    cout << "STORYLINE\n";
    cout << "* Gus the Great is the carnival's star magician.\n";
    cout << "* And he has disappeared just hours before the carnival's biggest magic show is supposed to start.\n";
    cout << "* Without him, the show cannot begin, and the carnival risks losing thousands of dollars.\n";
    cout << "* Your mission is to explore the carnival, collect clues, and find Gus before the show begins!\n";

    cout << "\nPress ENTER to continue...";
    string enter;
    getline(cin, enter);

    //displays rules
    cout << "\nIMPORTANT RULES\n";
    cout << "* You have 10 hours to find Gus\n";
    cout << "* Everytime you visit a location and complete its intended action, 5 minutes will pass\n";
    cout << "* Checking inventory or backing out of a location will not consume 5 minutes\n";

    cout << "\nPress ENTER to continue...";
    getline(cin, enter);

    cout << "\n";

    cout << string(120, '-') << "\n";

    bool gameOver = false;

    //This gameplay loops until the player has saved Gus
    while (!gameOver)
    {
        //calculate total win count
        player.winCount = typingGame.winCount + countingGame.winCount + timeGuessingGame.winCount + reactionGame.winCount + colorGame.winCount +
			memoryGame.winCount + archeryGame.winCount + trackingGame.winCount + gridGame.winCount + scanningGame.winCount;

        //calculate time
        int totalMin = player.clock % (12 * 60);
        int hour = (totalMin / 60) % 12;
        if (hour == 0) hour = 12;
        int min = totalMin % 60;
        string ampm;
        if (player.clock < 720)
            ampm = "AM";
        else
            ampm = "PM";

        //colors the time to give sense of urgency
        double t = (double)(player.clock - 720) / (1320 - 720);
        if (t < 0) t = 0;
        if (t > 1) t = 1;

        int r = (int)(t * 255);
        int g = (int)((1.0 - t) * 255);
        int b = 0;

        //display player status and important information
        cout << "\n--------------- PLAYER STATUS ---------------\n";
        cout << left << setw(17) << "Current Time:     "
            << "\033[38;2;" << r << ";" << g << ";" << b << "m"
            << right << setfill(' ') << setw(2) << hour << ":" << setfill('0') << setw(2) << min << " " << ampm
            << "\033[0m\n";
        cout << left << setw(17) << "Gus' Show Begins: " << "\033[38;2;255;0;0m" << "10:00 PM\033[0m\n\n";

        cout << "Tokens: " << player.tokenCount << "\n";
        cout << "Scrolls: " << player.scrollCount << "\n";

        //game ends if player runs out of time
        if (player.clock >= 1320)
        {
            gameOver = true;
        }
        else
        {
            player.actionCompleted = true;

            //shows location options at the start of every round
            cout << "\n----------------- LOCATIONS -----------------\n";
            cout << "GAMES                       DIFFICULTY   WINS\n";
            cout << "1. Typing Booth             *            " << typingGame.winCount << "\n";
            cout << "2. Counting Booth           *            " << countingGame.winCount << "\n";
            cout << "3. Time Guessing Booth      **           " << timeGuessingGame.winCount << "\n";
            cout << "4. Reaction Booth           ******       " << reactionGame.winCount << "\n";
            cout << "5. Color Booth              **           " << colorGame.winCount << "\n";
            cout << "6. Memory Booth             *****        " << memoryGame.winCount << "\n";
            cout << "7. Archery Booth            ***          " << archeryGame.winCount << "\n";
            cout << "8. Tracking Booth           **           " << trackingGame.winCount << "\n";
            cout << "9. Grid Booth               *****        " << gridGame.winCount << "\n";
            cout << "10. Scanning Booth          ****         " << scanningGame.winCount << "\n\n";

            cout << "FOOD\n";
            cout << "11. Chicken Wing Stand\n";
            cout << "12. Boba Stand\n\n";

            cout << "RIDES\n";
            cout << "13. House Of Balloons\n";
            cout << "14. Nightcrawler\n\n";

            cout << "SPECIAL\n";
            cout << "15. Gift Shop\n";
            cout << "16. Magic Tent\n";
            cout << "17. Inventory\n";

            //allows players to choose where to go
            int choice = getValidInt("\nWhere would you like to go? (1-18): ", 1, 18);
            cout << "\n" << string(45, '-') << "\n";

            //based on user input, assign the player's location pointer to a certain location
            //start the location's function
            if (choice == 1)
            {
                currentLocation = &typingGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << typingGame.tokensRewarded << " tokens\n";
                playTypingGame(player, typingGame, foodClue6, foodClue6Index, foodClue7Index, foodClueSet6);
            }
            else if (choice == 2)
            {
                currentLocation = &countingGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << countingGame.tokensRewarded << " tokens\n";
                playCountingGame(player, countingGame, foodClueSet3, foodClue3Index);
            }
            else if (choice == 3)
            {
                currentLocation = &timeGuessingGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << timeGuessingGame.tokensRewarded << " tokens\n";
                playTimeGuessingGame(player, timeGuessingGame);
            }
            else if (choice == 4)
            {
                currentLocation = &reactionGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << reactionGame.tokensRewarded << " tokens\n";
                playReactionGame(player, reactionGame, foodClue4Index);
            }
            else if (choice == 5)
            {
                currentLocation = &colorGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << colorGame.tokensRewarded << " tokens\n";
                playColorGame(player, colorGame);
            }
            else if (choice == 6)
            {
                currentLocation = &memoryGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << memoryGame.tokensRewarded << " tokens\n";
                playMemoryGame(player, memoryGame, foodClueSet1, foodClue1Index);
            }
            else if (choice == 7)
            {
                currentLocation = &archeryGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << archeryGame.tokensRewarded << " tokens\n";
                playArcheryGame(player, archeryGame, foodClue2Index);
            }
            else if (choice == 8)
            {
                currentLocation = &trackingGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << trackingGame.tokensRewarded << " tokens\n";
                playTrackingGame(player, trackingGame);
            }
            else if (choice == 9)
            {
                currentLocation = &gridGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << gridGame.tokensRewarded << " tokens\n";
                playGridGame(player, gridGame, foodClue5Index);
            }
            else if (choice == 10)
            {
                currentLocation = &scanningGame;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "REWARD: " << scanningGame.tokensRewarded << " tokens\n";
                playScanningGame(player, scanningGame, foodClue7Index);
            }
            else if (choice == 11)
            {
                currentLocation = &chickenWings;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                eatChickenWings(player, foodClue1, foodClue2, foodClue3, foodClue4);
            }
            else if (choice == 12)
            {
                currentLocation = &boba;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                eatBoba(player, foodClue5, foodClue6, foodClue7, foodClue8);
            }
            else if (choice == 13)
            {
                currentLocation = &houseOfBalloons;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "COST: " << houseOfBalloons.cost << " tokens\n";
                rideHouseOfBalloons(player, houseOfBalloons, rideClueSet1);
            }
            else if (choice == 14)
            {
                currentLocation = &nightcrawler;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                cout << "COST: " << nightcrawler.cost << " tokens\n";
                rideNightcrawler(player, nightcrawler, rideClueSet2);
            }
            else if (choice == 15)
            {
                currentLocation = &giftShop;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                visitGiftShop(player);
            }
            else if (choice == 16)
            {
                currentLocation = &magicTent;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                if (visitMagicTent(player)) //This if condition becomes true when Gus has been saved, making the game end
                {
                    gameOver = true;
                }
            }
            else if (choice == 17)
            {
                currentLocation = &inventory;
                cout << "\nYou arrive at...\n\n" << currentLocation->name << ": " << currentLocation->description;
                viewInventory(player);
            }

            //calculate some win counts for the food clues
            if (typingGame.winCount >= 1 && countingGame.winCount >= 1 && timeGuessingGame.winCount >= 1 && reactionGame.winCount >= 1 && colorGame.winCount >= 1 &&
                memoryGame.winCount >= 1 && archeryGame.winCount >= 1 && trackingGame.winCount >= 1 && gridGame.winCount >= 1 && scanningGame.winCount >= 1)
            {
                player.completed1WinEachGame = true;
            }
            if (typingGame.winCount >= 7 || countingGame.winCount >= 7 || timeGuessingGame.winCount >= 7 || reactionGame.winCount >= 7 || colorGame.winCount >= 7 ||
                memoryGame.winCount >= 7 || archeryGame.winCount >= 7 || trackingGame.winCount >= 7 || gridGame.winCount >= 7 || scanningGame.winCount >= 7)
            {
                player.completed7WinsFor1Game = true;
            }

            //if player completes food secret, player gets rewarded accordingly
            if (player.hasFoodClue8 && foodClue8Index == 0 && player.completed1WinEachGame)
            {
                if (!player.completedFoodClue8)
                {
                    player.completedFoodClue8 = true;
                    cout << "\nSECRET UNLOCKED: +5 Scrolls (For winning each game at least once)\n";
                    player.scrollCount += 5;
                }
            }
            if (player.hasFoodClue8 && foodClue8Index == 1 && player.completed7WinsFor1Game)
            {
                if (!player.completedFoodClue8)
                {
                    player.completedFoodClue8 = true;
                    cout << "\nSECRET UNLOCKED: +5 Scrolls (For winning any game 7 times)\n";
                    player.scrollCount += 5;
                }
            }

            //makes the player press enter to continue the next round, allowing for proper pacing
            cout << "\nPress ENTER to continue...";
            getline(cin, enter);

            //separates each round, making it easier to read
            cout << "\n" << string(120, '-') << "\n";

            //only happens if the player completes a meaningful round
            if (player.actionCompleted)
            {
                //player uses 5 minutes, so increment
                player.clock += 5;
            }
        }
    }

    //if player saved Gus
    if (player.savedGus)
    {
        cout << "\nYOU WIN\n";
    }
    //if player ran out of time
    else
    {
        cout << "\nGAME OVER: You are out of time. You failed to save Gus before the show began, and his whereabouts forever remain a mystery.\n";
    }

    return 0;
}