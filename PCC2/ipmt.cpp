#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <fstream>


using namespace std;

int main(int argc, char* argv[]) {

	int c;
	char *indextype = NULL;
	char *compression = NULL;
	char *patterns = NULL;

	while(1) {
		static struct option long_options[] = {
			{"indextype", required_argument, 0, 1},
			{"compression", required_argument, 0, 2},
			{"pattern", required_argument, 0, 'p'},
			{"help", no_argument, 0, 'h'}
		};

		int option_index = 0;

		c = getopt_long (argc, argv, "p:a", long_options, &option_index);

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

			case 'h':
				cout << "ipmt [index] [--indextype=suffixtree] [--indextype==suffixarray] [--compression=lz77] [--compression=lz78] [search] [-p --pattern patternfile] [pattern] file" << endl;
				return 0;

			default:
				printf("no option");
				return 0;
		}
	}

	
}