#include <iostream>
#include <string>    
#include <vector>
#include <hashmap>
#include <fstream>
#include <sstream>
using namespace std;


hashmap <string, int> stringNametoNumber = { //map of string names to their numbers
    {"Elow", 1}, {"A", 2}, {"D", 3}, {"G", 4}, {"B", 5}, {"Ehigh", 6}
};
vector <string> possibleChords = { //all possible chords - use as keys
    "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#",
    "Am", "A#m", "Bm", "Cm", "C#m", "Dm", "D#m", "Em", "Fm", "F#m", 
    "Gm", "G#m"
};


double delayTime = 1.0; //time to wait between notes in seconds - default half a second
bool isPlaying = false; //flag to check if the program is    playing a chord or not
int index = 0; //index to keep track of the current chord being played

//note - noteMap uses string noteName as key, value as [string, fingerPosition]
hashmap <string, [int, int]> noteMap = {}; //map of notes to their fret positions
hashmap <string, vector<string>> chordMap = {}; //map of chords to their notes
vector <string> chordScore = {}; //list of chords to autoplay

//cli stuff - use for parsing, delimiter
char betweenArgs = ' ';
char flagIndicator = '-';
char argIndicator = '/';


/*
*print info
*/

void printHelp() {
    cout << "Available commands:" << endl;
    cout << "delay <time> - set the delay time between notes" << endl;
    cout << "chords - print the available chords" << endl;
    cout << "allChords - print the list of all chords" << endl;
    cout << "currentChord - print the current chord being played" << endl;
    cout << "chordNear <value> - print the chords near the given value" << endl;
    cout << "score - print the list of chords to be played" << endl;
    cout << "addChord <chordName> <stringToPlay> <fretToPlay> - add a chord to the list of chords to be played" << endl;
    cout << "addToScore <chord> - add a chord to the list of chords to be played" << endl;
    cout << "deleteChord <index> - delete a chord at the specified index from the list of chords to be played" << endl;
    cout << "reset - reset the playing flag and clear the list of chords" << endl;
    cout << "play - start playing from the beginning of the song or continue playing from where it left off" << endl;
    cout << "   -r - start playing from the beginning of the song" << endl;
    cout << "pause - stop playing the chord" << endl;
    cout << "help - print the help message" << endl;
    cout << "exit - exit the program" << endl;
}

void printAvailableChords() { //these are all possible chords available to play
    cout << "Available chords:" << endl;
    for (const auto& chord : chordScore) {
        cout << chord << " ";
    }
    cout << endl;
}

void printChordsList() //this is the specific list of chords
{
    cout << "Available chords:" << endl;
    for (const auto& chord : possibleChords) {
        cout << chord << " | ";
    }
    cout << endl;
}

void printCurrentChord() { //print the current chord being played
    if (isPlaying) {
        cout << "Currently playing: " << chordScore[index] << endl;
    } else {
        cout << "Not currently playing any chord." << endl;
    }
}

void printChordsNear(int n) { //print the chords in a range from -n to n
    cout << "Chords near " << value << ":" << endl;
    for(int i = index - n; i <= index + n; i++) { //print the chords in the range
        if (i >= 0 && i < chordScore.size()) { //check for valid index
            cout << chordScore[i] << " | ";
        }
    }
}

void printChordScore() { //print the list of chords to be played
    cout << "Chord score:" << endl;
    for (const auto& chord : chordScore) {
        cout << chord << " | ";
    }
    cout << endl;
}

/*
*edit values
*/
void changeDelayTime(string newDelay) { //set time between chord changes
    delay = stod(delayGiven); //convert string to double
    if (delay < 0) { //check for negative time
        cout << "Delay time cannot be negative." << endl;
        return;
    }
    delayTime = delay;
    cout << "Delay time changed to: " << delayTime << " seconds" << endl;
}

void resetScore() { //reset the playing flag
    isPlaying = false;
    chordScore.clear(); //clear the list of chords
    cout << "Reset Score." << endl;
}

// void addToPossibleChords(int chordName, int strToPlay, int frtToPlay) 
// { //add a chord to the list of chords to be played - note must create new chord, and only then add to the list - notes added separately
//     if (chordName == "") { //check for empty chord name
//         cout << "Chord name cannot be empty." << endl;
//         return;
//     }
//     if (strToPlay < 1 || strToPlay > 6) { //check for invalid string number
//         cout << "Invalid string number. Must be between 1 and 6." << endl;
//         return;
//     }
//     if (frtToPlay < 0 || frtToPlay > 5) { //check for invalid fret number
//         cout << "Invalid fret number. Must be between 0 and 5." << endl;
//         return;
//     }
//     noteMap[chordName] = { strToPlay, frtToPlay }; //add the chord to the map of chords to their notes
//     cout << "Added chord: " << chordName << " to the list of chords to be played." << endl;
// }

void createChord(int chordName, string notesList)
{
    if (chordName == "") { //check for empty chord name
        cout << "Chord name cannot be empty." << endl;
        return;
    }
    if (notesList == "") { //check for empty notes list
        cout << "Notes list cannot be empty." << endl;
        return;
    }
    vector<string> notes; //vector to hold the notes in the chord
    stringstream ss(notesList); //use stringstream to parse the notes list
    string note; //string to hold the note
    while (ss >> note) { //get the notes from the list
        notes.push_back(note); //add the note to the vector of notes
    }
    chordMap[chordName] = notes; //add the chord to the map of chords to their notes
    cout << "Created chord: " << chordName << " with notes: " << notesList << endl;
}

