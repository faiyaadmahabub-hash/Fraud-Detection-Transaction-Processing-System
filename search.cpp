#include "fileManager.h"

// ============= BINARY SEARCH (ARRAYS) =============
int binarySearch(const DynamicArray& arr, const string& transactionType) {
    int size = arr.getSize();
    if (size == 0) return -1;

    const char* searchStr = transactionType.c_str();
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + ((right - left) >> 1);

        int cmp = fastStringCompare(arr[mid].transaction_type.c_str(), searchStr);

        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// ============= BINARY SEARCH (LINKED LISTS) =============
int binarySearchLL(const SinglyLinkedList& list, const string& transactionType) {
    int size = list.getSize();
    if (size == 0) return -1;

    Transaction* tempArr = convertListToArray(list);
    if (!tempArr) return -1;

    int left = 0;
    int right = size - 1;
    const char* searchStr = transactionType.c_str();
    int result = -1;

    while (left <= right) {
        int mid = left + ((right - left) >> 1);

        int cmp = fastStringCompare(tempArr[mid].transaction_type.c_str(), searchStr);

        if (cmp == 0) {
            result = mid;
            break;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    delete[] tempArr;
    return result;
}

// ============= INTERPOLATION SEARCH (ARRAYS) =============
int interpolationSearch(const DynamicArray& arr, const string& transactionType) {
    int size = arr.getSize();
    if (size == 0) return -1;

    const char* searchStr = transactionType.c_str();
    int low = 0;
    int high = size - 1;

    while (low <= high && high < size) {
        if (low == high) {
            return (fastStringCompare(arr[low].transaction_type.c_str(), searchStr) == 0) ? low : -1;
        }

        char searchChar = (transactionType.empty()) ? 'a' : toLowerChar(transactionType[0]);
        char lowChar = toLowerChar(arr[low].transaction_type.empty() ? 'a' : arr[low].transaction_type[0]);
        char highChar = toLowerChar(arr[high].transaction_type.empty() ? 'z' : arr[high].transaction_type[0]);

        int pos;
        if (highChar == lowChar) {
            pos = low;
        } else {
            long long numerator = (long long)(searchChar - lowChar) * (high - low);
            pos = low + (int)(numerator / (highChar - lowChar));
        }

        pos = maxInt(low, minInt(pos, high));

        int cmp = fastStringCompare(arr[pos].transaction_type.c_str(), searchStr);

        if (cmp == 0) {
            return pos;
        } else if (cmp < 0) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }
    return -1;
}

// ============= INTERPOLATION SEARCH (LINKED LISTS) =============
int interpolationSearchLL(const SinglyLinkedList& list, const string& transactionType) {
    int size = list.getSize();
    if (size == 0) return -1;

    Transaction* tempArr = convertListToArray(list);
    if (!tempArr) return -1;

    const char* searchStr = transactionType.c_str();
    int low = 0;
    int high = size - 1;
    int result = -1;

    while (low <= high && high < size) {
        if (low == high) {
            result = (fastStringCompare(tempArr[low].transaction_type.c_str(), searchStr) == 0) ? low : -1;
            break;
        }

        char searchChar = (transactionType.empty()) ? 'a' : toLowerChar(transactionType[0]);
        char lowChar = toLowerChar(tempArr[low].transaction_type.empty() ? 'a' : tempArr[low].transaction_type[0]);
        char highChar = toLowerChar(tempArr[high].transaction_type.empty() ? 'z' : tempArr[high].transaction_type[0]);

        int pos;
        if (highChar == lowChar) {
            pos = low;
        } else {
            long long numerator = (long long)(searchChar - lowChar) * (high - low);
            pos = low + (int)(numerator / (highChar - lowChar));
        }

        pos = maxInt(low, minInt(pos, high));

        int cmp = fastStringCompare(tempArr[pos].transaction_type.c_str(), searchStr);

        if (cmp == 0) {
            result = pos;
            break;
        } else if (cmp < 0) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }

    delete[] tempArr;
    return result;
}

// ============= EXPONENTIAL SEARCH (ARRAYS) =============
int exponentialSearch(const DynamicArray& arr, const string& transactionType) {
    int size = arr.getSize();
    if (size == 0) return -1;

    const char* searchStr = transactionType.c_str();

    if (fastStringCompare(arr[0].transaction_type.c_str(), searchStr) == 0) {
        return 0;
    }

    int bound = 1;
    while (bound < size && fastStringCompare(arr[bound].transaction_type.c_str(), searchStr) < 0) {
        bound <<= 1;
    }

    int left = bound >> 1;
    int right = minInt(bound, size - 1);

    while (left <= right) {
        int mid = left + ((right - left) >> 1);

        int cmp = fastStringCompare(arr[mid].transaction_type.c_str(), searchStr);

        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// ============= EXPONENTIAL SEARCH (LINKED LISTS) =============
int exponentialSearchLL(const SinglyLinkedList& list, const string& transactionType) {
    int size = list.getSize();
    if (size == 0) return -1;

    Transaction* tempArr = convertListToArray(list);
    if (!tempArr) return -1;

    const char* searchStr = transactionType.c_str();

    if (fastStringCompare(tempArr[0].transaction_type.c_str(), searchStr) == 0) {
        delete[] tempArr;
        return 0;
    }

    int bound = 1;
    while (bound < size && fastStringCompare(tempArr[bound].transaction_type.c_str(), searchStr) < 0) {
        bound <<= 1;
    }

    int left = bound >> 1;
    int right = minInt(bound, size - 1);
    int result = -1;

    while (left <= right) {
        int mid = left + ((right - left) >> 1);

        int cmp = fastStringCompare(tempArr[mid].transaction_type.c_str(), searchStr);

        if (cmp == 0) {
            result = mid;
            break;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    delete[] tempArr;
    return result;
}
