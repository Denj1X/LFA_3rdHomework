#include "pda.h"

PDA::PDA() { }

PDA::~PDA() { }

int PDA::getStareIndexByLitera(string l) {
    for(unsigned int i = 0; i < stari.size(); i++)
        if(l.compare(stari[i].litera) == 0)
            return stari[i].index;
    return -1;
}

int PDA::getCaracterIndexByLitera(string l) {
    for(unsigned int i = 0; i < caractere.size(); i++)
        if(l.compare(caractere[i].litera) == 0)
            return caractere[i].index;
    return -1;
}

PDA::simbol PDA::getSimbolByLitera(string l) {
    for(unsigned int j = 0; j < simboluri.size(); j++)
        if(simboluri[j].litera.compare(l) == 0)
            return simboluri[j];
    simbol s;
    s.index = -1;
    s.litera = "NULL";
    return s;
}

PDA::caracter PDA::getCaracterByLitera(string l) {
    for(unsigned int j = 0; j < caractere.size(); j++)
        if(caractere[j].litera.compare(l) == 0)
            return caractere[j];
    caracter s;
    s.index = -1;
    s.litera = "NULL";
    return s;
}

int PDA::getSimbolIndexByLitera(string l) {
    for(unsigned int j = 0; j < simboluri.size(); j++)
        if(simboluri[j].litera.compare(l) == 0)
            return simboluri[j].index;
    return -1;
}

int PDA::getStareInitialaIndex() {
    return this->stare_initiala.index;
}

bool PDA::isStareFinalaIndex(int index) {
    for(unsigned int i = 0; i < stari_finale.size(); i++)
        if(stari_finale[i].index == index)
            return true;
    return false;
}

int PDA::getLambdaSimbIndex() {
    return this->getSimbolIndexByLitera("L");
}

int PDA::getLambdaCaracterIndex() {
    return this->getCaracterIndexByLitera("L");
}

int PDA::getZ0CaracterIndex() {
    return this->getCaracterIndexByLitera("$");
}

istream &operator>>(istream &input, PDA &fa) {
    unsigned int nr;
    input >> nr;
    PDA::stare s;
    for(unsigned int i = 0; i < nr; i++) {
        input >> s.litera;
        s.index = i;
        fa.stari.push_back(s);
    }
    input>>nr;
    PDA::simbol s2;
    for(unsigned int i = 0; i < nr; i++) {
        input >> s2.litera;
        s2.index = i;
        fa.simboluri.push_back(s2);
    }
    input >> nr;
    PDA::caracter c;
    for(unsigned int i = 0; i < nr; i++) {
        input >> c.litera;
        c.index = i;
        fa.caractere.push_back(c);
    }
    input >> fa.stare_initiala.litera;
    fa.stare_initiala.index = fa.getStareIndexByLitera(fa.stare_initiala.litera);
    input >> nr;
    PDA::stare s3;
    for(unsigned int i = 0; i < nr; i++) {
        input >> s3.litera;
        s3.index = fa.getStareIndexByLitera(s3.litera);
        fa.stari_finale.push_back(s3);
    }
    input >> nr;
    string l;
    for(unsigned int i = 0; i < nr; i++) {
        PDA::tranzitie t;
        input >> l;
        t.plecare = fa.getStareIndexByLitera(l);
        input >> l;
        t.destinatie = fa.getStareIndexByLitera(l);
        input >> l;
        t.simb = fa.getSimbolByLitera(l);
        input >> l;
        t.caracter_citit = fa.getCaracterByLitera(l);
        input >> l;
        for(unsigned int i = 0; i < l.length(); i++) {
            string aux;
            aux = l.at(i);
            int index = fa.getCaracterIndexByLitera(aux);
            t.caractere_scrise.push_back(index);
        }
        cout<<'\n';
        fa.tranzitii.push_back(t);
    }
    fa.CitesteTranzitii();
    return input;
}

void PDA::CitesteTranzitii() {
    tabel.resize(this->stari.size());
    for(unsigned int i = 0; i < this->stari.size(); i++)
        tabel[i].resize(this->simboluri.size());
    for(unsigned int i = 0; i < this->tranzitii.size(); i++)
        tabel[tranzitii[i].plecare][tranzitii[i].simb.index].push_back(tranzitii[i].destinatie);
    vector< vector< vector<int> > >::iterator it_stari;
    vector< vector<int> >::iterator it_simb;
    for(it_stari = this->tabel.begin(); it_stari != this->tabel.end(); it_stari++) {
        for(it_simb = it_stari->begin(); it_simb != it_stari->end(); it_simb++) {
            if(it_simb->begin() == it_simb->end())
                it_simb->push_back(-1);
        }
    }
}

vector<int> PDA::getValidTranzition(int plecare, int destinatie, int simbol, int caracter_citit) {
    vector<int> r;
    if(plecare >= 0 && destinatie >= 0) {
        for(unsigned int i = 0; i < tranzitii.size(); i++) {
            if(tranzitii[i].plecare == plecare && tranzitii[i].destinatie == destinatie && tranzitii[i].simb.index == simbol) {
                if(tranzitii[i].caracter_citit.index == caracter_citit)
                    r.push_back(i);
            }
        }
    }
    return r;
}

