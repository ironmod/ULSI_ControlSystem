//infinite loop checking states the rocket is at to predict when to open the 
//doors to slow the rocket down and when to close them
void loop()
{
  feedbackValue=analogRead(feedbackPin);
  altOld=altNew;
       Serial.print("Updated Altitude 1loop: ");
       Serial.println(altNew);
       Serial.print("Altitude Oldloop: ");
       Serial.println(altOld);
       Serial.println(" ");
  altNew=altitudeRead();
  if (altNew !=0 )
  {
     newVel=(altNew-altOld)/(.05);
     //don't bother printing new statements unless the Rocket is moving
     if (altOld!=altNew)
     {
       Serial.print("Updated Altitude 1: ");
       Serial.println(altNew);
       Serial.print("Altitude Old: ");
       Serial.println(altOld);
     }
      avgVel=newVel+preVel;
      avgVel=avgVel/2;
      
     //only print if rocket is moving
     if (preVel!=newVel)
     {
       Serial.print("Average Velocity: ");
       Serial.println(avgVel);   
     }
     preVel=newVel;
     
   //once the rocket rises above 4,000 feet, we need to begin
   //predicting what the next height will be 
    if (altNew > 4200) 
    {
       prediction();
    }
    
  //datalog();  
  }
  Serial.flush();  //clean the data  
}//end void loop
