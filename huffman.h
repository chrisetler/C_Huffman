#include <stdlib.h>
typedef struct HuffmanCoding huff;

struct HuffmanCoding {
	char* stream[];
	ArrayList* nodeList;

};