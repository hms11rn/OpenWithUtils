// OpenWIthUtils.cpp : Defines the entry point for the application.
//
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "OpenWithUtils.h"
#include "Windows.h"
#include "winuser.h"
#include "shellapi.h"
#include <fstream>
#include <iostream>

using namespace std;


int main(int argc, char* argv[]) {
	if (argc < 3) {
		cout << "exit1 not enough arguments" << endl;
		return 1;
	}
	string type = argv[2];
	if (type != "cmd" && type != "cmdParent" && type!= "clipboardCopy" && type != "obj") {
		cout << "exit2 incorrect type options are: [cmd, cmdParent, clipBoardCopy]" << endl;
		return 1;
	}

	if (type == "cmd") {
		string run = "/K cd \"";
		run = run + argv[1] + "\" && cmd";
		const char* cstr = run.c_str();
		ShellExecute(0, "open", "cmd", cstr, 0, SW_SHOW);
		
		
		cout << "ran command - " << run << endl;
		return 0;
	} else if (type == "cmdParent") {
		string ogFile = argv[1];
		string fileParent = ogFile.substr(0, ogFile.find_last_of("/\\"));
		string run = "/K cd \"";
		run.append(fileParent);
		run.append("\" && cmd");
		const char* cstr = run.c_str();
		ShellExecute(0, "open", "cmd", cstr, 0, SW_SHOW);
		cout << "ran command - " << run << endl;
	}
	else if (type == "clipboardCopy") {
		const char* output = argv[1];
		const size_t len = strlen(output) + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
		memcpy(GlobalLock(hMem), output, len);
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}

	// very specific to x-plane .obj (same file extension different formats, so I am using this program to run it, detect which kind of format it is, then run the apporpriate app for it.
	else if (type == "obj") {
		if (argc < 5) {
			return 1;
		}
		
		string openWithDef = argv[3];
		string openWithXP = argv[4];
		ifstream file(argv[1]);
		string line1, line2, line3;
		boolean xpObj = false; // default set to false.
		if (file.is_open()) {
			int i = 0;
			boolean breakFlag = true;
			while (file.good() && breakFlag) {
				i++;
				switch(i) {
				case 1:
					file >> line1;
					break;
				case 2:
					file >> line2;
					break;
				case 3:
					file >> line3;
					breakFlag = false;
					break;

				}
			}
			cout << line1 << endl;
			cout << line2 << endl;
			cout << line3 << endl;

			string location = argv[1];
			if (line1.length() == 1 && line2 == "800" && line3 == "OBJ") {
				cout << "Running X-Plane Object Viewer: " << openWithXP.c_str() << ("\"" + location + "\"") << endl;
				ShellExecute(nullptr, "open", openWithXP.c_str(), ("\"" + location + "\"").c_str(), nullptr, SW_HIDE);
			}
			else {
				cout << "Regular Obj" << endl;
				string locationWQuotes = "\"" + location + "\"";
				ReplaceStringInPlace(openWithDef, "%2", locationWQuotes); // %2 is replace location
				cout << openWithDef << endl;
				if (argc == 6) {
					ShellExecute(nullptr, "open", openWithDef.c_str(), ("\"" + location + "\"" + argv[5]).c_str(), nullptr, SW_HIDE);
				}
				else {
					ShellExecute(nullptr, "open", openWithDef.c_str(), ("\"" + location + "\"").c_str(), nullptr, SW_HIDE);

				}
				
			}
			return 0;
		}
	}
	return 0;
}


