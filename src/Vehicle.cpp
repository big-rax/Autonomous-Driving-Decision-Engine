#include "Vehicle.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <limits>
#include <vector>
#include <queue>

using namespace std;

Vehicle::Vehicle() {
    end_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
}

std::string Vehicle::getq4() {
    std::string additionalObstacles = "";
    if (weather == "rain") {
        additionalObstacles = ", pothole, fallen tree";
    }
    return "What type of obstacle is it? (animal, pedestrian, stopped car, cone" + additionalObstacles + ") : ";
}

std::string Vehicle::getq5(const std::string& obstacleType) {
    return "Did the " + obstacleType + " appear suddenly? (yes/no) : ";
}

// ---------------- Input validation helpers ----------------

bool Vehicle::validateInput(const std::string& decision) {
    if (decision.empty()) {
        cout << "Error: Cannot be empty - Please enter a valid response.\n";
        return false;
    }
    if (!(alphaCheck(decision))) {
        cout << "Error: Non-Alphabetical characters not accepted - Please enter a valid response.\n";
        return false;
    }
    return true;
}

bool Vehicle::alphaCheck(const std::string& decision) {
    for (char d : decision) {
        if (!isalpha(static_cast<unsigned char>(d)) && !isspace(static_cast<unsigned char>(d))) {
            return false;
        }
    }
    return true;
}

void Vehicle::adjustInput(std::string& decision, const bool& eraseWhitespace) {
    std::string adjustedDecision = decision;

    if (eraseWhitespace) {
        for (size_t i = 0; i < adjustedDecision.length(); i++) {
            if (adjustedDecision[i] == ' ') {
                adjustedDecision.erase(i, 1);
                i--;
            }
        }
    }
    for (size_t i = 0; i < adjustedDecision.length(); i++) {
        adjustedDecision[i] = static_cast<char>(tolower(static_cast<unsigned char>(adjustedDecision[i])));
    }
    decision = adjustedDecision;
}

// ---------------- High-level input w/ validation ----------------

std::string Vehicle::getDecision(const std::string& question, const bool& eraseWhitespace) {
    std::string decision;
    bool validInput;
    do {
        validInput = true;
        cout << question;
        getline(cin, decision);
        if (!(validateInput(decision))) {
            validInput = false;
        }
    } while (!validInput);
    adjustInput(decision, eraseWhitespace);
    return decision;
}

std::string Vehicle::getValidChoice(const std::string& question, const std::vector<std::string>& validOptions, const bool& eraseWhitespace) {
    std::string decision;
    bool isValid = false;

    do {
        decision = getDecision(question, eraseWhitespace);
        for (auto& choice : validOptions) {
            if (decision == choice) {
                isValid = true;
                break;
            }
        }
        if (!isValid) {
            cout << "Error: Invalid option - Please enter : ";
            for (size_t i = 0; i < validOptions.size(); i++) {
                cout << "\"" << validOptions[i] << "\"";
                if (i < validOptions.size() - 1) {
                    cout << ", or ";
                }
            }
            cout << endl;
        }
    } while (!isValid);

    return decision;
}

// ---------------- Specific prompts ----------------

std::string Vehicle::returnDecision(const std::string& question) {
    std::vector<std::string> validOptions = { "yes", "no" };
    std::string decision = getValidChoice(question, validOptions, true);
    cout << endl;
    return decision;
}

std::string Vehicle::returnChoice1(const std::string& question) {
    std::vector<std::string> validOptions = { "highway", "city street", "residential street", "country road" };
    std::string decision = getValidChoice(question, validOptions, false);
    cout << endl;
    return decision;
}

std::string Vehicle::returnChoice2(const std::string& question) {
    std::vector<std::string> validOptions = { "clear", "rain", "fog", "snow" };
    std::string decision = getValidChoice(question, validOptions, true);
    cout << endl;
    return decision;
}

std::string Vehicle::returnChoice3(const std::string& question) {
    std::vector<std::string> validOptions = { "animal", "pedestrian", "stopped car", "cone" };
    if (weather == "rain") {
        validOptions.insert(validOptions.end(), { "pothole", "fallen tree" });
    }
    std::string decision = getValidChoice(question, validOptions, false);
    cout << endl;
    return decision;
}

// ---------------- Simulator main loop ----------------

void Vehicle::simulator() {
    cout << setw(19) << " " << "Autonomous Vehicle Simulator\n";
    cout << string(65, '-') << endl;

    do {
        roadType = returnChoice1(questions.q1);
        weather = returnChoice2(questions.q2);

        obstacleAhead = returnDecision(questions.q3);
        if (obstacleAhead == "yes") {
            obstacleType = returnChoice3(getq4());
            obstacleSuddenly = returnDecision(getq5(obstacleType));
        } else {
            obstacleType = "empty";
            obstacleSuddenly = "empty";
        }

        if (roadType != "highway") {
            if (obstacleType == "pedestrian") {
                pedestrianNearby = returnDecision(questions.q8);
            } else {
                pedestrianNearby = returnDecision(questions.q6);
            }
        } else {
            pedestrianNearby = "empty";
        }

        vehicleNear = returnDecision(questions.q7);

        dataLog[loopCounter].logRoadType = roadType;
        dataLog[loopCounter].logWeather = weather;
        dataLog[loopCounter].logPedestrianNearby = pedestrianNearby;
        dataLog[loopCounter].logObstacleAhead = obstacleAhead;
        dataLog[loopCounter].logObstacleType = obstacleType;
        dataLog[loopCounter].logObstacleSuddenly = obstacleSuddenly;
        dataLog[loopCounter].logVehicleNear = vehicleNear;

        DecisionMaker AIdecision;
        string decision = AIdecision.getDecision(roadType, obstacleType, obstacleSuddenly, pedestrianNearby, vehicleNear);
        bool crashOccurred = (decision.find("crash") != string::npos);
        decision += AIdecision.getAdjustment(weather, crashOccurred);
        dataLog[loopCounter].logDecision = decision;

        tempLogChoice = returnDecision("Display log data? (yes/no): ");
        if (tempLogChoice == "yes") {
            end_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
            cout << "Log data at " << ctime(&end_time) << endl;
            cout << "Road Type: " << dataLog[loopCounter].logRoadType << endl;
            cout << "Weather: " << dataLog[loopCounter].logWeather << endl;
            cout << "Pedestrian Nearby: " << dataLog[loopCounter].logPedestrianNearby << endl;
            cout << "Obstacle Ahead: " << dataLog[loopCounter].logObstacleAhead << endl;
            cout << "Obstacle Type: " << dataLog[loopCounter].logObstacleType << endl;
            cout << "Obstacle Suddenly: " << dataLog[loopCounter].logObstacleSuddenly << endl;
            cout << "Vehicle Near: " << dataLog[loopCounter].logVehicleNear << endl;
            cout << "Decision: " << dataLog[loopCounter].logDecision << endl;
        } else {
            cout << "Log data not displayed." << endl;
        }
        loopCounter++;

        bool validChoice;
        do {
            cout << "Continue simulation? (yes/no): ";
            getline(cin, simulateAgain);

            if (!(validateInput(simulateAgain))) {
                continue;
            }
            adjustInput(simulateAgain, true);

            if (simulateAgain == "yes") {
                continueSimulation = true;
                validChoice = true;
            } else if (simulateAgain == "no") {
                continueSimulation = false;
                validChoice = true;
            } else {
                cout << "Error: Invalid option - Please enter \"yes\" or \"no\".\n";
                validChoice = false;
            }
        } while (!validChoice);

    } while (continueSimulation);
}
