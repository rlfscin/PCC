#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <sstream>
#include "suffixarray.cpp"
#include "LZW.cpp"

using namespace std;

int main(int argc, char* argv[]) {

	int c;
	char *compression = NULL;
	char *patterns = NULL;
	bool count = false;

	while(1) {
		static struct option long_options[] = {
			{"compression", required_argument, 0,  1 },
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
				compression = optarg;
				break;

			case 'p':
				patterns = optarg;
				break;

			case 'c':
				count = true;
				break;

			case 'h':
				cout << "ipmt [index] [--compression=lz78] [--compression=lzw] [search] [-p --pattern patternfile] [pattern] file" << endl;
				return 0;

			default:
				printf("no option\n");
				return 0;
		}
	}

	if(strcmp(argv[optind], "index") == 0) {
		printf("option=%s, compression=%s\n\n", argv[optind], compression);
		/*
		* Pegar arquivo de entrada
		* Criar array de sufixo
		* Checar qual compressao no comprresion, default = lzw
		* Fazer compressao do array e do texto. guardar arquivo
		*/

		string fileName = argv[optind + 1];

		ifstream in;
		std::ostringstream contents;
		string contentsStr;
		in.open(fileName, std::ios::in | std::ios::binary);
		contents.str("");
		contents << in.rdbuf();
		in.close();
		contentsStr = contents.str();

		printf("Indexing file %s\n", fileName.c_str());
		suffixarray sa;
		vector<int> index = sa.index(contentsStr);
		printf("Index done!\n");

		string comp_lzw = "lzw";
		if(compression == NULL || compression == comp_lzw) {
			printf("Compressing file and its indexes with LZW\n");
			
			string suffixStr = "";
			for (int i = 0; i < index.size(); i++) {
				suffixStr += index[i];
				suffixStr += " ";
			}

			LZW lzw;
			vector<short> compressed = lzw.encode(suffixStr);

			string outfileName = fileName + ".idx";
			ofstream outfile (outfileName, ios::out | ios::binary);

			int sizeOfSuffixStr = compressed.size();
			printf("%d\n", sizeOfSuffixStr); // debug pra saber pq nao pega 1024
			outfile.write((const char*) &sizeOfSuffixStr, sizeof(int));

			for (int i = 0; i < compressed.size(); ++i) {
				outfile.write((const char*) &compressed[i], sizeof(short));
			}

			compressed = lzw.encode(contentsStr);

			int sizeOfCompressedText = compressed.size();
			outfile.write((const char*) &sizeOfCompressedText, sizeof(int));

			for (int i = 0; i < compressed.size(); ++i) {
				outfile.write((const char*) &compressed[i], sizeof(short));
			}

			outfile.close();

			printf("File compressed successfully\n\n");

		} else {
			printf("Compressing file %s with LZ78\n", fileName.c_str());
		}

	} else if(strcmp(argv[optind], "search") == 0){
		printf("option=%s\n\n", argv[optind]);

		string fileName = argv[optind + 1];
		printf("Uncompressing file %s\n", fileName.c_str());

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

		ifstream file(fileName, ios::in | ios::binary);

		int sizeOfSuffixStr;
		file.read((char *) &sizeOfSuffixStr, sizeof(int));

		vector<short> index(sizeOfSuffixStr);
		for(int i = 0; i < sizeOfSuffixStr; i++) {
			file.read((char* ) &index[i], sizeof(short));
		}

		int sizeOfCompressedText;
		file.read((char *) &sizeOfCompressedText, sizeof(int));

		vector<short> textCompressed(sizeOfCompressedText);
		for(int i = 0; i < sizeOfCompressedText; i++) {
			file.read((char* ) &textCompressed[i], sizeof(short));
		}
		file.close();

		LZW lzw;

		string pureText = lzw.decode(textCompressed);

		string outfileName = fileName + ".pedro";
		ofstream out (outfileName, ios::out | ios::binary);
		for (int i = 0; i < pureText.length(); i++) {
			out.write((char*) &pureText[i], sizeof(char));
		}
		out.close();

		printf("File uncompressed successfully\n\n");

	} else {
		printf("%s is not an option\n", argv[optind]);
	}
}