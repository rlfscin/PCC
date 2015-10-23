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

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include "KMP.cpp"

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
        cout << "ajuda custa 5 reais\n";
        return 0;
        break;

      default:
        abort();
    }
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

    // cout << v.size() << "\n";

    string con = contents.str();

    for(int i = 0; i < v.size(); i++) {
      cout << "<-- " << con.substr(v[i]-10, 20) << " -->\n";
    }

    cout << "FIM\n";
  } else {
    // read textfile
    ifstream in(argv[optind], std::ios::in | std::ios::binary);
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    string contentsStr = contents.str();

    // read patterns file
    ifstream patternsFile(patterns);
    string line;

    while(std::getline(patternsFile, line)) {
      kmp.renew(line);
      v = kmp.search(contentsStr);

      cout << v.size() << "\n";
    }
    cout << "FIM\n";
  }

	return 0;
}