/*
 * 
 * C by Nikodem Bartnik
 * http://www.instructables.com/member/Nikus/
 * https://spongepie.com/
 * http://arduinopolska.cba.pl/
 * 
 * 
 * 
 * 
 */


#include <LGSM.h>

String remoteNumber;
String command; 
String number = "";
String sms_text;
boolean call = false;
char charbuffer[20];


void setup(){

  Serial.begin(9600); 
}

void loop(){
  
  // add any incoming characters to the String:
  while (Serial.available() > 0){
    
   command = Serial.readString();
     command.trim();
     if(command.substring(0,1).equals("C")){

      number = command.substring(1, command.length());
      call = true;
     }else if( command.substring(0, 1).equals("S")){

       
      
      for(int i = 0; i <command.length(); i++){
        if(command.substring(i, i+1).equals("T")){
          sms_text = command.substring(i+1, command.length());
          number = command.substring(1, i);
          break;
      }
      }
     }

//********************************   SMS    *****************************

       if(sms_text.length() > 0){

          number.toCharArray(charbuffer, 20);
          LSMS.beginSMS(charbuffer);
          LSMS.print(sms_text);
          Serial.println("To: " + number);
          Serial.println("Text: " + sms_text);


        if(LSMS.endSMS()){
          
          Serial.println("SMS is sent");
        }else{
          Serial.println("SMS is not sent");
          delay(5000);
        }
        sms_text = "";
      }

     

      //******************   CALL  ************************
      if (number.length() < 20 && call == true){
        
        // let the user know you're calling:
        Serial.print("Calling to : ");
        Serial.println(number);
        Serial.println();

        // Call the remote number
        number.toCharArray(charbuffer, 20);
        
        // Check if the receiving end has picked up the call
        if(LVoiceCall.voiceCall(charbuffer)){
          
          Serial.println("Call Established. Enter line to end");
          // Wait for some input from the line
          while(Serial.read() !='\n');
          // And hang up
          LVoiceCall.hangCall();
        }
        Serial.println("Call Finished");
        number="";
        call = false;
        Serial.println("Enter phone number");
      }else{
        
        Serial.println("That's too long for a phone number or you forgot to add C or S"); 
        number = "";
        call = false;
      }
    }    
  } 
