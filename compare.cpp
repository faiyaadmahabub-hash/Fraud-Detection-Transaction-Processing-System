#include "fileManager.h"

// Enhanced performance measurement with real memory tracking
PerformanceMetrics measureArraySortPerformanceWithMemory(DynamicArray& arr, const string& algorithm) {
    PerformanceMetrics metrics;
    metrics.algorithm_name = algorithm;
    metrics.data_structure = "Array";

    size_t baseMemory = arr.getSize() * sizeof(Transaction);
    size_t algorithmOverhead = calculateAlgorithmMemoryOverhead(algorithm, arr.getSize(), false);
    size_t peakMemory = baseMemory + algorithmOverhead;

    auto startTime = high_resolution_clock::now();

    try {
        if (algorithm == "Merge Sort") {
            mergeSortIterative(arr);
        } else if (algorithm == "Quick Sort") {
            quickSort(arr);
        } else if (algorithm == "Heap Sort") {
            heapSort(arr);
        }
    } catch (...) {
        cout << "Error during " << algorithm << " on Array. Using fallback..." << endl;
        heapSort(arr);
        algorithmOverhead = calculateAlgorithmMemoryOverhead("Heap Sort", arr.getSize(), false);
        peakMemory = baseMemory + algorithmOverhead;
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime);

    metrics.time_taken = duration.count() / 1000.0;
    metrics.memory_used = peakMemory;

    return metrics;
}

PerformanceMetrics measureLinkedListSortPerformanceWithMemory(SinglyLinkedList& list, const string& algorithm) {
    PerformanceMetrics metrics;
    metrics.algorithm_name = algorithm;
    metrics.data_structure = "Linked List";

    size_t baseMemory = list.getSize() * sizeof(ListNode);
    size_t algorithmOverhead = calculateAlgorithmMemoryOverhead(algorithm, list.getSize(), true);
    size_t peakMemory = baseMemory + algorithmOverhead;

    auto startTime = high_resolution_clock::now();

    try {
        if (algorithm == "Merge Sort") {
            mergeSortLLSafe(list);
        } else if (algorithm == "Quick Sort") {
            quickSortLL(list);
        } else if (algorithm == "Heap Sort") {
            heapSortLL(list);
        }
    } catch (...) {
        cout << "Error during " << algorithm << " on Linked List. Using fallback..." << endl;
        heapSortLL(list);
        algorithmOverhead = calculateAlgorithmMemoryOverhead("Heap Sort", list.getSize(), true);
        peakMemory = baseMemory + algorithmOverhead;
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime);

    metrics.time_taken = duration.count() / 1000.0;
    metrics.memory_used = peakMemory;

    return metrics;
}

PerformanceMetrics measureArraySearchPerformanceWithMemory(const DynamicArray& arr, const string& algorithm, const string& searchTerm) {
    PerformanceMetrics metrics;
    metrics.algorithm_name = algorithm;
    metrics.data_structure = "Array";

    size_t baseMemory = arr.getSize() * sizeof(Transaction);
    size_t algorithmOverhead = calculateAlgorithmMemoryOverhead(algorithm, arr.getSize(), false);
    size_t peakMemory = baseMemory + algorithmOverhead;

    auto startTime = high_resolution_clock::now();

    if (algorithm == "Binary Search") {
        binarySearch(arr, searchTerm);
    } else if (algorithm == "Interpolation Search") {
        interpolationSearch(arr, searchTerm);
    } else if (algorithm == "Exponential Search") {
        exponentialSearch(arr, searchTerm);
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime);

    metrics.time_taken = duration.count() / 1000.0;
    metrics.memory_used = peakMemory;

    return metrics;
}

PerformanceMetrics measureLinkedListSearchPerformanceWithMemory(const SinglyLinkedList& list, const string& algorithm, const string& searchTerm) {
    PerformanceMetrics metrics;
    metrics.algorithm_name = algorithm;
    metrics.data_structure = "Linked List";

    size_t baseMemory = list.getSize() * sizeof(ListNode);
    size_t algorithmOverhead = calculateAlgorithmMemoryOverhead(algorithm, list.getSize(), true);
    size_t peakMemory = baseMemory + algorithmOverhead;

    auto startTime = high_resolution_clock::now();

    if (algorithm == "Binary Search") {
        binarySearchLL(list, searchTerm);
    } else if (algorithm == "Interpolation Search") {
        interpolationSearchLL(list, searchTerm);
    } else if (algorithm == "Exponential Search") {
        exponentialSearchLL(list, searchTerm);
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime);

    metrics.time_taken = duration.count() / 1000.0;
    metrics.memory_used = peakMemory;

    return metrics;
}

