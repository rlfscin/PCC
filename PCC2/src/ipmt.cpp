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
#include "lz78.cpp"

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

		c = getopt_long (argc, argv, "p:ch", long_options, &option_index);

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
				cout << "bin/ipmt [index] file [--compression=lz78] [--compression=lzw]" << endl;
				cout << "bin/ipmt [search] file [-p --pattern patternfile] [pattern] [-c]" << endl;
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
		string comp_lz78 = "lz78";
		if(compression == NULL || compression == comp_lzw) {
			printf("Compressing file with LZW\n");

			LZW lzw;
			string outfileName = fileName + ".idx";
			ofstream outfile (outfileName, ios::out | ios::binary);

			// writing lzw flag
			char c = 'w';
			outfile.write((const char*) &c, sizeof(char));
			// done writing

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

		} else if(compression == comp_lz78) {
			printf("Compressing file %s with LZ78\n", fileName.c_str());

			LZ78 lz78;

			string outfileName = fileName + ".idx";
			ofstream outfile (outfileName, ios::out | ios::binary);

			// writing lz78 flag
			char c = '7';
			outfile.write((const char*) &c, sizeof(char));
			// done writing

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

			map<int, pair<int, char> > compressed = lz78.encode(llcpStr);
			map<int, pair<int, char> >::iterator it;
			int qtd = compressed.size();
			outfile.write((const char*) &qtd, sizeof(int));
			for (it = compressed.begin(); it != compressed.end(); ++it)
			{
				outfile.write((const char*)&it->second.first, sizeof(int));
				outfile.write((const char*)&it->second.second, sizeof(char));
			}

			string rlcpStr = "";
			for (int i = 0; i < Rlcp.size(); i++) {
				rlcpStr += to_string(Rlcp[i]);
				rlcpStr += " ";
			}

			compressed = lz78.encode(rlcpStr);
			qtd = compressed.size();
			outfile.write((const char*) &qtd, sizeof(int));
			for (it = compressed.begin(); it != compressed.end(); ++it)
			{
				outfile.write((const char*)&it->second.first, sizeof(int));
				outfile.write((const char*)&it->second.second, sizeof(char));
			}
			// done compressing llcp and rlcp

			// compressing text
			compressed = lz78.encode(contentsStr);

			qtd = compressed.size();
			outfile.write((const char*) &qtd, sizeof(int));
			for (it = compressed.begin(); it != compressed.end(); ++it)
			{
				outfile.write((const char*)&it->second.first, sizeof(int));
				outfile.write((const char*)&it->second.second, sizeof(char));
			}
			// done compressing text

			printf("File compressed successfully\n\n");
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

		char c;
		file.read((char *) &c, sizeof(char));

		string pureText;
		vector<int> Llcp;
		vector<int> Rlcp;

		// reading indexes
		int sizeOfIndex;
		file.read((char *) &sizeOfIndex, sizeof(int));

		vector<int> index(sizeOfIndex);
		for(int i = 0; i < sizeOfIndex; i++) {
			file.read((char* ) &index[i], sizeof(int));
		}
		// done reading indexes

		if(c == 'w') {
			printf("Uncompressing file %s\n", fileName.c_str());

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
			pureText = lzw.decode(textCompressed);
			// done uncompressing text

			printf("Decompression done!\n");
			// printf("Saving file\n");

			// string outfileName = fileName.substr(0, fileName.length()-4) + ".teste";
			// ofstream out (outfileName, ios::out | ios::binary);
			// for (int i = 0; i < pureText.length(); i++) {
			// 	out.write((char*) &pureText[i], sizeof(char));
			// }
			// out.close();

			// printf("File saved successfully\n\n");

			printf("Uncompressing Llcp and Rlcp\n");

			string llcpStr = lzw.decode(compressedLlcp);
			int aux = 0;
			for (int i = 1; i < llcpStr.length(); i++) {
				if(llcpStr[i] == ' ') {
					Llcp.push_back(stoi(llcpStr.substr(aux, i-aux)));
					aux = i+1;
					i++;
				}
			}

			string rlcpStr = lzw.decode(compressedRlcp);
			aux = 0;
			for (int i = 1; i < rlcpStr.length(); i++) {
				if(rlcpStr[i] == ' ') {
					Rlcp.push_back(stoi(rlcpStr.substr(aux, i-aux)));
					aux = i+1;
					i++;
				}
			}
			printf("Decompression done!\n\n");
		} else if(c == '7') {
			printf("Uncompressing file %s\n", fileName.c_str());

			// reading llcp and rlcp
			int qtd;
			file.read((char *) &qtd, sizeof(int));

			map<int, pair<int, char> > compressedLlcp;

			int inteiro = 0;;
			char c = ' ';
			int aux = 0;

			for (int i = 0; i < qtd; i++) {
				file.read((char *) &inteiro, sizeof(int));
				file.read((char *) &c, sizeof(char));

				compressedLlcp.insert(pair<int, pair<int, char> >(aux, pair<int, char>(inteiro, c)));
				aux++;
			}

			file.read((char *) &qtd, sizeof(int));

			map<int, pair<int, char> > compressedRlcp;

			inteiro = 0;;
			c = ' ';
			aux = 0;

			for (int i = 0; i < qtd; i++) {
				file.read((char *) &inteiro, sizeof(int));
				file.read((char *) &c, sizeof(char));

				compressedRlcp.insert(pair<int, pair<int, char> >(aux, pair<int, char>(inteiro, c)));
				aux++;
			}
			// done reading llcp and rlcp

			// reading text
			file.read((char *) &qtd, sizeof(int));

			map<int, pair<int, char> > textCompressed;

			inteiro = 0;;
			c = ' ';
			aux = 0;

			for (int i = 0; i < qtd; i++) {
				file.read((char *) &inteiro, sizeof(int));
				file.read((char *) &c, sizeof(char));

				textCompressed.insert(pair<int, pair<int, char> >(aux, pair<int, char>(inteiro, c)));
				aux++;
			}
			// done reading text

			file.close();

			LZ78 lz78;

			// uncompressing text
			pureText = lz78.decode(textCompressed);
			// done uncompressing text

			printf("Decompression done!\n");
			// printf("Saving file\n");

			// string outfileName = fileName.substr(0, fileName.length()-4) + ".teste";
			// ofstream out (outfileName, ios::out | ios::binary);
			// for (int i = 0; i < pureText.length(); i++) {
			// 	out.write((char*) &pureText[i], sizeof(char));
			// }
			// out.close();

			// printf("File saved successfully\n\n");

			printf("Uncompressing Llcp and Rlcp\n");

			string llcpStr = lz78.decode(compressedLlcp);
			aux = 0;
			for (int i = 1; i < llcpStr.length(); i++) {
				if(llcpStr[i] == ' ') {
					Llcp.push_back(stoi(llcpStr.substr(aux, i-aux)));
					aux = i+1;
					i++;
				}
			}

			string rlcpStr = lz78.decode(compressedRlcp);
			aux = 0;
			for (int i = 1; i < rlcpStr.length(); i++) {
				if(rlcpStr[i] == ' ') {
					Rlcp.push_back(stoi(rlcpStr.substr(aux, i-aux)));
					aux = i+1;
					i++;
				}
			}
			printf("Decompression done!\n\n");
		}

		sarray sa;

		if(count) {
			for (int i = 0; i < theseAreThePatterns.size(); i++) {

				// gambiarra, sorry :(
				if(c == '7') {
					pureText = pureText.substr(0, pureText.length()-1);
				}
				vector<int> v = sa.match(pureText, theseAreThePatterns[i], index, Llcp, Rlcp);
				printf("Numbers of matches %lu\n", v.size());
			}
		} else {
			// gambiarra, sorry :(
			if(c == '7') {
				pureText = pureText.substr(0, pureText.length()-1);
			}

			for (int i = 0; i < theseAreThePatterns.size(); i++) {
				vector<int> v = sa.match(pureText, theseAreThePatterns[i], index, Llcp, Rlcp);
				
				printf("Printing matches for %s\n", theseAreThePatterns[i].c_str());

				int b;
				int e;
				for(int j = 0; j < v.size(); j++) {
					for(int k = v[j]; k > 0 && v[j]-k < 50; k--) {
						if(pureText[k] == '\n') {
							b = k+1;
							k = -1;
						} else {
							b = k;
						}
					}

					for(int k = v[j]; k < pureText.length() & k-v[j] < 50; k++) {
						if(pureText[k] == '\n') {
							e = k;
							k = pureText.length();
						} else {
							e = k;
						}
					}

					printf("%s\n", pureText.substr(b, e-b).c_str());
				}
			}
		}

		printf("\n");
	} else {
		printf("%s is not an option\n", argv[optind]);
	}
}