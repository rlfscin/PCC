//pmt [options] pattern textfile [textfile...]
// -e, --edit erro: busca aproximada 
// -p patternfile
// http://www.gnu.org/software/libc/manual/html_node/Getopt.html
// http://insanecoding.blogspot.com.br/2011/11/how-to-read-in-file-in-c.html

/*
* OPCOES DE BUSCA
* 1: pmt padrao arquivo.txt
* 2: pmt -e 3 padrao arquivo.txt
* 3: pmt -edit 3 padrao arquivo.txt
* 4: pmt -p padrao.txt arquivo.txt
* 5: pmt padrao arquivo.txt arquivo2.txt arquivo3.txt
* 6: pmt -e 3 -p pedrao.txt arquivo.txt pedro*.txt
*/

// argv[optind] -> you can use this variable to determine where the remaining non-option arguments begin. The initial value of this variable is 1.

#include "KMP.cpp"
#include <iostream>
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
        break;

      case 'p':
        patterns = optarg;
        break;

      case 'h':
        cout << "pmt [-e error] [-p patternsfile] [pattern] textfile\n";
        return 0;
        break;

      default:
        abort();
    }
  }

  vector<int> v;
  KMP kmp;

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


  string contentsStr;
  for (int i = 0; i < andTheseAreTheTextFiles.size(); i++) {
    ifstream in(andTheseAreTheTextFiles[i], std::ios::in | std::ios::binary);
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    contentsStr = contents.str();

    for (int j = 0; j < theseAreThePatterns.size(); j++) {
      kmp.renew(theseAreThePatterns[j]);
      v = kmp.search(contentsStr);

      printf("PADRAO '%s' PARA ARQUIVO '%s'\n", theseAreThePatterns[j].c_str(), andTheseAreTheTextFiles[i].c_str());
      for(int k = 0; k < v.size(); k++) {
        printf("%s\n", contentsStr.substr(v[k]-10, 20).c_str());
      }
      printf("---FIM---\n");
    }
  }

	return 0;
}