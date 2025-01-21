

def arrayToDecimal(arr, threshold):
    binaryValue = ''  #creates empty string to store bianry value 
    
    for num in arr:
        if num >= threshold:
            binaryValue += '1'  # add 1 to string if reading above threshold
        else:
            binaryValue += '0'  # add 0 to string if reading below threshold
    
    decimalValue = int(binaryValue, 2) #converts bianry string to decimal

    print(decimalValue)   #prints decimal value
    return decimalValue



#numbers = [0.5, 1.3, 0.7, 2.9, 3.2]   #array coming in 

thresholdValue = 2.0     #threshold 

decimalResult = arrayToDecimal(AnalogValue, thresholdValue)  #how to run the function 

#_____________________________________

int AnalogValue[5] = {0,0,0,0,0}
int AnalogPin[5] = {4,5,6,7,15} # keep 8 free for tone O/P music

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int i;
    for (i=0;i<5;i++) {
        AnalogValue[i]=analogRead(AnalogPin[i])
        Serial.print(AnalogValue[i]) #This prints the actual analog reading from the sensors
        Serial.print("\t") #tab over on screen
        if(i==4) {
            Serial.println("") #carriage return
            delay(600) # display new set of readings every 600mS
        }
    }
}