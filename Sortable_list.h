#pragma once
#include "List.h"
#include <cmath>

#ifdef STATISTICS
unsigned long long compares=0;
unsigned long long moves=0;
#define COMPARE ++compares
#define MOVE ++moves
#define INIT compares=0;moves=0
#define SUM cout<<compares<<" "<<moves<<endl;
#else
#define COMPARE 0 // Satisfy comma operator
#define MOVE
#define INIT
#define SUM
#endif

template <class Record>
class Sortable_list:public List<Record>{
public:
	// Sortable_list(std::initializer_list<Record> il):List(il);
	using List<Record>::List;
	using List<Record>::print;
	void insertion_sort();
private:
	using List<Record>::count;

#ifdef CONTIGUOUS
public:
	void selection_sort();
  void quick_sort();
  void heap_sort();
  // void print_heap()const;
private:
  void recursive_quick_sort(int low, int high); // quick_sort
  int partition(int low, int high); // quick_sort
  void insert_heap(const Record &current, int low, int high); // heap_sort
  void build_heap();
	int max_key(int,int);
	void swap(int,int);
	using List<Record>::entry;
#endif

#ifdef LINKED
public:
	void merge_sort();
private:
	using List<Record>::head;
#endif

};

#ifdef CONTIGUOUS
template <class Record>
void Sortable_list<Record>::insertion_sort(){
	INIT;
	if(count<=1)
		return;
	for(int first_unsorted=1;first_unsorted!=count;++first_unsorted){
		if( COMPARE,entry[first_unsorted-1]>entry[first_unsorted] ){
			Record current=entry[first_unsorted];
			MOVE;
			entry[first_unsorted]=entry[first_unsorted-1]; // First shift
			MOVE;
			int position=first_unsorted-1;
			while( position>=1 && (COMPARE,entry[position-1]>current) ){
				entry[position]=entry[position-1]; // 2nd+ shift
				MOVE;
				--position;
			}
			entry[position]=current;
			MOVE;
		}
	}
}
#endif

#ifdef LINKED
template <class Record>
void Sortable_list<Record>::insertion_sort(){
	if(count<=1)
		return;
	Node<Record> *last_sorted=head;
	Node<Record> *first_unsorted=head->next;
	do{
		if(first_unsorted->entry < head->entry){ 
			last_sorted->next=first_unsorted->next;
			first_unsorted->next=head;
			head=first_unsorted;
			first_unsorted=last_sorted->next;
		}else{
			Node<Record> *trailing=head, *current=trailing->next;
			// Travse sorted part
			while(current!=first_unsorted && current->entry < first_unsorted->entry){
				current=current->next;
				trailing=trailing->next;
			}
			if(current==first_unsorted){
				last_sorted = first_unsorted;
				first_unsorted = first_unsorted->next;
			}else{
				last_sorted->next = first_unsorted->next;
				trailing->next = first_unsorted;
				first_unsorted->next = current;
				first_unsorted=last_sorted->next;
			}
		}
	}while(first_unsorted!=nullptr); // Traverse unsorted part
}
#endif

#ifdef CONTIGUOUS
template <class Record>
void Sortable_list<Record>::selection_sort(){
	INIT;
	for(int position=count-1;position>0;--position){
		int max=max_key(0,position);
		swap(max,position);
	}
}
template <class Record>
int Sortable_list<Record>::max_key(int low,int high){
	int largest=low;
	for(int i=low+1;i<=high;++i){
		COMPARE;
		if(entry[i]>entry[largest])
			largest=i;
	}
	return largest;
}
template <class Record>
void Sortable_list<Record>::swap(int low,int high){
	MOVE;
	Record tmp=entry[low];
	MOVE;
	entry[low]=entry[high];
	MOVE;
	entry[high]=tmp;
}
#endif

