#  Proiect Smart Home Automatizat cu Arduino

Acest proiect prezintă o casă inteligentă la scară redusă, bazată pe placă Arduino UNO, care combină mai multe componente electronice pentru a automatiza acțiuni uzuale: controlul iluminării, ventilării și deschiderea ușii.

##  Obiectivele proiectului

- Automatizarea deschiderii ușii cu senzor ultrasonic și servomotor
- Controlul iluminării pe timp de noapte folosind un fotorezistor și LED-uri
- Activarea automată a ventilatorului în funcție de temperatură, cu senzor DHT11
- Afișarea temperaturii și umidității în timp real pe un LCD
- Creșterea confortului locatarilor prin soluții hands-free și consum redus de energie

##  Funcționalități implementate

1. **Deschiderea automată a ușii** – la apropierea unei persoane detectate de senzorul ultrasonic.
2. **Iluminare automată** – LED-uri activate când lumina ambientală scade sub un anumit prag.
3. **Ventilator automat** – controlat de un releu, pornește la temperaturi peste 26°C.
4. **Afișaj LCD** – arată temperatura și umiditatea în timp real.
5. **Feedback sonor** – buzzer care confirmă acțiunile sistemului.

##  Componente utilizate

- Arduino UNO R3
- Breadboard 830 puncte
- DHT11 – senzor de temperatură și umiditate
- Releu 5V
- Motor DC pentru ventilator
- Servomotor SG90 pentru ușă
- Senzor ultrasonic HC-SR04
- Buzzer pasiv
- Fotorezistor (LDR)
- LED-uri (x4)
- LCD 16x2 cu I2C
- Baterie 9V

##  Contribuții

- **Budur Armand-Cristian** – cod pentru buzzer, LED, fotorezistor, LCD; depanare și design documentație.
- **Dincă Marta** – macheta casei, montaj componente fizice, documentație.
- **Nedelcu Bianca-Nicoleta** – cod pentru releu, ventilator, servomotor și senzor ultrasonic; montaj electric și videoclipul demonstrativ.

##  Videoclip demonstrativ

 Vizualizează proiectul:  
[https://www.youtube.com/watch?v=tYilgjNoq7E](https://www.youtube.com/watch?v=tYilgjNoq7E)

##  Rezultate și testare

Fiecare componentă a fost testată individual, apoi integrată într-un sistem complet, capabil să ruleze simultan toate funcțiile fără întreruperi. S-au folosit tehnici de programare non-blocantă (`millis()` în loc de `delay()`) pentru funcționare paralelă.

##  Posibilități de extindere

Următorul pas este integrarea cu o platformă IoT (ESP8266/ESP32) pentru:

- Transmiterea datelor în cloud
- Control de la distanță prin aplicație mobilă
- Automatizări avansate pe bază de scenarii


