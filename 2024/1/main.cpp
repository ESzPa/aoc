#include <iostream>
#include <fstream>
#include <initializer_list>
#include <string>

class List{
private:
    int* array;
    int max;
    int size;

    void swap(int* val1, int* val2){
        int tmp = *val1;
        *val1 = *val2;
        *val2 = tmp;
    }

    int partition(int arr[], int low, int high){
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[high]);  
        return i + 1;
    }

    void quick(int arr[], int low, int high){
        if(low < high){
            int pi = partition(arr, low, high);
            quick(arr, low, pi-1);
            quick(arr, pi+1, high);
        }
    }

public:
    List(std::initializer_list<int> arr = {}, int max = 25) : max(max), size(0) {
        if(arr.size() > max) { exit(1); }
        this->array = (int*)malloc(this->max * sizeof(int));
        for(int e : arr){
            this->array[size++] = e;
        }
    }

    ~List(){
        free(this->array);
    }

    void append(int val){
        if(this->size >= this->max){
            this->max += 25;
            this->array = (int*)realloc(this->array, this->max * sizeof(int));
        }
        this->array[size++] = val;
    }

    int getsize(){
        return this->size;
    }

    void quicksort(){
        quick(this->array, 0, size-1);
    }

    int& operator[](int index){
        if(index < 0 || index > this->size){ exit(1); }
        return this->array[index];
    }

    const int& operator[](int index) const{
        if(index < 0 || index > this->size){ exit(1); }
        return this->array[index];
    }

    int* begin() {
        return this->array;
    }

    int* end() {
        return this->array + this->size;
    }
};

int main(void){
    List list1;
    List list2;
    std::ifstream file("input.txt");
    int val1, val2;
    while(file >> val1 >> val2){
        list1.append(val1);
        list2.append(val2);
    }
    file.close();

    list1.quicksort();
    list2.quicksort();

    int total = 0;
    for(int i = 0; i < list1.getsize(); i++){
        int diff = list1[i] - list2[i];
        total += diff < 0 ? -diff : diff;
    }

    std::cout << total << "\n";

    return 0;
}