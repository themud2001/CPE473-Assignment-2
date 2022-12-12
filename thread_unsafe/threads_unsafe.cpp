/*
    Assignment #2: Threading

    Moath Zayadneh - 136559
*/

#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <vector>
#include <algorithm>

using namespace std;

int dimension;
vector<vector<int>> imageArray;
vector<int> rowStartEnd;

void ReadFile(string fileName) {
    ifstream file(fileName);
    file >> dimension;

    if (file.is_open()) {
        for (int i = 0; i < dimension; i++) {
            imageArray.push_back(vector<int>());

            for (int j = 0; j < dimension; j++) {
                int pixel;

                file >> pixel;
                imageArray[i].push_back(pixel);
            }
        }
    } else {
        cout << "Cannot read the file!" << endl;
    }

    file.close();
}

void MedianFilter(int startRow, int endRow) {
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < dimension; j++) {
            vector<int> temp;

            if (i == 0) {
                if (j == 0) {
                    temp.push_back(0);
                    temp.push_back(imageArray[i - 1][j]);
                    temp.push_back(imageArray[i - 1][j + 1]);

                    temp.push_back(0);
                    temp.push_back(imageArray[i][j]);
                    temp.push_back(imageArray[i][j + 1]);

                    temp.push_back(0);
                    temp.push_back(imageArray[i + 1][j]);
                    temp.push_back(imageArray[i + 1][j + 1]);
                } else if (j == imageArray[i].size() - 1) {
                    temp.push_back(imageArray[i - 1][j - 1]);
                    temp.push_back(imageArray[i - 1][j]);
                    temp.push_back(0);

                    temp.push_back(imageArray[i][j - 1]);
                    temp.push_back(imageArray[i][j]);
                    temp.push_back(0);

                    temp.push_back(imageArray[i + 1][j - 1]);
                    temp.push_back(imageArray[i + 1][j]);
                    temp.push_back(0);
                } else {
                    temp.push_back(imageArray[i - 1][j - 1]);
                    temp.push_back(imageArray[i - 1][j]);
                    temp.push_back(imageArray[i - 1][j + 1]);

                    temp.push_back(imageArray[i][j - 1]);
                    temp.push_back(imageArray[i][j]);
                    temp.push_back(imageArray[i][j + 1]);

                    temp.push_back(imageArray[i + 1][j - 1]);
                    temp.push_back(imageArray[i + 1][j]);
                    temp.push_back(imageArray[i + 1][j + 1]);
                }
            }

            sort(temp.begin(), temp.end());

            imageArray[i][j] = temp[4];
        }
    }
}

void* Routine(void* threadID) {
    int64_t id = (int64_t)threadID;
    int startRow = rowStartEnd[id * 2];
    int endRow = rowStartEnd[id * 2 + 1];

    cout << "ThreadID=" << id << ", startRow=" << startRow << ", endRow=" << endRow << endl;
    MedianFilter(startRow, endRow);

    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    int numberOfThreads = stoi(argv[1]);
    pthread_t *threads = new pthread_t[numberOfThreads];

    ReadFile("in.txt");

    int rowsPerThread = dimension / numberOfThreads;
    int remainder = dimension % numberOfThreads;
    int offset = 0;

    for (int64_t i = 0; i < numberOfThreads; i++) {
        /*
            TODO: Implement it where the number of rows are less than the number of threads
        */

        int startRow = i * rowsPerThread + offset;
        int endRow = startRow + rowsPerThread;

        if (remainder) {
            offset++;
            endRow++;
            remainder--;
        }

        rowStartEnd.push_back(startRow);
        rowStartEnd.push_back(endRow);

        int returnCode = pthread_create(&threads[i], NULL, Routine, (void*)i);

        if (returnCode) {
            cout << "Error creating thread number: " << i << endl;
        }
    }

    for (int i = 0; i < numberOfThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}