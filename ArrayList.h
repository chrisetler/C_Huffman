/*Array List Library 'object*/
#include <stdlib.h>
#define STEPSIZE 10
typedef int Type;
typedef struct ArrayList ArrayList;

struct ArrayList {
	int capacity;
	int size;
	Type *arr;
};

//initialize a new arraylist
//default size of STEPSIZE items
void AL_init(ArrayList* list) {
	list->size = 0;
	list->capacity = STEPSIZE;
	list->arr = (Type*)malloc(sizeof(Type)*STEPSIZE);
}

//free up some extra space (reduce capacity to only what is needed)
void AL_free(ArrayList *list) {
	int *oldList = list->arr;
	int new_capacity = (list->size/STEPSIZE + 1)*STEPSIZE;
	list->arr = (Type*)malloc(sizeof(Type)*new_capacity);
	int i;
	for(i=0; i<list->size; i++) {
		list->arr[i] = oldList[i];
	}
	list->capacity = new_capacity;
	free(oldList);
}

//Add a single item to the end of the array
void AL_addItem(ArrayList* list, Type item) {
	int addAt = list->size;
	//make the array longer if needed
	if(list->capacity <= addAt) {
		int *oldList = list->arr;
		list->arr = (Type*)malloc(sizeof(Type)*(list->capacity+STEPSIZE));
		list->capacity = list->capacity + STEPSIZE;
		//copy over the old list
		int i;
		for(i=0; i<list->size; i++){
			list->arr[i] = oldList[i];
		}
		free(oldList);
	}
	//add the next item at [size]
	list->arr[list->size] = item;
	list->size = list->size + 1;
}

//Add an array of items to the end of the array
void AL_addItems(ArrayList* list, Type items[], int length) {
	int i=0;
	//int length = (sizeof(&items))/sizeof(Type);
	for(i=0; i<length; i++) {
		AL_addItem(list,items[i]);
	}
}

//add at a particular index if possible
int AL_addAt(ArrayList* list, Type item, int index) {
	if(index >= list->size) return -1;
	else {
		//create the array that needs to be shifted over
		int arr_to_shift_len = list->size-index;
		Type *arr_to_shift = (Type*)malloc(sizeof(Type)*(arr_to_shift_len));

		int i;
		for(i=0; i<(arr_to_shift_len); i++) {
			arr_to_shift[i] = list->arr[i+index];
		}
		//add the new item
		list->size = index;
		AL_addItem(list, item);
		//add the remaining list shifted over
		AL_addItems(list, arr_to_shift, arr_to_shift_len);
		free(arr_to_shift);
	}
}

//print the array
void AL_print(ArrayList *list) {
	printf("{");
	int length = list->size;
	int i;
	for (i=0; i<length; i++){
		printf("%i",list->arr[i]);
		if(i<(length-1)) printf(",");
	}
	printf("}\n");
}

//remove the item at a particular index
int AL_removeAt(ArrayList *list, int index) {
	if(index >= list->size) return -1;
	else {
		//create the array to be shifted over
		int arr_to_shift_len = list->size-index-1;
		Type *arr_to_shift = (Type*)malloc(sizeof(Type)*(arr_to_shift_len));

		int i;
		for(i=0; i<(arr_to_shift_len); i++) {
			arr_to_shift[i] = list->arr[i+index+1];
		}
		//shift over, overwriting the removed item
		list->size = index;
		AL_addItems(list, arr_to_shift, arr_to_shift_len);
		free(arr_to_shift);
		if(list->capacity - list->size > STEPSIZE ){
			AL_free(list);
		}
		return 0;
	}
}

//remove a particular item - the first instance of such item
int AL_remove(ArrayList *list, Type item) {
	int i;
	for(i=0; i<list->size; i++) {
		if(list->arr[i] == item){
			AL_removeAt(list,i);
			return 0;
		}
	}
	return -1;
}

//return the index of the item
int AL_indexOf(ArrayList* list, Type item) {
	int i;
	for(i=0; i<list->size; i++) {
		if(list->arr[i] == item) {
			return i;
		}
	}
	return -1;
}