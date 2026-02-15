#include "fileManager.h"

// ============= FILE OPERATIONS =============

string getWindowsPath() {
    string path;
    cout << "Enter the full Windows path to your CSV file: ";
    cout << "(Example: C:\\Users\\YourName\\Documents\\data.csv)" << endl;

    try {
        getline(cin, path);

        // Remove quotes if present and validate
        if (path.length() >= 2 && path.front() == '"' && path.back() == '"') {
            path = path.substr(1, path.length() - 2);
        }

        // Basic validation
        if (path.empty()) {
            cout << "Warning: Empty path provided." << endl;
        }

    } catch (...) {
        cout << "Error reading path input." << endl;
        path = "";
    }

    return path;
}

bool fileExists(const string& filename) {
    ifstream file(filename);
    bool exists = file.good();
    file.close();
    return exists;
}

// ============= DATA MANIPULATION =============

void swapTransactions(Transaction& a, Transaction& b) {
    Transaction temp = a;
    a = b;
    b = temp;
}

// ============= INPUT VALIDATION =============

int getSafeIntegerInput(int min, int max) {
    int value;
    while (true) {
        if (cin >> value) {
            if (value >= min && value <= max) {
                cin.ignore();
                return value;
            } else {
                cout << "Please enter a number between " << min << " and " << max << ": ";
            }
        } else {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

bool isValidSearchTerm(const string& searchTerm, const string validTypes[], int validTypeCount) {
    for (int i = 0; i < validTypeCount; i++) {
        if (validTypes[i] == searchTerm) {
            return true;
        }
    }
    return false;
}

// ============= STRING PROCESSING UTILITIES =============

// Inline character conversion
char toLowerChar(char c) {
    return (c >= 'A' && c <= 'Z') ? c + 32 : c;
}

// Fast string comparison for searching
int fastStringCompare(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        char c1 = (*str1 >= 'A' && *str1 <= 'Z') ? *str1 + 32 : *str1;
        char c2 = (*str2 >= 'A' && *str2 <= 'Z') ? *str2 + 32 : *str2;
        if (c1 != c2) return c1 - c2;
        str1++; str2++;
    }
    return (*str1) - (*str2);
}

bool fastLocationCompare(const Transaction& a, const Transaction& b) {
    const char* strA = a.location.c_str();
    const char* strB = b.location.c_str();

    while (*strA && *strB) {
        char charA = (*strA >= 'A' && *strA <= 'Z') ? *strA + 32 : *strA;
        char charB = (*strB >= 'A' && *strB <= 'Z') ? *strB + 32 : *strB;

        if (charA != charB) {
            return charA < charB;
        }
        strA++;
        strB++;
    }
    return *strA < *strB;
}

bool fastTypeCompare(const Transaction& a, const Transaction& b) {
    const char* strA = a.transaction_type.c_str();
    const char* strB = b.transaction_type.c_str();

    while (*strA && *strB) {
        char charA = (*strA >= 'A' && *strA <= 'Z') ? *strA + 32 : *strA;
        char charB = (*strB >= 'A' && *strB <= 'Z') ? *strB + 32 : *strB;

        if (charA != charB) {
            return charA < charB;
        }
        strA++;
        strB++;
    }
    return *strA < *strB;
}

// ============= MEMORY CALCULATION UTILITIES =============

size_t calculateAlgorithmMemoryOverhead(const string& algorithm, int dataSize, bool isLinkedList) {
    size_t overhead = 0;

    if (algorithm == "Merge Sort") {
        if (isLinkedList) {
            overhead = dataSize * sizeof(Transaction) * 2;
        } else {
            overhead = dataSize * sizeof(Transaction);
        }
    }
    else if (algorithm == "Quick Sort") {
        if (isLinkedList) {
            overhead = dataSize * sizeof(Transaction) + (dataSize * sizeof(int) * 2);
        } else {
            overhead = dataSize * sizeof(int) * 2;
        }
    }
    else if (algorithm == "Heap Sort") {
        if (isLinkedList) {
            overhead = dataSize * sizeof(Transaction);
        } else {
            overhead = 100;
        }
    }
    else if (algorithm == "Binary Search") {
        if (isLinkedList) {
            overhead = dataSize * sizeof(Transaction);
        } else {
            overhead = 50;
        }
    }
    else if (algorithm == "Interpolation Search") {
        if (isLinkedList) {
            overhead = dataSize * sizeof(Transaction);
        } else {
            overhead = 100;
        }
    }
    else if (algorithm == "Exponential Search") {
        if (isLinkedList) {
            overhead = dataSize * sizeof(Transaction);
        } else {
            overhead = 100;
        }
    }

    return overhead;
}

// ============= DATA STRUCTURE CONVERSION UTILITIES =============

// Helper function to convert linked list to array (eliminates redundant code)
Transaction* convertListToArray(const SinglyLinkedList& list) {
    int size = list.getSize();
    if (size == 0) return nullptr;

    Transaction* tempArr = new Transaction[size];
    ListNode* current = list.getHead();
    for (int i = 0; i < size && current != nullptr; i++) {
        tempArr[i] = current->data;
        current = current->next;
    }
    return tempArr;
}

// Helper function to convert linked list to array and back (eliminates redundant code)
void convertListToArrayAndSort(SinglyLinkedList& list, void (*sortFunction)(Transaction*, int)) {
    int n = list.getSize();
    if (n <= 1) return;

    Transaction* arr = new Transaction[n];
    ListNode* current = list.getHead();
    for (int i = 0; i < n; i++) {
        arr[i] = current->data;
        current = current->next;
    }

    sortFunction(arr, n);

    list.clear();
    for (int i = 0; i < n; i++) list.push_back(arr[i]);
    delete[] arr;
}

// ============= ARRAY SORT FUNCTIONS FOR HELPER =============

void mergeSortArrayByLocation(Transaction* arr, int n) {
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

void mergeSortArrayByType(Transaction* arr, int n) {
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

void quickSortArray(Transaction* arr, int n) {
    if (n <= 1) return;

    int* stack = new int[n * 2];
    int top = -1;

    stack[++top] = 0;
    stack[++top] = n - 1;

    while (top >= 0) {
        int high = stack[top--];
        int low = stack[top--];

        if (low < high) {
            // Partition
            Transaction pivot = arr[high];
            int i = low - 1;

            for (int j = low; j < high; j++) {
                if (fastLocationCompare(arr[j], pivot)) {
                    i++;
                    swapTransactions(arr[i], arr[j]);
                }
            }
            swapTransactions(arr[i + 1], arr[high]);
            int pi = i + 1;

            // Push left and right subarrays
            if (pi - 1 > low) {
                stack[++top] = low;
                stack[++top] = pi - 1;
            }
            if (pi + 1 < high) {
                stack[++top] = pi + 1;
                stack[++top] = high;
            }
        }
    }

    delete[] stack;
}

void heapSortArray(Transaction* arr, int n) {
    if (n <= 1) return;

    // Build heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        int parent = i;
        while (true) {
            int largest = parent;
            int left = 2 * parent + 1;
            int right = 2 * parent + 2;

            if (left < n && !fastLocationCompare(arr[left], arr[largest])) largest = left;
            if (right < n && !fastLocationCompare(arr[right], arr[largest])) largest = right;

            if (largest == parent) break;

            swapTransactions(arr[parent], arr[largest]);
            parent = largest;
        }
    }

    // Extract elements
    for (int i = n - 1; i > 0; i--) {
        swapTransactions(arr[0], arr[i]);

        int parent = 0;
        while (true) {
            int largest = parent;
            int left = 2 * parent + 1;
            int right = 2 * parent + 2;

            if (left < i && !fastLocationCompare(arr[left], arr[largest])) largest = left;
            if (right < i && !fastLocationCompare(arr[right], arr[largest])) largest = right;

            if (largest == parent) break;

            swapTransactions(arr[parent], arr[largest]);
            parent = largest;
        }
    }
}

// ============= JSON FORMATTING UTILITIES =============

void writeJsonString(ofstream& file, const string& str) {
    file << "\"";
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        if (c == '"') file << "\\\"";
        else if (c == '\\') file << "\\\\";
        else if (c == '\n') file << "\\n";
        else if (c == '\r') file << "\\r";
        else if (c == '\t') file << "\\t";
        else file << c;
    }
    file << "\"";
}

void writeJsonTransaction(ofstream& file, const Transaction& trans, bool isLast) {
    file << "    {\n";
    file << "      \"transaction_id\": "; writeJsonString(file, trans.transaction_id); file << ",\n";
    file << "      \"timestamp\": "; writeJsonString(file, trans.timestamp); file << ",\n";
    file << "      \"sender_account\": "; writeJsonString(file, trans.sender_account); file << ",\n";
    file << "      \"receiver_account\": "; writeJsonString(file, trans.receiver_account); file << ",\n";
    file << "      \"amount\": " << fixed << setprecision(2) << trans.amount << ",\n";
    file << "      \"transaction_type\": "; writeJsonString(file, trans.transaction_type); file << ",\n";
    file << "      \"merchant_category\": "; writeJsonString(file, trans.merchant_category); file << ",\n";
    file << "      \"location\": "; writeJsonString(file, trans.location); file << ",\n";
    file << "      \"device_used\": "; writeJsonString(file, trans.device_used); file << ",\n";
    file << "      \"is_fraud\": " << (trans.is_fraud ? "true" : "false") << ",\n";
    file << "      \"fraud_type\": "; writeJsonString(file, trans.fraud_type); file << ",\n";
    file << "      \"time_since_last_transaction\": " << trans.time_since_last_transaction << ",\n";
    file << "      \"spending_deviation_score\": " << trans.spending_deviation_score << ",\n";
    file << "      \"velocity_score\": " << trans.velocity_score << ",\n";
    file << "      \"geo_anomaly_score\": " << trans.geo_anomaly_score << ",\n";
    file << "      \"payment_channel\": "; writeJsonString(file, trans.payment_channel); file << ",\n";
    file << "      \"ip_address\": "; writeJsonString(file, trans.ip_address); file << ",\n";
    file << "      \"device_hash\": "; writeJsonString(file, trans.device_hash); file << "\n";
    file << "    }" << (isLast ? "\n" : ",\n");
}

// ============= DATA PROCESSING WITH ERROR HANDLING =============

bool imputeCSVData(const string& inputFile, const string& outputFile) {
    cout << "\nProcessing missing values..." << endl;

    ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        cout << "Error: Cannot open input file." << endl;
        return false;
    }

    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cout << "Error: Cannot create output file." << endl;
        inFile.close();
        return false;
    }

    string line;
    int totalRows = 0;
    int imputedFields = 0;

    // Copy header first
    if (getline(inFile, line)) {
        outFile << line << endl;
    }

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        // Manual CSV parsing to handle empty fields properly
        string processedLine = "";
        bool inQuotes = false;
        string currentField = "";
        int fieldCount = 0;

        for (size_t i = 0; i <= line.length(); i++) {
            char c = (i < line.length()) ? line[i] : ','; // Treat end of line as comma

            if (c == '"' && i < line.length()) {
                inQuotes = !inQuotes;
                currentField += c;
            } else if (c == ',' && !inQuotes) {
                // Process the current field
                if (fieldCount > 0) processedLine += ",";

                // Check if field is empty or contains only whitespace
                bool isEmpty = currentField.empty();
                if (!isEmpty) {
                    // Check if field contains only whitespace
                    bool onlyWhitespace = true;
                    for (size_t j = 0; j < currentField.length(); j++) {
                        if (currentField[j] != ' ' && currentField[j] != '\t' && currentField[j] != '\r' && currentField[j] != '\n') {
                            onlyWhitespace = false;
                            break;
                        }
                    }
                    isEmpty = onlyWhitespace;
                }

                if (isEmpty || currentField == "NULL" || currentField == "null" || currentField == "NA" || currentField == "n/a") {
                    processedLine += "Null";
                    imputedFields++;
                } else {
                    processedLine += currentField;
                }

                currentField = "";
                fieldCount++;
            } else if (i < line.length()) {
                currentField += c;
            }
        }

        outFile << processedLine << endl;
        totalRows++;
    }

    inFile.close();
    outFile.close();

    cout << "Processing completed: " << totalRows << " rows, " << imputedFields << " fields imputed" << endl;
    cout << "Dataset ready for analysis." << endl;

    return true;
}

bool processAndImputeData() {
    cout << "\n=== LOAD AND PROCESS FINANCIAL DATA ===" << endl;
    cout << "Enter CSV file path: ";

    string originalPath = getWindowsPath();

    if (!fileExists(originalPath)) {
        cout << "Error: File not found." << endl;
        return false;
    }

    try {
        cout << "Reducing to 100,000 rows..." << endl;

        string reducedPath = "reduced_dataset.csv";
        if (!reduceCSV(originalPath, reducedPath, 100000)) {
            cout << "Error: Failed to reduce dataset." << endl;
            return false;
        }

        analyzeData(reducedPath);

        string imputedPath = "imputed_dataset.csv";
        if (!imputeCSVData(reducedPath, imputedPath)) {
            cout << "Error: Failed to process data." << endl;
            return false;
        }

        cout << "Data processing completed successfully." << endl;
        return true;

    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return false;
    }
}
