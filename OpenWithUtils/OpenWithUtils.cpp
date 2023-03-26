// OpenWIthUtils.cpp : Defines the entry point for the application.
//
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "OpenWithUtils.h"
#include "Windows.h"
#include "shellapi.h"
#include "winuser.h"

using namespace std;


int main(int argc, char* argv[]) {	
	if (argc != 3) {
		cout << "exit1 not enough arguments" << endl;
		return 0;
	}
	string type = argv[2];
	if (type != "cmd" && type != "cmdParent" && type!= "openWith") {
		cout << "exit2" << endl;
		return 0;
	}

	if (type == "cmd") {
		string run = "/K cd \"";
		run.append(argv[1]);
		run.append("\" && cmd");
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
	
	return 0;
}
