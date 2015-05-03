//
// my_car
//
// Description of the project
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		徐征宇
// 				徐征宇
//
// Date			15/4/23 下午12:55
// Version		<#version#>
//
// Copyright	© 徐征宇, 2015年
// Licence		<#license#>
//
// See         ReadMe.txt for references
//

#include            "Arduino.h"
#include            "SR04.h"
#define motor1Pin   4          //控制电机1驱动端
#define motor1PWM   5          //控制电机1使能端(PWM）
#define motor2Pin   7          //控制电机2驱动端
#define motor2PWM   6          //控制电机2使能端(PWM)
#define leftSensor  8          //左传感器端
#define midSensor   9          //中间传感器端
#define rightSensor 10         //右传感器端
#define TRIG_PIN    11          //超声波发射端
#define ECHO_PIN    12          //超声波接收端


/* 各函数声明 */
void motor(char pin, char pwmpin, char state, int val);
void runforward(int speed);
void runbackward(int speed);
void stop();
void turnL(int speed);
void turnR(int speed);
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

/* 设定初始值 */
void setup()
{
    pinMode(leftSensor, INPUT);//左边的传感器
    pinMode(midSensor, INPUT);//中间的传感器
    pinMode(rightSensor, INPUT);//右边的传感器
}

/* 主循环 */
void loop() {
    char num1,num2,num3,num;
    int distance;
    
    distance = sr04.Distance();
    while (distance<6) {
        stop();
        distance = sr04.Distance();
    }
    
    /*获取三个传感器的电平高低*/
    num1=digitalRead(leftSensor);
    num2=digitalRead(midSensor);
    num3=digitalRead(rightSensor);
    
    num=num1*100+num2*10+num3;//组合成一个数字
    
    switch (num) {
        case 11:    //当左偏
            turnR(120); //右转
            delay(2);
            while (1) {
                num2=digitalRead(midSensor); //再次检测中间传感器
                if (num2==0) {               //若中间传感器还在黑线外
                    turnR(120);              //继续右转
                    delay(2);
                }
                else                         //否则跳出
                    break;
            }
            break;
            
        case 110:    //当右偏
            turnL(120); //左转
            delay(2);
            while (1) {
                num2=digitalRead(midSensor);  //再次检测中间传感器
                if (num2==0) {                //若中间传感器还在黑线外
                    turnL(120);               //继续左转
                    delay(2);
                }
                else                          //否则跳出
                    break;
            }
            break;
        case 0:
            stop();
            break;
            
        default:
            runforward(120);                  //默认先前行驶
            delay(2);
            break;
    }
}

/**************************
 函数名:电机驱动函数
 参数:
 (1).电机驱动端
 (2).电机使能端
 (3).设定状态值:1 向前；2 向后；3 停止
 (4).速度(0-255)
 返回值:无
 *************************/
void motor(char pin, char pwmpin, char state, int speed)
{
    pinMode(pin, OUTPUT);//设定驱动端口为输出
    
    if (state==1) {//向前
        analogWrite(pwmpin, speed);//为使能端口赋不同的电压
        digitalWrite(pin, 1);//驱动端口为高电平
    }
    else if (state==2){//向后
        analogWrite(pwmpin, speed);
        digitalWrite(pin, 0);//驱动端口为低电平
    }
    else if (state==0)//停止
    {
        analogWrite(pwmpin, 0);//为使能端口为0
        digitalWrite(pin, 0);//驱动端口低电平
    }
}

/**************************
 函数名:向前
 参数:速度(0-255)
 返回值:无
 *************************/
void runforward(int speed)//前进
{
    motor(motor1Pin, motor1PWM, 1, speed);
    motor(motor2Pin, motor2PWM, 1, speed);
}

/**************************
 函数名:向后
 参数:速度(0-255)
 返回值:无
 *************************/
void runbackward(int speed)//后退
{
    motor(motor1Pin, motor1PWM, 2, speed);
    motor(motor2Pin, motor2PWM, 2, speed);
}

/**************************
 函数名:停止
 参数:无
 返回值:无
 *************************/
void stop()//停止
{
    motor(motor1Pin, motor1PWM, 0, 0);
    motor(motor2Pin, motor2PWM, 0, 0);
}

/**************************
 函数名:向左
 参数:速度(0-255)
 返回值:无
 *************************/
void turnL(int speed)//左转
{
    motor(motor1Pin, motor1PWM, 1, speed);
    motor(motor2Pin, motor2PWM, 0, 0);
}

/**************************
 函数名:向右
 参数:速度(0-255)
 返回值:无
 *************************/
void turnR(int speed)//右转
{
    motor(motor1Pin, motor1PWM, 0, 0);
    motor(motor2Pin, motor2PWM, 1, speed);
}