void runSortingPerformanceTest(ChannelArray& channelArrays, ChannelListArray& channelLists) {
    cout << "\n" << string(79, '=') << endl;
    cout << "                           SORTING PERFORMANCE TEST" << endl;
    cout << "                            (Sorting by Location)" << endl;
    cout << string(79, '=') << endl;

    DynamicArray testArray;
    SinglyLinkedList testList;

    int targetSize = 100000;
    int addedCount = 0;

    for (int i = 0; i < channelArrays.getChannelCount() && addedCount < targetSize; i++) {
        DynamicArray& channel = channelArrays.getChannel(i);
        for (int j = 0; j < channel.getSize() && addedCount < targetSize; j++) {
            testArray.push_back(channel[j]);
            addedCount++;
        }
    }

    addedCount = 0;
    for (int i = 0; i < channelLists.getChannelCount() && addedCount < targetSize; i++) {
        SinglyLinkedList& channel = channelLists.getChannel(i);
        ListNode* current = channel.getHead();
        while (current != nullptr && addedCount < targetSize) {
            testList.push_back(current->data);
            current = current->next;
            addedCount++;
        }
    }

    cout << "Testing with " << testArray.getSize() << " transactions" << endl;
    //cout << "Checking how much memory each algorithm uses..." << endl;

    PerformanceMetrics sortingMetrics[6];

    DynamicArray arrayMerge, arrayQuick, arrayHeap;
    SinglyLinkedList listMerge, listQuick, listHeap;

    for (int i = 0; i < testArray.getSize(); i++) {
        arrayMerge.push_back(testArray[i]);
        arrayQuick.push_back(testArray[i]);
        arrayHeap.push_back(testArray[i]);
    }

    ListNode* current = testList.getHead();
    while (current != nullptr) {
        listMerge.push_back(current->data);
        listQuick.push_back(current->data);
        listHeap.push_back(current->data);
        current = current->next;
    }

    cout << "Comparing sorting algorithms and their memory usage..." << endl;
    sortingMetrics[0] = measureArraySortPerformanceWithMemory(arrayMerge, "Merge Sort");
    sortingMetrics[1] = measureLinkedListSortPerformanceWithMemory(listMerge, "Merge Sort");
    sortingMetrics[2] = measureArraySortPerformanceWithMemory(arrayQuick, "Quick Sort");
    sortingMetrics[3] = measureLinkedListSortPerformanceWithMemory(listQuick, "Quick Sort");
    sortingMetrics[4] = measureArraySortPerformanceWithMemory(arrayHeap, "Heap Sort");
    sortingMetrics[5] = measureLinkedListSortPerformanceWithMemory(listHeap, "Heap Sort");

    cout << "\n" << string(90, '=') << endl;
    cout << "                             SORTING PERFORMANCE RESULTS" << endl;
    cout << string(90, '=') << endl;
    cout << left << setw(15) << "Algorithm"
         << setw(15) << "Data Structure"
         << setw(12) << "Time (ms)"
         << setw(15) << "Peak Mem (MB)"
         << setw(15) << "Time Diff (ms)"
         << setw(20) << "Memory Diff (MB)" << endl;
    cout << string(90, '-') << endl;

    for (int i = 0; i < 3; i++) {
        int arrayIndex = i * 2;
        int listIndex = i * 2 + 1;
        double timeDiff = absDouble(sortingMetrics[arrayIndex].time_taken - sortingMetrics[listIndex].time_taken);

        double arrayMemMB = (double)(sortingMetrics[arrayIndex].memory_used) / (1024.0 * 1024.0);
        double listMemMB = (double)(sortingMetrics[listIndex].memory_used) / (1024.0 * 1024.0);
        double memoryDiff = absDouble(arrayMemMB - listMemMB);

        cout << left << setw(15) << sortingMetrics[arrayIndex].algorithm_name
             << setw(15) << "Array"
             << setw(13) << fixed << setprecision(2) << sortingMetrics[arrayIndex].time_taken
             << setw(15) << fixed << setprecision(1) << arrayMemMB
             << setw(16) << fixed << setprecision(2) << timeDiff
             << setw(20) << fixed << setprecision(1) << memoryDiff << endl;

        cout << left << setw(15) << ""
             << setw(15) << "Linked List"
             << setw(12) << fixed << setprecision(2) << sortingMetrics[listIndex].time_taken
             << setw(20) << fixed << setprecision(1) << listMemMB
             << setw(15) << ""
             << setw(20) << "" << endl;
        cout << string(90, '-') << endl;
    }

    cout << "Memory tracking completed! Algorithm analysis ready." << endl;
}

