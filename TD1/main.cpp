#include "summit.hpp"
#include <iostream>
#include <vector>

int generateRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

vector<Summit> generateRandomSummits(int count) {
    vector<Summit> summits;
    for (int i = 0; i < count; ++i) {
        Summit summit;
        summit.setNumber(i + 1);
        summits.push_back(summit);
    }
    return summits;
}

vector<Summit> generateRandomLinksBetweenSummits(const vector<Summit>& summits) {
    vector<Summit> linkedSummits = summits;

    for (size_t summitIndex = 1; summitIndex < linkedSummits.size(); ++summitIndex) {
        int parentIndex = generateRandomNumber(0, static_cast<int>(summitIndex) - 1);

        vector<Summit> parentLinks = linkedSummits[parentIndex].getSummits();
        parentLinks.push_back(linkedSummits[summitIndex]);
        linkedSummits[parentIndex].setSummits(parentLinks);

        vector<Summit> summitLinks = linkedSummits[summitIndex].getSummits();
        summitLinks.push_back(linkedSummits[parentIndex]);
        linkedSummits[summitIndex].setSummits(summitLinks);
    }

    return linkedSummits;
}

// void displaySummitsLinkedToASummit(const Summit& summit) {
//     cout << "Summit Number: " << summit.getNumber() << endl;
//     cout << "Linked Summits: ";
//     for (const Summit& linkedSummit : summit.getSummits()) {
//         cout << linkedSummit.getNumber() << " ";
//     }
//     cout << endl;
// }

void displayWeightedTree(const vector<Summit>& summits) {
    cout << "Arbre pondere :" << endl;

    for (const Summit& summit : summits) {
        cout << "Sommet " << summit.getNumber() << " -> ";

        for (const Summit& linkedSummit : summit.getSummits()) {
            cout << linkedSummit.getNumber() << " ";
        }

        cout << endl;
    }
}


int main() {
    int randomNumber = generateRandomNumber(5, 10);
    vector<Summit> randomSummits = generateRandomSummits(randomNumber);
    randomSummits = generateRandomLinksBetweenSummits(randomSummits);

    displayWeightedTree(randomSummits);

    return 0;
}