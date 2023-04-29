// #include <stdint.h>
// #include <Arduino.h>
// #include "serial.h"

// const uint8_t numChars = 3720; // 'x y z\n'*512
// char serialBuffer[numChars];   // an array to store the received data

// void recvWithEndMarker() {
//   static uint8_t ndx = 0;
//   static uint8_t commaIdx = 0;
//   char endMarker = '\n';
//   char rc;
//   while (port.available() > 0) {
//     rc = port.read();
//     if (rc != endMarker) {
//       serialBuffer[ndx] = rc;
//       ndx++;
//       if (ndx >= numChars) {
//           ndx = numChars - 1;
//       }
//     } else {
//       serialBuffer[ndx] = '\0'; // terminate the string
//       char tempChars[numChars];
//       strcpy(tempChars, serialBuffer);
//       Serial.println(tempChars);
//       // this temporary copy is necessary to protect the original data
//       //   because strtok() used in parseData() replaces the commas with \0
//       memset(receivedFloat, 0, sizeof(int)*numPackets);
//       parseData(tempChars);
//       Serial.println("serial:\t"+(String)receivedFloat[0] + ", " + (String)receivedFloat[1]);
//       ndx = 0;
//     }
//   }
// }

// void parseData(char* tempChars) {      // split the data into its parts
//   char * token; // this is used by strtok() as an index
//   int idx = 0;
//   token = strtok(tempChars, ",");
  
//   while ( token != NULL) {
//     float floatToken = atof(token); // process previous one to int (atof to float, etc.)
// //    Serial.println(floatToken);
//     receivedFloat[idx] = floatToken;
//     idx++;
//     token = strtok(NULL, ","); // prepare for the next
//   }
// }