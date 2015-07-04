#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "huffmanchar.h"
#include "BTreeNode.h"
#include "ArrayList.h"
//#include "text.h"

void sort_dict(char* char_list, int* P, int size);
void populate_dict(char* stream, char** char_list_ptr, int** P_ptr, int* num_of_chars);
void print_dict(char* char_list, int* P,char** encoding, int size);
void gen_encoding_numbers(char* char_list, int* P, char** encoding, int size);
void sort_arr(ArrayList* P_arr, ArrayList* index_arr); 
int power(int base, int exp);
void read_text_from_file(char* fname, char** char_stream);
double calc_entropy(int* P, int size);
double calc_bits_per_char(int* P, char** encoding, int size);

int main(void) {
	//char stream[] = "this is an example of huffman coding";
	char fname[] = "huffman.txt";
	char* stream;
	read_text_from_file(fname, &stream);
	char* char_list;
	int* P;
	
	int size;
	
	//need pointer to pointers to change char_list and P dynamically (allocate memory)
	char** char_list_ptr = &char_list;
	int** P_ptr = &P;

	//populate the character dictionary
	populate_dict(stream, char_list_ptr, P_ptr, &size);
	char* encoding[size];
	char** encoding_ptr = encoding;
	//encoding = (char*)malloc(sizeof()*size);
	//run the algorithm and generate the encoding
	gen_encoding_numbers(char_list, P, encoding, size);

	double entr =  calc_entropy(P, size);
	double bpc = calc_bits_per_char(P, encoding, size);
	//print entropy
	printf("Entropy: %lf\n", entr);
	printf("Avg. Bits per char: %lf\n", bpc);
	printf("Compression Rate over ASCII: %lf%c\n", (1-bpc/8.0)*100,37);
	//print the dictionary
	printf("Dictionary\n");
	print_dict(char_list, P, encoding, size);
	return 0;
}

//open the text file 
void read_text_from_file(char* fname, char** char_stream) {
	//create the file ptr
	FILE *file_ptr = NULL;
	
	file_ptr = fopen(fname,"r");
	if(file_ptr == NULL){
		printf("Could not open file. Sorry.\n");
		//exit(0);
	}
	else {
		
		char* stream = malloc(sizeof(char) * 10485760); //10MB ASCII max - around 10 times the length of Jane Eyre
		int chars_read = 0;
		while(!(feof(file_ptr))){
			stream[chars_read] = fgetc(file_ptr);
			//printf( "%c", stream[chars_read]);
			chars_read++;
		}
		//printf("\n");
		stream[chars_read] = '\0'; //to be safe
		chars_read++;

		//close the file and shrink the string array down to size
		fclose(file_ptr);
		*char_stream = malloc(sizeof(char)*chars_read);
		int i;
		for(i=0; i<chars_read; i++) {
			if(stream[i] < 0) stream[i] = '\0';
			(*char_stream)[i] = stream[i];
			if ((*char_stream)[i] == '\0') break;
		}
		free(stream);
	}

	

}

//simple selection sort 
void sort_dict(char* char_list, int* P, int size) {
	int greatest_P = 0;
	int greatest_P_index = 0;
	int sorted_index = 0;
	while(sorted_index < size) {
		greatest_P = 0;
		int i;
		for (i=sorted_index; i<size; i++) {
			if(P[i] > greatest_P) {
				greatest_P = P[i];
				greatest_P_index = i;
			}
		}
		int temp_P = P[sorted_index];
		char temp_char = char_list[sorted_index];
		P[sorted_index] = P[greatest_P_index];
		char_list[sorted_index] = char_list[greatest_P_index];
		P[greatest_P_index] = temp_P;
		char_list[greatest_P_index] = temp_char;
		sorted_index++;
	}
}

//go through the text stream and get a list of each character and its appearences, and then sort it.
//stream - the text stream WITH a null terminator
//char_list - list of chars
//P - the corresponding number of times it appears
void populate_dict(char* stream, char** char_list_ptr, int** P_ptr, int* num_of_chars) {
	//make arrays at pointers 
	char* char_list = (char*)*char_list_ptr;
	int* P = (int*)*P_ptr;
	char_list = (char*)malloc(sizeof(char)*10000); //large to be safe, will be reduced to proper size once populated
	P = (int*)malloc(sizeof(int)*10000);
	int size=0;
	int i=0;
	while(stream[i] != '\0') {
		int j;
		int char_in_list = 0;
		for(j=0; j<size; j++) {
			if (stream[i] == char_list[j]) {
				P[j]++;
				char_in_list = 1;
				break;
			}
		}
		if(char_in_list == 0) {
			//add it to the list
			char_list[size] = stream[i];
			P[size] = 1;
			size++;
		}
		i++;
	}
	//now, reduce the arrays to their proper size;
	char* old_char = char_list;
	int* old_P = P;
	char_list = (char*)malloc(sizeof(char)*size);
	P = (int*)malloc(sizeof(int)*size);

	int k;
	for(k=0; k<size; k++) {
		char_list[k] = old_char[k];
		P[k] = old_P[k];
	}
	free(old_char);
	free(old_P);
	//populate_dict(stream,char_list,P,&size);
	*P_ptr = P;
	*char_list_ptr = char_list;
	sort_dict(char_list, P, size);
	*num_of_chars = size;
}

void print_dict(char* char_list, int* P,char** encoding, int size) {
	int i;
	for(i=0; i<size; i++) {
		printf("%c %i %s\n",char_list[i],P[i],encoding[i]);
	}
}

