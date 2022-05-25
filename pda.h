#ifndef PDA_H_INCLUDED
#define PDA_H_INCLUDED

#include <bits/stdc++.h>
using namespace std;

class PDA {
public:
    struct simbol {
        string litera;
        int index;
    }; //citit de la tastatura
    struct caracter  {
        string litera;
        int index;
    }; //citit din stiva
    struct tranzitie {
        simbol simb;
        caracter caracter_citit;
        vector<int> caractere_scrise;
        int plecare;
        int destinatie;
    };
    struct stare {
        string litera;
        int index;
    };
    PDA();
    virtual ~PDA();
    bool CheckWord(string word);
    friend istream &operator>>(istream &input, PDA &pda);
    void CitesteTranzitii();
    void Afiseaza();
    int getLambdaSimbIndex();
    int getStivaCaracter();
    int getZ0CaracterIndex();
    int getLambdaCaracterIndex();
    vector<int> getValidTranzition(int plecare, int destinatie, int simbol, int caracter_citit);
    int getStareIndexByLitera(string l);
    int getCaracterIndexByLitera(string l);
    caracter getCaracterByLitera(string l);
    simbol getSimbolByLitera(string l);
    int getSimbolIndexByLitera(string l);
    int getStareInitialaIndex();
    bool isStareFinalaIndex(int index);

private:
    stare stare_initiala;
    vector<simbol> simboluri;
    vector<caracter> caractere;
    vector<stare> stari;
    vector<stare> stari_finale;
    vector<tranzitie> tranzitii;
    stack<int> stiva;
    string caracter_nul;
    string simbol_nul;
    vector< vector< vector<int> > > tabel;
    void AfiseazaMatrice();
    bool check(PDA &nfa, int stare_index, string word, unsigned int position, stack<int> stiva);
};

#endif // PDA_H_INCLUDED