bool PDA::check(PDA &lnfa, int stare_index, string word, unsigned int position, stack<int> stiva) {
    int simbIndex;
    vector<int>::iterator it;
    if(position < word.length()) {
        string s(1, word[position]);
        simbIndex = lnfa.getSimbolIndexByLitera(s); //indexul simbolului de pe linie/sageata/tranzitie
        if(simbIndex == -1) { cout<<"Simbol invalid, return false"<<'\n'; return false;}
        if(simbIndex != lnfa.getLambdaCaracterIndex()) {
            int caracter_index = stiva.top();
            for(it = lnfa.tabel[stare_index][simbIndex].begin(); it != lnfa.tabel[stare_index][simbIndex].end(); it++) {
                //ia caracter de pe stivA SI FA CHECK CU EL LA TRANZITIE
                vector<int> vt = getValidTranzition(stare_index, *it, simbIndex, caracter_index);
                for(unsigned int y = 0; y < vt.size(); y++) {
                    int t = vt[y];
                    stack<int> aux_stiva = stiva;
                    int position2 = position;
                    if(caracter_index != lnfa.getLambdaCaracterIndex())
                    aux_stiva.pop(); //sterge caracterul din top
                    for(int c = lnfa.tranzitii[t].caractere_scrise.size()-1; c >= 0; c--) {
                        if(lnfa.tranzitii[t].caractere_scrise[c] != lnfa.getLambdaCaracterIndex())
                            aux_stiva.push(lnfa.tranzitii[t].caractere_scrise[c]);
                    }
                    if(check(lnfa, *it, word, position2+1, aux_stiva) == true)
                        return true;
                }
            }
        }
    }
    else if(lnfa.isStareFinalaIndex(stare_index)) return true;
        int lambda_simbIndex = lnfa.getLambdaSimbIndex();
        if(lambda_simbIndex == -1) { return false;}
        //cout<<lambda_simbIndex<<'\n';
        int caracter_index = stiva.top();
        //int test = 0;
        for(it = lnfa.tabel[stare_index][lambda_simbIndex].begin(); it != lnfa.tabel[stare_index][lambda_simbIndex].end(); it++) {
            vector<int> vt = getValidTranzition(stare_index, *it, lambda_simbIndex, caracter_index);
            //if(t != -1)//este tranzitie valida
            for(unsigned int y = 0; y < vt.size(); y++) {
                //test = 1;
                stack<int> aux_stiva = stiva;
                int position2 = position;
                int t = vt[y];
                if(caracter_index != lnfa.getLambdaCaracterIndex())
                aux_stiva.pop(); //sterge caracterul din top
                for(int c = lnfa.tranzitii[t].caractere_scrise.size()-1; c >= 0; c--) {
                        if(lnfa.tranzitii[t].caractere_scrise[c] != lnfa.getLambdaCaracterIndex())
                            aux_stiva.push(lnfa.tranzitii[t].caractere_scrise[c]);
                    }
                if(check(lnfa, *it, word, position2, aux_stiva))
                    return true;
            }
        }
    return false;
}

bool PDA::CheckWord(string word) {
    stack<int>s;
    stiva = s;
    stiva.push(getZ0CaracterIndex());
    return check(*this, this->getStareInitialaIndex(), word, 0, stiva);
}

void PDA::Afiseaza() {
    cout<<"Nr_stari: "<<this->stari.size()<<'\n';
    cout<<"Stari: ";
    for(unsigned int i = 0; i < this->stari.size(); i++)
        cout<<"(i:"<<this->stari[i].index<<",l:"<<this->stari[i].litera<<") ";
    cout<<'\n';
    cout<<"Nr simboluri: "<<this->simboluri.size()<<'\n';
    cout<<"Simboluri: ";
    for(unsigned int i = 0; i < this->simboluri.size(); i++)
        cout<<"(i:"<<this->simboluri[i].index<<",l:"<<this->simboluri[i].litera<<") ";
    cout<<'\n';
    cout<<"Nr caractere: "<<this->caractere.size()<<'\n';
    cout<<"Caractere: ";
    for(unsigned int i = 0; i < this->caractere.size(); i++)
        cout<<"(i:"<<this->caractere[i].index<<",l:"<<this->caractere[i].litera<<") ";
    cout<<'\n';
    cout<<"Starea initiala: (i:"<<this->stare_initiala.index<<",l:"<<this->stare_initiala.litera<<") ";
    cout<<'\n';
    cout<<"Nr stari finale: "<<this->stari_finale.size()<<'\n';
    cout<<"Starile finale: ";
    for(unsigned int i = 0; i < this->stari_finale.size(); i++)
        cout<<"(i:"<<this->stari_finale[i].index<<",l:"<<this->stari_finale[i].litera<<") ";
    cout<<'\n';
    cout<<"Nr tranzitii: "<<this->tranzitii.size()<<'\n';
    for(unsigned int i = 0; i < this->tranzitii.size(); i++) {
        cout<<"(plecare_index: "<<this->tranzitii[i].plecare<<", destintatie_index: "<<this->tranzitii[i].destinatie
        <<" ,litera_simbol: "<<this->tranzitii[i].simb.litera<<" ,simbol_index: "<<this->tranzitii[i].simb.index;
        cout<<", litera_caracter_scos: "<<tranzitii[i].caracter_citit.litera<<", litera_caractere_scrise: ";
        for(unsigned c = 0; c < (this->tranzitii[i].caractere_scrise).size(); c++)
            cout<<tranzitii[i].caractere_scrise[c]<<" ";
        cout<<") "<<'\n';
    }
    cout<<'\n';
    this->AfiseazaMatrice();
}

void PDA::AfiseazaMatrice() {
    vector< vector< vector<int> > >::iterator it_stari;
    vector< vector<int> >::iterator it_simb;
    vector<int>::iterator it_elem;
    for(it_stari = this->tabel.begin(); it_stari != this->tabel.end(); it_stari++) {
        for(it_simb = it_stari->begin(); it_simb != it_stari->end(); it_simb++) {
            for(it_elem = it_simb->begin(); it_elem != it_simb->end(); it_elem++)
                cout<<*it_elem<<" ";
            cout<<"; ";
        }
        cout<<'\n';
    }
}
