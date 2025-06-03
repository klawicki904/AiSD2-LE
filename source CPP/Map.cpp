
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

#include <iostream>
#include "AdjacencyList.h"
#include "Matrix.h"

using namespace std;

int main()
{
    AdjacencyList nList;
    Matrix graphMatrix , matrix2;
   /* if (!nList.readFileToGraph("./daneZwagami2.txt"))
    {
        return -1;
    }*/
   
    if (!graphMatrix.readFileToGraph3("./daneZeWszystkim.txt"))
    {
        return -1;
    }

    if (!matrix2.readFileToGraph3("./daneEksperymentalne.txt"))
    {
        return -1;
    }


    graphMatrix.maxFlowMinCost2();
    matrix2.maxFlowMinCost2();
    return 0;
}