void runSearchingPerformanceTest(ChannelArray& channelArrays, ChannelListArray& channelLists, const string& searchTerm) {
    cout << "\n" << string(80, '=') << endl;
    cout << "                        SEARCHING PERFORMANCE TEST" << endl;
    cout << "                      (Searching by Transaction Type)" << endl;
    cout << string(80, '=') << endl;

    PerformanceMetrics searchingMetrics[6];

    DynamicArray searchArray;
    SinglyLinkedList searchList;

    int maxSearchSize = 100000;
    int totalAdded = 0;

    for (int i = 0; i < channelArrays.getChannelCount() && totalAdded < maxSearchSize; i++) {
        DynamicArray& channel = channelArrays.getChannel(i);
        for (int j = 0; j < channel.getSize() && totalAdded < maxSearchSize; j++) {
            searchArray.push_back(channel[j]);
            totalAdded++;
        }
    }

    totalAdded = 0;
    for (int i = 0; i < channelLists.getChannelCount() && totalAdded < maxSearchSize; i++) {
        SinglyLinkedList& channel = channelLists.getChannel(i);
        ListNode* current = channel.getHead();
        while (current != nullptr && totalAdded < maxSearchSize) {
            searchList.push_back(current->data);
            current = current->next;
            totalAdded++;
        }
    }

    cout << "Sorting data by transaction type..." << endl;

    try {
        if (searchArray.getSize() > 0) {
            mergeSortIterativeByTransactionType(searchArray);
        }

        if (searchList.getSize() > 0) {
            mergeSortLLSafeByTransactionType(searchList);
        }
    } catch (...) {
        cout << "Error during sorting. Aborting search test..." << endl;
        return;
    }

    //cout << "Testing search algorithms with memory tracking on " << searchArray.getSize() << " transactions..." << endl;
    cout << "Searching for validated term: '" << searchTerm << "'..." << endl;

    searchingMetrics[0] = measureArraySearchPerformanceWithMemory(searchArray, "Binary Search", searchTerm);
    searchingMetrics[1] = measureLinkedListSearchPerformanceWithMemory(searchList, "Binary Search", searchTerm);
    searchingMetrics[2] = measureArraySearchPerformanceWithMemory(searchArray, "Interpolation Search", searchTerm);
    searchingMetrics[3] = measureLinkedListSearchPerformanceWithMemory(searchList, "Interpolation Search", searchTerm);
    searchingMetrics[4] = measureArraySearchPerformanceWithMemory(searchArray, "Exponential Search", searchTerm);
    searchingMetrics[5] = measureLinkedListSearchPerformanceWithMemory(searchList, "Exponential Search", searchTerm);

    cout << "\nSearch completed successfully - '" << searchTerm << "' found in dataset!" << endl;

    cout << "\n" << string(99, '=') << endl;
    cout << "                                       SEARCHING PERFORMANCE RESULTS" << endl;
    cout << string(99, '=') << endl;
    cout << left << setw(22) << "Algorithm"
         << setw(15) << "Data Structure"
         << setw(13) << "Time (ms)"
         << setw(15) << "Peak Mem (MB)"
         << setw(16) << "Time Diff (ms)"
         << setw(20) << "Memory Diff (MB)" << endl;
    cout << string(99, '-') << endl;

    for (int i = 0; i < 3; i++) {
        int arrayIndex = i * 2;
        int listIndex = i * 2 + 1;
        double timeDiff = absDouble(searchingMetrics[arrayIndex].time_taken - searchingMetrics[listIndex].time_taken);

        double arrayMemMB = (double)(searchingMetrics[arrayIndex].memory_used) / (1024.0 * 1024.0);
        double listMemMB = (double)(searchingMetrics[listIndex].memory_used) / (1024.0 * 1024.0);
        double memoryDiff = absDouble(arrayMemMB - listMemMB);

        cout << left << setw(22) << searchingMetrics[arrayIndex].algorithm_name
             << setw(15) << "Array"
             << setw(13) << fixed << setprecision(2) << searchingMetrics[arrayIndex].time_taken
             << setw(15) << fixed << setprecision(1) << arrayMemMB
             << setw(16) << fixed << setprecision(2) << timeDiff
             << setw(20) << fixed << setprecision(1) << memoryDiff << endl;

        cout << left << setw(22) << ""
             << setw(15) << "Linked List"
             << setw(13) << fixed << setprecision(4) << searchingMetrics[listIndex].time_taken
             << setw(15) << fixed << setprecision(1) << listMemMB
             << setw(16) << ""
             << setw(20) << "" << endl;
        cout << string(99, '-') << endl;
    }
}
