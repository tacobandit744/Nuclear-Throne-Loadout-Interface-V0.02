////////////////////////////////////////////////////////////////////
// Made by "Tacobandit"                                           //
// 02 February 2016                                               //
// Version 0.02                                                   //
////////////////////////////////////////////////////////////////////
// Second attempt at creating a save editing UI for Nuclear Throne//
// This Second prototype has a totally text based UI              //
// It allows for the user to make multiple edits                  //
// and adds exception handling :D								  //
////////////////////////////////////////////////////////////////////

// Ignore the fruit names, I like naming things after fruit :D

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void editValue(ifstream &reader, ofstream &newSave, string name) {
	cout << "Enter the number of the weapon you wish to replace. Refer to the NT weapon guide." << endl;
	string weapon;
	cin >> weapon;
	string core;
	string pit = "\"charData_" + name + "\":"; // Finds the location of the character info we want to edit
	string relish = "\"cgld\": ";
	getline(reader, core); // core holds the entire save file
	string fruitbasket = core; // fruitbasket will eventually be the new save file
	std::size_t kiwi = core.find(pit); // kiwi is the character location of the start of the character info we are looking for
	std::size_t finder; // This finds the first "cgld" value in the string
	unsigned int position = 0; // This will hold the "cgld" position for the character we want
	while (kiwi > position) {
		finder = core.find(relish);
		if (finder != std::string::npos) {
			if (kiwi > position) {
				if (finder == 0) { // if "cgld" is literally the first thing in the string
					position += 8; 
					core = core.substr(finder+8, core.length() + 1); // slice off that "cgld"
				}
				else {
					position += finder;
					core = core.substr(finder, core.length() + 1); // slice off another chunk
				}
			}
			else {
				position += finder; // we now have the location of the character specific "cgld" field
			}
		}
	}
	string replacement = relish + weapon + "."; // eg "cgld": 100.
	while (replacement.length() < 18) { // appends extra 0's for simplicity's sake.
		replacement += "0";
	}
	replacement += ",";
	fruitbasket.replace(position, 19, replacement);
	newSave << fruitbasket; // new save file created
//	reader.seekg(0L, ios::beg);
//	newSave.seekp(0L, ios::beg);
}

void displayFile(fstream &reader) { // I don't remember where I was going with this
	string output;
	while (!reader.eof()) {
		getline(reader, output);
		cout << output;
	}
}

bool inputChecker(string input) {
	std::ostringstream converter; // used to convert string to int
	string coconut; // for comparison
	bool sentinel = false;
	size_t finder;
	for (int i = 1; i < 13; i++) {
		converter << i;
		coconut = converter.str(); // puts the int into coconut
		finder = coconut.find(input);
		if (finder != std::string::npos && coconut.length() == input.length()) { // if we have a match
			sentinel = true;
			break;
		}
		converter.str(""); // empties converter
	}
	return sentinel;
}

int main() {
	ifstream reader("oldSave.sav", ios::in); // This reads the old save file
	ifstream tempSave; // This will be the intermediate save file that is read, should the user choose to make multiple edits.
	ofstream tempHold; // This will be the intermediate save file that is written, should the user choose to make multiple edits.
	ofstream newSave("nuclearthrone.sav", ios::out); // This will be the final save file
	if (reader) {
		string charSelect = ""; // Stores the number that represents the character
		size_t finder;
		int multiChange = 0; // used as a check to determine if tempSave.sav needed
		do{
			cout << "Hello. What character would you like to edit?\n\n";
			cout << "1 - fish\n2 - crystal\n3 - eyes\n4 - melting\n5 - plant\n6 - YV\n7 - Steroids\n" <<
				"8 - robot\n9 - chicken\n10 - rebel\n11 - horror\n12 - rogue\n";
			cout << "type \'exit\' to exit the program\n";
			cin >> charSelect;
			cin.ignore(); // Ignores the enter key input
			finder = charSelect.find("exit");
			if (finder != std::string::npos) {
				cout << "bye!";
			}
			else if (!inputChecker(charSelect)) { // if not correct input
				cout << "That's not a valid character. Try again.\n";
			}
			else {
				if (multiChange > 0){
					tempSave.open("tempSave.sav", ios::in);
					newSave.open("nuclearthrone.sav", ios::out);
					editValue(tempSave, newSave, charSelect);
					newSave.close();
					tempSave.close();
					tempSave.open("nuclearthrone.sav", ios::in);
					string holder;
					getline(tempSave, holder);
					tempHold.open("tempSave.sav", ios::out);
					tempHold << holder; // tempSave.sav now holds intermediate save file
					tempHold.close();
					tempSave.close();
				}
				else {
					editValue(reader, newSave, charSelect);
					reader.close();
					newSave.close();
					tempSave.open("nuclearthrone.sav", ios::in);
					string holder;
					getline(tempSave, holder);
					tempHold.open("tempSave.sav", ios::out);
					tempHold << holder; // tempSave.sav now holds intermediate save file
					tempSave.close();
					tempHold.close();
					multiChange++;
				}
			}
		} while (finder == std::string::npos);
//		reader.close();
//		newSave.close();
		return 0;
	}
	else {
		cout << "Error! File not opened successfully!";
		int holdup;
		cin >> holdup;
	}
	return 0;
}
