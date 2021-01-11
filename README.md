# Rail

**Cescon Francesco, Pinton Leonardo, Schiavo Carlotta**

2° assegnamento

## Line description e timetable
Sono state create 2 linee:
1. Venezia-Bologna (```line_description.txt```)
2. Milano-Venezia (```line_description_1.txt```)

I file:
* ```timetables.txt``` e ```timetables_1.txt``` sono per la linea (1)
* ```timetables_2.txt``` e ```timetables_3.txt``` sono per la linea (2)

## Struttura del codice
Sono state realizzate 4 classi pricipali:

* **Line**: rappresentante la linea ferroviaria, suddivisa in zone;
* **Station**: che si occupa di rappresentare le stazioni e le loro caratteristiche, divisa in due classi derivate per le due tipologie di stazioni;
* **Train**: che rappresenta i treni, divisa in tre sottoclassi per i vari tipi di treni. Questa contiene le informazioni del treno quali direzione, posizione sulla linea, ritardo e tabella oraria;
* **Rail**: nella cartella Rail_Class, che si occua di gestire il caricamento dei dati da file, la creazioni delle istanze delle varie classi e di elaborare la simulazione del traffico.

### Simulatore
La funzione ```Rail::simulate()```, dopo aver caricato la linea e la timetable relativa, si occuperà di effettivamente fare la simulazione. Questa viene fatta in modalità "post-eventuale" tramite un ciclo minuto per minuto.

Con il termine "post-eventuale" si intende che si va ad aggiorare lo stato del treno entro i limiti di quello che avrebbe potuto fare se fosse rimasto inalterato. Se, ad esempio, il treno avrebbe superato un parcheggio nel minuto appena trascorso, vado a verificare se fosse necessario che nel parcheggio si fermasse. Questo differisce da una simulazione che va, invece, a decidere come muovere un treno "sapendo" che andrebbe ad incrociarne un altro successivamente.
