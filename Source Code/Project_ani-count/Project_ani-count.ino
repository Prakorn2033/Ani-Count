#include <WiFi.h>
#include <MQTTClient.h>

#define TRIG_PIN 9 //ต่อ TRIG เข้าช่อง 9
#define ECHO_PIN 10 //ต่อ ECHO เข้าช่อง 10

const char WIFI_SSID[] = "Prakorn-303A-5G"; // ชื่อ Wi-Fi
const char WIFI_PASSWORD[] = "0919930186";  // รหัสผ่าน Wi-Fi

const char MQTT_BROKER_ADDRESS[] = "mqtt-dashboard.com";  //MQTT Broker ADDRESS
const int MQTT_PORT = 1883; // ใช้ 1883 จะง่ายกว่า
const char MQTT_CLIENT_ID[] = "ani_count_62070115";  // ใส่ Client ID ที่ไม่ซ้ำ
const char MQTT_USERNAME[] = ""; // ถ้าไม่ต้องการ ให้เว้นว่าง
const char MQTT_PASSWORD[] = ""; // ถ้าไม่ต้องการ ให้เว้นว่าง

const char PUBLISH_TOPIC[] = "ani-count";    // Topic สำหรับ publish
const char SUBSCRIBE_TOPIC[] = "ani-count";  // Topic สำหรับ subscribe

WiFiClient network;
MQTTClient mqtt(256);

int targetCount = 0; // กำหนดค่าที่ต้องการนับไว้เป็น 0 เพื่อรับค่า
int currentCount = 0; // กำหนดจำนวนที่นับได้ไว้เป็น 0 เพื่อรับค่า
unsigned long startTime = 0; // กำหนดเวลา
bool counting = false; //กำหนด loop ให้ยังไม่ทำงาน

void setup() {
    Serial.begin(9600); //เซ็ตค่า Serial Monitor
    pinMode(TRIG_PIN, OUTPUT); //กำหนด Pin Output
    pinMode(ECHO_PIN, INPUT); //กำหนด Pin Input

    setupWiFi();
    connectToMQTT();
}

void loop() {
    mqtt.loop();

    if (counting) {    
        long distance = measureDistance();
        // เช็คระยะทางและเพิ่มจำนวน
        if (distance < 20) { // กำหนดระยะตรงนี้ ปัจจุบันระยะที่นับได้ต้องไม่ถึง 20 ซม
            currentCount++;
            String message1 = "นับได้: " + String(currentCount);
            mqtt.publish(PUBLISH_TOPIC, message1.c_str()); // ส่งค่าขึ้น mqtt เพื่อบอกว่านับจำนวนน้องๆได้เท่าไรแล้ว
            delay(1000);
            String message2 = "จำนวนน้องที่มี: " + String(targetCount);
            mqtt.publish(PUBLISH_TOPIC, message2.c_str()); // ส่งค่าขึ้น mqtt เพื่อบอกว่าน้องที่เรามีเท่าไร
            delay(1000);
        }

        else if (millis() - startTime > 10000) { // ระยะเวลาก่อนเข้าเงื่อนไขเพื่อเช็คว่านับแล้วได้ผลรับเป็นอย่างไร ปัจจุบัน 10 วินาที
            
            if (currentCount == targetCount  && (targetCount != 0)) { // หากจำนวนน้องที่นับกับน้องที่ผ่านประตูไปตรงกันจะเข้าเงื่อนไขนี้
                mqtt.publish(PUBLISH_TOPIC, "ยินดีด้วยน้องทั้งหมดยังอยู่ครบ"); // แจ้งไปที่ MQTT ว่าน้องครบ
                currentCount = 0; // รีเซ็ตค่าที่นับได้
                counting = false; // หยุด loop
            } else if (currentCount > targetCount) { // หากจำนวนน้องที่นับกับน้องที่ผ่านประตูไปเกินจะเข้าเงื่อนไขนี้
                mqtt.publish(PUBLISH_TOPIC, "ยินดีด้วยคุณได้น้องเพิ่ม ????"); // แจ้งไปที่ MQTT ว่าน้องมีมาเกิน
                currentCount = 0; // รีเซ็ตค่าที่นับได้
                counting = false; // หยุด loop
            } else if ((currentCount != targetCount) && counting) { // หากจำนวนน้องที่นับกับน้องที่ผ่านประตูไปมีไม่ถึงจะเข้าเงื่อนไขนี้
                mqtt.publish(PUBLISH_TOPIC, "น้องมีไม่ครบ !!!!!!"); // แจ้งไปที่ MQTT ว่าน้องขาดหายไป
                currentCount = 0; // รีเซ็ตค่าที่นับได้
                counting = false; // หยุด loop
              }
        }
    }
}

void setupWiFi() { // ฟังก์ชั่น setupWiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected: " + String(WiFi.localIP()));
}

void connectToMQTT() { // ฟังก์ชั่น ต่อกับ MQTT
    mqtt.begin(MQTT_BROKER_ADDRESS, MQTT_PORT, network);
    mqtt.onMessage(messageHandler);

    while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("MQTT Connected!");
    mqtt.subscribe(SUBSCRIBE_TOPIC);
    
    
    mqtt.publish(PUBLISH_TOPIC, "กรุณาใส่จำนวนที่ต้องการนับ"); // ข้อความเมื่อเริ่ม subscribe บน MQTT
}

void messageHandler(String &topic, String &payload) { // ฟังก์ชั่นที่จะรับค่ามาจาก MQTT ที่ publish ขึ้นไป
    Serial.println("Received: " + payload);
    
    
    if (payload.length() > 0) { // ตรวจสอบว่า payload ไม่ว่าง
        
        int value = payload.toInt(); // แปลง payload เป็น int

        // เอาไว้เช็คค่าก่อนนำไปนับเพื่อไม้ให้ตัวเลขที่การนับเปลื่ยน
        // ตรวจสอบว่าค่า payload มีตัวเลขหรือไม่
        if (String(value) == payload) {
            targetCount = value; // ถ้าตรงกันให้กำหนดค่า
            Serial.print("ใส่จำนวนที่ต้องการนับ: "); // ส่งค่าขึ้น Serial Monitor เพื่อเช็คจำนวนที่ต้องการใส่
            Serial.println(targetCount);

            if (!counting) {
                currentCount = 0; // เนับใหม่เมื่อ counting เป็น false
                startTime = millis(); // เริ่มจับเวลา
                counting = true; // เริ่ม loop
        }
            
        } else {
            Serial.println("ค่าที่ได้รับไม่ใช่ตัวเลข!"); // ใช้เช็คค่าตัวที่รับมาเฉยๆ
        }
    } else {
        Serial.println("ค่าที่ได้รับเป็นค่าว่าง!"); // ใช้เช็คค่าตัวที่รับมาเฉยๆ
    }
}


long measureDistance() { // ฟังก์ชั่น Ultrasonic Sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = duration * 0.034 / 2; // สมการที่ได้หลังจากรับค่าจาก Ultrasonic Sensor เป็น cm
    return distance;
}
