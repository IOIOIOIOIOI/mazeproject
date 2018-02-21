#ifndef FONT_H
#define FONT_H

void  mssmStrokeCharacter(bool mono, int character );
void  mssmStrokeString(bool mono, const char *string );
int  mssmStrokeWidth(bool mono, int character );
int  mssmStrokeLength(bool mono, const char* string );
double  mssmStrokeHeight(bool mono);




#endif // FONT_H
