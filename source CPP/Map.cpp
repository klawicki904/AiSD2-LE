// Ten plik zawiera funkcjê „main”. W nim rozpoczyna siê i koñczy wykonywanie programu.

// algorytm Forda-Fulkersona u¿ywa³ macierzy s¹siedztwa, bo 
// z list¹ s¹siedztwa by³ problem przy przechodzeniu w drug¹ stronê
// problem zosta³ naprawiony, teraz algorytm u¿ywa listy s¹siedztwa

/*
Do zrobienia:
1. Dokumentacja (dokument tekstowy i arkusz/harmonogram)

2. Interface konsolowy - program, który bêdzie:
- wczytywa³ z pliku i zapisywa³ do pliku informacje o node'ach i drogach;
- generowa³ plik ze œcie¿kami, który bêdzie od razu dzia³a³ dla algorytmu poszukuj¹cego
- przechowywa³ dane po wczytaniu
- pozwala³ na wyœwietlanie danych w konsoli
- pozwala³ na modyfikacjê danych w konsoli
- dba³ o prawid³owoœæ wprowadzonych danych
przyk³adowy plik:
0.5 (konwersja jêczmienia na piwo)
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
4 (iloœæ wierzcho³ków) 8 (iloœæ po³¹czeñ)
1 2 30.0
1 3 20.0
1 4 30.0
2 5 7.0
2 6 3.2
3 5 8.2
4 5 8.5
4 6 6.9

3. Modyfikacja algorytmu przep³ywu dla problemu projektu (ten plik):
3a. Pole, przez które przechodzi œcie¿ka powiêkszaj¹ca musi aktualizowaæ maksymalny przep³yw dla ka¿dej krawêdzi, która z niego wychodzi
np. jeœli pole ma 30 zbo¿a i wychodz¹ z niego 2 strza³ki po 30, po przeprowadzeniu 25 zbo¿a przez œcie¿kê trzeba ustawiæ wszystkie strza³ki na max(obecny, 30-25)
3b. Browarnia, przez któr¹ przechodzi œcie¿ka powiêkszaj¹ca musi aktualizowaæ przep³yw dla ka¿dej krawêdzi, która do niego prowadzi, ¿eby nie dosz³o do przekroczenia pojemnoœci browarni

4. Browary przemna¿aj¹ce przechodz¹c¹ wartoœæ

5. GUI (bêdzie dba³o o prawid³owy przep³yw miêdzy polami i browarami)


?. Generator grafów (plik z po³¹czeniami) do testowania algorytmu:
Wejœcie: liczba ca³kowita - iloœæ node'ów
Wyjœcie: plik tekstowy mo¿liwy do przetworzenia przez algorytm

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

//double beerConvertsionRate = 0.5; // jeszcze nie u¿yte
//// mo¿emy zapisaæ ka¿dy typ w oddzielnej klasie, ale bêd¹ przechowywaæ co najwy¿ej swoje po³o¿enie i jedn¹ zmienn¹
//// (jeszcze nie u¿yte) rodzaj wêz³a (¿adne (Ÿród³o lub ujœcie), pole, browarnia, karczma)
//enum NodeType
//{
//    none,
//    field,
//    brewery,
//    pub
//};
//// (jeszcze nie u¿yte) struktura wektor do przechowania po³o¿enia wierzcho³ka
//struct Vector2
//{
//    int x = 0;
//    int y = 0;
//};


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

   // nList.printGraph();
    //graphMatrix.printGraph();
    
    cout << nList.fordFulkerson() << endl;
    cout << graphMatrix.fordFulkerson() << endl;


   /* for (Node i : graphMatrix.listVertives) {
        if (i.capacity != -1) {
            cout <<i.id << " " << i.x << endl;
        }
    }*/
    //cout << graphMatrix.fordFulkerson2() << endl;

    
    return 0;
}