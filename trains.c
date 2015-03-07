#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h> 
#include <semaphore.h>

/* Stejná data pro všechny vlaky.
 * Možnost definovat jako globální proměnné. */
typedef struct {
	pthread_mutex_t mutex;
	sem_t semaphore;
	int platforms;
	int * station;
} Data;

/* Wrapper pro předávání parametrů vláknu */
typedef struct {
	Data * data;
	int id;
} Tuple;

/* Vrátí náhodnou hodnotu z rozsahu */
int randomRange(int min, int max) {
	return rand() % (max - min + 1) + min;
}

/* Vrátí aktuální čas */
struct tm getTime() {
	time_t now = time(0);
	return *localtime(&now);
}

/* Vrátí čas za daný počet sekund */
struct tm getTimeAfter(int seconds) {
	time_t now = time(0) + seconds;
	return *localtime(&now);
}

/* Funkce vyšle vlak na náhodně dlouhou cestu */
void leave(int id) {
	int t = randomRange(5, 50); /* 5 - 50 s */
	struct tm now = getTime();
	struct tm end = getTimeAfter(t);

	printf("[%02d:%02d] Vlak %d vyjíždí na trať. Očekávaný příjezd v %02d:%02d.\n",
		now.tm_min, now.tm_sec, id, end.tm_min, end.tm_sec);

	/* cesta vlakem k semaforu */
	sleep(t);
}

/* Funkce zařizující odbavení vlaku na nádraží */
void enter(int id, Data * data) {
	int i = 0, t = randomRange(5, 20); /* 5 - 20 s */
	struct tm now, end;
	
	/* vyhýbku může obsluhovat max jeden vlak
	 * ostatní vlaky na nástupišti musí počkat */
	pthread_mutex_lock(&data->mutex);
	while((data->station)[i])
		i++;

	(data->station)[i] = id;
	pthread_mutex_unlock(&data->mutex);

	now = getTime();
	end = getTimeAfter(t);

	printf("[%02d:%02d] Vlak %d vjel na nástupiště %d. Očekávaný odjezd v %02d:%02d.\n",
		now.tm_min, now.tm_sec, id, i, end.tm_min, end.tm_sec);

	/* zastavení vlaku na náhodnou dobu */	
	sleep(t);

	/* uvolnění nástupiště pro další vlaky */
	pthread_mutex_lock(&data->mutex);
	(data->station)[i] = 0;
	pthread_mutex_unlock(&data->mutex);
}

/* Hlavní funkce vlákna */
void * train(Tuple * arg) {
	Data * data = arg->data;

	while(1) {
		leave(arg->id);

		sem_wait(&data->semaphore);
		enter(arg->id, data);
		sem_post(&data->semaphore);
	}

	return NULL;
}

int main(int argc, char ** argv) {
	pthread_attr_t attr;
	pthread_t * trains;
	Tuple * args;
	Data data;
	int i, n;

	/* aby šla vlákna spojit a dodržovala frontu před semaforem */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

	/* Načtení vstupu */
	if (argc != 3 || sscanf(argv[1], "%d", &data.platforms) != 1 ||
		data.platforms <= 0 || sscanf(argv[2], "%d", &n) != 1 || n <= 0) {
		printf("usage: %s <počet nástupišť> <počet vlaků>\n", argv[0]);
		return 1;
	}

	printf("Zmáčkni ENTER pro ukončení....\n");

	/* inicializace zámku a semaforu */
	pthread_mutex_init(&data.mutex, NULL);
	sem_init(&data.semaphore, 0, data.platforms);

	/* alokace paměti pro stanici, vlaky (vlákna) a argumetny pro vlákna */
	data.station = calloc(n, sizeof(int));
	trains = malloc(n * sizeof(pthread_t));
	args = malloc(n * sizeof(Tuple));

	/* spuštění simulace */
	for (i = 0; i < n; i++) {
		args[i].data = &data;
		args[i].id = i + 1;
		pthread_create(trains + i, &attr, (void *(*)(void *))train, args + i);
	}
	
	/* ukončení simulace */
	getchar();
	for (i = 0; i < n; i++) {
		pthread_cancel(trains[i]);
		pthread_join(trains[i], NULL);
	}

	/* uvolnění všech využívaných prostředků */
	free(data.station);
	free(trains);
	free(args);

	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&data.mutex);
	sem_destroy(&data.semaphore);

	return 0;
}