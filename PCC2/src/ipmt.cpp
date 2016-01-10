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
#include "sarray.cpp"
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

		sarray sa;
		vector<int> index = sa.index(contentsStr);

		printf("Index done!\n");

		// converting index to string
		// string suffixStr = "";
		// for (int i = 0; i < index.size(); i++) {
		// 	suffixStr += to_string(index[i]);
		// 	suffixStr += " ";
		// }
		// done converting

		printf("Calculating Llcp and Rlcp\n");

		int textlen = contentsStr.length();
		vector<int> Llcp(textlen);
		vector<int> Rlcp(textlen);
		memset(&Llcp[0], -1, textlen*sizeof(Llcp[0]));
		memset(&Rlcp[0], -1, textlen*sizeof(Llcp[0]));

		sa.compute_LRlcp(contentsStr, index, Llcp, Rlcp, 0, textlen - 1);

		printf("Calculation done!\n");

		string comp_lzw = "lzw";
		if(compression == NULL || compression == comp_lzw) {
			printf("Compressing file with LZW\n");

			LZW lzw;
			string outfileName = fileName + ".idx";
			ofstream outfile (outfileName, ios::out | ios::binary);

			// writing indexes
			int sizeOfIndex = index.size();
			outfile.write((const char*) &sizeOfIndex, sizeof(int));
			for (int i = 0; i < index.size(); ++i) {
				outfile.write((const char*) &index[i], sizeof(int));
			}
			// done writing indexes

			// compressing llcp and rlcp
			string llcpStr = "";
			for (int i = 0; i < Llcp.size(); i++) {
				llcpStr += to_string(Llcp[i]);
				llcpStr += " ";
			}

			vector<short> compressed = lzw.encode(llcpStr);
			int llcpCompressed = compressed.size();
			outfile.write((const char*) &llcpCompressed, sizeof(int));
			for (int i = 0; i < compressed.size(); ++i) {
				outfile.write((const char*) &compressed[i], sizeof(short));
			}

			string rlcpStr = "";
			for (int i = 0; i < Rlcp.size(); i++) {
				rlcpStr += to_string(Rlcp[i]);
				rlcpStr += " ";
			}

			compressed = lzw.encode(rlcpStr);
			int rlcpCompressed = compressed.size();
			outfile.write((const char*) &rlcpCompressed, sizeof(int));
			for (int i = 0; i < compressed.size(); ++i) {
				outfile.write((const char*) &compressed[i], sizeof(short));
			}
			// done compressing llcp and rlcp

			// compressing text
			compressed = lzw.encode(contentsStr);

			int sizeOfCompressedText = compressed.size();
			outfile.write((const char*) &sizeOfCompressedText, sizeof(int));

			for (int i = 0; i < compressed.size(); ++i) {
				outfile.write((const char*) &compressed[i], sizeof(short));
			}
			// done compressing text

			outfile.close();

			printf("File compressed successfully\n\n");

		} else {
			printf("Compressing file %s with LZ78\n", fileName.c_str());
		}

	} else if(strcmp(argv[optind], "search") == 0){
		printf("option=%s\n\n", argv[optind]);

		string fileName = argv[optind + 1];

		vector<string> theseAreThePatterns;

		printf("Getting patterns\n");

		// getting the patterns
		if(patterns == NULL) {
			// only one pattern
			theseAreThePatterns.push_back(argv[optind + 2]);
		} else {
			// file with patterns
			ifstream patternsFile(patterns);
			string line;

			while(getline(patternsFile, line)) {
				theseAreThePatterns.push_back(line);
			}
		}
		// done getting patterns

		printf("Done!\n");

		/*
		* Pegar arquivo de entrada
		* Descomprimir e fazer busca
		* Imprimir ocorrencias
		*/

		ifstream file(fileName, ios::in | ios::binary);

		printf("Uncompressing file %s\n", fileName.c_str());

		// reading indexes
		int sizeOfIndex;
		file.read((char *) &sizeOfIndex, sizeof(int));

		vector<int> index(sizeOfIndex);
		for(int i = 0; i < sizeOfIndex; i++) {
			file.read((char* ) &index[i], sizeof(int));
		}
		// done reading indexes

		// reading llcp and rlcp
		int sizeOfCompressedLlcp;
		file.read((char *) &sizeOfCompressedLlcp, sizeof(int));

		vector<short> compressedLlcp(sizeOfCompressedLlcp);
		for(int i = 0; i < sizeOfCompressedLlcp; i++) {
			file.read((char* ) &compressedLlcp[i], sizeof(short));
		}

		int sizeOfCompressedRlcp;
		file.read((char *) &sizeOfCompressedRlcp, sizeof(int));

		vector<short> compressedRlcp(sizeOfCompressedRlcp);
		for(int i = 0; i < sizeOfCompressedRlcp; i++) {
			file.read((char* ) &compressedRlcp[i], sizeof(short));
		}
		// done reading llcp and rlcp

		// reading text
		int sizeOfCompressedText;
		file.read((char *) &sizeOfCompressedText, sizeof(int));

		vector<short> textCompressed(sizeOfCompressedText);
		for(int i = 0; i < sizeOfCompressedText; i++) {
			file.read((char* ) &textCompressed[i], sizeof(short));
		}
		// done reading text

		file.close();

		LZW lzw;

		// uncompressing text
		string pureText = lzw.decode(textCompressed);
		// done uncompressing text

		printf("Decompression done!\n");
		printf("Saving file\n");

		string outfileName = fileName.substr(0, fileName.length()-4) + ".teste";
		ofstream out (outfileName, ios::out | ios::binary);
		for (int i = 0; i < pureText.length(); i++) {
			out.write((char*) &pureText[i], sizeof(char));
		}
		out.close();

		printf("File saved successfully\n\n");

		printf("Uncompressing Llcp and Rlcp\n");

		vector<int> Llcp;
		string llcpStr = lzw.decode(compressedLlcp);
		int aux = 0;
		for (int i = 1; i < llcpStr.length(); i++) {
			if(llcpStr[i] == ' ') {
				Llcp.push_back(stoi(llcpStr.substr(aux, i-aux)));
				aux = i+1;
				i++;
			}
		}

		vector<int> Rlcp;
		string rlcpStr = lzw.decode(compressedRlcp);
		aux = 0;
		for (int i = 1; i < rlcpStr.length(); i++) {
			if(rlcpStr[i] == ' ') {
				Rlcp.push_back(stoi(rlcpStr.substr(aux, i-aux)));
				aux = i+1;
				i++;
			}
		}
		printf("Decompression done!\n");

		sarray sa;

		for (int i = 0; i < theseAreThePatterns.size(); i++) {
			vector<int> v = sa.match(pureText, theseAreThePatterns[i], index, Llcp, Rlcp);
			printf("Numbers of matches %lu\n", v.size());
		}

		printf("\n");
	} else {
		printf("%s is not an option\n", argv[optind]);
	}
}