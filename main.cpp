#include "fileManager.h"

int main() {
    ChannelArray channelArrays;
    ChannelListArray channelLists;

    bool dataProcessed = false;
    bool dataLoaded = false;

    DynamicArray lastSearchResults;
    string lastSearchTerm = "";
    bool hasSearchResults = false;

    int choice;

    while (true) {
        try {
            displayMenu();
            choice = getSafeIntegerInput(1, 7);

            bool showContinuePrompt = true;

            switch (choice) {
            case 1: {
                dataProcessed = processAndImputeData();
                if (dataProcessed) {
                    dataLoaded = false;
                    hasSearchResults = false;
                }
                break;
            }

            case 2: {
                if (!dataProcessed) {
                    displayGeneralError("Please process data first (Option 1).");
                    cin.get();
                    showContinuePrompt = false;
                    break;
                }

                cout << "\n=== LOAD DATA AND SEPARATE BY PAYMENT CHANNEL ===" << endl;

                try {
                    string imputedPath = "imputed_dataset.csv";
                    if (!fileExists(imputedPath)) {
                        displayGeneralError("Processed dataset not found. Run Option 1 first.");
                        cin.get();
                        showContinuePrompt = false;
                        break;
                    }

                    analyzePaymentChannels(imputedPath);

                    auto arrayStartTime = high_resolution_clock::now();
                    loadAndSeparateArray(imputedPath, channelArrays);
                    auto arrayEndTime = high_resolution_clock::now();
                    auto arrayTime = duration_cast<milliseconds>(arrayEndTime - arrayStartTime);

                    auto listStartTime = high_resolution_clock::now();
                    loadAndSeparateLinkedList(imputedPath, channelLists);
                    auto listEndTime = high_resolution_clock::now();
                    auto listTime = duration_cast<milliseconds>(listEndTime - listStartTime);

                    int totalArrayTransactions = 0;
                    for (int i = 0; i < channelArrays.getChannelCount(); i++) {
                        totalArrayTransactions += channelArrays.getChannel(i).getSize();
                    }

                    int totalListTransactions = 0;
                    for (int i = 0; i < channelLists.getChannelCount(); i++) {
                        totalListTransactions += channelLists.getChannel(i).getSize();
                    }

                    dataLoaded = true;
                    hasSearchResults = false;

                    displayLoadingSeparationPerformance(arrayTime.count(), listTime.count(), totalArrayTransactions, totalListTransactions);
                    displayChannelSeparation(channelArrays);
                    displayChannelSeparationLL(channelLists);

                } catch (const exception& e) {
                    displayGeneralError(e.what());
                    cin.get();
                    showContinuePrompt = false;
                }
                break;
            }

            case 3: {
                if (!dataLoaded) {
                    displayGeneralError("Please load data first (Option 2).");
                    cin.get();
                    showContinuePrompt = false;
                    break;
                }

                try {
                    runSortingPerformanceTest(channelArrays, channelLists);

                    // Auto-store sorted data after sorting
                    cout << "\nAutomatically storing sorted data..." << endl;
                    storeSortedDataAfterSorting(channelArrays);

                } catch (const exception& e) {
                    displayGeneralError(e.what());
                    cin.get();
                    showContinuePrompt = false;
                }
                break;
            }

            case 4: {
                if (!dataLoaded) {
                    displayGeneralError("Please load data first (Option 2).");
                    cin.get();
                    showContinuePrompt = false;
                    break;
                }

                string validTypes[50];
                int validTypeCount = 0;
                getValidTransactionTypes(channelArrays, validTypes, validTypeCount);

                if (validTypeCount == 0) {
                    displayGeneralError("No transaction types found in the dataset.");
                    cin.get();
                    showContinuePrompt = false;
                    break;
                }

                bool continueSearching = true;

                while (continueSearching) {
                    string searchTerm;
                    bool validInput = false;
                    bool firstAttempt = true;

                    while (!validInput) {
                        if (firstAttempt) {
                            displaySearchMenu();
                            firstAttempt = false;
                        }
                        displayValidTypes(validTypes, validTypeCount);

                        cout << "\n Enter transaction type: ";
                        getline(cin, searchTerm);

                        if (searchTerm.empty()) {
                            displayGeneralError("Please enter a valid search term.");
                            cin.get();
                            continue;
                        }

                        if (isValidSearchTerm(searchTerm, validTypes, validTypeCount)) {
                            validInput = true;
                            displaySearchSuccess(searchTerm);
                        } else {
                            displaySearchError(searchTerm);
                            cin.get();
                        }
                    }

                    cout << "\n Starting search performance test..." << endl;

                    try {
                        runSearchingPerformanceTest(channelArrays, channelLists, searchTerm);

                        lastSearchResults.clear();
                        lastSearchTerm = searchTerm;

                        for (int i = 0; i < channelArrays.getChannelCount(); i++) {
                            const DynamicArray& channel = channelArrays.getChannel(i);
                            for (int j = 0; j < channel.getSize(); j++) {
                                if (channel[j].transaction_type == searchTerm) {
                                    lastSearchResults.push_back(channel[j]);
                                }
                            }
                        }

                        hasSearchResults = (lastSearchResults.getSize() > 0);

                        if (hasSearchResults) {
                            cout << "\nFound " << lastSearchResults.getSize() << " matching transactions for '" << searchTerm << "'" << endl;
                        }

                    } catch (const exception& e) {
                        displayGeneralError(e.what());
                        cin.get();
                        showContinuePrompt = false;
                    }

                    displaySearchContinuationMenu();
                    int nextChoice = getSafeIntegerInput(1, 2);

                    if (nextChoice == 2) {
                        continueSearching = false;
                    }
                }
                break;
            }

            case 5: {
                if (!dataProcessed) {
                    displayGeneralError("Please process data first (Option 1).");
                    cin.get();
                    showContinuePrompt = false;
                    break;
                }

                try {
                    storeFullReducedDataset();
                } catch (const exception& e) {
                    displayGeneralError(e.what());
                    cin.get();
                    showContinuePrompt = false;
                }
                break;
            }

            case 6: {
                if (!dataLoaded) {
                    displayGeneralError("Please load data first (Option 2).");
                    cin.get();
                    showContinuePrompt = false;
                    break;
                }

                try {
                    storeChannelData(channelArrays);
                } catch (const exception& e) {
                    displayGeneralError(e.what());
                    cin.get();
                    showContinuePrompt = false;
                }
                break;
            }

            case 7: {
                displaySystemShutdown();
                return 0;
            }
            }

            if (showContinuePrompt) {
                displayContinuePrompt();
                cin.ignore();
                cin.get();
            }

        } catch (const exception& e) {
            displayGeneralError("Unexpected error: " + string(e.what()));
            cin.get();
        }
    }

    return 0;
}
