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
vector<vector<int>> cloneArray;
vector<int> rowStartEnd;

int numberOfBright = 0, numberOfDark = 0, numberOfNormal = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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

void WriteFile(string fileName) {
    ofstream file(fileName);

    if (file.is_open()) {
        file << "The output image is" << endl;

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                file << cloneArray[i][j] << " ";
            }

            file << endl;
        }
    } else {
        cout << "Cannot write to the file!" << endl;
    }

    file.close();
}

void MedianFilter(int startRow, int endRow, int &bright, int &dark, int &normal) {
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < dimension; j++) {
            vector<int> temp;

            if (dimension == 1) {
                temp.push_back(0);
                temp.push_back(0);
                temp.push_back(0);

                temp.push_back(0);
                temp.push_back(imageArray[i][j]);
                temp.push_back(0);

                temp.push_back(0);
                temp.push_back(0);
                temp.push_back(0);
            } else if (i == 0) {
                if (j == 0) {
                    temp.push_back(0);
                    temp.push_back(0);
                    temp.push_back(0);

                    temp.push_back(0);
                    temp.push_back(imageArray[i][j]);
                    temp.push_back(imageArray[i][j + 1]);

                    temp.push_back(0);
                    temp.push_back(imageArray[i + 1][j]);
                    temp.push_back(imageArray[i + 1][j + 1]);
                } else if (j == imageArray[i].size() - 1) {
                    temp.push_back(0);
                    temp.push_back(0);
                    temp.push_back(0);

                    temp.push_back(imageArray[i][j - 1]);
                    temp.push_back(imageArray[i][j]);
                    temp.push_back(0);

                    temp.push_back(imageArray[i + 1][j - 1]);
                    temp.push_back(imageArray[i + 1][j]);
                    temp.push_back(0);
                } else {
                    temp.push_back(0);
                    temp.push_back(0);
                    temp.push_back(0);

                    temp.push_back(imageArray[i][j - 1]);
                    temp.push_back(imageArray[i][j]);
                    temp.push_back(imageArray[i][j + 1]);

                    temp.push_back(imageArray[i + 1][j - 1]);
                    temp.push_back(imageArray[i + 1][j]);
                    temp.push_back(imageArray[i + 1][j + 1]);
                }
            } else if (i == imageArray.size() - 1) {
                if (j == 0) {
                    temp.push_back(0);
                    temp.push_back(imageArray[i - 1][j]);
                    temp.push_back(imageArray[i - 1][j + 1]);

                    temp.push_back(0);
                    temp.push_back(imageArray[i][j]);
                    temp.push_back(imageArray[i][j + 1]);

                    temp.push_back(0);
                    temp.push_back(0);
                    temp.push_back(0);
                } else if (j == imageArray[i].size() - 1) {
                    temp.push_back(imageArray[i - 1][j - 1]);
                    temp.push_back(imageArray[i - 1][j]);
                    temp.push_back(0);

                    temp.push_back(imageArray[i][j - 1]);
                    temp.push_back(imageArray[i][j]);
                    temp.push_back(0);

                    temp.push_back(0);
                    temp.push_back(0);
                    temp.push_back(0);
                } else {
                    temp.push_back(imageArray[i - 1][j - 1]);
                    temp.push_back(imageArray[i - 1][j]);
                    temp.push_back(imageArray[i - 1][j + 1]);

                    temp.push_back(imageArray[i][j - 1]);
                    temp.push_back(imageArray[i][j]);
                    temp.push_back(imageArray[i][j + 1]);

                    temp.push_back(0);
                    temp.push_back(0);
                    temp.push_back(0);
                }
            } else {
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
            cloneArray[i][j] = temp[4];

            if (cloneArray[i][j] > 200) {
                bright++;
            } else if (cloneArray[i][j] < 50) {
                dark++;
            } else {
                normal++;
            }
        }
    }
}

void* Routine(void* threadID) {
    int64_t id = (int64_t)threadID;
    int startRow = rowStartEnd[id * 2];
    int endRow = rowStartEnd[id * 2 + 1];
    int bright = 0, dark = 0, normal = 0;

    cout << "ThreadID=" << id << ", startRow=" << startRow << ", endRow=" << endRow << endl;
    MedianFilter(startRow, endRow, bright, dark, normal);
    cout << "ThreadID=" << id << ", numOfBright=" << bright << ", numOfDark=" << dark << ", numOfNormal=" << normal << endl;

    pthread_mutex_lock(&mutex);
    numberOfBright += bright;
    numberOfDark += dark;
    numberOfNormal += normal;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    int numberOfThreads = stoi(argv[1]);
    pthread_t *threads = new pthread_t[numberOfThreads];

    ReadFile("in.txt");
    cloneArray.resize(dimension, vector<int>());
    copy(imageArray.begin(), imageArray.end(), cloneArray.begin());

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

    cout << "Main: numOfBright=" << numberOfBright << ", numOfDark=" << numberOfDark << ", numOfNormal=" << numberOfNormal << endl;
    WriteFile("out.txt");

    return 0;
}