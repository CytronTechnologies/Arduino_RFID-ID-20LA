//Example application for RFID ID-20LA reader
//Color mixing for RGB led using RFID card/key chain
//More info available at http://tutorial.cytron.com.my/

byte tagID[12];        //store the data and checksum of card read

//Change here with your tag ID
const byte red_tag[10]={0x00,0x09,0x00,0x00,0x08,0x05,0x09,0x0E,0x0B,0x04};      //card to toggle red led
const byte green_tag[10]={0x03,0x04,0x00,0x00,0x05,0x0C,0x0B,0x04,0x06,0x01};    //card to toggle green led
const byte blue_tag[10]={0x03,0x08,0x00,0x00,0x01,0x0F,0x05,0x03,0x0D,0x08};     //card to toggle blue led

boolean tagread=false;

//state of led output
boolean red_state=1;
boolean green_state=1;
boolean blue_state=1;

//RGB led pin declaration
const int red_pin=8;
const int green_pin=9;
const int blue_pin=10;

void setup()
{
  pinMode(red_pin,OUTPUT);
  pinMode(green_pin,OUTPUT);
  pinMode(blue_pin,OUTPUT);
  Serial.begin(9600);
  led();
}

void loop()
{
  
  if(Serial.available()>=13)      //Make sure all the frame is received
  {
    if(Serial.read()==0x02)       //Check for the start byte = 0x02
    {
      tagread=true;                //New tag is read
      for(int index=0;index<sizeof(tagID);index++)
      {
        byte val=Serial.read();
        
        //convert from ASCII value to value range from 0 to 9 and A to F
        if( val >= '0' && val <= '9')
        val = val - '0';
        else if( val >= 'A' && val <= 'F')
        val = 10 + val - 'A';
        
        tagID[index]=val;
      }
    }
    else
    {
      tagread=false;                //Discard and skip
    }
  }
  
  if(tagread==true)                 //New tag is read
  {
    print_tag();                    //Display the tag ID
    compare_tag();                  //Compare tag ID with registered tag
    led();                          //Display LED
    clear_tag();                    //Clear the tag ID and ready for next tag
    tagread=false;
  }
  delay(10);
}

void print_tag()                    //Subroutine to display the tagID content
{
  Serial.print("Tag ID: ");        
  for(int index=0;index<10;index++)
  {
    Serial.print(tagID[index],HEX);  //Display tagID
  }
  Serial.print("\r\nChecksum: ");
  Serial.print(tagID[10],HEX);       //Display checksum
  Serial.println(tagID[11],HEX);
}

void clear_tag()                    //Subroutine to clear the tagID content
{  
  for(int index=0;index<sizeof(tagID);index++)
  {
    tagID[index]=0;
  }
}

void compare_tag()
{
  boolean red_temp=false;
  boolean green_temp=false;
  boolean blue_temp=false;
  
  for(int index=0;index<10;index++)
  {
    if(tagID[index]==red_tag[index])      //compare red tag
    {
      red_temp=true;
    }
    else 
    {
      red_temp=false;
    }
    
    if(tagID[index]==green_tag[index])    //compare green tag
    {
      green_temp=true;
    }
    else 
    {
      green_temp=false;
    }
    
    if(tagID[index]==blue_tag[index])    //compare blue tag
    {
      blue_temp=true;
    }
    else 
    {
      blue_temp=false;
    }  
  }
  if(red_temp==true)red_state^=1;
  if(green_temp==true)green_state^=1;
  if(blue_temp==true)blue_state^=1;
}

void led()
{
  digitalWrite(red_pin,red_state);
  digitalWrite(green_pin,green_state);
  digitalWrite(blue_pin,blue_state);
}
