# Cross-Docking Truck Scheduling

Progetto di ottimizzazione discreta per il corso di **Advanced Scheduling Systems**  
Università degli Studi di Udine — A.A. 2025/2026

---

## Descrizione del problema

In un centro di smistamento (*cross-dock*), un insieme di camion in entrata (inbound) deve scaricare merce su porte di ingresso, e un insieme di camion in uscita (outbound) deve caricare merce dalle porte di uscita. Il problema consiste nello stabilire la **sequenza di attracco** dei camion alle porte disponibili, tenendo conto dei vincoli di precedenza tra operazioni inbound e outbound (la merce deve prima essere scaricata, poi caricata).

**Obiettivo:** minimizzare il **makespan**, ovvero il tempo totale necessario per completare tutte le operazioni di scarico e carico.

Il problema è assimilabile a un **Flow Shop Scheduling** a due stadi con vincoli di sincronizzazione tra i due flussi di camion.

---

## Struttura del progetto

```
crossdock-truck-scheduling/
├── Instances/              # Istanze di test in formato .dzn
│   ├── cdp1.dzn
│   ├── cdp2.dzn
│   ├── cdp5.dzn
│   ├── cdp10.dzn
│   └── cdp20.dzn
├── CD_BacktrackingOpt/     # Solver C++ basato su backtracking con branch-and-bound
│   ├── BacktrackingOpt.hh          # Template generico di backtracking ottimizzante
│   ├── BacktrackingCDOpt.hh        # Header specializzazione per Cross-Docking
│   ├── BacktrackingCDOpt.cc        # Implementazione specializzazione
│   ├── CD_Data.hh                  # Strutture dati Input/Output
│   ├── CD_Data.cc                  # Implementazione lettura istanza e output
│   ├── TestCDOpt.cc                # Main: esegue il solver su un file istanza
│   └── Makefile                    # Build system
└── README.md
```

---

## Formato delle istanze (`.dzn`)

```
InboundTrucks = <n>;
OutboundTrucks = <m>;
InboundDoors = <di>;
OutboundDoors = <do>;
ReleaseTimes = [<r0>, <r1>, ..., <rn-1>];
UnloadingTimes = [<u0>, <u1>, ..., <un-1>];
LoadingTimes = [<l0>, <l1>, ..., <lm-1>];
TransferMatrix = [| <t00>, <t01>, ...
                  | <t10>, <t11>, ...
                  ...];
```

- `ReleaseTimes[i]`: tempo di disponibilità del camion inbound `i`
- `UnloadingTimes[i]`: durata dello scarico del camion inbound `i`
- `LoadingTimes[j]`: durata del carico del camion outbound `j`
- `TransferMatrix[i][j]`: tempo di trasferimento della merce dal camion inbound `i` al camion outbound `j` (0 se non c'è dipendenza)

---

## Compilazione

```bash
cd CD_BacktrackingOpt
make
```

Richiede un compilatore C++11 o superiore (g++ raccomandato).

---

## Esecuzione

```bash
./TestCDOpt.exe ../Instances/cdp5.dzn
```

Oppure in modalità interattiva:

```bash
./TestCDOpt.exe
Input file name: ../Instances/cdp5.dzn
```

**Output atteso:**
```
First solution [...] (current cost 42)
New best solution [...] (current cost 38, previous 42)
Best solution found [0, 2, 1, 3, 4] (makespan = 38, 127 nodes visited)
```

---

## Metodo di soluzione

Il solver implementa un **backtracking branch-and-bound** che esplora lo spazio delle soluzioni costruendo iterativamente la sequenza dei camion inbound alle porte di ingresso. Ad ogni livello dell'albero di ricerca:

1. **`FirstOfLevel()`**: assegna il primo camion inbound disponibile alla porta corrente.
2. **`NextOfLevel()`**: prova il successivo camion inbound non ancora assegnato.
3. **`Feasible()`**: verifica che i vincoli di precedenza e capacità delle porte siano rispettati al livello corrente.
4. **`NonImprovingBranch()`**: potatura per bound — se il costo parziale supera già il miglior makespan trovato, il ramo viene abbandonato.
5. **`Cost()`**: calcola il makespan completo della soluzione al livello foglia.

L'albero viene esplorato in profondità (*depth-first*), con potatura aggressiva che elimina tutti i rami non miglioranti.

---

## Riferimento bibliografico

Yi, J., Scholz-Reiter, B., & Shi, Y. (2021). *Truck scheduling problem in a cross-docking system with release dates and time windows*. Journal of Industrial and Systems Engineering.

---

## Autore

Edoardo Scudeller — Ingegneria Gestionale, Università degli Studi di Udine
