const int a0 = A4;                     
const int a1 = A5;                 
const int WR = A3;
const int RD = A2;
double result = 0;                          //result of operations
int operatorx = '0';                        //character for symbols
int temp;                                   //temp to save the value of pressed col
int counter = 0;
double num1 = 0;
double num2 = 0;
int negcount = 0;
String welc = "Hello!";
bool neg,neg2;
const byte row = 4;
const byte col = 4;
byte RowPins[row] = {0, 1, 2, 3};                           //KEYPAD rows connected with pin 0 : 3 of 8255
byte ColPins[col] = {4, 5, 6, 7};                           //KEYPAD columns connected with pin 4 : 7 of 8255


char keypad[row][col] = {'7', '8', '9', '/',
                        '4', '5', '6', '*',
                        '1', '2', '3', '-',
                        '.', '0', '=', '+'
                       };

void setup() {

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(a0, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(WR, OUTPUT);
}

void fun_in() {                               //Keypad In cols
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);


}

void fun_out() {                              //Keypad out cols
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

}

////////////////////////////////////////////////////////////////////////////////////////////////
void keypad_press()
{
  // set all rows = 1
  for (int i = 0; i < 4; i++)
  { 
    //select PortC
    digitalWrite(a0, 0); 
    digitalWrite(a1, 1);
    
    digitalWrite(RowPins[i], 1); //set a  row  = 1

    digitalWrite(WR, 0);// enable WR
    delay(1);
    digitalWrite(WR, 1);// disable WR

    digitalWrite(i, 0);// set selected row = 0 low
    
    digitalWrite(WR, 0);
    delay(1);
    digitalWrite(WR, 1);

    // to know the selected col
    for (int j = 4; j <= 7; j++)
    {

      fun_in(); 
      digitalWrite(RD, 0);// enable RD
      temp = digitalRead(j); //num of the pressed/selected col 


      delay(11);
      digitalWrite(RD, 1);//disable RD

      fun_out();

      if (temp == 0)
      {
        //if the pressed key is symbol ( * , + , - , / , = , . )
        if ((keypad[i][j - 4] == keypad[0][3]) | (keypad[i][j - 4] == keypad[1][3]) | (keypad[i][j - 4] == keypad[2][3]) | (keypad[i][j - 4] == keypad[3][3]) | (keypad[i][j - 4] == keypad[3][2]) | (keypad[i][j - 4] == keypad[3][0]))
        { 
          delay(1);
          if (keypad[i][j - 4] == '.')  //  ON||OFF SWITCH
          {
            writeOnlcd(0x01,0);       // CLEAR COMMAND SO RS = 0
            writeOnlcd(0x06,0);       // CURSOR++
            operatorx = '0' ;
            num1 = 0 ;
            num2 = 0 ;
            negcount = 0;
            break;
          }  
          
          if (keypad[i][j - 4] == '=')
          {
            writeOnlcd('=',1);
            operation();
            operatorx = '0' ;
            num1 = 0 ;
            num2 = 0 ;
            break;
          }
          if(keypad[i][j - 4] == '-' && num1 == 0)
          {
            writeOnlcd('-',1);
            operatorx ='0';
            neg=true;
          }
          else
          {
            if(keypad[i][j - 4] == '-' && num2 == 0 && operatorx != '0')
            {
              negcount++;
              writeOnlcd(keypad[i][j - 4],1);
            }
            else
            {
               operatorx = keypad[i][j - 4] ;
               writeOnlcd(operatorx,1);
            }
           
          }
           
        }
        else
        {

          writeOnlcd(keypad[i][j - 4],1);
          delay(2);
          
          if (operatorx == '0') 
          {
            num1 = ( num1 * 10 ) + ( keypad[i][j - 4]) - '0'  ; 
          }

         else
          {
            if(negcount == 1)
            {
              num2 = ( num2 * 10 ) +  (keypad[i][j - 4]) - '0'  ;
              neg2 = true;
            }
            else
            {
              num2 = ( num2 * 10 ) +  (keypad[i][j - 4]) - '0'  ;
            }
          }
          break;
        }
      }
    }
    digitalWrite(i, 1);                         // SET ROW AGAIN
    digitalWrite(WR, 0); 
    delay(1);
    digitalWrite(WR, 1); 
  }

}

