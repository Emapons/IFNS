## Istruzioni per il DCR

### Prima della misura:
Trovare il valore di *Treshold* [mV] con un *gate* [ms] fissato per la prima frequenza $\nu_1$. Vedere la scheda per maggiori dettagli)

Si definisce: $DCR = \frac{\nu_2}{\nu_1}$.

Salvare come immagine questo grafico.

### Una volta prese le misure:
- Impostare la variabile *count* con (numero set di misure - 1)
- Impostare il tempo medio $\tau$ con il tempo di misura *gate*
- Se i tutti i bin sono $n>>1$ allora non cambiare niente, altrimenti fittare la poissoniana
    - Se è gaussiano verificare (a vista??) che $\sigma\approx \sqrt{\mu}$ per ogni grafico
    - Nella GUI si ossono vedere i valori già fittati in basso a destra
- Ricordarsi di segnarsi tutte le temperature dei vari set di dati nel file *dcrTmp.txt* (mettere solo una colonna di temperatre in ordine)
- Salvare tutti i set in file separati nominati dcr0.txt, dcr1.txt, ecc... (oppure cambiare nome). Vanno salvati nella stessa cartella della macro
- Modificare le unità di misura sul grafico e i titoli se necessario.
- Avviare il programma
> **_NOTA:_** uscire da root prima di richiamare nuovamente la macro

### Da completare ancora:
- [ ] 

### Problemi (magari anche da chedere in laboratorio):
- [ ] Cosa rappresenta $\sigma_{\bar r}$ nelle schede?
- [ ] Devo fare la media di tutti gli $r$ e utilizzare $\sigma_{\bar r}$ come incertezza? (ho fatto così nel programma)
- [ ] Che errore associare alle temperature? ( nel programma ho fatto in modo di mettere un solo errore uguali per tutti)
- [ ] Che funzione fittare per il rate in funzione della temperatura? (nel programma ho messo polinomio primo ordine) 

