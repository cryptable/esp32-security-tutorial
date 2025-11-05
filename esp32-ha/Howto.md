Integration of RC522 with Home Assistant
========================================

Introduction
------------

This is a overview where the RC522 reader will be connected to a Home Assistant Virtual Box. When a tag is held in front of the reader a LED will be enbled as a switch to open the door.

Installation of Home Assistant
------------------------------

### Pre-requisites:

- Working version of VirtualBox 7.x
- 32 Gb free space
- minimum 4 Gb memory


You need to download the Virtual Box image of [Home Assistant Linux](https://www.home-assistant.io/installation/linux) and unpack the zip-file under the *Virtual VMs*-folder.

```Warning
CHECK YOU ARE NOT PUTTING IT ON One-Drive (Your Documents-folder is most of the time on One-Drive). It will be very slow or even the vm won't work!
```

### Create the virtual machine

Load the appliance image into your virtual machine hypervisor. (Note: You are free to assign as much resources as you wish to the VM, please assign enough based on your add-on needs).

Minimum recommended assignments:

- 2 GB RAM
- 32 GB Storage
- 2vCPU

1. Create a new virtual machine and call it **Home Assistant**.
2. Select type Linux and version Linux 2.6 / 3.x / 4.x (64-bit).
3. Under Hardware, select the amount of memory and number of CPUs. Then, select Enable EFI.

**Make sure EFI is enabled. If EFI is not enabled, HAOS won’t boot.**

4. Under Hard Disk, select Use an existing virtual hard disk file, select the unzipped VDI file from above.
5. Then go to Network > Adapter 1. Choose Bridged Adapter and choose your network adapter.

```
 Please keep in mind that the bridged adapter only functions over a hardwired Ethernet connection. Using Wi-Fi on your VirtualBox host is unsupported.
```

6. Then go to Audio and choose Intel HD Audio as audio controller.

### Start up your virtual machine

1. Select the **Home Assistant** Virtual Machine and press the *Start* button
2. Observe the boot process of the Home Assistant Operating System.
3. Once completed, you will be able to reach Home Assistant on [](http://homeassistant.local:8123).

Now you can create an account (username/password) on the system, which you should remember ofcourse. Use Keepass2 to help you with these accounts.

Visit the system and have a look around Home Assistant.

When encountering any problem, check the Home Assistant web-site.

ESPHome.io for Home Assistant
-----------------------------

### Installation

You have a working **Home Assistant**. You also had a look in the dashboard and web interface. It is time now to install the [ESPHome Add On](https://esphome.io/guides/getting_started_hassio).

1) Click on *Settings*
2) Select *Add-ons*
3) Click on *ADD-ON Store*
4) Search for **ESPHome** and select id
5) Click on Install

### Configuration for ESP32

Now you need to add your [ESP32 with RC522](https://esphome.io/components/binary_sensor/rc522.html) to your Home Assistant.

1) Click on *Settings*
2) Select *Add-ons*
3) Click on the *ESPHome* small card
4) On the ESPHome overview card, you'll find *OPEN WEB-UI* button. Click on the button.

You are now in the ESPHome configuration site of Home Assistant. It is time to add your RC522 to open the door.

1) Click on *+ NEW DEVICE*
2) Connect your ESP32 with your USB cable to the computer (check you are using Chrome, because programming through the browser works certainly with Chrome), click *CONTINUE*
3) Give it a name **Deur 001** and the network ssid and its password, click *NEXT*
4) Select the *ESP32*
5) Click *INSTALL* (first backup the displayed key if necessary)
6) Use *Plug into this computer* and the firmware will be written to your ESP32

If step 6 gives you problems, you can also download the firmware and write it using esptool.exe or use the website [Open ESPHome Web](https://web.esphome.io/?dashboard_install)

Now you ESP32 can connect to Home Assistant, but you RC522 is not yet configured.

Add your new device to home assistant so you can perform automations.

1) Goto *Settings*
2) Select *Devices and Services*
3) You see *Dear 001* in the discovered devices, click on **Add**

Goto the seperate offline network of you IoT Devices: (*IOT-LOKAAL*)

### Configure the RC522

For the configuration of your RC522 to your ESP32, you will need to build a new firmware with the configuration of RC522.

1) Click the *EDIT* button on your device
2) This shows an YAML editor and you need to add the configuration define here under.

```
# RC522 configuration
spi:
  clk_pin: GPIO18
  miso_pin: GPIO19
  mosi_pin: GPIO23

rc522_spi:
  cs_pin: GPIO05
  reset_pin: GPIO21
  on_tag:
    then:
      - homeassistant.tag_scanned: !lambda 'return x;'
```

3) Click on *INSTALL*
4) Select *Wirelessly* or repeat step 6 from above.

### Configure the LED as Door lock

For the configuration of your door will be a LED on your ESP32, you will need to build a new firmware again with the configuration of the LED.

```
# LED to open the door
light:
  - platform: status_led
    name: 'Door switch'
    pin: GPIO12
```

### Automation when Mifare Tag is Scanned

When you keep your Tag (FOB or Card) in front of the scanner nothing happens, because Home Assistant hasn't been programmed yet to do something. You could see the tags which were scanned under *Settings* -> *Tags* 

So let's introduce an automation.

1) Select *Settings*
2) Click on *Automations and Scenes*
3) Click on *Add Automation*
4) Click on *Add Automation* again
5) On the screen of New Automations:
  1) Click on *Add Trigger*
  2) Search for **Tag**, Select the *Tag*
  3) Click on dropdown button (arrow down), you should find your scanned tags, when you were testing the tags.
  4) Under **Then**, click on *Add Action*
  5) Select *Light*
  6) Under Goals, click on *+ Devices* and select your device 'Deur 001'
  7) Click *Save*
  8) Give it a name and click *Save* again
