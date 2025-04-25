# LibraryPAO
Progetto Biblioteca per Esame PAO 2025

## Idee per Polimorfismo Non Banale

### 1. Metodo di prestito con comportamenti diversi
` virtual LoanInfo calculateLoan(int days) const = 0; `

- **Libri**: prestito standard di 30 giorni
- **Film**: prestito breve di 7 giorni
- **Articoli**: solo consultazione in sede

### 2. Metodo di visualizzazione con interfacce diverse

` virtual QWidget* createDetailsWidget() const = 0; `

Ogni tipo di media crea un widget Qt personalizzato con layout e controlli specifici

### 3. Metodo di esportazione in formati diversi

` virtual void exportToFormat(const QString& format) const = 0; `

- Per i **libri**: potrebbe generare una citazione bibliografica in stile APA/MLA
- Per i **film**: potrebbe generare una scheda tecnica per festival cinematografici
- Per gli **articoli**: potrebbe generare una citazione scientifica completa

## Funzionalità aggiuntive interessanti
### 1. Filtri intelligenti
- Filtra per genere (diversi per ogni tipo di media)
- Filtra per intervallo di anni
- Filtra per disponibilità

### 2. Statistiche
- Grafico della distribuzione per anno
- Conteggio per genere/tipo
- Media pagine per libro, durata per film, ecc.

## Persistenza dei dati
- Usa QJsonDocument per JSON

## Classi e attributi
### Media (superclasse)
#### Attributi
- titolo
- anno
- genere
- id
- disponibile
- nprestiti
- proxDisp
#### Metodi
- mostraDettagli
- toJson
- fromJson
- calcolaPrestito
----

### Libro
#### Attributi
- autore
- editore 
- pagine
- isbn
#### Metodi
- mostraDettagli
- calcolaPrestito
----

### Film
#### Attributi
- regista
- durata
- cast
#### Metodi
- mostraDettagli
- calcolaPrestito
----

### Articolo
#### Attributi
- autore
- volume
- rivista (nome rivista)
- pagine
#### Metodi
- mostraDettagli
- calcolaPrestito
