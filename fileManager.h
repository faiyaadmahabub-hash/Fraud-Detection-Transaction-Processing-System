#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// ============= INLINE UTILITY FUNCTIONS =============
inline int minInt(int a, int b) { return (a < b) ? a : b; }
inline int maxInt(int a, int b) { return (a > b) ? a : b; }
inline double absDouble(double x) { return (x < 0) ? -x : x; }
inline int absInt(int x) { return (x < 0) ? -x : x; }

// ============= DATA STRUCTURES =============

struct Transaction {
    string transaction_id;
    string timestamp;
    string sender_account;
    string receiver_account;
    double amount;
    string transaction_type;
    string merchant_category;
    string location;
    string device_used;
    bool is_fraud;
    string fraud_type;
    double time_since_last_transaction;
    double spending_deviation_score;
    int velocity_score;
    double geo_anomaly_score;
    string payment_channel;
    string ip_address;
    string device_hash;

    Transaction() : amount(0.0), is_fraud(false), time_since_last_transaction(0.0),
                   spending_deviation_score(0.0), velocity_score(0), geo_anomaly_score(0.0) {}
};

struct ListNode {
    Transaction data;
    ListNode* next;

    ListNode(const Transaction& trans) : data(trans), next(nullptr) {}
};

struct PerformanceMetrics {
    double time_taken;
    size_t memory_used;
    string algorithm_name;
    string data_structure;

    PerformanceMetrics() : time_taken(0.0), memory_used(0) {}
};

// ============= CLASSES =============

class DynamicArray {
private:
    Transaction* arr;
    int capacity;
    int size;

public:
    DynamicArray();
    ~DynamicArray();
    void resize();
    void push_back(const Transaction& trans);
    void clear();
    Transaction& operator[](int index);
    const Transaction& operator[](int index) const;
    int getSize() const;
};

class SinglyLinkedList {
private:
    ListNode* head;
    ListNode* tail;
    int size;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();
    void push_back(const Transaction& trans);
    void clear();
    int getSize() const;
    ListNode* getHead() const;
    void setHead(ListNode* newHead);
};

class ChannelArray {
private:
    DynamicArray* channels;
    string* channelNames;
    int channelCount;
    int maxChannels;

public:
    ChannelArray();
    ~ChannelArray();
    void addChannel(const string& name);
    int findChannel(const string& name) const;
    DynamicArray& getChannel(int index);
    const DynamicArray& getChannel(int index) const;
    string getChannelName(int index) const;
    int getChannelCount() const;
};

class ChannelListArray {
private:
    SinglyLinkedList* channels;
    string* channelNames;
    int channelCount;
    int maxChannels;

public:
    ChannelListArray();
    ~ChannelListArray();
    void addChannel(const string& name);
    int findChannel(const string& name) const;
    SinglyLinkedList& getChannel(int index);
    const SinglyLinkedList& getChannel(int index) const;
    string getChannelName(int index) const;
    int getChannelCount() const;
};

// ============= CORE FUNCTIONS =============

// CSV Processing
bool reduceCSV(const string& inputFile, const string& outputFile, int targetRows);
void analyzeData(const string& filename);
void analyzePaymentChannels(const string& filename);

// Data Loading
void loadAndSeparateArray(const string& filename, ChannelArray& channelArrays);
void loadAndSeparateLinkedList(const string& filename, ChannelListArray& channelLists);

// ============= SORTING ALGORITHMS =============

void mergeSortIterative(DynamicArray& arr);
void mergeSortIterativeByTransactionType(DynamicArray& arr);
void mergeSortLLSafe(SinglyLinkedList& list);
void mergeSortLLSafeByTransactionType(SinglyLinkedList& list);
void quickSort(DynamicArray& arr);
void quickSortLL(SinglyLinkedList& list);
void heapSort(DynamicArray& arr);
void heapSortLL(SinglyLinkedList& list);

// ============= SEARCHING ALGORITHMS =============

int binarySearch(const DynamicArray& arr, const string& transactionType);
int binarySearchLL(const SinglyLinkedList& list, const string& transactionType);
int interpolationSearch(const DynamicArray& arr, const string& transactionType);
int interpolationSearchLL(const SinglyLinkedList& list, const string& transactionType);
int exponentialSearch(const DynamicArray& arr, const string& transactionType);
int exponentialSearchLL(const SinglyLinkedList& list, const string& transactionType);

// ============= PERFORMANCE TESTING =============

void runSortingPerformanceTest(ChannelArray& channelArrays, ChannelListArray& channelLists);
void runSearchingPerformanceTest(ChannelArray& channelArrays, ChannelListArray& channelLists, const string& searchTerm);

// ============= STORAGE FUNCTIONS =============

void storeFullReducedDataset();
void storeChannelData(const ChannelArray& channelArrays);
void displayChannelStorageMenu();
void storeSortedDataAfterSorting(const ChannelArray& channelArrays);
bool exportTransactionsToJson(const DynamicArray& transactions, const string& filename);
bool loadFullDataset(const string& filename, DynamicArray& allData);

// ============= DISPLAY FUNCTIONS =============

void displayMenu();
void displayChannelSeparation(const ChannelArray& channelArrays);
void displayChannelSeparationLL(const ChannelListArray& channelLists);
void getValidTransactionTypes(const ChannelArray& channelArrays, string validTypes[], int& validTypeCount);
void displayValidTypes(const string validTypes[], int validTypeCount);
void displayLoadingSeparationPerformance(int arrayTime, int listTime, int totalArrayTransactions, int totalListTransactions);
void displaySearchMenu();
void displaySearchContinuationMenu();
void displaySearchError(const string& searchTerm);
void displaySearchSuccess(const string& searchTerm);
void displayGeneralError(const string& message);
void displaySystemShutdown();
void displayContinuePrompt();

// ============= UTILITY FUNCTIONS =============

// File Operations
string getWindowsPath();
bool fileExists(const string& filename);

// Data Manipulation
void swapTransactions(Transaction& a, Transaction& b);

// Input Validation
int getSafeIntegerInput(int min = 1, int max = 7);
bool isValidSearchTerm(const string& searchTerm, const string validTypes[], int validTypeCount);

// String Processing Utilities
char toLowerChar(char c);
int fastStringCompare(const char* str1, const char* str2);
bool fastLocationCompare(const Transaction& a, const Transaction& b);
bool fastTypeCompare(const Transaction& a, const Transaction& b);

// Memory Calculation Utilities
size_t calculateAlgorithmMemoryOverhead(const string& algorithm, int dataSize, bool isLinkedList);

// Data Structure Conversion Utilities
Transaction* convertListToArray(const SinglyLinkedList& list);
void convertListToArrayAndSort(SinglyLinkedList& list, void (*sortFunction)(Transaction*, int));

// Array Sort Functions for Helper
void mergeSortArrayByLocation(Transaction* arr, int n);
void mergeSortArrayByType(Transaction* arr, int n);
void quickSortArray(Transaction* arr, int n);
void heapSortArray(Transaction* arr, int n);

// JSON Formatting Utilities
void writeJsonString(ofstream& file, const string& str);
void writeJsonTransaction(ofstream& file, const Transaction& trans, bool isLast = false);

// Data Processing with Error Handling
bool imputeCSVData(const string& inputFile, const string& outputFile);
bool processAndImputeData();

#endif
