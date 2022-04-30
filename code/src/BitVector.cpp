/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */
#include "BitVector.h"

int BitVector::readNextItemFromFile(FILE* inputFileStream){
	if (! inputFileStream){
		char* message = new char[2048];
		sprintf(message, "Cannot open input file for reading");
		throw std::ios_base::failure(message);
		delete []message;
	}

	// variable declaration 
	long returnInteger, beforeReturn;
	char input[255];
	bool spaceOrTabAfterDigit = false; // boolean flag for a space or tab after a digit on the same line
	bool isDigitFound = false; // boolean flag for digits found on a given line
	int numCounter = 0; // counter for how many digits are in a number 

	

		// reading the next line from the inputfiles stream
		fgets(input, 255, inputFileStream);
		
		if (input[0] == '\n') //recall the fuction itself if the first position is a newline 
		{	
				return readNextItemFromFile(inputFileStream);

		}
		
		for (int i = 0; i < strlen(input); i++)//looping through the elements of the new line to check various if statements
		{
			if (input[i] == '\0')// leave the loop when a null is found within the line
			{
				break; 
			}	
			
			
			//checking if the new line starts with a number, space, tab, -, new line or carriage return
			// in which case recall the readNextItem function
			if (!isdigit(input[i]) && input[i] != ' ' && input[i] != '\t' && input[i] != '-' && input[i] != '\n' && input[i] != '\r')
			{	
				return readNextItemFromFile(inputFileStream);
			} 
			// checking if the line has an - followed by something other than a number
			// recall the function on itself
			if ((input[i] == '-') && !isdigit(input[i + 1]))
			{	
					return readNextItemFromFile(inputFileStream);
			}
			// checking if the number if followed by a a space or a tab and updating the spaceOrTabAfterDigit bool variable
			if (spaceOrTabAfterDigit != true && isdigit(input[i]) && (input[i + 1] == ' ' || input[i + 1] == '\t'))
			{
				spaceOrTabAfterDigit = true;
			}
			// recall the function if there's a digit, space and another digit
			else if (spaceOrTabAfterDigit && isdigit(input[i]))
					return readNextItemFromFile(inputFileStream);

			//if the is a digit, use numCounter to keep track of the number of digits on the line
			// Seting the bool variable to true to indicate that a variable is found
			if (isdigit(input[i]))
			{
				isDigitFound = true;
				
				numCounter++;
			}
			// recall the function itself if the number is longer than 12 dugits because 2147483647 is less than 12 digits
			// and 2147483647 is the largest input
			if (numCounter >= 12)
			{
					return readNextItemFromFile(inputFileStream);
			}
			// recall the function if the number is followed by a space, tab, new line feed or a carriage return
			if (isDigitFound && !isdigit(input[i]) && input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\r')
					return readNextItemFromFile(inputFileStream);
		}
		// if numcounter is still = 0 then there is no number on that line so the function should recall itself 
		if(numCounter == 0)
		{
				return readNextItemFromFile(inputFileStream); // return an integer flag to the calling function
		}
		beforeReturn = atoi(input); // using atoi to get the integer value of the line that was read as char array
		
		// if the integer is between the valid range of max and min integer inputs, then set it to the variable that will be returned
		if (beforeReturn >= -2147483648   && beforeReturn <= 2147483647 ) {
			returnInteger = beforeReturn; 
		}
		else{// else if the value is not in range then recall the function itself
			return readNextItemFromFile(inputFileStream); 
			
		}

	return returnInteger;// return statement for the readNextInt function
}




int BitVector::processFile(char *inputFilePath, char *outputFilePath){
	FILE* inFileStream = fopen(inputFilePath, "r");
	if (! inFileStream){
		char* message = new char[2048];
		sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
		throw std::ios_base::failure(message);
		delete []message;
	}

	FILE* outFileStream = fopen(outputFilePath, "w");
	if (! outFileStream){
		char* message = new char[2048];
		sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
		throw std::ios_base::failure(message);
		delete []message;
	}
		
	LogManager::writePrintfToLog(LogManager::Level::Status, "BitVector::processFile", "Starting the processing");

	/*
	Your code to process a file should be written after these comments.
	*/
		
	// Variable declarion
	BitArray emptyBit;
	bool isEmpt;
	BitArray * currBitArray= new BitArray[41944];// an array of bitarrays

	int index;
	int val;
		 

	while (!feof(inFileStream))
	{
		int number = readNextItemFromFile(inFileStream);

			index = floor( ( number - (-2147483648)) / 102400);// calculating the the position of the array in the container array
			 val = number - (-2147483648 +(102400 * index));// callculating the the position of the number in the sub bitarray 
             isEmpt = currBitArray[index].isEmpti();// checking if the bitarray at that index is empty
			//if the subarray at that index is empty, then the bit 1 is set to the the calculatedposition of the number in the array

			if (isEmpt == true){ 
					currBitArray[index].setBit(val, 1); 
					currBitArray[index].setNotEmpti();// setting a flag to mark the array as not empty, signal that the function is not empty
					if(!feof(inFileStream)){//if the end of the file is not reached, then print  1 to the output file, since this is the first time of seeing this number
					fprintf(outFileStream, "1\n");
					}
				} else{ // else if the bitarray is not empty, check the values at the index of the number within the bitarray
					if (currBitArray[index].getBit(val) == true){ //if the index of the number is true, print 0
						if(!feof(inFileStream)){//if the end of the file is not reached, then print 0 to the output file since the number was seen before
						 fprintf(outFileStream, "0\n");
						}
					}else//if the index of te number within the bitaaray is false, then print 1 to say it seen for the first time
					{ 
					currBitArray[index].setBit(val, 1);
					if(!feof(inFileStream)){
					 fprintf(outFileStream, "1\n");
					//  std::cout<<"1\n";
					}
					}




    } 
	}
	fflush(outFileStream);
	fclose(outFileStream);
	fclose(inFileStream);	


}

int BitVector::generateTestCases(char* outputFilePath, int min, int max, int numberOfEntries, int errorTypes){
	if (false)
		return 0;
	FILE* outFileStream = fopen(outputFilePath, "w");
	if (! outFileStream){
		char* message = new char[2048];
		sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
		throw std::ios_base::failure(message);
		delete []message;
	}
	int currInt;
	int maxRandomToAddError = 5;
	for (int i=0; i< numberOfEntries; i++){
		currInt = getRandomInt(min, max);
		if (errorTypes == 0){
			fprintf(outFileStream, "%d\n", currInt);
		}
		if (errorTypes == 1){
			if (getRandomInt(0, maxRandomToAddError) == 3){
				int intNum = getRandomInt(0, maxRandomToAddError);
				float floatNum = ((float) intNum) / 100;
				fprintf(outFileStream, "%f\n", floatNum);
			}
			if (getRandomInt(0, maxRandomToAddError) == 2)
				fprintf(outFileStream, "%c", generateRandomWhiteSpace());
			fprintf(outFileStream, "%d\n", currInt);
		}

		if (errorTypes == 2){
			if (getRandomInt(0, maxRandomToAddError) == 2)
				fprintf(outFileStream, "%c\n", generateRandomWhiteSpace());
			fprintf(outFileStream, "%d\n", currInt);
		}

		if (errorTypes == 3){
			if (getRandomInt(0, maxRandomToAddError) == 2){
				std::string randomStr = genRandomString(3);
				fprintf(outFileStream, "%s\n", randomStr.c_str());
			}
			fprintf(outFileStream, "%d\n", currInt);
		}
	}
	fflush(outFileStream);
	fclose(outFileStream);
}

int BitVector::generateTestCases(char* outputFolderPath){
	char filePath[1024];
	int numElements;
	int minValue[2] = {-1023, INT_MIN};
	int maxValue[2] = {1023, INT_MAX};
	char dataType[2][5] = {"easy", "hard"};

	for (int k=0; k<2; k++){
		for (int i=0; i<4; i++){
			sprintf(filePath, "%s/%s_sample_0%d.txt", outputFolderPath, dataType[k], i+1);
			numElements = getRandomInt(500000, 800000);
			//numElements = 10;
			LogManager::writePrintfToLog(LogManager::Level::Status, "BitVector::generateTestCases",
				"numElements=%d", numElements);
			generateTestCases(filePath, minValue[k], maxValue[k], numElements, i);

			sprintf(filePath, "%s/%s_test_0%d.txt", outputFolderPath, dataType[k], i+1);
			numElements = getRandomInt(500000, 800000);
			LogManager::writePrintfToLog(LogManager::Level::Status, "BitVector::generateTestCases",
				"numElements=%d", numElements);
			generateTestCases(filePath, minValue[k], maxValue[k], numElements, i);
		}
	}
}
