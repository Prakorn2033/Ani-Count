# Ani-Count

Physical Computing Project 2024 - IT KMITL

## Introduction to Ani-Count

สวัสดีครับ ใน Physical Computing Project ที่ผมจะมานำเสนอในโปรเจค Ani-Count 
คือเครื่องนับจำนวนสัตว์เลี้ยงที่เราเลี้ยงไว้ด้วยการนำเรื่องนี้ไปติดกับประตู และเซ็ตค่าผ่าน MQTT ได้เลยครับ
หรือก็คือเราสามารถส่งค่าที่ต้องการนับผ่าน Broker และให้ Ani-Count ช่วยเรานับสัตว์เลี้ยงที่ผ่านประตูอย่างง่ายดาย
เมื่อนับเสร็จ Ani-Count จะทำการแจ้งผ่าน Broker ที่เราเซ็ตค่าไว้ว่าจำนวนที่เรานับนั้นครบหรือไม่หรือมีสัตว์เลี้ยงของเรา
หายไปหรือไม่นั้นเอง

## Overview

Ani-Count ของเราพัฒนาด้วยภาษา C และใช้บอร์ด Arduino UNO R4 WiFi เป็นบอร์ด Arduino รุ่นต่อจาก UNO R3 ใช้ชิพ RA4M1 จาก Renesas เป็น 
Arm Cortex-M4 48 MHz 32 kB RAM 256 kB Flash ทำงานร่วมกับ ESP32-S3 กับ HC-SR04 SENSOR เป็น MODULE ULTRASONIC 
เป็น SENSOR ใช้วัดระยะทางของวัตถุ หรือสิ่งกีดขวางที่อยู่ด้านหน้า SENSOR โดยใช้หลักการส่งคลื่นเสียง ULTRASONIC ออกไป และรับคลื่นเสียงที่สะท้อนกลับมา
อุปกรณ์ทั้ง2ตัวใช้ในการรับค่าระยะทางที่แมวต้องเดินผ่าน และสุดท้าย MQTT เป็นโปรโตคอลการส่งข้อความที่อิงตามมาตรฐาน หรือชุดของกฎที่ใช้สำหรับการสื่อสารระหว่างเครื่องต่อเครื่อง
เซ็นเซอร์อัจฉริยะ อุปกรณ์สวมใส่ และอุปกรณ์ Internet of Things (IoT) เพื่อนำข้อความหรือค่าที่ได้จาก MQTT มาใช้ในการสั่ง Ani-Count รับค่าจำนวนที่ต้องการในการนับ

## Contributors

| รูป | รหัสนักศึกษา     | ชื่อ                  | ส่วนที่รับผิดชอบ               |
| --- | -------- | --------------------- | ------------------------------ |
|   <img height="150" src="img/Nam.png" width="150"/>  | 62070115 | นาย ปรากรณ์ คำเภา       | File System Type               |

## นำเสนอ

| รูป | ชื่อ                  | ตำแหน่ง               |
| --- | --------------------- | ------------------------------ |
|   <img height="150" src="img/Nam.png" width="150"/>  | รศ.ดร. ปานวิทย์ ธุวะนุติ       | อาจารย์ประจำวิชา Physical Computing            |
|   <img height="150" src="img/Nam.png" width="150"/>  | รศ.ดร. กิติ์สุชาต พสุภา      | อาจารย์ประจำวิชา Physical Computing            |

## Tools

- **Arduino IDE :** ใช้พัฒนา Source code ภาษา C เพื่อทำงานกับ บอร์ด Arduino UNO R4 WiFi และ ULTRASONIC

- **HiveQM :** ใช้ส่งค่าที่ต้องการเข้า arduino IDE เพื่อนำค่าไปใช้ใน Source code

## Source

- ### Github and Youtube
    - Using the HC-SR04 Ultrasonic Distance Sensor with Arduino.https://www.youtube.com/watch?v=6F1B_N6LuKw.
    - Week 12 - Digital To Analog.https://github.com/TaeTanakrit0089/PhysicalComputing-167/tree/main/labs/labs12-DigitalToAnalog.
    - Week 13 - Internet of Things.https://github.com/TaeTanakrit0089/PhysicalComputing-167/tree/main/labs/labs13-InternetOfThing.
    - สาธิตการเชื่อมต่อ MQTT.https://www.youtube.com/watch?v=xo3Y3RMGqbE.