int nodesWOParents(BTreeNode* nodes, int size) {
	int i;
	int batmans = 0;
	for (i=0; i<size; i++) {
		if(nodes[i].hasParent == 0) {
			batmans++;
		}
	}
	return batmans;
}

//selection sort on the arrays of remaining items
void sort_arr(ArrayList* P_arr, ArrayList* index_arr) {
	int greatest_P = 0;
	int greatest_P_index = 0;
	int sorted_index = 0;
	
	while (sorted_index < P_arr->size) {
		greatest_P = 0;
		greatest_P_index = 0;
		int i;
		for(i=sorted_index; i<P_arr->size; i++) {
			if(P_arr->arr[i] > greatest_P) {
				greatest_P = P_arr->arr[i];
				greatest_P_index = i;
			}
		}
		int temp_P = P_arr->arr[sorted_index];
		int temp_P_index = index_arr->arr[sorted_index];
		P_arr->arr[sorted_index] = P_arr->arr[greatest_P_index];
		index_arr->arr[sorted_index] = index_arr->arr[greatest_P_index];
		P_arr->arr[greatest_P_index] = temp_P;
		index_arr->arr[greatest_P_index] = temp_P_index;
		sorted_index++;
	}
}
int power(int base, int exp) {
	int i;
	int ret = 1;
	for(i=0; i<exp; i++) {
		ret *= base;
	}
	return ret;
}

//the algorithm itself
void gen_encoding_numbers(char* char_list, int* P, char** encoding, int size) {
	//for N different characters, you need 2N-1 nodes on the tree
	BTreeNode nodes[2*size-1];
	int num_of_curr_nodes = size;
	ArrayList P_array;
	AL_init(&P_array);
	ArrayList P_return_index;
	AL_init(&P_return_index);
	//add the first N nodes
	//nodes WO parents added to A.L. w/ corresponding indicies
	int i;
	for(i=0; i<size; i++) {
		BN_init(&nodes[i]);
		nodes[i].value = P[i];
		AL_addItem(&P_array,P[i]);
		AL_addItem(&P_return_index,i);
	}
	//while there are more than one nodes w/o parents, combine more
	//printf("Nodes without parents \n");
	while(P_array.size > 1) {
		//sort both of the arrays
		sort_arr(&P_array, &P_return_index);
		//printf("P value:");
		//AL_print(&P_array);
		//printf("Corresponding Index:");
		//AL_print(&P_return_index);
		//printf("\n");
		//combine the last two by making a node whose value is the sum of them, and make that the parent.
		//the one with the larger value goes on the right.
		int nodesRem = P_array.size;
		int P_of_smallest = P_array.arr[nodesRem-1];
		int P_of_second_smallest = P_array.arr[nodesRem-2];
		int index_of_smallest = P_return_index.arr[nodesRem-1];
		int index_of_second_smallest = P_return_index.arr[nodesRem-2];
		BN_init(&nodes[num_of_curr_nodes]);
		nodes[num_of_curr_nodes].value = P_of_smallest + P_of_second_smallest;
		BN_add_left_child(&nodes[num_of_curr_nodes], &nodes[index_of_second_smallest]);
		BN_add_right_child(&nodes[num_of_curr_nodes], &nodes[index_of_smallest]);
		//remove the two nodes that now have parents from the array list
		AL_removeAt(&P_array, nodesRem-2);
		AL_removeAt(&P_array, nodesRem-2);
		AL_removeAt(&P_return_index, nodesRem-2);
		AL_removeAt(&P_return_index, nodesRem-2);
		//and add the new node
		AL_addItem(&P_array, P_of_smallest+P_of_second_smallest);
		AL_addItem(&P_return_index, num_of_curr_nodes);
		//and increment the num_of_curr_nodes
		num_of_curr_nodes++;
	}	
	//now that the tree hath been construct'ed, build'th thine encoding numbers (Genesis 19:74)
	int length = size;
	for(i=0; i<length; i++) {
		ArrayList encodingVals;
		AL_init(&encodingVals);
		BTreeNode* currNode = &nodes[i];
		while(currNode->hasParent == 1) {
			AL_addItem(&encodingVals, currNode->onParentSide);
			currNode = currNode->parent_ptr;
		}
		//now go through this list backwards to create the code
		char* encoding_curr_value = malloc(sizeof(char)*(encodingVals.size+1));
		int j;
		for(j=encodingVals.size-1; j>=0; j--) {
			if (encodingVals.arr[j] == 1) {
				encoding_curr_value[encodingVals.size - 1 - j] = '1';
			}
			else encoding_curr_value[encodingVals.size - 1 - j] = '0';
		}
		encoding_curr_value[encodingVals.size] = '\0';
		encoding[i] = encoding_curr_value;
	}

}

//Shannon entropy in bits
double calc_entropy(int* P, int size) {
	double ent = 0;
	int totalchar = 0;
	int i;
	for(i=0; i<size; i++) {
		totalchar += P[i];
	}
	for(i=0; i<size; i++) {
		double P_db = (double)P[i] / totalchar;
		ent += P_db * (log(P_db)/log(2));
	}	
	return -ent;
}

int encoding_len(char* encoding_char) {
	int i=0;
	while(encoding_char[i] != '\0') {
		i++;
	}
	return i;
}

//Weighted avg num of bits per char in file
double calc_bits_per_char(int* P, char** encoding, int size) {
	int sum_of_chars = 0;
	double weighted_avg = 0.0;
	int i;
	for(i=0; i<size; i++) {
		weighted_avg += P[i] * encoding_len(encoding[i]);
		sum_of_chars += P[i];
	}
	return weighted_avg/sum_of_chars;
}