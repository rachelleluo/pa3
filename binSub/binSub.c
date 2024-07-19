#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
// https://www.tutorialspoint.com/c_standard_library/limits_h.htm
#include <limits.h>

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP

    // first, read the minuend (number to be subtracted from)
    char buff;
    bool minuend[CHAR_BIT]; // suggested that you store bits as array of bools; minuend[0] is the LSB
    for (int i=CHAR_BIT-1; 0<=i; i--) { // read MSB first as that is what comes first in the file
        if (fread(&buff, sizeof(char), 1, fp) != 1) {
            perror("Error reading next char\n");
            exit(EXIT_FAILURE);
        }
        // Extract individual bits from buff and store them in minuend array
        minuend[i] = (buff & 1);
    }

    // notice that you are reading two different lines; caution with reading
    fseek(fp, 1, SEEK_CUR);

    // second, read the subtrahend (number to subtract)
    bool subtrahend[CHAR_BIT]; // suggested that you store bits as array of bools; subtrahend[0] is the LSB
    for (int i=CHAR_BIT-1; 0<=i; i--) { // read MSB first as that is what comes first in the file
        if (fread(&buff, sizeof(char), 1, fp) != 1) {
            perror("Error reading next char\n");
            exit(EXIT_FAILURE);
        }
        // Extract individual bits from buff and store them in minuend array
        subtrahend[i] = (buff & 1);
    }

    // WE WILL DO SUBTRACTION BY NEGATING THE SUBTRAHEND AND ADD THAT TO THE MINUEND

    // Negate the subtrahend
    // flip all bits
    for(int i = CHAR_BIT-1; i >= 0; i--){
        subtrahend[i] = !subtrahend[i];
    }

    // add one
    bool carry = true; // to implement the 'add one' logic, we do binary addition logic with carry set to true at the beginning
    for (int i=0; i<CHAR_BIT; i++) { // iterate from LSB to MSB
        if (subtrahend[i] == true && carry == true) {
            subtrahend[i] = false;
        }else if(subtrahend[i] == false && carry == true) {
            subtrahend[i] = true;
            carry = false;
        }
    }

    // Add the minuend and the negated subtrahend
    int difference[CHAR_BIT];
    carry = false;
    for (int i = 0; i < CHAR_BIT ; i++) {
        int sum = (int)subtrahend[i] + (int)minuend[i] + (int)carry;
        difference[i] = sum % 2;
        carry = sum/2;
    }
    

    // print the difference bit string
    for (int i=CHAR_BIT-1; 0<=i; i--)
        printf("%d",difference[i]);

    return EXIT_SUCCESS;

} 
