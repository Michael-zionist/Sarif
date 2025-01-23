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



numbers = [0.5, 1.3, 0.7, 2.9, 3.2]   #sample array coming in 

thresholdValue = 2.0     #threshold value

decimalResult = arrayToDecimal(numbers, thresholdValue)  #how to run the function 