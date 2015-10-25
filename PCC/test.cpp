//
//  test.cpp
//  PCC
//
//  Created by Rubens Lopes de Farias Silva on 10/20/15.
//  Copyright (c) 2015 Rubens Lopes de Farias Silva. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include "KMP.cpp"
#include "wu_manber.cpp"

using namespace std;
int main(){
    KMP kmp("filhos");
    vector<int> v = kmp.search("Ouviram do Ipiranga as margens plácidasDe um povo heróico o brado retumbante,E o sol da liberdade, em raios fúlgidos,Brilhou no céu da pátria nesse instante.Se o penhor dessa igualdadeConseguimos conquistar com braço forte,Em teu seio, ó liberdade,Desafia o nosso peito a própria morte!Ó Pátria amada,Idolatrada,Salve! Salve!Brasil, um sonho intenso, um raio vívidoDe amor e de esperança à terra desce,Se em teu formoso céu, risonho e límpido,A imagem do Cruzeiro resplandece.Gigante pela própria natureza,És belo, és forte, impávido colosso,E o teu futuro espelha essa grandeza.Terra adorada,Entre outras mil,És tu, Brasil,Ó Pátria amada!Dos filhos deste solo és mãe gentil,Pátria amada,Brasil!");
    for (int i =0; i < v.size(); i++) {
        cout << v[i] << endl;
    }
    cout << "Fim" << endl;
    v = wu_manber("Rubens Lopes de Rubens Silva Rubens", "Rubens", 0);
    for (int i =0; i < v.size(); i++) {
        cout << v[i] << endl;
    }
    cout << "Fim";

    
}