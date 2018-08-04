#include <DS3231.h>
#include"tube.h"
DS3231  rtc(SDA, SCL);
#define Hz 11/8    //1到1.5左右都可以
int te[12][4] = {
  {0, 0, 0, 1,},
  {0, 0, 1, 0,},
  {0, 0, 1, 1,},
  {0, 1, 0, 0,},
  {0, 1, 0, 1,},
  {0, 1, 1, 0,},
  {0, 1, 1, 1,},
  {1, 0, 0, 0,},
  {1, 0, 0, 1,},
  {1, 0, 1, 0,},
  {0, 0, 0, 0,},
  {1, 0, 1, 1,},
};
int t[8][8] =
{ {1, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 1},
};
int sjx[28][8] =//11:. 12: .其余一一对应
{ {1, 12, 0, 4, 8, 5, 9, 6},// Steins;Gate世界線（1.048596%）
  //2036年作为标准值的世界线
  {0, 12, 0, 0, 0, 0, 0, 0},//0.000000%
  //α世界线收束范围（约0%～约1%）由300人委员会支配的反乌托邦（绝望乡）的世界
  {0, 12, 5, 7, 1, 0, 2, 4},//0.571024%
  {0, 12, 5, 7, 1, 0, 1, 5},//0.571015%
  {0, 12, 5, 2, 3, 3, 0, 7},//0.523307%
  {0, 12, 5, 2, 3, 2, 9, 9},//0.523299%
  {0, 12, 5, 0, 9, 7, 3, 6},//0.509736%
  {0, 12, 4, 5, 6, 9, 2, 3},//0.456923%
  {0, 12, 4, 5, 6, 9, 1, 4},//0.456914%
  {0, 12, 4, 5, 6, 9, 0, 3},//0.456903%
  {0, 12, 4, 0, 9, 4, 3, 1},//0.409431%
  {0, 12, 4, 0, 9, 4, 2, 0},//0.409420%
  {0, 12, 3, 3, 7, 1, 8, 7},//0.337187%
  {0, 12, 3, 3, 7, 1, 6, 1},//0.337161%
  {0, 12, 3, 3, 4, 5, 8, 1},//0.334581%
  {0, 12, 3, 3, 7, 3, 3, 7},//0.337337%
  {0, 12, 3, 3, 7, 1, 9, 9},//0.337199%
  {0, 12, 3, 2, 8, 4, 0, 3},//0.328403%
  //β世界线收束范围（约1%～约2%）未来爆发第三次世界大战、死亡57亿人的世界
  {1, 12, 1, 3, 0, 4, 2, 6},//1.130426%
  {1, 12, 1, 3, 0, 2, 3, 8},//1.130238%
  {1, 12, 1, 3, 0, 2, 0, 5},//1.130205%
  {1, 12, 1, 2, 9, 8, 4, 8},//1.129848%
  {1, 12, 1, 3, 0, 2, 1, 2},//1.130212%
  {1, 12, 1, 3, 0, 2, 1, 1},//1.130211%
  {1, 12, 1, 3, 0, 2, 0, 9},//1.130209%
  {1, 12, 1, 3, 0, 2, 0, 8},//1.130208%
  {1, 12, 1, 3, 0, 2, 0, 6},//1.130206%
  {1, 12, 1, 3, 0, 2, 0, 5},//1.130205%
};
int *point_sjx=&sjx[0][0];

void setup() {
  rtc.begin();
  /*
  rtc.setDOW(SATURDAY);       //设置星期几，例如 SUNDAY
  rtc.setTime(16, 11, 50);    //设置时间 12:00:00 (24小时制)
  rtc.setDate(4, 8, 2018);    //设置日期 日、月、年
  */
  Serial.begin(115200);
  randomSeed(analogRead(A7));//A7引脚悬空，以获取一个随机的数值作为随机种子，这样每次开机，获取的值都不同，得到的是真正的随机数
  Tube::tube_init();//初始化引脚
  delay(2000);//开机前2秒作为启动时间，让DS3231稳定下来后再读取时间
}

