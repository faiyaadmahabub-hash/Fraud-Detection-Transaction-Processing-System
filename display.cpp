#include "fileManager.h"

// ============= MAIN MENU DISPLAYS =============

void displayMenu() {
    cout << "\n" << string(66, '=') << endl;
    cout << "                        WELCOME TO KODE" << endl;
    cout << "     Financial Data Management System & Performance Comparison" << endl;
    cout << string(66, '=') << endl;
    cout << "1. Load and Process Financial Data" << endl;
    cout << "2. Load Data & Separate by Payment Channel" << endl;
    cout << "3. Sort Transactions by Location" << endl;
    cout << "4. Search for Transaction Type" << endl;
    cout << "5. Store Full Reduced Dataset" << endl;
    cout << "6. Store Data by Payment Channel" << endl;
    cout << "7. Exit" << endl;
    cout << string(66, '=') << endl;
    cout << "Enter choice (1-7): ";
}
// ============= DATA STRUCTURE DISPLAYS =============

void displayChannelSeparation(const ChannelArray& channelArrays) {
    cout << "\n=== PAYMENT CHANNEL SEPARATION (ARRAY) ===" << endl;

    for (int i = 0; i < channelArrays.getChannelCount(); i++) {
        cout << channelArrays.getChannelName(i) << ": " << channelArrays.getChannel(i).getSize() << " transactions" << endl;
    }
}

void displayChannelSeparationLL(const ChannelListArray& channelLists) {
    cout << "\n=== PAYMENT CHANNEL SEPARATION (LINKED LIST) ===" << endl;

    for (int i = 0; i < channelLists.getChannelCount(); i++) {
        cout << channelLists.getChannelName(i) << ": " << channelLists.getChannel(i).getSize() << " transactions" << endl;
    }
}

// ============= PERFORMANCE DISPLAYS =============

void displayLoadingSeparationPerformance(int arrayTime, int listTime, int totalArrayTransactions, int totalListTransactions) {
    cout << "\n" << string(66, '=') << endl;
    cout << "              LOAD + SEPARATION PERFORMANCE COMPARISON" << endl;
    cout << string(66, '=') << endl;
    cout << left << setw(20) << "Data Structure"
         << setw(15) << "Time (ms)"
         << setw(15) << "Transactions"
         << setw(15) << "Speed (trans/s)" << endl;
    cout << string(66, '-') << endl;

    cout << left << setw(20) << "Dynamic Array"
         << setw(15) << arrayTime
         << setw(15) << totalArrayTransactions
         << setw(15) << (arrayTime > 0 ? (totalArrayTransactions * 1000) / arrayTime : 0) << endl;

    cout << left << setw(20) << "Linked List"
         << setw(15) << listTime
         << setw(15) << totalListTransactions
         << setw(15) << (listTime > 0 ? (totalListTransactions * 1000) / listTime : 0) << endl;

    cout << string(66, '-') << endl;
    cout << "DIFFERENCES:" << endl;
    cout << "Time difference: " << absInt(arrayTime - listTime) << " ms" << endl;
    cout << "Speed difference: " << absInt((totalArrayTransactions * 1000) / arrayTime - (totalListTransactions * 1000) / listTime) << " trans/s" << endl;
}

// ============= SEARCH INTERFACE DISPLAYS =============

void displaySearchMenu() {
    cout << "\n" << string(50, '=') << endl;
    cout << " SEARCH FOR TRANSACTION TYPE" << endl;
    cout << string(50, '=') << endl;
}

void displaySearchContinuationMenu() {
    cout << "\n" << string(50, '-') << endl;
    cout << "What would you like to do next?" << endl;
    cout << string(50, '-') << endl;
    cout << "1. Search for another transaction type" << endl;
    cout << "2. Return to main menu" << endl;
    cout << string(50, '-') << endl;
    cout << "Enter choice (1-2): ";
}

// ============= TRANSACTION TYPE DISPLAYS =============

void getValidTransactionTypes(const ChannelArray& channelArrays, string validTypes[], int& validTypeCount) {
    validTypeCount = 0;

    for (int i = 0; i < channelArrays.getChannelCount(); i++) {
        const DynamicArray& channel = channelArrays.getChannel(i);
        for (int j = 0; j < channel.getSize() && j < 100; j++) {
            string currentType = channel[j].transaction_type;

            // Check if already exists
            bool exists = false;
            for (int k = 0; k < validTypeCount; k++) {
                if (validTypes[k] == currentType) {
                    exists = true;
                    break;
                }
            }

            if (!exists && validTypeCount < 50) {
                validTypes[validTypeCount] = currentType;
                validTypeCount++;
            }
        }
    }
}

void displayValidTypes(const string validTypes[], int validTypeCount) {
    cout << "\nAvailable Transaction Types:" << endl;
    cout << "" << string(45, '-') << endl;

    for (int i = 0; i < validTypeCount; i++) {
        cout << " " << left << setw(2) << (i + 1) << ". "
             << setw(10) << validTypes[i];
        if ((i + 1) % 3 == 0) cout << endl;
    }
    if (validTypeCount % 3 != 0) cout << endl;

    cout << "" << string(45, '-') << endl;
    cout << "Total: " << validTypeCount << " types available" << endl;
}

// ============= MESSAGE DISPLAYS =============

void displaySearchError(const string& searchTerm) {
    cout << "\n" << string(45, '-') << endl;
    cout << "[Error] '" << searchTerm << "' is not valid" << endl;
    cout << "Please choose from the list above" << endl;
    cout << string(45, '-') << endl;
    cout << "Press Enter to try again...";
}

void displaySearchSuccess(const string& searchTerm) {
    cout << "\n[Success] Valid search term: '" << searchTerm << "'" << endl;
}

void displayGeneralError(const string& message) {
    cout << "\n[Error] " << message << endl;
    cout << "Press Enter to try again...";
}

void displaySystemShutdown() {
    cout << "\n" << string(40, '-') << endl;
    cout << "System shutdown. Thank you!" << endl;
    cout << string(40, '-') << endl;
}

void displayContinuePrompt() {
    cout << "\nPress Enter to continue...";
}
