#include "Input/input.h"
#include "Output/output.h"
#include "Recognize/recognize.h"

#include "RTES.h"
#include <stdio.h>

int main(void) {
    buffer_t buffer = createBuffer(100);
    
    for (size_t i = 0; i < 50; i++) {
        insertIntoBuffer(&buffer, 5);
    }   

    printStatusBuffer(&buffer);

    removeFromBuffer(&buffer, 50);
    printStatusBuffer(&buffer);
    
    for (size_t i = 0; i < 60; i++) {
        insertIntoBuffer(&buffer, 5);
    }
    printStatusBuffer(&buffer);   

    removeFromBuffer(&buffer, 10);
    printStatusBuffer(&buffer);
 
    return 0;  
   
}
