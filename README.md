# pz

## O projektu

- **pz** je konzolna aplikacija koja demonstrira efikasnu upotrebu paralelizma pri radu sa fajlovima. Aplikacija deli ulazne fajlove na blokove, obrađuje ih konkurentno u zasebnim nitima i sastavlja rezultat, postižuči značajno ubrzanje u poređenju sa serijskim pristupom. 

- Kompresija/dekompresija se vrše upotrebom Hafmanovog koda.

## Funkcionalnosti

- **Paralelna kompresija** - ulazni fajl se deli na blokove koji se kompresuju istovremeno u više niti
- **Paralelna dekompresija** - kompresovani fajl se razlaže i dekompresuje paralelno

## ThreadPool

- **ThreadPool** implementacija preuzeta sa: [repo](https://github.com/progschj/threadpool).
