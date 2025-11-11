HAP ac 2 gebruik voor Labo
==========================

Introductie
-----------

HAP ac 2 is een router die gebruikt werd binnen het labo netwerking van IoT. Nu kunnen we deze gebruiken voor ons labo voor de Wifi en Home Assistant. Home Assistant en ESPHome gebruiken mDNS om hun devices te vinden. Deze optie wordt meestal in enterprise omgevingen niet toegelaten, maar wel in Home netwerken. Dit labo gaat over het beveiligen van niet en Enterprise IoT toepassingen, maar ook Home IoT devices. Deze worden ook geïnstalleerd door onze studenten en moeten dan ook beveiligd worden.

Setup van HAP ac Lite
---------------------

We vertrekken van een factory-reset-ed HAP ac 2 router. 

- wlan1 (2Ghz): voor IoT devices en Laptops die de IoT devices gebruiken
- ether1-2 (ethernet): Voor server van de leraar (zoals Pi met MQTT server)
- wlan2 (5Ghz): Uplink naar *bletchley*

### Stappenplan

#### wlan2 wordt WAN

1) Selecteer **Interfaces** uit het menu
2) Selecteer de **Interfaces List**-tab
3) Klik op de **WAN** interface
4) Selecteer *wlan2* uit de lijst al de **Interface**
5) Klik **Apply** en dan klik op **OK**

#### wlan1, ether1, ether2, ether3, ether4, ether5 vormen een bridge

We verwijderen de *wlan2* uit de *bridge* lijst en voegen er de *ether1* aan toe.

1) Selekteer **Bridge** uit het menu
2) Selekteer de **Ports**-tab
3) Klik op **wlan2** in de tabel
4) Klik vervolgens op **Remove**
5) Klik op **Add New**
6) Selekteer de *ether1* uit de lijst van **Interface**
7) Klik **Apply** en dan klik op **OK**

#### Voeg de paswoord van Bletchley toe aan de router

1) Selekteer **Wireless** uit het menu
2) Selekteer de **Security Profiles**-tab
3) Klik op **Add New**
4) Geef de volgende gegevens in:
  - Name: **bletchley**
  - Mode: **dynamic keys**
  - Selekteer enkel **WPA2 PSK** als *Authentication Types*
  - Vul het *bletchley* paswoord in **WPA2 Pre-Shared Key** invul veld
5) Klik **Apply** en dan klik op **OK**

#### Connecteer de wlan2 op bletchley netwerk

1) Selekteer **Wireless** uit het menu
2) Selekteer de **Wifi Interfaces**-tab
3) Klik op de **wlan2** interface uit het tabel
4) Pas de volgende velden aan:
  - Mode: **station**
  - SSID: **bletchley**
  - Security Profile: **bletchley**
5) Klik **Apply** en dan klik op **OK**

Check dat er data wordt uitgewisseld in *Tx* en *Rx* velden van het tabel.
Nu kan je van de Wifi afkoppelen en zou je via de Mikrotik op het internet moeten geraken. 

**Vergeet niet het MAC adres van wlan2 toe te voegen aan je bletchley account**

#### Configureer de wlan1 voor IoT devices

We gaan de *wlan1* interface herconfigureren naar **IOT-LOKAAL** met WPA2-PSK **77778888** 

1) Selekteer **Wireless** uit het menu
2) Selekteer de **Security Profiles**-tab
3) Klik op **Add New**
4) Geef de volgende gegevens in:
  - Name: **IOT-LOKAAL**
  - Mode: **dynamic keys**
  - Selekteer enkel **WPA2 PSK** als *Authentication Types*
  - Vul het **77778888** paswoord in **WPA2 Pre-Shared Key** invul veld
5) Klik **Apply** en dan klik op **OK**
6) Selekteer de **Wifi Interfaces**-tab
7) Klik op de **wlan1** interface uit het tabel
8) Pas de volgende velden aan:
  - SSID: **IOT-LOKAAL**
  - Security Profile: **IOT-LOKAAL**
9) Klik **Apply** en dan klik op **OK**

