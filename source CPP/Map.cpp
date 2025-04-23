// Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.

// algorytm Forda-Fulkersona używał macierzy sąsiedztwa, bo 
// z listą sąsiedztwa był problem przy przechodzeniu w drugą stronę
// problem został naprawiony, teraz algorytm używa listy sąsiedztwa

/*
Do zrobienia:
1. Dokumentacja (dokument tekstowy i arkusz/harmonogram)

2. Interface konsolowy - program, który będzie:
- wczytywał z pliku i zapisywał do pliku informacje o node'ach i drogach;
- generował plik ze ścieżkami, który będzie od razu działał dla algorytmu poszukującego
- przechowywał dane po wczytaniu
- pozwalał na wyświetlanie danych w konsoli
- pozwalał na modyfikację danych w konsoli
- dbał o prawidłowość wprowadzonych danych
przykładowy plik:
0.5 (konwersja jęczmienia na piwo)
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
4 (ilość wierzchołków) 8 (ilość połączeń)
1 2 30.0
1 3 20.0
1 4 30.0
2 5 7.0
2 6 3.2
3 5 8.2
4 5 8.5
4 6 6.9

3. Modyfikacja algorytmu przepływu dla problemu projektu (ten plik):
3a. Pole, przez które przechodzi ścieżka powiększająca musi aktualizować maksymalny przepływ dla każdej krawędzi, która z niego wychodzi
np. jeśli pole ma 30 zboża i wychodzą z niego 2 strzałki po 30, po przeprowadzeniu 25 zboża przez ścieżkę trzeba ustawić wszystkie strzałki na max(obecny, 30-25)
3b. Browarnia, przez którą przechodzi ścieżka powiększająca musi aktualizować przepływ dla każdej krawędzi, która do niego prowadzi, żeby nie doszło do przekroczenia pojemności browarni

4. Browary przemnażające przechodzącą wartość

5. GUI (będzie dbało o prawidłowy przepływ między polami i browarami)


?. Generator grafów (plik z połączeniami) do testowania algorytmu:
Wejście: liczba całkowita - ilość node'ów
Wyjście: plik tekstowy możliwy do przetworzenia przez algorytm

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
    if (!nList.readFileToGraph("./daneZwagami.txt"))
    {
        return -1;
    }
    if (!graphMatrix.readFileToGraph("./daneZeWszystkim.txt"))
    {
        return -1;
    }
    cout << nList.fordFulkerson() << endl;
    cout << graphMatrix.fordFulkerson() << endl;

    return 0;
}