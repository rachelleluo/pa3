#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // first, read the binary number representation of float point number
    //32 digit binary number breakdown ex: 01001001000101101001100011000100
    /*
    0                         1001 0010                           001 0110 1001 1000 1100 0100
                                                                       significant number
                                                              put a 1. in front of this number and 
    sign         biased exponent-> convert this to a num        move the decimal place over the
    (+ or -)     then true exponent = biased exponent - 127         true exponent many times
                                                                ex: 0010110 becomes 1.0010110 with 
                                                               true exponent 5 becomes 100101.10
                                                                now solve for 100101.10 using base 2
    */


    char buff;
    int original[32]; // suggested that you store bits as array of bools; minuend[0] is the LSB
    for (int i=EXP_SZ+FRAC_SZ; i>=0; i--) { // read MSB first as that is what comes first in the file
        if (fread(&buff, sizeof(char), 1, fp) != 1) {
            perror("Error reading next char\n");
            exit(EXIT_FAILURE);
        }
        original[i] = (buff & 1);
    }

    bool sign = true;
    if(original[31] == 1){
        sign = false;
    }

    // float number = *(float *)&binary; // you are not allowed to do this.

    // E --> exponent
    int expIndex = 0;
    int exponent[8];
    while(expIndex != 8){
        exponent[expIndex] = original[30-expIndex];
        expIndex += 1;
    }

    int expNum = 0;
    int power = 0;
    for(int i = 7; i >= 0; i--){
        expNum += exponent[i] * pow(2, power);
        power += 1;
    }

    int trueExp = expNum - 127;



    // M --> mantissa
    int manIndex = 23;
    int mantissa[24];
    mantissa[0] = 1;
    while(manIndex != 0){
        mantissa[manIndex] = original[23-manIndex];
        manIndex -= 1;
    }

    double finalAns = 0.0;
    int sigNumLeft = 0;
    int sigPowerLeft = 0;
    double sigNumRight = 0;
    int sigPowerRight = -1;

    if(abs(trueExp) > 23){
        printf("%f", finalAns);
        return EXIT_SUCCESS;
    }else if(trueExp >= 0){
        for(int i = (trueExp); i >= 0; i--){
            sigNumLeft += mantissa[i] * pow(2, sigPowerLeft);
            sigPowerLeft += 1;
        }
        //now do right side
        for(int i = (trueExp + 1); i < 23; i++){
            if(mantissa[i] == 1){
                sigNumRight += mantissa[i] * pow(2, (sigPowerRight));
            }
            sigPowerRight -= 1;
        }
    }else if(trueExp < 0){
        //only do right side
        sigPowerRight = trueExp;
        for(int i = 0; i < 23; i++){
            sigNumRight += mantissa[i] * pow(2, sigPowerRight);
            sigPowerRight -= 1;
        }
    }

    finalAns = sigNumLeft + sigNumRight;
    if(!sign){
        finalAns *= -1;
    }

    printf("%e\n", finalAns);

    return EXIT_SUCCESS;

}
