//Predict the next height the rocket will be at
//and open the doors to slow it down
void prediction()    
{
lnConst=0;     
yMax=0;        
lnValue=0;

   lnConst= (avgVel*avgVel) + VtSq;  
   lnConst= lnConst/VtSq;
   lnValue = log(lnConst);
   yMax=lnValue*VtMult;
   yMax=yMax + altNew;
   
   //Serial.print("Ymax= ");
   //Serial.println(yMax);
 //once the equation has predicted that the rocket will be at 5280 feet
 //we need to open the doors to slow it down. Once the Rocket has hit the 1 mile marker
 //we will then shut the doors to allow steady free fall
 if (yMax > brake_altitude && digitalRead(relayOpen_pos)==1 && abs(yMax)<7000)  //activates the doors/Relay Module
  {
    digitalWrite(relayClose_pos, HIGH);
    digitalWrite(relayClose_neg, HIGH);
    delay(1);
    
    //while loop makes sure the actuator does not extend past set limit
    feedbackValue=analogRead(feedbackPin);
   while(feedbackValue>actuator_limit)
    {
      feedbackValue=analogRead(feedbackPin);
        digitalWrite(relayOpen_pos, LOW);
        digitalWrite(relayOpen_neg, LOW);
      //Serial.print("Doors opened at (ft): ");
      //Serial.println(altNew);
      SD_open=altNew;
      trigger_check=1;
    }
    
    //turn of the motor and hold in place once 
    //actuator feedback limit has been reached
    if(trigger_check==1){
      feedbackValue=analogRead(feedbackPin);
      digitalWrite(relayOpen_pos, HIGH);
      digitalWrite(relayOpen_neg, HIGH);
      //Serial.println("Doors Stopped (reading from actuator feedback):");
      //Serial.println(feedbackValue);
    }
   }
   
  //close doors and hold shut until landing
 if (yMax <= brake_altitude && digitalRead(relayOpen_pos)==1 && trigger_check==1) 
  {                                              
    digitalWrite(relayOpen_pos, HIGH);
    digitalWrite(relayOpen_neg, HIGH);    
    delay(1);
    digitalWrite(relayClose_pos, LOW);
    digitalWrite(relayClose_neg, LOW);
    //Serial.print("Doors closed at (ft): ");
    //Serial.println(altNew);
    SD_close=altNew;
  }
// datalog(); //log on the data onto the SD card
  Serial.flush();
  return;
}
