#include <stdio.h>
#include <stdlib.h>

/*Const arrays , they are used to translate bits from card into actual readable represenation from card*/
char* valuesDisp[] = { "Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King"};
char* typeDisp[] = { "Hearts","Diamonds","Clubs","Spades"};
char* colourDisp[] = {"Black","Red"};

/*Encoding mechanisms to increase readability*/
typedef unsigned char card;
typedef unsigned char pairs;
enum bitMaskOptions { colour = 0, type = 1, value = 2 };
typedef enum bitMaskOptions cardOptions;

/*Bit Mask , used with & operation to extract the bits representing
raw type/val/colour values in the card.
__Note: raw bits need to be shifted by 'n' shiftmask bits to get true value
from encoded binary structure of card*/
const unsigned char valueBitMask = 13;
const unsigned char typeBitMask = 48;
const unsigned char colourBitMask = 64;
const unsigned char invalidCardBitMask = 128;
#define maxUnsignedCharBits 8

/*Shift Mask , used for determining how many
bits we need to move the right , in order
to change its bit representation into its
actual corresponding value and or index from
the above array of type/val/colour*/
const unsigned char valueIndexShiftMask = 0;
const unsigned char typeIndexShiftMask = 4;
const unsigned char colourIndexShiftMask = 6;
unsigned char deck[52];

/*Bit wise operations , used to get values from card data type*/
unsigned char getBits(unsigned char* c, unsigned char* shiftNBitsToR, unsigned char* bitMask);
unsigned char shiftBits(unsigned char* s, int* n);
unsigned char getBitFromCard(card* c, cardOptions option);
unsigned char getCValue(unsigned char* v);
unsigned char getCType(unsigned char* t);
unsigned char getCColour(unsigned char* c);

/*Function prototypes to do basic operations to  cards in deck array*/
void printCard(card c);
void printDeck(card deck[52]);
void fillDeck(card deck[52]);
void shuffle(card deck[52]);

int compareFace(const void* c1, const void* c2);
pairs findpairs(card* hand);

int main()
{
	card c = 254;	
	card d = getBitFromCard(&c,value);
	printf("%d\n", d);
	printCard(c);
	return 0;
}


unsigned char getBitFromCard(card* c, cardOptions option)
{
	switch (option)
	{
		case 0:
		{
			return getBits(c,&colourIndexShiftMask,valueBitMask);
		}
		break;

		case 1:
		{
			return getBits(getCType(c), typeIndexShiftMask, &typeBitMask);
		}
		break;

		case 2:
		{
			return getBits(getCValue(c), valueIndexShiftMask, &valueBitMask);
		}
		break;

		default:
		{
			return (unsigned char) 0;
		}
		break;
	}
}

void printCard(card c)
{
	unsigned char valPos, colourPos, typePos;
	 
	valPos = getBits(&c,&valueIndexShiftMask, &valueBitMask);
	//colourPos = getTrueValues(c, colour);
	//typePos = getTrueValues(c, type);
	//printf("%s of %s, is %s\n", valuesDisp[valPos], 1,1);// &colourDisp[colourPos], &typeDisp[typePos]);

}



unsigned char getBits(unsigned char* c , unsigned char* shiftNBitsToR , unsigned char* bitMask)
{
	return (unsigned char) (((*c) >> (*shiftNBitsToR)) & (*bitMask));
}

unsigned char shiftBits(unsigned char* s, int* n)
{
	return (*s) >> (*n);
}

unsigned char getAlignedBits(unsigned char* shiftedBits , int* numberOfBits)
{
	return *shiftedBits & *numberOfBits;
}

unsigned char getCValue(unsigned char* v)
{
	return  (unsigned char) ((*v) & valueBitMask);
}

unsigned char getCType(unsigned char* t)
{
	return (*t) & typeBitMask;
}

unsigned char getCColour(unsigned char* c)
{
	return (*c) & colourBitMask;
}
