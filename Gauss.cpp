#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

class tablice{          //posluguje sie klasa, zeby z wieksza latwoscia tworzyc funkcje odwolujace sie do tablic
private:
    double** A;
    double* b;
    unsigned rozmiar;
    unsigned* kolejnosc;    //tablica wskaznikow utworzona po wczytaniu danych
public:
    tablice(){};            //wymagany konstruktor
    void odczyt();
    void wyniki();          //funkcja liczaca wynik oraz wyswietlajaca go w odpowiedniej kolejnosci
    double formulka(double w1, double w2, double p){
        return (w2-p*w1);
    };
    void gaussczesciowy(unsigned n);    //jeden krok obliczen w tablicy n x n
    void kolejnoscdeklaracja();
    void gauss();           //zadanie 1
    void gausskolumny();    //zadanie 2
    void gausswiersze();    //... 2
    void gausspelny();      //zadanie 3
    ~tablice(){
        cout << "zwalnianie pamieci...";
        delete A;
        delete b;
        delete kolejnosc;
    };
};

void tablice::odczyt(){         //praktycznie kopia, potlumaczylem tylko niektore slowa zgodnie z nazwami przyjetymi w klasie
    ifstream source_file("dane.csv");
    if (!source_file.is_open())
    {
        cout <<"The file has not been opened!"<<endl;
    }
    source_file >> rozmiar;
    A = new double*[rozmiar];
    A[0] = new double[rozmiar*rozmiar];
    for(unsigned i = 1; i< rozmiar; i++)
        A[i] = A[i-1] + rozmiar;
    b = new double[rozmiar];
    char semicolumn;
    for (unsigned i = 0; i < rozmiar+1; i++)
        source_file >> semicolumn;

    for (unsigned i = 0; i < rozmiar; i++)
    {
        for (unsigned j = 0; j < rozmiar; j++)
        {
            source_file >> A[i][j];
            source_file >> semicolumn;
        }
        source_file >> semicolumn;
        source_file >> b[i];
    }
    source_file.close();
    kolejnoscdeklaracja();      //deklaracja kolejnosci nastepuje po kazdym wczytaniu danych
}

void tablice::gaussczesciowy(unsigned n){
    double p;
    for(unsigned i = (n+1); i < rozmiar; i++){
        p = A[i][n]/A[n][n];
        for(unsigned j = n; j < rozmiar; j++){
            A[i][j] = formulka(A[n][j], A[i][j], p);
        }
        b[i] = formulka(b[n], b[i], p);
    }
}

void tablice::wyniki(){
    for(int a = (rozmiar-1); a >= 0; a--){          //tu bylby problem z typem "unsigned", nie moze miec wartosci -1, wyskakuje ostrzezenie :(
        for(int n = (rozmiar-1); n > a; n--){
            b[a] -= A[a][n]*b[n];                   //przepisuje wartosci w jednowymiarowej tablicy na wyniki macierzy
        }
        b[a] = b[a]/A[a][a];
    }
    for (unsigned i = 0; i < rozmiar; i++){
        cout << "x" << (i+1) << "= ";
        for(unsigned j = 0; j < rozmiar; j++){      //posluguje sie tu tablica wskaznikow nawet jezeli uzytkownik oblicza tylko najprostszym sposobem
            if(i == kolejnosc[j]){                  //wiem, ze nie jest to idealne rozwiazanie
                cout << b[j] << endl;
                j = rozmiar;
            }
        }
    }
}

void tablice::kolejnoscdeklaracja(){
    kolejnosc = new unsigned[rozmiar];
    for(int i = 0; i < rozmiar; i++) kolejnosc[i] = i;
}

void tablice::gauss(){
    for(unsigned n = 0; n < rozmiar; n++) gaussczesciowy(n);
    wyniki();
}

