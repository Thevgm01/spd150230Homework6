#ifndef _PROGRAM6_H_
#define _PROGRAM6_H_

/* Steven Dunn
 * spd150230
 * SE 3377.501
 */

#include <stdint.h>

const int maxStringLength = 25;

class BinFileHeader {
 public:
  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;
};

class BinFileRecord {
 public:
  uint8_t strLength;
  char stringBuffer[maxStringLength];
};

#endif /* _PROGRAM6_H_ */