void loop()
{
  int light = analogRead(A6);
  Serial.println(light);
  if (light < 800)//是否为白天？
    showtime(rtc.getTimeStr());
  else
    Tube::tube_turnoff();
  //divergence(point_sjx,0);//单独显示世界线变动率
  //showdate(rtc.getDateStr());//单独显示日期
  //showtemperture(rtc.getTemp());//单独显示温度
  //rtc.getDOWStr()
  //rtc.getDateStr()
  //rtc.getTimeStr()
  //rtc.getTemp()
}

void Display(int num, int com) //num:0~12  ;com:1~8
{
  if (num == 0)
    num += 10;
  Tube::tube_turnoff();//去除余辉
  digitalWrite(com + 5, LOW);     //位选
  for (int i = 0; i < 4; i++)
    digitalWrite(14 + i, te[num - 1][3 - i]); //14
}

void divergence(int *a, int ran)
{
  int i;
  for (i = 0; i < 8; i++)
  {
    Display(*(a + 8 * ran + i), i + 1);
    delay(Hz);
  }
}
void showdate(char*b)
{
  int i, dat[8]; //年月日
  //年
  dat[0] = *(b + 6);
  dat[1] = *(b + 7);
  dat[2] = *(b + 8);
  dat[3] = *(b + 9);
  //月
  dat[4] = *(b + 3);
  dat[5] = *(b + 4);
  //日
  dat[6] = (*b);
  dat[7] = *(b + 1);
  for (i = 0; i < 8; i++)
  {
    Display(dat[i] - 48, i + 1);//-48是对ASCII码值进行运算                          
    delay(Hz);
    Display(12, 4);
    Display(12, 6);
  }
}
void showtime(char*c)
{
  int num, i, tim[8];
  if ((*(c + 7) - 48) % 2 == 0)
    num = 12+48;
  else
    num = 11+48;
  tim[0] = (*c);
  tim[1] = *(c + 1);
  tim[2] = num;
  tim[3] = *(c + 3);
  tim[4] = *(c + 4);
  tim[5] = num;
  tim[6] = *(c + 6);
  tim[7] = *(c + 7);
  for(i=0;i<8;i++)
    tim[i]-=48;
  if (tim[7] == 0 && tim[6] == 0/* && tim[4] == 0 && tim[3] == 3 */ ) //1分钟一次防中毒程序（模拟世界线变动）
  {
    ran();
    int x = random(0, 28);//从0到27中随机取一个值，即从28条世界线随机取一个世界线
    for (i = 0; i < 250; i++)
    {
      divergence(point_sjx, x);
      delay(Hz);
    }
  }
  if (tim[7] == 0 && tim[6] == 3) //秒钟每次到30时显示日期、温度
  {
    for (i = 0; i < 200; i++)
    {
      showdate(rtc.getDateStr());//显示日期
      delay(Hz);
    }
    for (i = 0; i < 250; i++)
    {
      showtemperture(rtc.getTemp());//显示温度
      delay(Hz);
    }
  }
  for (i = 1; i <= 8; i++)//继续显示时间
  {
    Display(tim[i-1], i);
    delay(Hz);
  }
}
void showtemperture(float t)
{
  int i, tem[8]; //温度
  tem[1] = t / 10;
  tem[2] = int(t) % 10;
  tem[3] = (t - tem[1] * 10 - tem[2]) / 0.1;
  tem[4] = (t - tem[1] * 10 - tem[2] - tem[3] * 0.1) / 0.01;
  tem[6] = getweek(rtc.getDOWStr());//显示现在周几
  for (i = 1; i <= 8; i++)
  {
    Display(tem[i-1], i);
    delay(Hz);
    Display(11, 4);
  }
}
int getweek(char*a)//把字符串转化成数字
{
  int w;
  if (a == "Monday")
    w = 1;
  if (a == "Tuesday")
    w = 2;
  if (a == "Wednesday")
    w = 3;
  if (a == "Thursday")
    w = 4;
  if (a == "Friday")
    w = 5;
  if (a == "Saturday")
    w = 6;
  if (a == "Sunday")
    w = 7;
  return w;
}
void ran()//随机数程序，模拟世界线变动效果
{
  int i, j;
  for (j = 0; j < 250; j++)
  {
    for (i = 1; i <= 8; i++)
    {
      Display(random(1, 11), i);
      delay(Hz);
    }
  }
}
