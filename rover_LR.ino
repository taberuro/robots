#include <ServoInput.h>
#include <ServoSmooth.h>
#include <GyverMotor.h>

ServoInputPin<2> ch2(1020, 1980); //2 канал - <Пин> Название(диапазон) / Channel 2 - <Pin> Name (range)
const float Deadzone_ch2 = 0.10;  //мёртвая зона 2 канала - 10%  
ServoInputPin<3> ch1(1020, 1980); //1 канал / Channel 1
const float Deadzone_ch1 = 0.10;  //мёртвая зона 1 канала - 10%

//блок пинов подключения серв

// LF     RF
//     ^^
// 
//     ^^
// LR     RR
const int SERVO_LF = 19; // передний левый серв
const int SERVO_LR = 21; // задняя левый серв
const int SERVO_RF = 23; // передний правый серв 
const int SERVO_RR = 25; // задний правый серв

// блок моторов
// 1(LF)4(RF)
//     ^^
// 2(LM)5(RM)
//     ^^
// 3(LR)6(RR)
// левая сторона
GMotor motor_LF(DRIVER2WIRE, 7,6); //Мотор левый передний (тип драйвера, IN1, IN2) 
GMotor motor_LM(DRIVER2WIRE, 11,10); //Мотор левый средний 
GMotor motor_LR(DRIVER2WIRE, 14,15); //Мотор левый задний 
// правая сторона
GMotor motor_RF(DRIVER2WIRE, 4,5); //Мотор правый передний 
GMotor motor_RM(DRIVER2WIRE, 8,9); //Мотор правый средний 
GMotor motor_RR(DRIVER2WIRE, 13,12); //Мотор правый задний 

// блок подключения серв
ServoSmooth servo_LF; //Серва левая передняя 
ServoSmooth servo_LR; //Серва левая задняя 
ServoSmooth servo_RF; //Серва правая передняя 
ServoSmooth servo_RR; //Серва правая задняя 


uint32_t servo_ch1_Timer;
int val_ch1;
int val_ch2;

void setup() {

Serial.begin(9600);  

//блок направления вращения двигателя (FORWARD/BACKWARD/STOP/AUTO) 
motor_LF.setMode(AUTO); // левый передний
motor_LM.setMode(AUTO); // левый средний
motor_LR.setMode(AUTO); // левый задний
motor_RF.setMode(AUTO); // правый передний
motor_RM.setMode(AUTO); // правый средний
motor_RR.setMode(AUTO); // правый задний

// блок ускорения двигателя от 0 до 100
motor_LF.setSmoothSpeed(20); // левый передний
motor_LM.setSmoothSpeed(20); // левый средний
motor_LR.setSmoothSpeed(20); // левый задний
motor_RF.setSmoothSpeed(20); // правый передний
motor_RM.setSmoothSpeed(20); // правый средний
motor_RR.setSmoothSpeed(20); // правый задний

servo_LF.attach(SERVO_LF, 550, 2500, 88);  // (pin, min, max, target)
servo_LF.setAccel(0); // Ускорение (0 - управление отключено\макс. ускорение) 
servo_LF.setSpeed(400); // Скорость поворота сервопривода: 1-700 
servo_LF.setDirection(NORMAL); // Направление вращения сервы (NORMAL/REVERSE)
servo_LF.setAutoDetach(false); // Автоотключение сервопривода (true/false) 

servo_LR.attach(SERVO_LR, 550, 2500, 86);  // (pin, min, max, target)
servo_LR.setAccel(0); // Ускорение (0 - управление отключено\макс. ускорение) 
servo_LR.setSpeed(400); // Скорость поворота сервопривода: 1-700
servo_LR.setDirection(REVERSE); // Направление вращения сервы (NORMAL/REVERSE)
servo_LR.setAutoDetach(false); // Автоотключение сервопривода (true/false) 

servo_RF.attach(SERVO_RF, 550, 2500, 96);  // (pin, min, max, target)
servo_RF.setAccel(0); // Ускорение (0 - управление отключено\макс. ускорение)
servo_RF.setSpeed(400); // Скорость поворота сервопривода: 1-700
servo_RF.setDirection(NORMAL); // Направление вращения сервы (NORMAL/REVERSE)
servo_RF.setAutoDetach(false); // Автоотключение сервопривода (true/false) 
 
servo_RR.attach(SERVO_RR, 550, 2500, 91);  // (pin, min, max, target)
servo_RR.setAccel(0); // Ускорение (0 - управление отключено\макс. ускорение)
servo_RR.setSpeed(400); // Скорость поворота сервопривода: 1-700
servo_RR.setDirection(REVERSE); // Направление вращения сервы (NORMAL/REVERSE)
servo_RR.setAutoDetach(false); // Автоотключение сервопривода (true/false) 
}