///////////////////////////////////////////////////////////////////////


void operation() 
{
  
  if (operatorx == '+')
  {
    if (neg == true){num1=num1*-1; neg=false;}
    if (neg2 == true){num2*=-1; neg2=false;}
    result = (num1 + num2);
    String str = String(result);
    displayOnLCD(str);
  }

  if (operatorx == '-')
  {
    if (neg == true){num1=num1*-1; neg=false;}
    if (neg2 == true){num2*=-1; neg2=false;}
    result = (num1 - num2);
    String str = String(result);
    displayOnLCD(str);

  }
  if (operatorx == '*')
  {
    if (neg == true){num1=num1*-1; neg=false;}
    if (neg2 == true){num2*=-1; neg2=false;}
    result = (num1 * num2);
    String str = String(result);
    displayOnLCD(str);
  }
  if (operatorx == '/')
  {
    if (num2 == 0) {
      writeOnlcd('E',1);
      delay(1);
      num1 = 0;
      num2 = 0;
      operatorx = '0';
    }
    else
    {
      if (neg == true){num1=num1*-1; neg=false;}
      if (neg2 == true){num2*=-1; neg2=false;}
      result = (num1 / num2);
      String str = String(result);
      displayOnLCD(str);
    }

  }


}

void displayOnLCD(String msg) 
{
  for (int i = 0; i < 8; i++)
  {
    writeOnlcd(msg[i],1);
    delay(1);

  }
  delay(1);
}

void Welcome_Lcd(String msg) 
{
  for (int i = 0; i < 8; i++)
  {
    writeOnlcd(msg[i],1);
    delay(1);

  }
  delay(1);
}


//To send a command to LCD, RS=0  & To send Data to LCD, RS=1 
void writeOnlcd(int num , int rs) {
   //port B is selected
  digitalWrite(a0, 1);
  digitalWrite(a1, 0);
  for (int i = 0; i < 8; i++)//For loop to enter i to the pins of port B for 8255
  {
    digitalWrite(i, (num >> i) & 0x01);     // Here we make anding for i and 1 to take it to port b
  }
  
  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
  
   //port A is selected
  digitalWrite(a0, 0); 
  digitalWrite(a1, 0);
  //to send command make E=1 
  digitalWrite(6, rs);
  digitalWrite(7, 1);
  
  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
  
  //then make E=0  HIGH TO LOW PULSE
  digitalWrite(6, rs);
  digitalWrite(7, 0);
  
  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
}





void loop() {

  //if condition to set control word
  if (counter == 0)
  {
    digitalWrite(RD, 1);   //READ DISABLED
    digitalWrite(WR, 1);  //WRITE DISABLED

    //Selecting Control Register of 8255
    digitalWrite(a0, 1); 
    digitalWrite(a1, 1);  

    digitalWrite(7, 1);  // Input/Output mode not BSR
    digitalWrite(6, 0);  // MODE 0
    digitalWrite(5, 0);  // MODE 0
    digitalWrite(4, 0);  // portA Output
    digitalWrite(3, 1);  // PORTC UPPER Input "cols"
    digitalWrite(2, 0);  // MODE 0
    digitalWrite(1, 0);  // portB Output
    digitalWrite(0, 0);  // PORTC LOWER Output "rows"

    digitalWrite(WR, 0);//WRITE ENABLED
    delay(10);
    digitalWrite(WR, 1);//WRITE DISABLED

    writeOnlcd(0x38,0);//intialize lcd
    delay(30);
    writeOnlcd(0x0E,0);//display cursor
    delay(30);
    writeOnlcd(0x01,0);//clear lcd
    delay(30);
    writeOnlcd(0x06,0);//shift cursor right
    delay(30);
    Welcome_Lcd(welc);
    delay(1000);
    writeOnlcd(0x01,0);//clear lcd
    delay(30);
    writeOnlcd(0x0F,0);//Cursor Blinking
    counter = 1;
    
  }
  keypad_press();
}
