#include "fileManager.h"

// ============= MERGE SORT (ARRAYS) =============
void mergeSortIterative(DynamicArray& arr) {
    int n = arr.getSize();
    if (n <= 1) return;

    Transaction* temp = new Transaction[n];
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - 1; left += 2 * size) {
            int mid = minInt(left + size - 1, n - 1);
            int right = minInt(left + 2 * size - 1, n - 1);

            if (mid < right) {
                int i = left, j = mid + 1, k = left;

                for (int x = left; x <= mid; x++) temp[x] = arr[x];

                while (i <= mid && j <= right) {
                    if (fastLocationCompare(temp[i], arr[j])) {
                        arr[k++] = temp[i++];
                    } else {
                        arr[k++] = arr[j++];
                    }
                }
                while (i <= mid) arr[k++] = temp[i++];
            }
        }
    }
    delete[] temp;
}

void mergeSortIterativeByTransactionType(DynamicArray& arr) {
    int n = arr.getSize();
    if (n <= 1) return;

    Transaction* temp = new Transaction[n];
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - 1; left += 2 * size) {
            int mid = minInt(left + size - 1, n - 1);
            int right = minInt(left + 2 * size - 1, n - 1);

            if (mid < right) {
                int i = left, j = mid + 1, k = left;

                for (int x = left; x <= mid; x++) temp[x] = arr[x];

                while (i <= mid && j <= right) {
                    if (fastTypeCompare(temp[i], arr[j])) {
                        arr[k++] = temp[i++];
                    } else {
                        arr[k++] = arr[j++];
                    }
                }
                while (i <= mid) arr[k++] = temp[i++];
            }
        }
    }
    delete[] temp;
}

// ============= MERGE SORT (LINKED LISTS) =============
void mergeSortLLSafe(SinglyLinkedList& list) {
    convertListToArrayAndSort(list, mergeSortArrayByLocation);
}

void mergeSortLLSafeByTransactionType(SinglyLinkedList& list) {
    convertListToArrayAndSort(list, mergeSortArrayByType);
}

// ============= QUICK SORT (ARRAYS) =============
static int partition(DynamicArray& arr, int low, int high) {
    Transaction pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (fastLocationCompare(arr[j], pivot)) {
            i++;
            swapTransactions(arr[i], arr[j]);
        }
    }
    swapTransactions(arr[i + 1], arr[high]);
    return i + 1;
}

static void quickSortHelper(DynamicArray& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

void quickSort(DynamicArray& arr) {
    if (arr.getSize() > 1) {
        quickSortHelper(arr, 0, arr.getSize() - 1);
    }
}

// ============= QUICK SORT (LINKED LISTS) =============
void quickSortLL(SinglyLinkedList& list) {
    convertListToArrayAndSort(list, quickSortArray);
}

// ============= HEAP SORT (ARRAYS) =============
static void heapify(DynamicArray& arr, int n, int i) {
    while (true) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && !fastLocationCompare(arr[left], arr[largest])) largest = left;
        if (right < n && !fastLocationCompare(arr[right], arr[largest])) largest = right;

        if (largest == i) break;

        swapTransactions(arr[i], arr[largest]);
        i = largest;
    }
}

void heapSort(DynamicArray& arr) {
    int n = arr.getSize();
    if (n <= 1) return;

    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swapTransactions(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ============= HEAP SORT (LINKED LISTS) =============
void heapSortLL(SinglyLinkedList& list) {
    convertListToArrayAndSort(list, heapSortArray);
}
