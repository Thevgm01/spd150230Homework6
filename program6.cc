/* Steven Dunn
 * spd150230
 * SE 3377.501
 */

#include <iostream>
#include <fstream>  // For ifstream
#include <sstream>  // For stringstream
#include <string>
#include "cdk.h"
#include "program6.h"

#define FILE_NAME "binfile.bin" // Input file

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 18
#define MATRIX_NAME_STRING FILE_NAME

using namespace std;

int main() {

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;  // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  // Yes, the example on eLearning has titles like "a", "b", "c", etc. for the rows and the columns
  // However, I found this boring, so I switched them up
  // In case you were wondering why it may look different :)
  const char 		*rowTitles[] = {"", "A", "B", "C", "D", "E"};
  const char 		*columnTitles[] = {"", "1", "2", "3", "4", "5"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  // Initialize the Cdk screen.
  // Make sure the putty terminal is large enough
  window = initscr();
  cdkscreen = initCDKScreen(window);

  //Start CDK Colors
  initCDKColor();

  // Create the matrix.  Need to manually cast (const char**) to (char **)
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL) {
    printf("Error creating Matrix\n");
    _exit(1);
  }

  // Display the Matrix
  drawCDKMatrix(myMatrix, true);

  ifstream bInfile(FILE_NAME, ios::in | ios::binary); // Open the binary file
  if(!bInfile.good()){ // Check if the file opened properly
    printf("Error opening binary file %s", FILE_NAME);
    _exit(1);
  }

  BinFileHeader* header = new BinFileHeader(); // Store header information
  BinFileRecord* record = new BinFileRecord(); // Store record information
  stringstream ss; // For casting uint to string
  
  bInfile.read((char*) header, sizeof(BinFileHeader)); // Load the header
  
  ss << "0x" << std::uppercase << std::hex << header->magicNumber; // Read the magic number (in uppercase hex)
  setCDKMatrixCell(myMatrix, 1, 1, ("Magic: " + ss.str()).c_str());
  
  ss.str(""); ss.clear(); ss << std::nouppercase << std::dec << header->versionNumber; // Reset, read the version number
  setCDKMatrixCell(myMatrix, 1, 2, ("Version: " + ss.str()).c_str());

  ss.str(""); ss.clear(); ss << header->numRecords; // Reset, read the number of records
  setCDKMatrixCell(myMatrix, 1, 3, ("NumRecords: " + ss.str()).c_str());
  
  // Figure out the maximum number of entries to read (can never be more than 4)
  int max = header->numRecords;
  if(max > 4) max = 4;

  // For each record...
  for(int i = 0; i < max; i++){
    bInfile.read((char*) record, sizeof(BinFileRecord)); // Load the record
    int l = record->strLength;
    ss.str(""); ss.clear(); ss << l; // Read the length
    setCDKMatrixCell(myMatrix, i+2, 1, ("Length: " + ss.str()).c_str());
    setCDKMatrixCell(myMatrix, i+2, 2, record->stringBuffer);
  }

  // Close the file
  bInfile.close();

  // Display the updated matrix
  drawCDKMatrix(myMatrix, true);

  // So we can see results, pause until a key is pressed
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
