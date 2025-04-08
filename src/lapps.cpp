
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std;

const int LSA_SUCCEEDED = 0;
const int LSA_FOUND     = 10;
const int LSA_NOTFOUND  = 20;

void readAppDesktop(const auto& desktopFile, const auto& execForName) {
  ifstream appfile(desktopFile.path());
  string str;
  string appName;
  string appExec;
  bool appIsHide = false;
  while (getline(appfile, str)) {
    if (str.find("GenericName") != string::npos)
      continue;
    if (str.find("Name=") != string::npos && appName=="")
      appName = str.substr(str.find("Name=")+5,str.length());
    if (str.find("Exec=") != string::npos && appExec=="") {
      appExec = str.substr(str.find("Exec=")+5,str.length());
      if (appExec.find(" ") != string::npos) {
	appExec = appExec.substr(0, appExec.find("%"));
      }
    }
    if (str.find("NoDisplay=true") != string::npos)
      appIsHide = true;
  }
  if (appIsHide == false) {
    if (execForName != "") {
      if (execForName == appName) {
	cout << appExec << "\n";
	exit(LSA_FOUND);
      }
    }
    else
      cout << appName << "\n";
  }
}

void readApps(const auto& appDir, const auto& execForName) {
  namespace fs = std::filesystem;
  for (const auto& entry : fs::directory_iterator(appDir)) {
    if(entry.path().extension() == ".desktop") 
      readAppDesktop(entry, execForName);
  }
}

int main(int argc, char *argv[])
{
  vector<string> defaultAppsDirs {"/usr/share/applications",
   				  "/usr/local/share/applications"};
  string execForName;
  if (argc > 1)
    execForName = argv[1];
  for (const auto& appDir : defaultAppsDirs) {
    readApps(appDir,  execForName);
  }
  if (execForName == "")
    return LSA_SUCCEEDED;
  return LSA_NOTFOUND;
}
