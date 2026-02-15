#include "fileManager.h"

void storeFullReducedDataset() {
    cout << "\n=== STORING FULL REDUCED DATASET ===" << endl;

    string imputedPath = "imputed_dataset.csv";
    if (!fileExists(imputedPath)) {
        displayGeneralError("Imputed dataset not found. Run Option 1 first.");
        return;
    }

    DynamicArray allData;
    cout << "Loading full reduced dataset (100,000 records)..." << endl;

    if (!loadFullDataset(imputedPath, allData)) {
        displayGeneralError("Failed to load dataset.");
        return;
    }

    cout << "Loaded " << allData.getSize() << " transactions." << endl;
    cout << "Storing to JSON..." << endl;

    if (exportTransactionsToJson(allData, "full_reduced_dataset.json")) {
        cout << "\n[SUCCESS] Full reduced dataset stored!" << endl;
        cout << "File: full_reduced_dataset.json" << endl;
        cout << "Records: " << allData.getSize() << endl;
    } else {
        displayGeneralError("Failed to store dataset.");
    }
}

void storeSortedDataAfterSorting(const ChannelArray& channelArrays) {
    cout << "\n=== STORING SORTED DATA ===" << endl;

    DynamicArray allSortedData;

    // Combine all channel data
    for (int i = 0; i < channelArrays.getChannelCount(); i++) {
        const DynamicArray& channel = channelArrays.getChannel(i);
        for (int j = 0; j < channel.getSize(); j++) {
            allSortedData.push_back(channel[j]);
        }
    }

    if (allSortedData.getSize() == 0) {
        displayGeneralError("No data available to store.");
        return;
    }

    cout << "Sorting " << allSortedData.getSize() << " transactions by location..." << endl;
    mergeSortIterative(allSortedData);

    cout << "Storing sorted data to JSON..." << endl;

    if (exportTransactionsToJson(allSortedData, "sorted_by_location.json")) {
        cout << "\n[SUCCESS] Sorted data stored!" << endl;
        cout << "File: sorted_by_location.json" << endl;
        cout << "Records: " << allSortedData.getSize() << endl;
        cout << "Sorted by: Location (Ascending)" << endl;
    } else {
        displayGeneralError("Failed to store sorted data.");
    }
}

void storeChannelData(const ChannelArray& channelArrays) {
    cout << "\n" << string(50, '=') << endl;
    cout << "      STORE DATA BY PAYMENT CHANNEL" << endl;
    cout << string(50, '=') << endl;

    bool continueStoring = true;

    while (continueStoring) {
        displayChannelStorageMenu();
        int channelChoice = getSafeIntegerInput(1, 5);

        string targetChannel;
        string outputFilename;

        switch (channelChoice) {
            case 1:
                targetChannel = "card";
                outputFilename = "card_transactions.json";
                break;
            case 2:
                targetChannel = "wire_transfer";
                outputFilename = "wire_transfer_transactions.json";
                break;
            case 3:
                targetChannel = "UPI";
                outputFilename = "upi_transactions.json";
                break;
            case 4:
                targetChannel = "ACH";
                outputFilename = "ach_transactions.json";
                break;
            case 5:
                cout << "Returning to main menu..." << endl;
                return;
            default:
                displayGeneralError("Invalid selection.");
                continue;
        }

        // Find the channel data
        int channelIndex = channelArrays.findChannel(targetChannel);
        if (channelIndex == -1) {
            cout << "No data found for channel: " << targetChannel << endl;
            continue;
        }

        const DynamicArray& channelData = channelArrays.getChannel(channelIndex);
        cout << "\nStoring " << channelData.getSize() << " " << targetChannel << " transactions..." << endl;

        if (exportTransactionsToJson(channelData, outputFilename)) {
            cout << "\n[SUCCESS] Channel data stored!" << endl;
            cout << "File: " << outputFilename << endl;
            cout << "Records: " << channelData.getSize() << endl;
            cout << "Channel: " << targetChannel << endl;
        } else {
            displayGeneralError("Failed to store channel data.");
        }

        cout << "\nStore another channel? (1=Yes, 2=No): ";
        int nextChoice = getSafeIntegerInput(1, 2);

        if (nextChoice == 2) {
            continueStoring = false;
        }
    }
}

