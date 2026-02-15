#include "fileManager.h"

// Optimized Singly Linked List Implementation with Tail Pointer
SinglyLinkedList::SinglyLinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

void SinglyLinkedList::push_back(const Transaction& trans) {
    ListNode* newNode = new ListNode(trans);

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

void SinglyLinkedList::clear() {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

int SinglyLinkedList::getSize() const {
    return size;
}

ListNode* SinglyLinkedList::getHead() const {
    return head;
}

void SinglyLinkedList::setHead(ListNode* newHead) {
    head = newHead;
    if (head == nullptr) {
        tail = nullptr;
    } else {
        tail = head;
        while (tail->next != nullptr) {
            tail = tail->next;
        }
    }
}

// ChannelListArray Implementation
ChannelListArray::ChannelListArray() {
    maxChannels = 20;
    channelCount = 0;
    channels = new SinglyLinkedList[maxChannels];
    channelNames = new string[maxChannels];
}

ChannelListArray::~ChannelListArray() {
    delete[] channels;
    delete[] channelNames;
}

void ChannelListArray::addChannel(const string& name) {
    if (channelCount < maxChannels) {
        channelNames[channelCount] = name;
        channelCount++;
    }
}

int ChannelListArray::findChannel(const string& name) const {
    for (int i = 0; i < channelCount; i++) {
        if (channelNames[i] == name) {
            return i;
        }
    }
    return -1;
}

SinglyLinkedList& ChannelListArray::getChannel(int index) {
    return channels[index];
}

const SinglyLinkedList& ChannelListArray::getChannel(int index) const {
    return channels[index];
}

string ChannelListArray::getChannelName(int index) const {
    if (index >= 0 && index < channelCount) {
        return channelNames[index];
    }
    return "";
}

int ChannelListArray::getChannelCount() const {
    return channelCount;
}

// Direct load and separate for Linked Lists
void loadAndSeparateLinkedList(const string& filename, ChannelListArray& channelLists) {
    cout << "\n=== Linked List Load & Split ===" << endl;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header

    int loadedCount = 0;
    int skippedCount = 0;
    auto startTime = high_resolution_clock::now();

    cout << "Processing and separating transactions directly into linked lists..." << endl;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        Transaction trans;
        string field;

        try {
            // More lenient parsing - handle "Null" values properly
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

            // Only skip if transaction_id is missing (less strict condition)
            if (trans.transaction_id.empty() || trans.transaction_id == "Null") {
                skippedCount++;
                continue;
            }

            // Direct separation during loading
            string channel = trans.payment_channel.empty() ? "Unknown" : trans.payment_channel;

            // Find or create channel
            int channelIndex = channelLists.findChannel(channel);
            if (channelIndex == -1) {
                channelLists.addChannel(channel);
                channelIndex = channelLists.getChannelCount() - 1;
            }

            // Add directly to appropriate channel list
            channelLists.getChannel(channelIndex).push_back(trans);
            loadedCount++;

        } catch (...) {
            skippedCount++;
            continue;
        }
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(endTime - startTime);

    file.close();

    cout << "Summarizing..." << endl;
    cout << "\nSuccessfully processed: " << loadedCount << " transactions" << endl;
    if (skippedCount > 0) {
        cout << "Skipped due to errors: " << skippedCount << " transactions" << endl;
    }
    cout << "Payment Channels: " << channelLists.getChannelCount() << "" << endl;
    cout << "Total time: " << duration.count() << " ms" << endl;
    cout << "Performance: " << (loadedCount * 1000) / duration.count() << " transactions/second" << endl;
}
