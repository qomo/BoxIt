
#include <FlexiTimer2.h>
#include <Servo.h>


const int led_pin = 13;			// default to pin 13
boolean IS_OPEN_STATE = false;
int SEVEN_HOURS = 10;
int open_count = SEVEN_HOURS; //7h

Servo myservo;  // create servo object to control a servo

const int echopin=3; // echo接3端口
const int trigpin=2; // trig接2端口

void secondClock()
{
static boolean output = HIGH;
  
  digitalWrite(led_pin, output);
  output = !output;
  
  if(!IS_OPEN_STATE){
    open_count--;
  }
  
  if(open_count<=0) {
    IS_OPEN_STATE = true;
  }else{
    IS_OPEN_STATE = false;
  }
  
}

boolean isBoxOpen() {
  // 通过超声波测距判断盒子的开关状态
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW); //发一个10ms的高脉冲去触发TrigPin
  float distance = pulseIn(echopin,HIGH);//接收高电平时间
  distance = distance/58.0;//计算距离
  Serial.print(distance);  //输出距离
  Serial.println("cm");  //输出单位
  if(20<distance && distance<1000){
    Serial.println("BoxOpen");
    open_count = SEVEN_HOURS;
    return true;
  }else{
    Serial.println("Box not Open");
    return false;
  }
}

void setLock(boolean is_lock) {
  if (!isBoxOpen()&&is_lock) {
    // lock
    Serial.println("lock");
    myservo.write(135);
    delay(15);
  }else{
    // open
    Serial.println("unlock");
    myservo.write(45);
    delay(15);
  }
}

void setup()
{
  Serial.begin(9600);  
  pinMode(echopin,INPUT); //设定echo为输入模式
  pinMode(trigpin,OUTPUT);//设定trig为输出模式

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
  pinMode(led_pin, OUTPUT);

  FlexiTimer2::set(1000, 1.0/1000, secondClock); // call every 500 1ms "ticks"
  // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();
}

void loop()
{
  setLock(!IS_OPEN_STATE);
  delay(500);
  Serial.print("open_count=");
  Serial.println(open_count);
}


