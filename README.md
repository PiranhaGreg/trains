# Simulace vláčků s využítím POSIXových vláken

Vytvořeno pouze k naučení POSIXových vláken a jejich synchronizace (zámky, semafory).

### Spuštění

Testováno na Linuxu. Ve Windows je potřeba podporu POSIXových standardů [zapnout](http://www.codeproject.com/Tips/564154/Making-Windows-Ultimate-POSIX-compatible).

```shell
$ gcc -Wall -pedantic trains.c -o trains -lpthread
$ ./trains <počet nástupišť> <počet vlaků>
```

### Ukázka běhu

```shell
$ ./trains 2 4
Zmáčkni ENTER pro ukončení....
[53:33] Vlak 1 vyjíždí na trať. Očekávaný příjezd v 53:49.
[53:33] Vlak 2 vyjíždí na trať. Očekávaný příjezd v 53:38.
[53:33] Vlak 4 vyjíždí na trať. Očekávaný příjezd v 54:03.
[53:33] Vlak 3 vyjíždí na trať. Očekávaný příjezd v 54:07.
[53:38] Vlak 2 vjel na nástupiště 0. Očekávaný odjezd v 53:44.
[53:44] Vlak 2 vyjíždí na trať. Očekávaný příjezd v 54:12.
[53:49] Vlak 1 vjel na nástupiště 0. Očekávaný odjezd v 54:04.
[54:03] Vlak 4 vjel na nástupiště 1. Očekávaný odjezd v 54:20.
[54:04] Vlak 1 vyjíždí na trať. Očekávaný příjezd v 54:20.
[54:07] Vlak 3 vjel na nástupiště 0. Očekávaný odjezd v 54:25.
[54:20] Vlak 4 vyjíždí na trať. Očekávaný příjezd v 55:05.
[54:20] Vlak 1 vjel na nástupiště 1. Očekávaný odjezd v 54:36.
[54:25] Vlak 3 vyjíždí na trať. Očekávaný příjezd v 54:36.
[54:25] Vlak 2 vjel na nástupiště 0. Očekávaný odjezd v 54:41.
```

### Licence

Kód je možné dále volně využívat ;-).