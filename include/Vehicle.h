#pragma once
#include <string>
#include <vector>
#include <queue>
#include <ctime>
#include <chrono>
#include "DecisionMaker.h"
#include "LogData.h"

class Vehicle {
private:
    struct Question {
        std::string q1 = "What type of road are you driving on? (highway, city street, residential street, country road) : ";
        std::string q2 = "What is the weather condition? (clear, rain, fog, snow) : ";
        std::string q3 = "Is there an obstacle in front of you? (yes/no) : ";
        std::string q6 = "Is there a pedestrian nearby? (yes/no) : ";
        std::string q7 = "Is there another vehicle next to you? (yes/no) : ";
        std::string q8 = "Are there other pedestrians nearby? (yes/no) : ";
    };

    std::string getq4();
    std::string getq5(const std::string& obstacleType);

    DecisionMaker decisionMaker;
    LogData log;

public:
    bool continueSimulation = true;
    std::string roadType, weather, pedestrianNearby, obstacleAhead, extraObstacles, obstacleType, obstacleSuddenly, vehicleNear;
    std::string simulateAgain = "yes";
    Question questions;
    LogData dataLog[50];
    std::string tempLogChoice = "";
    int loopCounter = 0;
    std::time_t end_time;

    bool validateInput(const std::string& decision);
    bool alphaCheck(const std::string& decision);
    void adjustInput(std::string& decision, const bool& eraseWhitespace);

    std::string getDecision(const std::string& question, const bool& eraseWhitespace);
    std::string getValidChoice(const std::string& question, const std::vector<std::string>& validOptions, const bool& eraseWhitespace);

    std::string returnDecision(const std::string& question);
    std::string returnChoice1(const std::string& question);
    std::string returnChoice2(const std::string& question);
    std::string returnChoice3(const std::string& question);

    void simulator();
    Vehicle();
};