void tablice::gausswiersze(){
    unsigned wiersz; double temp;           //"temp" ma wiecej niz jedno zastosowanie, w dwoch oddzielnych petlach
    for(unsigned n = 0; n < rozmiar; n++){
        wiersz = n;
        temp = A[n][n];
        for(unsigned i = n; i < rozmiar; i++){
                if(fabs(A[i][n]) > temp){
                    temp = fabs(A[i][n]);
                    wiersz = i;
                }
        }
        if(wiersz == n) gaussczesciowy(n);
        else{
            for(unsigned i = n; i < rozmiar; i++){
                temp = A[wiersz][i];
                A[wiersz][i] = A[n][i];
                A[n][i] = temp;
            }
            temp = b[wiersz];
            b[wiersz] = b[n];
            b[n] = temp;
            gaussczesciowy(n);
        }
    }
    wyniki();
}

void tablice::gausskolumny(){
    unsigned kolumna, utemp; double temp;       //utemp jest mi potrzebne do podstawienia przy zamianie kolejnosci

    for(unsigned n = 0; n < rozmiar; n++){
        kolumna = n;
        temp = A[n][n];
        for(unsigned i = n; i < rozmiar; i++){
            if(fabs(A[n][i]) > temp){
                temp = fabs(A[n][i]);
                kolumna = i;
            }
        }
        if(kolumna == n) gaussczesciowy(n);
        else{
            utemp = kolejnosc[kolumna];
            kolejnosc[kolumna] = kolejnosc[n];
            kolejnosc[n] = utemp;
            for(unsigned i = 0; i < rozmiar; i++){
                temp = A[i][kolumna];
                A[i][kolumna] = A[i][n];
                A[i][n] = temp;
            }
            gaussczesciowy(n);
        }
    }
    wyniki();
}

void tablice::gausspelny(){
    unsigned kolumna, wiersz, utemp; double temp;
    for(unsigned n = 0; n < rozmiar; n++){
        kolumna = n;
        wiersz = n;
        temp = A[n][n];
        for(unsigned i = n; i < rozmiar; i++){
            for(unsigned j = n; j < rozmiar; j ++){
                if(fabs(A[i][j]) > temp){
                temp = fabs(A[i][j]);
                wiersz = i;
                kolumna = j;
                }
            }
        }
        if(wiersz != n){
            for(unsigned i = n; i < rozmiar; i++){
                temp = A[wiersz][i];
                A[wiersz][i] = A[n][i];
                A[n][i] = temp;
            }
            temp = b[wiersz];
            b[wiersz] = b[n];
            b[n] = temp;
        }
        if(kolumna == n) gaussczesciowy(n);
        if(kolumna != n){
            utemp = kolejnosc[kolumna];
            kolejnosc[kolumna] = kolejnosc[n];
            kolejnosc[n] = utemp;
            for(unsigned i = 0; i < rozmiar; i++){
                temp = A[i][kolumna];
                A[i][kolumna] = A[i][n];
                A[i][n] = temp;
            }
            gaussczesciowy(n);
        }
    }
    wyniki();
}

int main()
{
    tablice zbiory;                 //zupelnie pusta deklaracja tablic
    unsigned wybor = 0;
    bool nieodczytano = true;       //"nieodczytano" zapobiega wszelkim bledom zwiazanym z przypadkowym uruchomieniem przed otwarciem pliku
    while(wybor != 5){
        wybor = 0;
        cout << "1\twczytanie danych z pliku" << endl <<
        "2\tmetoda eliminacji Gaussa bez wyboru elementu" << endl <<
        "31\tmetoda eliminacji Gaussa  z wyborem elementu w wierszach" << endl <<
        "32\tmetoda eliminacji Gaussa z wyborem elementu w kolumnach" << endl <<
        "4\tmetoda eliminacji Gaussa z pelnym wyborem elementu" << endl <<
        "5\twyjscie z programu" << endl;
        cin >> wybor;
        switch(wybor){
            case 1:
                zbiory.odczyt();
                nieodczytano = false;
                break;
            case 2:
                if(nieodczytano) break;
                zbiory.gauss();
                break;
            case 31:
                if(nieodczytano) break;
                zbiory.gausswiersze();
                break;
            case 32:
                if(nieodczytano) break;
                zbiory.gausskolumny();
                break;
            case 4:
                if(nieodczytano) break;
                zbiory.gausspelny();
                break;
        }
    }
    return 0;
}
