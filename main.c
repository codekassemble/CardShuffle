#include <stdio.h>
#include <stdlib.h>

/*Const arrays , they are used to translate bits from card into actual readable represenation from card*/
char* valuesDisp[] = { "Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King"};
char* suitDisp[] = { "Hearts","Diamonds","Clubs","Spades"};
char* colourDisp[] = {"Black","Red"};

/*Encoding mechanisms to increase readability*/
typedef unsigned char card;
typedef unsigned char pairs;
enum bitMaskOptions { colour = 0, suit = 1, value = 2 };
typedef enum bitMaskOptions cardOptions;
enum bitDirection { left = 0 , right = 1};
typedef enum bitDirection bitDirection;

/*Bit Mask , used with & operation to extract the bits representing
raw type/val/colour values in the card.
__Note: raw bits need to be shifted by 'n' shiftmask bits to get true value
from encoded binary structure of card*/
const unsigned char valueBitMask = 15;
const unsigned char suitMask = 96;
const unsigned char colourBitMask = 16;
const unsigned char invalidCardBitMask = 128;
#define maxUnsignedCharBits 8

/*Shift Mask , used for determining how many
bits we need to move the right , in order
to change its bit representation into its
actual corresponding value and or index from
the above array of type/val/colour*/
const unsigned char valueBitsetOffset = 0;
const unsigned char suitBitOffset = 5;
const unsigned char colourBitsetOffset = 4;
unsigned char deck[52];

/*Bit wise operations , used to get values from card data type*/
unsigned char getBits(unsigned char* c, unsigned char* bitMask);
unsigned char shiftBits(unsigned char* s, int* n , enum bitDirection);
unsigned char addBits(unsigned char* c, unsigned char* bits);
unsigned char getBitSetFromCard(card* c, cardOptions option);



/*Function prototypes to do basic operations to  cards in deck array*/
void printCard(card c);
void printDeck(card deck[52]);
void fillDeck(card deck[52]);
void shuffle(card deck[52]);

int compareFace(const void* c1, const void* c2);
pairs findpairs(card* hand);

int main()
{
	card  deck[52];
	fillDeck(deck);
	//printCard(deck[0]);
	card d = convertBitSetToIndex(deck[1], value);
	printf("%d\n", d);
	return 0;
}

void fillDeck(card deck[52])
{
	size_t deckIndex = 0;
	size_t maxNumColours = 4;
	size_t maxNumCards = 13;
	size_t colorBitOffset = 5;
	unsigned char preBit = 0;

		for (size_t colour = 0; colour < maxNumColours; colour++)
		{
			preBit = shiftBits(&colour, &colorBitOffset, left);
			for (size_t value = 0; value < maxNumCards; value++)
			{
				/*We use bitwise operation to  ADD bits to  existing bitset
				AND is used to extract bits*/
				deck[deckIndex++] = addBits(&preBit,&value);

				/*Debug statement  to show what value of card we have created*/
				//printf("index is : %d times\n value of card is %d\n", deckIndex,deck[deckIndex-1]);		
			}
		}	
}


unsigned char getBitSetFromCard(card* c, cardOptions option)
{
	switch (option)
	{
		case 0:
		{
			return getBits(c,&colourBitMask);
		}
		break;

		case 1:
		{
			return getBits(c, &suitMask);
		}
		break;

		case 2:
		{
			return getBits(c, &valueBitMask);
		}
		break;

		default:
		{
			return (unsigned char) 0;
		}
		break;
	}
}

/*Function takes a bitset and aligns bits to the *disp Array index */
unsigned char convertBitSetToIndex(card* c, cardOptions option)
{
	unsigned char bitSet = getBitSetFromCard(c, option);
	switch (option)
	{
		case colour:
		{
			return shiftBits(bitSet, colourBitsetOffset, right);
		}
		break;

		case suit:
		{
			return shiftBits(bitSet, suitBitOffset, right);
		}
		break;
		case value:
		{
			return shiftBits(bitSet, valueBitsetOffset, right);
		}
		break;

		default:
		{
			return invalidCardBitMask;
		}
		break;
	}
}

void printCard(card c)
{
	unsigned char valPos, colourPos, suitPos;
	 
	valPos = getBitSetFromCard(&c,value);
	colourPos = getBitSetFromCard(&c, colour);
	suitPos = getBitSetFromCard(&c, suit);
	printf("%s of %s, is %s\n", valuesDisp[valPos], colourDisp[colourPos],suitDisp[suitPos]);

}

unsigned char getBits(unsigned char* c , unsigned char* bitMask)
{
	return (unsigned char) (*c) & (*bitMask);
}

unsigned char addBits(unsigned char* c, unsigned char* bits)
{
	return (*c) | (*bits);
}

unsigned char shiftBits(unsigned char* s, int* n, bitDirection b)
{
	if (b == right)
	{
		return (*s) >> (*n);

	}
	else
	{
		return (*s) << (*n);
	}
}



