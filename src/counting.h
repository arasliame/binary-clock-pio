#include <ledfuncs.h>

void countUp(unsigned long start) {
  for (unsigned long count = start; count <= maxCount; count++) {
    numToBits(count, matrix);
    displayBits(matrixHeight, matrix);
  }
}

void countDown(unsigned long start) {
  if (start > maxCount) {
    err(matrixHeight, matrix); //blink if number is too big
  } 
  else {
    for (unsigned long count = start; count > 0; count--) {
      numToBits(count, matrix);
      displayBits(matrixHeight, matrix);
    }
  }
}