
#ifndef EDITNUM10_H_
#define EDITNUM10_H_


unsigned int editNum10(unsigned char chnl, unsigned char SorE, unsigned int num, unsigned char row, unsigned char col);
char* Num10toStr10(unsigned int num);
char* Str10toDis10(char* num_str);
unsigned char Num10Check(char* n);//Input Format: "########.##"
unsigned int Dis10toNum10(char* n);//Input Format: "########.##"

#endif
