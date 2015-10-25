#include "KMP.cpp"
#include "wu_manber.cpp"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <getopt.h>

using namespace std;

int main(int argc, char* argv[]) {
	int c;

	char *error = NULL;
	char *patterns = NULL;
  int e = -1;

  while (1) {
    static struct option long_options[] = {
      {"pattern", required_argument, 0, 'p'},
      {"edit", required_argument, 0, 'e'},
      {"help", no_argument, 0, 'h'}
    };

    int option_index = 0;

    c = getopt_long (argc, argv, "p:e:h", long_options, &option_index);

    if(c == -1)
      break;

    switch (c) {
      case 'e':
        error = optarg;
        e = atoi(error);
        break;

      case 'p':
        patterns = optarg;
        break;

      case 'h':
        cout << "pmt [-e error] [-p patternsfile] [pattern] textfile [textfile...]" << endl;
        return 0;
        break;

      default:
        abort();
    }
  }

  vector<string> allArgs(argv, argv + argc);
  vector<string> theseAreThePatterns;
  vector<string> andTheseAreTheTextFiles;

  if(patterns == NULL) {
    // get the only pattern
    theseAreThePatterns.push_back(allArgs[optind]);

    // get all the textfiles
    for (int i = optind + 1; i < allArgs.size(); ++i) {
      andTheseAreTheTextFiles.push_back(allArgs[i]);
    }
  } else {
    // read patterns file
    ifstream patternsFile(patterns);
    string line;

    // get all the patterns
    while(getline(patternsFile, line)) {
      theseAreThePatterns.push_back(line);
    }

    // get all the textfiles
    for (int i = optind; i < allArgs.size(); ++i) {
      andTheseAreTheTextFiles.push_back(allArgs[i]);
    }
  }


  vector<int> v;
  KMP kmp;
  string contentsStr;

  ifstream in;
  std::ostringstream contents;
  if(error == NULL){
    // exact string matching (KMP)

    // string line;
    // int lineNumber;
    for (int i = 0; i < andTheseAreTheTextFiles.size(); i++) {
      in.open(andTheseAreTheTextFiles[i], std::ios::in | std::ios::binary);
      contents.str("");
      contents << in.rdbuf();
      in.close();
      contentsStr = contents.str();

      for (int j = 0; j < theseAreThePatterns.size(); j++) {
        kmp.renew(theseAreThePatterns[j]);
        v = kmp.search(contentsStr);

        for(int k = 0; k < v.size(); k++) {
          printf("%s\n", contentsStr.substr(v[k], 20).c_str());
        }
        printf("--- END PATTERN %d FOR FILE %d---\n", j+1, i+1);
      }

      // strategy 2: line by line
      // in.open(andTheseAreTheTextFiles[i], std::ios::in | std::ios::binary);
      // lineNumber = 1;

      // for (int j = 0; j < theseAreThePatterns.size(); j++) {
      //   kmp.renew(theseAreThePatterns[j]);

      //   while(getline(in, line)) {
      //     v = kmp.search(line);

      //     for (int k = 0; k < v.size(); ++k) {
      //       // printf("line %d at %d\n", lineNumber, v[k]);
      //       printf("%s\n", line.c_str());
      //     }

      //     lineNumber++;
      //   }
      // }
      // in.close();
    }
  } else {
    for (int i = 0; i < andTheseAreTheTextFiles.size(); i++) {
      in.open(andTheseAreTheTextFiles[i], std::ios::in | std::ios::binary);
      contents.str("");
      contents << in.rdbuf();
      in.close();
      contentsStr = contents.str();

      for (int j = 0; j < theseAreThePatterns.size(); j++) {
        v = wu_manber(contentsStr, theseAreThePatterns[j], e);

        for(int k = 0; k < v.size(); k++) {
          printf("%s\n", contentsStr.substr(v[k], 20).c_str());
        }
        printf("--- END PATTERN %d FOR FILE %d---\n", j+1, i+1);
      }
    }
  }

	return 0;
}