void deleteChordAtIndex(int index) { //delete a chord at a specific index
    if (index < 0 || index >= chordScore.size()) { //check for invalid index
        cout << "Invalid index. Must be between 0 and " << chordScore.size() - 1 << "." << endl;
        return;
    }
    chordScore.erase(chordScore.begin() + index); //remove the chord at the specified index
    cout << "Deleted chord at index: " << index << endl;
}

void addChordToScore(string chord) { //add a chord to the list of chords to be played
    if (chord == "") { //check for empty chord name
        cout << "Chord name cannot be empty." << endl;
        return;
    }
    if (find(possibleChords.begin(), possibleChords.end(), chord) == possibleChords.end()) { //check if the chord is valid
        cout << "Invalid chord name. Must be one of the available chords." << endl;
        return;
    }
    chordScore.push_back(chord); //add the chord to the list of chords to be played
    cout << "Added chord: " << chord << " to the list of chords to be played." << endl;
}

/*
*playback functionality
*/

void startPlaying()//play from 
{
    isPlaying = true; //set the flag to true
    cout << "Started playing." <<endl;
}

void pausePlaying() { //stop playing the chord
    isPlaying = false;
    cout << "Stopped playing." << endl;
}

void continuePlaying()
{
    isPlaying = true; //set the flag to true
    cout << "Continuing to play." << endl;
}

void restartPlaying() { //restart the playing flag
    index = 0; //reset the index to 0
    isPlaying = true; //set the flag to true
    cout << "Restarted playing from the beginning of the song." << endl;
}

void playChord(string chord) { //play a chord
    //INSERT FUNCTIONALITY HERE - use the noteMap to get the fret positions and play the notes
    int positions[2] = noteMap[chord]; //get the fret positions from the map
    int stringToPlay = positions[0]; //get the string to play from the map
    int fretToPlay = positions[1]; //get the fret to play from the map
    //play the note here - use the string and fret to play the note

    cout << "Playing chord: " << chord << endl; //remove after debugging  
    //play the chord here - use the noteMap to get the fret positions and play the notes
    //use the delayTime to wait between notes
}

void parseCommand(string command) { //parse the command and execute the appropriate function
    stringstream ss(command); //use stringstream to parse the command
    string arg; //string to hold the argument
    ss >> arg; //get the first argument
    if (arg == "delay") { //check for delay command
        string delayGiven; //string to hold the delay time
        ss >> delayGiven; //get the delay time
        changeDelayTime(delayGiven); //change the delay time between notes
    } 
    else if (arg == "chords") { //check for chords command //= switch this to p for print, -chords and whatnot for what to print?
        printAvailableChords(); //print the available chords
    }
    else if(arg == "allChords") { //check for all chords command
        printChordsList(); //print the list of all chords
    }
    else if (arg == "currentChord") { //check for current chord command
        printCurrentChord(); //print the current chord being played
    } 
    else if (arg == "chordNear") { //check for chord near command
        int value; //int to hold the value to check for chords near
        ss >> value; //get the value to check for chords near
        printChordsNear(value); //print the chords in the range from -n to n
    } 
    else if (arg == "score") { //check for score command
        printChordScore(); //print the list of chords to be played
    } 
    else if (arg == "addChord") { //check for add chord command
        string chordName; //string to hold the chord name
        int strToPlay, frtToPlay; //int to hold the string and fret positions to play
        ss >> chordName >> strToPlay >> frtToPlay; //get the chord name and positions to play
        addToPossibleChords(chordName, strToPlay, frtToPlay); //add the chord to the list of chords to be played
    } 
    else if (arg == "addToScore") { //check for add to score command
        string chord; //string to hold the chord name to add to score
        ss >> chord; //get the chord name to add to score
        addChordToScore(chord); //add the chord to the list of chords to be played
    } 
    else if (arg == "deleteChord") { //check for delete chord command
        int index; //int to hold the index of the chord to delete
        ss >> index; //get the index of the chord to delete
        deleteChordAtIndex(index); //delete the chord at the specified index from the list of chords to be played
    } 
    else if (arg == "reset") { //check for reset command
        resetScore(); //reset the playing flag and clear the list of chords
    } 
    else if (arg == "play") { //check for play command - start playing from beginning of song or continue playing from where it left off
    {
        string playType; //string to hold the type of play command
        ss >> playType; //get the type of play command
        if (playType == "-r") { //check for start play command
            startPlaying(); //start playing from the beginning of the song
        } 
        else() { 
            continuePlaying(); //continue playing from where it left off
        }    
    }
    else if (arg == "pause") { //check for pause command
        pausePlaying(); //stop playing the chord
    } 
    else if (arg == "help") { //check for help command
        printHelp(); //print the help message
    }
     else if (arg == "exit") { //check for exit command
        exit(0); //exit the program
    } 
    else {
        cout << "Invalid command. Type /help for a list of available commands." << endl; //invalid command - print error message
    }
}