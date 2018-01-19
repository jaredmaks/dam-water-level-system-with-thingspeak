# dam-water-level-system-with-thingspeak
I DID THIS PROJECT WITH MY DEKUT GROUP MEMBERS.//https://github.com/DekutIOTDataScience/level_system_thingspeak_beta
this project, dam water level system measures the level of  water fluctuations i a pond,dam reservoir,stores the data that is the range,using ultrasonic sensor,sends the data to the cloud for storing and livestreaming,this data is later used for trainning and machine learning.
A level system using ultrasonic range finder. The reading is sent to thingspeak via ESP8286 for storage and a backup SD Card incase the data fails to be sent to Thingspeak Server. A DS3231 timer is used to attach time to the value recorded on the SD card.

Being a Beta version it has the following characteristics which we will improve in future.

The Nucleo F411RE tries to send data via hotspot using ESP8286 even though no wifi hotspot is available. Inconsistency of data storage.Thingspeak and SD card might store data in intervals if wifi is not strong enough.

Future Improvements: Consistent recording on sd card for a while before trying to check wifi availability.Hence consistent storage.

Overcoming the Inconsistency Since there is timestamp attached to data in the sd card and data stored in ThingSpeak Servers a sorting software can be used to inject data recorded on the sd card to csv file downloaded from thingspeak when doing analysis.


![img_20180119_120945](https://user-images.githubusercontent.com/30165974/35169038-1a7b7ec0-fd6c-11e7-9489-37a00773c84e.jpg)
