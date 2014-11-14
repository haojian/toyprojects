//Haojian's Code for arduino car
//Oct. 18 2013.

#include <AFMotor.h>
#include <Servo.h> 

#define max_para_cnt 5
#define max_para_len 16

//DC motors
AF_DCMotor motor1(3);    //FOR LEFT DC MOTORS
AF_DCMotor motor2(2);    //FOR RIGHT DC MOTORS
AF_DCMotor motor3(1);
AF_DCMotor motor4(4);    

//DC servos
Servo servoLR;
Servo servoUD;


char para[max_para_cnt][max_para_len];
int paraInt[max_para_cnt];

char cmd;
int pc=0;
int pl=0;

void output(const char* str, int x){
  Serial.print(str);
  Serial.println(x);
}

class motorTask{
  public: 
    motorTask(AF_DCMotor* m){
      motor = m;
      isTask = false;
    };
    
    //make an appointments
    void addTask(uint8_t dir, uint8_t speed){
      isTask = true;
      mSpeed = speed;
      mDir = dir;
    };
    
    //execute the appointment.
    void doTask(){
      if(isTask){
        motor->setSpeed(mSpeed);
        motor->run(mDir);
        isTask = false;
      }
    };
    
    //清除当前预约的动作
    void clearTask(){
      isTask = false;
    }
  private:
    AF_DCMotor* motor;
    bool isTask;
    uint8_t mDir;
    uint8_t mSpeed;
};

//舵机的控制类,专门用来控制舵机
class servoTask{
  public:
    servoTask(Servo* s, int pin, int degree){
      servo = s;
      servo->write(degree);
      updateTime = millis();
      currentDegree = degree;
      targetDegree = degree;
    };
    
    void addTask(int degree){
      targetDegree = degree;
    };
    
    //执行动作,向着目标角度转去.因为舵机不是一个及时响应的设备,所以每转动一定角度后都需要延时,为了不阻塞主循环,所以加上了时间判定
    void doTask(){
      if(currentDegree != targetDegree){
        unsigned long now = millis();
        if(now - updateTime >= 10){
          targetDegree>currentDegree?currentDegree++:currentDegree--;
          servo->write(currentDegree);
          updateTime = now;
          //output("currentDegree",currentDegree);
        }
      }
    };
    
    void clearTask(){
      targetDegree = currentDegree;
    }
  private:
    Servo* servo;
    int currentDegree;
    int targetDegree;
    unsigned long updateTime;
};

//binding task manager to DC motors
motorTask mTaskR(&motor2);
motorTask mTaskL(&motor1);
//binding task manager to LED light
motorTask mLedFront(&motor3);
motorTask mLedBack(&motor4);

//binding task manager to Servo
servoTask sTaskLR(&servoLR, 9, 90);
servoTask sTaskUD(&servoUD, 10, 135);


void doPara(){
  for(int i=0; i<max_para_cnt; i++)
  {
    if(paraInt[i]==9999)
      paraInt[i] = 0;
  }
  Serial.print("Do para : ");
  output(para[0],paraInt[1]);
  if(para[0][0]=='G'){  //Go
    uint8_t dir = paraInt[1]>=10?BACKWARD:(paraInt[1]<=10?FORWARD:RELEASE);
    uint8_t speed = abs(paraInt[1]);
    if(para[0][1]=='L'){
      mTaskL.addTask(dir,speed);
    }
    else if(para[0][1]=='R'){
      mTaskR.addTask(dir,speed);
    }
  }
  else if(para[0][0]=='C'){  //See
    uint8_t degree = abs(paraInt[1]);
    if(para[0][1]=='V'){//UD
      sTaskUD.addTask(degree);
    }
    else if(para[0][1]=='H'){//LR
      sTaskLR.addTask(degree);
    }
  }
  else if(para[0][0]=='L'){  //LED
    uint8_t dir = abs(paraInt[1])==0?RELEASE:FORWARD;
    uint8_t brightness = abs(paraInt[1]);
    brightness = brightness>35?35:brightness; //controle the voltage to protect led
    if(para[0][1]=='F'){
      mLedFront.addTask(dir,brightness);
    }
    else if(para[0][1]=='B'){
      mLedBack.addTask(dir,brightness);
    }
  }
}


void resolveSerial() {
   while(Serial.available()){
      cmd = Serial.read();
      if( (cmd>='A' && cmd<='Z') || (cmd>='a' && cmd<='z') || (cmd>='0' && cmd<='9') || cmd=='-' || cmd=='.'){
        para[pc][pl] = cmd;
        if(cmd>='0' && cmd<='9'){
          if(paraInt[pc]==9999){
            if(cmd!='0'){
              paraInt[pc]=0-(cmd-'0');
            }
          }
        else {
          paraInt[pc]*=10;
          if(paraInt[pc]>=0)
            paraInt[pc]+=cmd-'0';
          else
            paraInt[pc]-=cmd-'0';
        }
      }else if(cmd == '-'){
        if(paraInt[pc]==0)
          paraInt[pc] = 9999;
        else
          paraInt[pc]*=-1;
      }
      pl++;
    }
    else if(cmd == '$'){
      doPara();
      memset(para,0,sizeof(char)*max_para_cnt*max_para_len);
      memset(paraInt,0,sizeof(paraInt));
      pc = 0;
      pl = 0;
    }
    else {
      if(pl>0){
        pc++;
        if(pc>=max_para_cnt){
          Serial.println("Error : pc too long");
        }
        pl = 0;
      }
    }
   }
}

void setup() {   
  memset(para,0,sizeof(char)*max_para_cnt*max_para_len);
  memset(paraInt,0,sizeof(paraInt));  
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Hello! Haojian!");
  
  motor1.setSpeed(0); 
  motor2.setSpeed(0); 
  motor3.setSpeed(0);
  motor4.setSpeed(0);

  servoLR.attach(9);
  servoUD.attach(10);
}

void loop(){
  resolveSerial();
  mTaskL.doTask();
  mTaskR.doTask();  
  sTaskLR.doTask();
  sTaskUD.doTask();
  mLedFront.doTask();
  mLedBack.doTask();
}


