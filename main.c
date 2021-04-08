#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
struct pairs { int numOfPairs; unsigned char highestPair; };



/*Bit Mask , used with & operation to extract the bits representing
raw type/val/colour values in the card.
__Note: raw bits need to be shifted by 'n' shiftmask bits to get true value
from encoded binary structure of card*/
const unsigned char valueBitMask = 15;
const unsigned char suitMask = 96;
const unsigned char colourBitMask = 16;
const unsigned char invalidCardBitMask = 128;
#define maxUnsignedCharBits 8
#define deckSize 52
#define numOfPlayers 5
#define numCardsPerHand 5
#define maxNumPairs = numCardsPerHand/2;

/*Shift Mask , used for determining how many
bits we need to move the right , in order
to change its bit representation into its
actual corresponding value and or index from
the above array of type/val/colour*/
const unsigned char valueBitsetOffset = 0;
const unsigned char suitBitOffset = 5;
const unsigned char colourBitsetOffset = 4;
unsigned char deck[deckSize];
unsigned char hands[numOfPlayers][numCardsPerHand];
struct pairs p;

/*Bit wise operations , used to get values from card data type*/
unsigned char getBits(unsigned char* c, unsigned char* bitMask);
unsigned char shiftBits(unsigned char* s, unsigned char* n , bitDirection b);
unsigned char addBits(unsigned char* c, unsigned char* bits);
unsigned char getBitSetFromCard(card* c, cardOptions option);
unsigned char convertBitSetToIndex(card* c, cardOptions option);

/*Function prototypes to do basic operations to  cards in deck array*/
void printCard(card c);
void printDeck(card deck[deckSize]);
void fillDeck(card deck[deckSize]);
void shuffle(card deck[deckSize]);
void drawHands(card deck[deckSize], card hands[numOfPlayers][numCardsPerHand]);

int compareFace(const void* c1, const void* c2);
struct pairs findpairs(card** hand);
void findAllPairs(card** playerHands);
void printpairs(struct pairs* p);

int main()
{
	srand(time(0));
	card  deck[deckSize];
	fillDeck(deck);
	printDeck(deck);
	shuffle(deck);
	printDeck(deck);
	drawHands(deck, hands);
	findAllPairs(hands);
	return 0;
}

void printpairs(struct pairs* p)
{
	if (p != NULL)
	{
		if (p->numOfPairs > 0)
		{
			printf("%s %d\n", "The number of pairs are:", p->numOfPairs);
			printf("%s %d\n", "The highest pair is", p->highestPair);
		}
		else
		{
			printf("%s\n", "There are no matching pairs\n");
		}
	}
}

void findAllPairs(card** playerHands )
{
	struct pairs p[numOfPlayers];
	for (size_t i = 0; i < numOfPlayers; i++)
	{
		p[i] = findpairs(&playerHands[i]);
	}
}

void checkNULL(void* p)
{
	if (p == NULL)
	{
		EXIT_FAILURE;
	}
}

struct pairs findpairs(card** hands , int playerID)
{
	card currentValue = 128;
	struct pairs p;

	for (size_t i = 0; i < playerID; i++)
	{
		for (size_t j = 0; j < numCardsPerHand; j++)
		{
			if (hands[i][j] < invalidCardBitMask)
			{
				currentValue = getBitSetFromCard(hands[i][j], value);
				int occurances = 1;
				for (size_t k = 0; k < numCardsPerHand && k!=j && hands[i][k] < invalidCardBitMask; k++)
				{
					if (getBitSetFromCard(hands[i][k],value) == currentValue)
					{
						occurances++;
						addBits(hands[i][k], invalidCardBitMask);
					}
				}
				p.numOfPairs = (int) occurances/2;
				if (p.numOfPairs >= 1)
				{
					if (p.highestPair < currentValue)
					{
						p.highestPair = currentValue;
					}
				}
			}		
		}
	}
	return p;
}

int compareFace(const void* c1, const void* c2)
{
	unsigned char value_1 = getBitSetFromCard((unsigned char) c1, value);
	unsigned char value_2 = getBitSetFromCard((unsigned char) c2, value);

	if ( value_1 > value_2 )
	{
		return 1;
	}
	else if( value_1 ==  value_2 )
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void drawHands(card deck[deckSize], card hands[numOfPlayers][numCardsPerHand])
{
	int deckIndex = 0;
	for (size_t i = 0; i < numOfPlayers; i++)
	{
		for (size_t j = 0; j < numCardsPerHand; j++)
		{
			hands[i][j] = deck[deckIndex];
		}
	}
}

void shuffle(card deck[deckSize])
{
	card temp;
	for (size_t i = 0; i < deckSize; i++)
	{
		int randIndex = rand()%deckSize;
		temp = deck[i];
		deck[i] = deck[randIndex];
		deck[randIndex] = temp;
	}
}


void printDeck(card deck[deckSize])
{
	for (size_t i = 0; i < deckSize; i++)
	{
		/*Insert blank line for each suit , ie every 13 cards*/
		if (i % 13 == 0)
		{
			printf("\n");
		}
		printCard(deck[i]);
	}
}

void fillDeck(card deck[deckSize])
{
	size_t deckIndex = 0;
	size_t maxNumSuit = 4;
	size_t maxNumCards = 13;
	const unsigned char colourBit = colourBitMask;
	unsigned char preBit = 0;

	for (size_t suit = 0; suit < maxNumSuit; suit++)
	{
		preBit = shiftBits(&suit, &suitBitOffset, left);
			
		if (suit < 2)
		{
			preBit = addBits(&preBit, &colourBit);
		}
			
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

/*Function that is designed to get values,colour or suits from card using
bitwise operation , this is to simplify & operation*/
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

/*Function takes a bitset and aligns bits to the *disp Array index 
ie take raw value and right align the bits to proper numerical value*/
unsigned char convertBitSetToIndex(card* c, cardOptions option)
{
	unsigned char bitSet = getBitSetFromCard(c, option);
	switch (option)
	{
		case colour:
		{
			return shiftBits(&bitSet,&colourBitsetOffset, right);
		}
		break;

		case suit:
		{
			return shiftBits(&bitSet, &suitBitOffset, right);
		}
		break;

		case value:
		{
			return shiftBits(&bitSet, &valueBitsetOffset, right);
		}
		break;

		default:
		{
			return invalidCardBitMask;
		}
		break;
	}
}

/*Display the card formatting in readable format for user*/
void printCard(card c)
{
	unsigned char valPos, colourPos, suitPos;

	valPos = convertBitSetToIndex(&c,value);
	colourPos = convertBitSetToIndex(&c, colour);
	suitPos = convertBitSetToIndex(&c, suit);
	printf("%s of %s, is %s\n", valuesDisp[valPos], colourDisp[colourPos],suitDisp[suitPos]);
}
/*Bitwise operation designed to get the value of bits from card
Bitmask is binary encoded value for & operation*/
unsigned char getBits(unsigned char* c , unsigned char* bitMask)
{
	return (unsigned char) (*c) & (*bitMask);
}

/*Add bits to existing bit field*/
unsigned char addBits(unsigned char* c, unsigned char* bits)
{
	return (*c) | (*bits);
}

/*Shift the bits left or right by n amount to get bit aligned values*/
unsigned char shiftBits(unsigned char* s, unsigned char* n, bitDirection b)
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