#ifdef LINKED
template <class Record>
Node<Record> *divide_from(Node<Record> *sub_list)
{
	Node<Record> *x2=sub_list;
	Node<Record> *x1=sub_list;
	Node<Record> *bak=nullptr;
	while(x2&&x2->next){
		bak=x1;
		x1=x1->next;
		x2=x2->next->next;
	}
	// Odd amount of elements
	if(x2){
		bak=x1->next;
		x1->next=nullptr;
		return bak;
	}
	// Even amount of elements or no element
	if(bak)
		bak->next=nullptr;
	return x1;
}
template <class Record>
/// @return Head of the combined list
Node<Record> *merge(Node<Record> *first,Node<Record> *second){
	Node<Record> *combined=nullptr;
	Node<Record> **last_sorted=&combined; // points to the last node of sorted list
	while(true){
		#define F {\
			*last_sorted=first;\
			last_sorted=&((*last_sorted)->next);\
			first=first->next;\
		}
		#define S {\
			*last_sorted=second;\
			last_sorted=&((*last_sorted)->next);\
			second=second->next;\
		}
		if(first){
			if(second)
				if(first->entry<=second->entry)
					F
				else
					S
			else
				F
		}else if(second){
			S
		}else{
			break;
		}
	}
	return combined;
}
template <class Record>
void recursive_merge_sort(Node<Record> **sub_list){
	if(*sub_list && (*sub_list)->next){
		Node<Record> *second_half = divide_from(*sub_list);
		recursive_merge_sort(sub_list);
		recursive_merge_sort(&second_half);
		*sub_list = merge(*sub_list, second_half);
	}
}
template <class Record>
void Sortable_list<Record>::merge_sort(){
	recursive_merge_sort(&head);
}
#endif

#ifdef CONTIGUOUS
template<class Record>
void Sortable_list<Record>::quick_sort(){
  recursive_quick_sort(0, count - 1);
}
template<class Record>
void Sortable_list<Record>::recursive_quick_sort(int low, int high){
  int pivot_position;
  if (low < high) {
   // Otherwise, no sorting is needed.
  pivot_position = partition(low, high);
  recursive_quick_sort(low, pivot_position - 1);
  recursive_quick_sort(pivot_position + 1, high);
  }
}
template<class Record>
int Sortable_list<Record>::partition(int low, int high){
  Record pivot;
  swap(low, (low + high)/2);
  pivot = entry[low]; // First entry is now pivot.

  int last_small=low;
  for(int i=low+1;i<=high;++i){
    if(entry[i]>=pivot)
      continue;
    ++last_small;
    swap(last_small,i);
  }
  swap(low,last_small);
  return last_small;
}
#endif

#ifdef CONTIGUOUS
template<class Record>
void Sortable_list<Record>::heap_sort(){
  Record current; // temporary storage for moving entries
  int last_unsorted; // Entries beyond last_unsorted have been sorted.
  build_heap(); // First phase: Turn the list into a heap.
  for(last_unsorted = count - 1; last_unsorted > 0; last_unsorted -- ) {
    current = entry[last_unsorted]; // Extract the last entry from the list.
    entry[last_unsorted] = entry[0]; // Move top of heap to the end
    insert_heap(current, 0, last_unsorted - 1); // Restore the heap
  }
}
template<class Record>
void Sortable_list<Record>::insert_heap(const Record &current, int low, int high){
  int p=low;
  while(true){
    int L=2*p+1;
    int R=2*p+2;
    #define PROMOTE_L {entry[p]=entry[L];p=L;}
    #define PROMOTE_R {entry[p]=entry[R];p=R;}
    #define PROMOTE_C {entry[p]=current;break;}
    if(L>high){ // p: leaf
      PROMOTE_C
    }else{
      if(R>high){ // p: left child only
        if(current>entry[L])
          PROMOTE_C
        else
          PROMOTE_L
      }else{ // p: 2 children
        if(entry[L]>entry[R]){
          if(current>entry[L])
            PROMOTE_C
          else
            PROMOTE_L
        }else{
          if(current>entry[R])
            PROMOTE_C
          else
            PROMOTE_R
        }
      }
    }
  }
}
template<class Record>
void Sortable_list<Record>::build_heap(){
  int low; // All entries beyond the position low form a heap.
  for (low = count/2 - 1; low >= 0; low -- ) {
    Record current = entry[low];
    insert_heap(current, low, count - 1);
  }
}
// template<class Record>
// void Sortable_list<Record>::print_heap()const{
//   int index=0;
//   int i=0;
//   while(true){
//     for(int j=1; j<=pow(2,i); ++j){
//       cout<<entry[index]<<" ";
//       ++index;
//       if(index>count-1)
//         goto end;
//     }
//     cout<<endl;
//     ++i;
//   }
//   end: return;
// }
#endif