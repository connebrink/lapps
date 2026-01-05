
#include <sstream>
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

void readAppDesktop(const auto &desktopFile,
                    const auto &localeId,
                    const auto &execForName,
                    vector<string>& appNames) {
  ifstream appfile(desktopFile.path());
  string str;
  string appName;
  string appNameDefault;
  string appNameLocale;
  string appExec;
  bool appIsHide = false;
  while (getline(appfile, str)) {
    if (str.find("Name") != string::npos && appName == "") {
      if (str.find("GenericName") != string::npos)
        continue;
      ostringstream localeName;
      localeName << "Name";
      localeName << "[" << localeId << "]=";
      if (str.find(localeName.str()) != string::npos) {
        appNameLocale = str.substr(str.find(localeName.str()) +
                             localeName.str().length(), str.length());
      }
      if (str.find("Name=") != string::npos) {
	appNameDefault = str.substr(str.find("Name=") + 5, str.length());
      }
    }
    
    if (str.find("Exec=") != string::npos && appExec=="") {
      appExec = str.substr(str.find("Exec=")+5,str.length());
      if (appExec.find(" ") != string::npos) {
	appExec = appExec.substr(0, appExec.find("%"));
      }
    }
    
    if (str.find("NoDisplay") != string::npos && str.find("true") != string::npos)
      appIsHide = true;
  }

  appName = appNameLocale;
  if (appName == "")
    appName = appNameDefault;
  
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
}

void readApps(const auto& appDir, const auto& execForName, vector<string>& appNames) {
  namespace fs = std::filesystem;
  for (const auto& entry : fs::directory_iterator(appDir)) {
    if(entry.path().extension() == ".desktop") 
      readAppDesktop(entry, "en" ,execForName, appNames);
  }
}

int main(int argc, char *argv[])
{
  vector<string> appNames;
  vector<string> defaultAppsDirs {
    "/usr/local/share/applications",
    "/usr/share/applications"};
  string execForName;
  for (int i = 1; i < argc; ++i) {
    string arg{argv[i]};
    if (arg.length() >= 2 && arg[0] == '-' && arg[1] == 'D' )  
      defaultAppsDirs.insert(defaultAppsDirs.begin(),arg.substr(2, arg.length()));
    else
      execForName = arg;
  }
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
