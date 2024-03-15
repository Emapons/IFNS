# Istruzioni per il RC (Raggi Cosmici)
### Prima della misura:
Seguire la scheda.
Prendere le misure della dimensione dello scintillatore (anche l'altezza) e della distanza dei due.

### Una volta prese le misure:
- Salvare i dati su un file in colonna *rc.txt*
- Segnarsi il tempo di *gate*
- decommentare (se serve) la linea con il resize del bin
- scegliere e impostare il range del fit dei dati (di base sono tutti)
- (usare una gaussiana o poissoniana???)
- Avviare il programma
> **_NOTA:_** uscire da root prima di richiamare nuovamente la macro

### Da completare ancora:
- [ ] Fare la seconda parte, fittare il Bethe-Bloch 

### Problemi:
- Il rebin è necessario?
- fare solo il fitt del picco-parte sinistra dei dati con una gaussiana?
- Ci sono correzioni da fare per il fuori asse?
- Il picco a 0 a cosa corrisponde?
