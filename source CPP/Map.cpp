// Ten plik zawiera funkcj? „main”. W nim rozpoczyna si? i ko?czy wykonywanie programu.

// algorytm Forda-Fulkersona u?ywa? macierzy s?siedztwa, bo 
// z list? s?siedztwa by? problem przy przechodzeniu w drug? stron?
// problem zosta? naprawiony, teraz algorytm u?ywa listy s?siedztwa

/*
Do zrobienia:
1. Dokumentacja (dokument tekstowy i arkusz/harmonogram)

2. Interface konsolowy - program, który b?dzie:
- wczytywa? z pliku i zapisywa? do pliku informacje o node'ach i drogach;
- generowa? plik ze ?cie?kami, który b?dzie od razu dzia?a? dla algorytmu poszukuj?cego
- przechowywa? dane po wczytaniu
- pozwala? na wy?wietlanie danych w konsoli
- pozwala? na modyfikacj? danych w konsoli
- dba? o prawid?owo?? wprowadzonych danych
przyk?adowy plik:
0.5 (konwersja j?czmienia na piwo)
FIELDS
1 0.1 1.2 FIELD 30
BREWERIES
2 3.5 2.2 BREWERY 20
3 4.5 3.2 BREWERY 15
4 5.5 4.2 BREWERY 10
PUBS
5 5.6 2.7 PUB
6 6.7 3.8 PUB
ROADS
4 (ilo?? wierzcho?ków) 8 (ilo?? po??cze?)
1 2 30.0
1 3 20.0
1 4 30.0
2 5 7.0
2 6 3.2
3 5 8.2
4 5 8.5
4 6 6.9

3. Modyfikacja algorytmu przep?ywu dla problemu projektu (ten plik):
3a. Pole, przez które przechodzi ?cie?ka powi?kszaj?ca musi aktualizowa? maksymalny przep?yw dla ka?dej kraw?dzi, która z niego wychodzi
np. je?li pole ma 30 zbo?a i wychodz? z niego 2 strza?ki po 30, po przeprowadzeniu 25 zbo?a przez ?cie?k? trzeba ustawi? wszystkie strza?ki na max(obecny, 30-25)
3b. Browarnia, przez któr? przechodzi ?cie?ka powi?kszaj?ca musi aktualizowa? przep?yw dla ka?dej kraw?dzi, która do niego prowadzi, ?eby nie dosz?o do przekroczenia pojemno?ci browarni

4. Browary przemna?aj?ce przechodz?c? warto??

5. GUI (b?dzie dba?o o prawid?owy przep?yw mi?dzy polami i browarami)


?. Generator grafów (plik z po??czeniami) do testowania algorytmu:
Wej?cie: liczba ca?kowita - ilo?? node'ów
Wyj?cie: plik tekstowy mo?liwy do przetworzenia przez algorytm

*/

//#include "Node.h"
//#include "Field.h"
//#include "Brewery.h"
//#include "Pub.h"

#include <iostream>
#include "AdjacencyList.h"
#include "Matrix.h"
//#include <algorithm>
//#include <iomanip>

using namespace std;


int main()
{
    AdjacencyList nList;
    Matrix graphMatrix;
   /* if (!nList.readFileToGraph("./daneZwagami2.txt"))
    {
        return -1;
    }*/
   
    if (!graphMatrix.readFileToGraph("./daneZwagami2.txt"))
    {
        return -1;
    }
   // cout << nList.edmonsKarp() << endl;
    //cout << graphMatrix.edmonsKarp() << endl;
   //cout << graphMatrix.edmonsKarp();
    graphMatrix.maxFlowMinCost();
    return 0;
}