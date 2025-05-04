#include <Servo.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

// ---------------------- Servo Definitions ----------------------
Servo servos[6]; // servo1 to servo6 mapped as servos[0] to servos[5]

// ---------------------- System Variables ----------------------
int reset = 0;
int waitTime = 10;
int base_fret = 20;
double delayTime = 1.0; // seconds between chords
bool isPlaying = false;
int playIndex = 0;

vector<string> possibleChords = {
  "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#",
  "Am", "A#m", "Bm", "Cm", "C#m", "Dm", "D#m", "Em", "Fm", "F#m",
  "Gm", "G#m"
};

map<string, int> stringNameToNumber = {
  {"Elow", 1}, {"A", 2}, {"D", 3}, {"G", 4}, {"B", 5}, {"Ehigh", 6}
};

int fretPositions[5] = {0, 30, 60, 90, 120}; //fret positions for 5 frets in degrees

// chordScore stores chords to play
vector<string> chordScore;

// noteMap: maps chord names to vector of (string, fret)
map<string, vector<pair<int, int>>> noteMap;

// ---------------------- Setup ----------------------
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 6; i++) {
    servos[i].attach(i + 3); // Use pins 3-8 (you can adjust)
    servos[i].write(reset);
  }
  Serial.println("Ready. Type 'help' for commands.");
}

// ---------------------- Helper Functions ----------------------
void printHelp() {
  Serial.println("Available commands:");
  Serial.println("delay <seconds>");
  Serial.println("addToScore <chord>");
  Serial.println("addChord <chord> <str> <fret> ... (up to 6 pairs)");
  Serial.println("play [-r]");
  Serial.println("pause");
  Serial.println("reset");
  Serial.println("score");
  Serial.println("currentChord");
  Serial.println("exit");
}

void printChordScore() {
  Serial.println("Chord score:");
  for (const auto& chord : chordScore) {
    Serial.print(chord); Serial.print(" | ");
  }
  Serial.println();
}

void resetScore() {
  chordScore.clear();
  isPlaying = false;
  playIndex = 0;
  Serial.println("Chord score reset.");
}

void addChordToScore(String chord) {
  chordScore.push_back(chord.c_str());
  Serial.print("Added to score: "); Serial.println(chord);
}

void addChord(String chord, vector<pair<int, int>> notes) {
  noteMap[chord] = notes;
  Serial.print("Defined chord: "); Serial.println(chord.c_str());
}

void playChord(String chord) {
  if (noteMap.find(chord.c_str()) == noteMap.end()) {
    Serial.println("Chord not defined!");
    return;
  }

  Serial.print("Playing chord: "); Serial.println(chord);
  for (auto [str, fret] : noteMap[chord.c_str()]) {
    if (str >= 1 && str <= 6) {
      servos[str - 1].write(base_fret * fret);
      delay(waitTime);
    }
  }
  delay((int)(delayTime * 1000));
}

// Updated playChord function
void playChord(String chord) {
  if (noteMap.find(chord.c_str()) == noteMap.end()) {
    Serial.println("Chord not defined!");
    return;
  }

  Serial.print("Playing chord: ");
  Serial.println(chord);

  // Iterate through the notes in the chord
  for (auto [str, fret] : noteMap[chord.c_str()]) {
    if (str >= 1 && str <= 6) {
      if (fret >= 0 && fret < 5) { // Ensure fret is within range
        servos[str - 1].write(fretPositions[fret]); // Set servo to fret position
        delay(waitTime); // Wait for the servo to move
      } else {
        Serial.print("Invalid fret: ");
        Serial.println(fret);
      }
    } else {
      Serial.print("Invalid string: ");
      Serial.println(str);
    }
  }

  delay((int)(delayTime * 1000)); // Delay between chords
}

// ---------------------- Command Parser ----------------------
void parseCommand(String input) {
  input.trim();
  if (input.length() == 0) return;

  vector<String> args;
  int idx = 0;
  while (idx >= 0) {
    int next = input.indexOf(' ', idx);
    if (next == -1) {
      args.push_back(input.substring(idx));
      break;
    }
    args.push_back(input.substring(idx, next));
    idx = next + 1;
  }

  String cmd = args[0];
  cmd.trim();

  if (cmd == "help") {
    printHelp();
  } else if (cmd == "delay" && args.size() >= 2) {
    delayTime = args[1].toFloat();
    Serial.print("Delay time set to "); Serial.print(delayTime); Serial.println(" seconds.");
  } else if (cmd == "addToScore" && args.size() >= 2) {
    addChordToScore(args[1]);
  } else if (cmd == "addChord" && args.size() >= 4) {
    vector<pair<int, int>> notes;
    for (int i = 2; i + 1 < args.size(); i += 2) {
      int str = args[i].toInt();
      int fret = args[i + 1].toInt();
      notes.emplace_back(str, fret);
    }
    addChord(args[1], notes);
  } else if (cmd == "play") {
    if (args.size() >= 2 && args[1] == "-r") {
      playIndex = 0;
    }
    isPlaying = true;
  } else if (cmd == "pause") {
    isPlaying = false;
  } else if (cmd == "reset") {
    resetScore();
  } else if (cmd == "score") {
    printChordScore();
  } else if (cmd == "currentChord") {
    if (playIndex < chordScore.size()) {
      Serial.print("Current: ");
      Serial.println(chordScore[playIndex]);
    } else {
      Serial.println("Not playing.");
    }
  } else {
    Serial.println("Unknown command.");
  }
}

// ---------------------- Loop ----------------------
String serialBuffer = "";

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      parseCommand(serialBuffer);
      serialBuffer = "";
    } else {
      serialBuffer += c;
    }
  }

  if (isPlaying && playIndex < chordScore.size()) {
    playChord(chordScore[playIndex]);
    playIndex++;
    if (playIndex >= chordScore.size()) {
      isPlaying = false;
      Serial.println("Playback finished.");
    }
  }
}
