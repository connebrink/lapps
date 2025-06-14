
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <algorithm>

using namespace std;

const int LSA_SUCCEEDED = 0;
const int LSA_FOUND     = 10;
const int LSA_NOTFOUND  = 20;

void readAppDesktop(const auto& desktopFile, const auto& execForName, vector<string>& appNames) {
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
    if (str.find("NoDisplay") != string::npos && str.find("true") != string::npos)
      appIsHide = true;
  }
  if (appIsHide == false) {
    if (execForName != "") {
      if (execForName == appName) {
	cout << appExec << "\n";
	exit(LSA_FOUND);
      }
    }
    else {
      if (find(appNames.begin(), appNames.end(), appName) == appNames.end()) {
	appNames.push_back(appName);
      }
    }
  }
  else {
    auto p = find(appNames.begin(), appNames.end(), appName);
    if (p != appNames.end())
      appNames.erase(p);
  }
}

void readApps(const auto& appDir, const auto& execForName, vector<string>& appNames) {
  namespace fs = std::filesystem;
  for (const auto& entry : fs::directory_iterator(appDir)) {
    if(entry.path().extension() == ".desktop") 
      readAppDesktop(entry, execForName, appNames);
  }
}

int main(int argc, char *argv[])
{
  vector<string> defaultAppsDirs {"/usr/share/applications",
   				  "/usr/local/share/applications",
				  "/home/clas/.local/share/applications"};
  string execForName;
  if (argc > 1)
    execForName = argv[1];
  vector<string> appNames;
  for (const auto& appDir : defaultAppsDirs) {
    readApps(appDir,  execForName, appNames);
  }
  if (execForName == "") {
    for (const auto& aname : appNames)
      cout << aname << "\n";
    return LSA_SUCCEEDED;
  }
  return LSA_NOTFOUND;
}
