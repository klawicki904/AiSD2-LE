generated_map1.txt - połączenia jednokierunkowe, ale przy wypisywaniu ścieżek indeksy prawdopodobnie się nie zgadzają (te ostatnie mają zbyt dużą wartość)
generated_map2.txt - połączenia dwukierunkowe; program się zawiesza

Warto wspomnieć o tym, że połączenia dwukierunkowe nie są wypisane po kolei indeksami, ale na zmianę, to znaczy:
1->10
10->1
2->5
5->2
...

Ponadto:
- pomiędzy punktem startowym a polami nie ma połączeń dwukierunkowych
- pomiędzy karczmami a punktem końcowym nie ma połączeń dwukierunkowych