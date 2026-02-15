#include "fileManager.h"

void analyzeData(const string& filename)
{
    cout << "\n=== Data Analysis Process ===" << endl;
    cout << "Analyzing dataset for missing values..." << endl;

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Cannot open file: " << filename << endl;
        return;
    }

    string line;
    int missingCounts[18] = {0};
    string columnNames[18] =
    {
        "transaction_id", "timestamp", "sender_account", "receiver_account",
        "amount", "transaction_type", "merchant_category", "location",
        "device_used", "is_fraud", "fraud_type", "time_since_last_transaction",
        "spending_deviation_score", "velocity_score", "geo_anomaly_score",
        "payment_channel", "ip_address", "device_hash"
    };

    int totalRows = 0;

    // Skip header
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string field;
        int fieldIndex = 0;

        while (getline(ss, field, ',') && fieldIndex < 18)
        {
            if (field.empty() || field == " " || field == "NULL" || field == "null")
            {
                missingCounts[fieldIndex]++;
            }
            fieldIndex++;
        }
        totalRows++;
    }

    file.close();

    cout << "\n=== Missing Value Analysis Results ===" << endl;
    cout << "Total rows analyzed: " << totalRows << endl;
    cout << setw(25) << "Column Name" << setw(15) << "Missing Count" << setw(15) << "Percentage" << endl;
    cout << string(55, '-') << endl;

    bool hasMissingValues = false;
    for (int i = 0; i < 18; i++)
    {
        double percentage = (double)missingCounts[i] / totalRows * 100;
        cout << setw(25) << columnNames[i]
             << setw(15) << missingCounts[i]
             << setw(14) << fixed << setprecision(2) << percentage << "%" << endl;

        if (missingCounts[i] > 0)
        {
            hasMissingValues = true;
        }
    }

    if (!hasMissingValues)
    {
        cout << "\nNo missing values found in the dataset." << endl;
    }
    else
    {
        cout << "\nMissing values detected. Proceeding with imputation..." << endl;
    }
}

void analyzePaymentChannels(const string& filename) {
    cout << "Analysing transaction payment channels..." << endl;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header

    // Array to store unique channel names (no STL containers)
    const int MAX_CHANNELS = 50;
    string uniqueChannels[MAX_CHANNELS];
    int channelCounts[MAX_CHANNELS];
    int uniqueChannelCount = 0;
    int totalTransactions = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        // Extract payment channel field (field 16, index 15)
        stringstream ss(line);
        string field;
        string paymentChannel = "";

        // Skip to payment channel field
        for (int i = 0; i <= 15; i++) {
            if (getline(ss, field, ',')) {
                if (i == 15) { // payment_channel field
                    paymentChannel = field;
                    if (paymentChannel == "Null" || paymentChannel == "NULL" || paymentChannel.empty()) {
                        paymentChannel = "Unknown";
                    }
                    break;
                }
            }
        }

        if (paymentChannel.empty()) {
            paymentChannel = "Unknown";
        }

        // Check if channel already exists
        bool found = false;
        for (int i = 0; i < uniqueChannelCount; i++) {
            if (uniqueChannels[i] == paymentChannel) {
                channelCounts[i]++;
                found = true;
                break;
            }
        }

        // Add new channel if not found
        if (!found && uniqueChannelCount < MAX_CHANNELS) {
            uniqueChannels[uniqueChannelCount] = paymentChannel;
            channelCounts[uniqueChannelCount] = 1;
            uniqueChannelCount++;
        }

        totalTransactions++;
    }

    file.close();

    cout << "\n=== Payment Channel Analysis Results ===" << endl;
    cout << "Total transactions analyzed: " << totalTransactions << endl;
    cout << "Unique payment channels found: " << uniqueChannelCount << endl;
    cout << "\nChannel Distribution:" << endl;
    cout << string(50, '-') << endl;

    for (int i = 0; i < uniqueChannelCount; i++) {
        double percentage = (double)channelCounts[i] / totalTransactions * 100;
        cout << left << setw(20) << uniqueChannels[i]
             << setw(10) << channelCounts[i]
             << setw(10) << fixed << setprecision(1) << percentage << "%" << endl;
    }
    cout << string(50, '-') << endl;
}
