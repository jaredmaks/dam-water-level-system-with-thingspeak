# dam-water-level monitoring -system-with-thingspeak
I DID THIS PROJECT WITH MY DEKUT GROUP MEMBERS.//https://github.com/DekutIOTDataScience/level_system_thingspeak_beta
this project, dam water level system measures the level of  water fluctuations i a pond,dam reservoir,stores the data that is the range,using ultrasonic sensor,sends the data to the cloud for storing and livestreaming,this data is later used for trainning and machine learning.
A level system using ultrasonic range finder. The reading is sent to thingspeak via ESP8286 for storage and a backup SD Card incase the data fails to be sent to Thingspeak Server. A DS3231 timer is used to attach time to the value recorded on the SD card.

Being a Beta version it has the following characteristics which we will improve in future.

The Nucleo F411RE tries to send data via hotspot using ESP8286 even though no wifi hotspot is available. Inconsistency of data storage.Thingspeak and SD card might store data in intervals if wifi is not strong enough.

Future Improvements: Consistent recording on sd card for a while before trying to check wifi availability.Hence consistent storage.

Overcoming the Inconsistency Since there is timestamp attached to data in the sd card and data stored in ThingSpeak Servers a sorting software can be used to inject data recorded on the sd card to csv file downloaded from thingspeak when doing analysis.


![img_20180119_120945](https://user-images.githubusercontent.com/30165974/35169038-1a7b7ec0-fd6c-11e7-9489-37a00773c84e.jpg)

![img_20180119_144032](https://user-images.githubusercontent.com/30165974/35169338-42c22a2c-fd6d-11e7-86d0-8070f14997f3.jpg)




![img_20180119_144003](https://user-images.githubusercontent.com/30165974/35169878-3b08f188-fd6f-11e7-8a54-9e9a0273b2c8.jpg)

this is the distribution and mapping aspect of folum on thinspeak
![11](https://user-images.githubusercontent.com/30165974/35180506-07e2fc8c-fdc3-11e7-8ed0-0ba661ecd1c6.PNG)![2](https://user-images.githubusercontent.com/30165974/35180510-13e13576-fdc3-11e7-983f-333785e6bde3.PNG)
![3](https://user-images.githubusercontent.com/30165974/35180516-1ba51a70-fdc3-11e7-8a98-269c64df23eb.PNG)
![qq](https://user-images.githubusercontent.com/30165974/35180529-4a0cda38-fdc3-11e7-8893-2d935d83f83c.PNG)



