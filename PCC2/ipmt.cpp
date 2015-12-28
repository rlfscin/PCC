#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

int main(int argc, char* argv[]) {

	int c;
	char *indextype = NULL;
	char *compression = NULL;
	char *patterns = NULL;
	bool count = false;

	while(1) {
		static struct option long_options[] = {
			{"indextype", 	required_argument, 0,  1 },
			{"compression", required_argument, 0,  2 },
			{"pattern", 	required_argument, 0, 'p'},
			{"count", 		no_argument,	   0, 'c'},
			{"help", 		no_argument, 	   0, 'h'}
		};

		int option_index = 0;

		c = getopt_long (argc, argv, "", long_options, &option_index);

		if(c == -1)
			break;

		switch (c) {
			case 1:
				indextype = optarg;
				break;

			case 2:
				compression = optarg;
				break;

			case 'p':
				patterns = optarg;
				break;

			case 'c':
				count = true;
				break;

			case 'h':
				cout << "ipmt [index] [--indextype=suffixtree] [--indextype==suffixarray] [--compression=lz77] [--compression=lz78] [search] [-p --pattern patternfile] [pattern] file" << endl;
				return 0;

			default:
				printf("no option\n");
				return 0;
		}
	}

	printf("indextype=%s, compression=%s, option=%s\n", indextype, compression, argv[optind]);

	if(strcmp(argv[optind], "index") == 0) {
		printf("Here we do the index\n");

		/*
		* Checar qual index no indextype, default = ?
		* Checar qual compressao no comprresion, default = lz77
		* Pegar arquivo de entrada
		* Fazer a indexacao e depois compressao. guardar arquivo
		*/

	} else if(strcmp(argv[optind], "search") == 0){
		printf("Here we do the search\n");

		vector<string> theseAreThePatterns;

		if(patterns == NULL) {
			// only one pattern
			theseAreThePatterns.push_back(argv[++optind]);
		} else {
			// file with patterns
			ifstream patternsFile(patterns);
			string line;

			while(getline(patternsFile, line)) {
				theseAreThePatterns.push_back(line);
			}
		}

		/*
		* Pegar arquivo de entrada
		* Descomprimir e fazer busca
		* Imprimir ocorrencias
		*/

	} else {
		printf("%s is not an option\n", argv[optind]);
	}
}