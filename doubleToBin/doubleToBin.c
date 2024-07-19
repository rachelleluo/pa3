#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EXP_SZ 11
#define FRAC_SZ 52

int main(int argc, char *argv[]) {

    // SETUP

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return 0;
    }

    // first, read the number
    double value;
    fscanf(fp, "%lf", &value);

    if(abs(value) != value){
        printf("1_");
        value = value * -1;
    }else{
        printf("0_");
    }

    //convert decimal number to binary number
    //take left side  THIS WORKS
    int leftVal = (int)value;
    int leftCounter = 0;
    int leftTemp = leftVal;
    while(leftTemp > 0){
        leftTemp = leftTemp/2;
        leftCounter += 1;
    }

    int leftBin[leftCounter];
    int leftRemainder = 0;
    int leftIndex = leftCounter - 1;
    while(leftVal > 0){
        leftRemainder = leftVal % 2;
        leftBin[leftIndex] = leftRemainder;
        leftVal = leftVal/2;
        leftIndex -= 1;
    }


    //take right side
    double rightVal = value - ((int)value);
    int rightCounter = 0;
    double rightTemp = rightVal;
    if(rightTemp > 0){
        while(rightTemp != 1){
            if((rightTemp * 2) > 1){
                rightTemp = (rightTemp * 2) - 1;
            }else{
                rightTemp = (rightTemp * 2);
            }
            rightCounter += 1;
        }
    }

    int rightBin[rightCounter];
    int rightRemainder = 0;
    int rightIndex = rightCounter - 1;
    if(rightVal > 0){
        while(rightVal != 1){
            if((rightVal * 2) > 1){
                rightVal = (rightVal * 2) - 1;
                rightRemainder = 1;
            }else{
                rightVal = (rightVal * 2);
                rightRemainder = 0;
            }
            rightBin[rightIndex] = rightRemainder;
            rightIndex -= 1;
        }
        rightBin[rightCounter - 1] = 1;
    }


    //creating exponent binary num
    int trueExp;
    int biasedExpNum;
    if(leftCounter > 0){
        trueExp = leftCounter - 1;
        biasedExpNum = 1023 + trueExp;
    }else if(leftCounter <= 0 && rightCounter > 0){
        trueExp = 0;
        int j = 0;
        while(rightBin[j] != 1){
            j+=1;
            trueExp += 1;
        }
        biasedExpNum = 1023 - (trueExp + 1);
    }else if(leftCounter == 0 && rightCounter == 0){
        biasedExpNum = 0;
    }

    int biasedExp[11];
    int expRemainder = 0;
    int i = 10;
    if(biasedExpNum != 0){
        while(biasedExpNum > 0){
            expRemainder = biasedExpNum % 2;
            biasedExp[i] = expRemainder;
            biasedExpNum = biasedExpNum/2;
            i -= 1;
        }
    }else{
        for(int i = 0; i < 11; i++){
            biasedExp[i] = 0;
        }
    }


    for(int i = 0; i < 11; i++){
        printf("%d", biasedExp[i]);
    }
    printf("_");

    //printing mantissa

    for(int i = 0; i < leftCounter; i++){
        leftBin[i] += 0;
    }
    int mantissaCounter = 0;
    for(int i = 1; i < leftCounter; i++){
        printf("%d", leftBin[i]);
        mantissaCounter += 1;
    }
    for(int i = rightCounter - 2; i >= 0; i--){
        printf("%d", rightBin[i]);
        mantissaCounter += 1;
    }
    while(mantissaCounter != 52){
        printf("0");
        mantissaCounter += 1;
    }
}
