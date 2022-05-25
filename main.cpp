#include <bits/stdc++.h>
#include "pda.h"
using namespace std;
ifstream f("test.txt");
ofstream g("output.txt");
int main()
{   PDA pda;
    f >> pda;
    int nr_cuv;
    f >> nr_cuv;
    pda.Afiseaza();
    for(int i = 0; i < nr_cuv; i++) {
        string cuvant;
        f >> cuvant;
        bool t = pda.CheckWord(cuvant);
        if(t == false)
            g << "Nu este bun cuvantul: " << cuvant << '\n';
        else g << "Este bun cuvantul: " << cuvant << '\n';
    }
    g << "LAMBDA: " << pda.getLambdaSimbIndex();
    return 0;
}