void displayChannelStorageMenu() {
    cout << "1. Card Transactions" << endl;
    cout << "2. Wire Transfer Transactions" << endl;
    cout << "3. UPI Transactions" << endl;
    cout << "4. ACH Transactions" << endl;
    cout << "5. Return to Main Menu" << endl;
    cout << string(50, '=') << endl;
    cout << "Enter choice (1-5): ";
}

bool loadFullDataset(const string& filename, DynamicArray& allData) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file: " << filename << endl;
        return false;
    }

    string line;
    getline(file, line); // Skip header

    int loadedCount = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        Transaction trans;
        string field;

        try {
            getline(ss, trans.transaction_id, ',');
            getline(ss, trans.timestamp, ',');
            getline(ss, trans.sender_account, ',');
            getline(ss, trans.receiver_account, ',');

            getline(ss, field, ',');
            if (field.empty() || field == "Null" || field == "NULL") {
                trans.amount = 0.0;
            } else {
                try {
                    trans.amount = stod(field);
                } catch (...) {
                    trans.amount = 0.0;
                }
            }

            getline(ss, trans.transaction_type, ',');
            if (trans.transaction_type == "Null" || trans.transaction_type == "NULL") {
                trans.transaction_type = "Unknown";
            }

            getline(ss, trans.merchant_category, ',');
            if (trans.merchant_category == "Null" || trans.merchant_category == "NULL") {
                trans.merchant_category = "Unknown";
            }

            getline(ss, trans.location, ',');
            if (trans.location == "Null" || trans.location == "NULL") {
                trans.location = "Unknown";
            }

            getline(ss, trans.device_used, ',');
            if (trans.device_used == "Null" || trans.device_used == "NULL") {
                trans.device_used = "Unknown";
            }

            getline(ss, field, ',');
            if (field == "Null" || field == "NULL" || field.empty()) {
                trans.is_fraud = false;
            } else {
                trans.is_fraud = (field == "1" || field == "true" || field == "TRUE");
            }

            getline(ss, trans.fraud_type, ',');
            if (trans.fraud_type == "Null" || trans.fraud_type == "NULL") {
                trans.fraud_type = "";
            }

            getline(ss, field, ',');
            if (field.empty() || field == "Null" || field == "NULL") {
                trans.time_since_last_transaction = 0.0;
            } else {
                try {
                    trans.time_since_last_transaction = stod(field);
                } catch (...) {
                    trans.time_since_last_transaction = 0.0;
                }
            }

            getline(ss, field, ',');
            if (field.empty() || field == "Null" || field == "NULL") {
                trans.spending_deviation_score = 0.0;
            } else {
                try {
                    trans.spending_deviation_score = stod(field);
                } catch (...) {
                    trans.spending_deviation_score = 0.0;
                }
            }

            getline(ss, field, ',');
            if (field.empty() || field == "Null" || field == "NULL") {
                trans.velocity_score = 0;
            } else {
                try {
                    trans.velocity_score = stoi(field);
                } catch (...) {
                    trans.velocity_score = 0;
                }
            }

            getline(ss, field, ',');
            if (field.empty() || field == "Null" || field == "NULL") {
                trans.geo_anomaly_score = 0.0;
            } else {
                try {
                    trans.geo_anomaly_score = stod(field);
                } catch (...) {
                    trans.geo_anomaly_score = 0.0;
                }
            }

            getline(ss, trans.payment_channel, ',');
            if (trans.payment_channel == "Null" || trans.payment_channel == "NULL") {
                trans.payment_channel = "Unknown";
            }

            getline(ss, trans.ip_address, ',');
            if (trans.ip_address == "Null" || trans.ip_address == "NULL") {
                trans.ip_address = "";
            }

            getline(ss, trans.device_hash);
            if (trans.device_hash == "Null" || trans.device_hash == "NULL") {
                trans.device_hash = "";
            }

            allData.push_back(trans);
            loadedCount++;

        } catch (...) {
            continue;
        }
    }

    file.close();
    return true;
}

bool exportTransactionsToJson(const DynamicArray& transactions, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot create JSON file: " << filename << endl;
        return false;
    }

    file << "[\n";

    for (int i = 0; i < transactions.getSize(); i++) {
        writeJsonTransaction(file, transactions[i], (i == transactions.getSize() - 1));
    }

    file << "]\n";

    file.close();
    return true;
}
