//pmt [options] pattern textfile [textfile...]
// -e, --edit erro: busca aproximada 
// -p patternfile
// http://www.gnu.org/software/libc/manual/html_node/Getopt.html
// http://stackoverflow.com/questions/8774200/making-command-line-programs-with-arguments

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

// #include <ctype.h>
// #include <stdlib.h>
// #include <stdio.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "KMP.cpp"

int main(int argc, char* argv[]) {
	int c;

	char *error = NULL;
	char *patterns = NULL;

	while ((c = getopt (argc, argv, "e:p:")) != -1)

  switch (c) { 
    case 'e':
      error = optarg;
      break;
    case 'p':
    	patterns = optarg;
    	break;
    default:
      abort ();
  }

  vector<int> v;
  KMP kmp;

  if(patterns == NULL) {
    // read textfile
    ifstream in(argv[optind + 1], std::ios::in | std::ios::binary);
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();

    kmp.renew(argv[optind]);
    v = kmp.search(contents.str());
    cout << v.size() << "\n";
    cout << "FIM\n";
  } else {
    // read textfile
    ifstream in(argv[optind], std::ios::in | std::ios::binary);
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();

    ifstream patternsFile(patterns);
    string line;

    while(std::getline(patternsFile, line)) {
      kmp.renew(line);
      v = kmp.search(contents.str());
      cout << v.size() << "\n";
    }
    cout << "FIM\n";
  }


  // for (int i =0; i < v.size(); i++) {
  //   cout << v[i] << endl;
  // }

  // cout << v.size() << "\n";


	// printf ("error = %s, patterns = %s, Non-option = %s\n", error, patterns, argv[optind]);

	return 0;
}