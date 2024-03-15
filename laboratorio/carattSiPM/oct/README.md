## Istruzioni per il OCT
### Prima della misura:
Impostare il *gate* uguale a quello del DCR. 

Effettuare lo stesso fit di [dcr](../dcr) e trovare il valore di *treshold* per $\nu_2$ (secondo gradino) ($V_2\approx2V_1$).

robabilità che sia avvenuto il DCR al secondo livello casualmente è: $\nu_{2rand}=\nu_1^2 \tau$ (dovrebbe essere bassa).

### Una volta prese le misure:
- Impostare la variabile *count* con (numero set di misure - 1)
- Impostare il tempo medio $\tau$ con il tempo di misura *gate* (uguale ad DCR)
- Se i tutti i bin sono $n>>1$ allora non cambiare niente, altrimenti fittare la poissoniana
    - Se è gaussiano verificare (a vista??) che $\sigma\approx \sqrt{\mu}$ per ogni grafico
    - Nella GUI si ossono vedere i valori già fittati in basso a destra
- Ricordarsi di prendere le temperature di ogni set di dati e salvarle nel file *octTmp.txt* (mettere solo una colonna di temperature in ordine)
- Salvare tutti i set in file separati nominati oct0.txt, oct1.txt, ecc... (oppure cambiare nome). Vanno salvati nella stessa cartella della macro
- Modificare le unità di misura e i titoli del grafico se necessario
- Avviare il programma
> **_NOTA:_** uscire da root prima di richiamare nuovamente la macro

### Da completare ancora:
- [ ] 

### Problemi:
- [ ] Molti si ereditano da quelli in [dcr](../dcr)