void loop() {
  
 int val_ch1 = ch1.mapDeadzone(-254, 254, Deadzone_ch1); // карта обработки ( диапазон -254 -- 254 ) с учётом мёртвой зоны ( -254_-25.4 -- 25.4_254 )
 int val_ch2 = ch2.mapDeadzone(-254, 254, Deadzone_ch2); // карта обработки ( диапазон -254 -- 254 ) с учётом мёртвой зоны ( -254_-25.4 -- 25.4_254 )


  // Движение вперёд и назад 
  if(val_ch1 == 0 & val_ch2 != 0) { 
    
    // Крутим движки вторым каналом 
    motor_LF.smoothTick(val_ch2); 
    motor_LM.smoothTick(val_ch2);
    motor_LR.smoothTick(val_ch2);
    motor_RF.smoothTick(val_ch2);
    motor_RM.smoothTick(val_ch2);
    motor_RR.smoothTick(val_ch2);

    // Направление вращения двигателей
    motor_LF.setDirection(REVERSE);  
    motor_LM.setDirection(REVERSE);
    motor_LR.setDirection(REVERSE);
    motor_RF.setDirection(NORMAL);
    motor_RM.setDirection(NORMAL);
    motor_RR.setDirection(NORMAL);

    // Поворачиваем сервы в среднее положение для движения прямо 
    boolean state1 = servo_LF.tick(); 
    boolean state2 = servo_LR.tick();
    boolean state3 = servo_RF.tick();
    boolean state4 = servo_RR.tick();
    servo_LF.setTargetDeg(88);
    servo_LR.setTargetDeg(86);
    servo_RF.setTargetDeg(96); 
    servo_RR.setTargetDeg(91); 

    Serial.print(val_ch2);            // первый график - установленная скорость
    Serial.print(',');
    Serial.println(motor_LF._duty);  // второй график - реальный сигнал на мотор 
    // задержка
    delay(500);  
  }  

  // Разворот на месте
  if(val_ch1 != 0 && val_ch2 == 0) {   

    // Крутим движки первым каналом 
    motor_LF.smoothTick(val_ch1); 
    motor_LM.smoothTick(val_ch1);
    motor_LR.smoothTick(val_ch1);
    motor_RF.smoothTick(val_ch1);
    motor_RM.smoothTick(val_ch1);
    motor_RR.smoothTick(val_ch1);

    // Реверс двигателей правого борта
    motor_RF.setDirection(REVERSE); 
    motor_RM.setDirection(REVERSE);
    motor_RR.setDirection(REVERSE);

    // Поворачиваем сервы на 45 градусов 
    boolean state5 = servo_LF.tick(); 
    boolean state6 = servo_LR.tick();
    boolean state7 = servo_RF.tick();
    boolean state8 = servo_RR.tick();
    servo_LF.setTargetDeg(133);
    servo_LR.setTargetDeg(131);
    servo_RF.setTargetDeg(51);
    servo_RR.setTargetDeg(46);
  
    Serial.print(val_ch1);            
    Serial.print(',');
    Serial.println(motor_RF._duty);  
    // задержка
    delay(500);  
  }


  // Вперёд - назад с поворотами
  if(val_ch1 != 0 & val_ch2 != 0) { 

    // Крутим движки вторым каналом
    motor_LF.smoothTick(val_ch2); 
    motor_LM.smoothTick(val_ch2);
    motor_LR.smoothTick(val_ch2);
    motor_RF.smoothTick(val_ch2);
    motor_RM.smoothTick(val_ch2);
    motor_RR.smoothTick(val_ch2);

    // Направление вращения двигателей
    motor_LF.setDirection(REVERSE); 
    motor_LM.setDirection(REVERSE);
    motor_LR.setDirection(REVERSE);
    motor_RF.setDirection(NORMAL);
    motor_RM.setDirection(NORMAL);
    motor_RR.setDirection(NORMAL);

    // Поворачиваем сервы первым каналом
    boolean state9 = servo_LF.tick(); 
    boolean state10 = servo_LR.tick();
    boolean state11 = servo_RF.tick();
    boolean state12 = servo_RR.tick();
    if (millis() - servo_ch1_Timer >= 40) {
    servo_ch1_Timer = millis();
    int newPos = map(val_ch1, -254, 254, 1200, 1800); //(550, 2500) 
    servo_LF.setTarget(newPos);
    servo_LR.setTarget(newPos);
    servo_RF.setTarget(newPos);
    servo_RR.setTarget(newPos); 
    }

    Serial.print(val_ch2);            
    Serial.print(',');
    Serial.println(motor_LM._duty);  
    // задержка
    delay(500);  
  }  

  
  //стоим на месте
  if(val_ch1 == 0 & val_ch2 == 0) { 
    motor_LF.smoothTick(val_ch2);
    motor_LM.smoothTick(val_ch2);
    motor_LR.smoothTick(val_ch2);
    motor_RF.smoothTick(val_ch2);
    motor_RM.smoothTick(val_ch2);
    motor_RR.smoothTick(val_ch2);

    motor_LF.setDirection(REVERSE);
    motor_LM.setDirection(REVERSE);
    motor_LR.setDirection(REVERSE);
    motor_RF.setDirection(NORMAL);
    motor_RM.setDirection(NORMAL);
    motor_RR.setDirection(NORMAL);

    boolean state13 = servo_LF.tick();
    boolean state14 = servo_LR.tick();
    boolean state15 = servo_RF.tick();
    boolean state16 = servo_RR.tick();
    servo_LF.setTargetDeg(88);
    servo_LR.setTargetDeg(86);
    servo_RF.setTargetDeg(96); 
    servo_RR.setTargetDeg(91); 

    Serial.print(val_ch2);            
    Serial.print(',');
    Serial.println(motor_RM._duty);  
    // задержка
    delay(500);  
  }  

  
}
