#include "fileManager.h"

bool reduceCSV(const string& inputFile, const string& outputFile, int targetRows)
{
    if (targetRows <= 0)
    {
        cout << "Error: Target rows must be greater than 0." << endl;
        return false;
    }

    if (targetRows > 10000000)
    {
        cout << "Error: Target rows too large (max: 10,000,000)." << endl;
        return false;
    }

    // First pass: Count valid lines
    ifstream countFile(inputFile);
    if (!countFile.is_open())
    {
        cout << "Error: Cannot open input file: " << inputFile << endl;
        return false;
    }

    string line;
    int totalValidLines = 0;

    // Skip header
    if (getline(countFile, line)) {
        // Header found
    } else {
        cout << "Error: Input file is empty." << endl;
        countFile.close();
        return false;
    }

    // Count valid data lines
    while (getline(countFile, line))
    {
        if (!line.empty() && line.find(',') != string::npos) // Basic validation
        {
            totalValidLines++;
        }
    }
    countFile.close();

    cout << "Original dataset size: " << totalValidLines << " valid data lines" << endl;

    if (totalValidLines == 0)
    {
        cout << "Error: No valid data lines found." << endl;
        return false;
    }

    //Comment just for now:
    //cout << "Reducing dataset to exactly " << targetRows << " rows..." << endl;

    // Second pass: Write exactly targetRows
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile.is_open() || !outFile.is_open())
    {
        cout << "Error: Cannot open files for processing." << endl;
        return false;
    }

    try
    {
        // Copy header
        if (getline(inFile, line))
        {
            outFile << line << endl;
        }

        int actualTarget = (totalValidLines < targetRows) ? totalValidLines : targetRows;
        int writtenLines = 0;

        if (totalValidLines <= targetRows)
        {
            // Copy all valid lines
            while (getline(inFile, line) && writtenLines < totalValidLines)
            {
                if (!line.empty() && line.find(',') != string::npos)
                {
                    outFile << line << endl;
                    writtenLines++;
                }
            }
        }
        else
        {
            // Calculate sampling interval
            double interval = (double)totalValidLines / actualTarget;
            double nextTargetPosition = 0.0;
            int validLineCount = 0;

            while (getline(inFile, line) && writtenLines < actualTarget)
            {
                if (!line.empty() && line.find(',') != string::npos)
                {
                    if (validLineCount >= (int)nextTargetPosition)
                    {
                        outFile << line << endl;
                        writtenLines++;
                        nextTargetPosition += interval;
                    }
                    validLineCount++;
                }
            }
        }

        inFile.close();
        outFile.close();

        cout << "Data reduction completed successfully." << endl;

        return true;

    }
    catch (const exception& e)
    {
        cout << "Error during reduction: " << e.what() << endl;
        inFile.close();
        outFile.close();
        return false;
    }
}
