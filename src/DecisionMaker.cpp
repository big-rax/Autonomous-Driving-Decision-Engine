#include "DecisionMaker.h"
#include <iostream>
#include <string>
#include <queue>

using namespace std;

// ---------------- DecisionTree ----------------

DecisionMaker::DecisionTree::DecisionTree() : root(nullptr) {}

DecisionMaker::DecisionTree::~DecisionTree() {
    destroyTree(root);
}

void DecisionMaker::DecisionTree::destroyTree(TreeNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->middle);
    destroyTree(node->right);
    delete node;
}

DecisionMaker::TreeNode* DecisionMaker::DecisionTree::createTreeNode(const std::string& decision) {
    TreeNode* newNode = new TreeNode();
    newNode->decision = decision;
    return newNode;
}

void DecisionMaker::DecisionTree::createRegularTree(const std::string& obstacleType) {
    root = createTreeNode("");

    root->left = createTreeNode("");
    root->left->left  = createTreeNode("Change lanes to bypass the " + obstacleType + ".");
    root->left->right = createTreeNode("Come to a stop to avoid hitting the " + obstacleType + " or the vehicle next to you.");

    root->right = createTreeNode("");
    root->right->left  = createTreeNode("Change lanes to bypass the " + obstacleType + " and avoid hitting the pedestrian.");
    root->right->right = createTreeNode("Come to a stop to avoid hitting the pedestrian, the " + obstacleType + ", or the car nearby.");
}

void DecisionMaker::DecisionTree::createSuddenlyTree(const std::string& obstacleType, const std::string& sideObstacle) {
    root = createTreeNode("");

    root->left = createTreeNode("");
    root->left->right = createTreeNode("");
    root->left->right->left  = createTreeNode("Swerve into the " + sideObstacle + " to avoid crashing into the " + obstacleType + " or the vehicle nearby.");
    root->left->right->right = createTreeNode("Slow down, but run over the " + obstacleType + ".");

    root->right = createTreeNode("");
    root->right->right = createTreeNode("");
    root->right->right->left   = createTreeNode("Hit the brakes, swerve out of your lane, and crash into the vehicle nearby to avoid hitting the pedestrians.");
    root->right->right->middle = createTreeNode("Hit the brakes, but crash into the " + obstacleType + ".");
    root->right->right->right  = createTreeNode("Slow down, but run over the " + obstacleType + ".");
}

// ---------------- DecisionMaker helpers ----------------

std::string DecisionMaker::getTier(const std::string& obstacleType) {
    if (obstacleType == "pedestrian") {
        return "left";
    }
    if (obstacleType == "animal" || obstacleType == "stopped car" || obstacleType == "fallen tree") {
        return "middle";
    }
    if (obstacleType == "pothole" || obstacleType == "cone") {
        return "right";
    }
    return "right";
}

std::string DecisionMaker::getSideObstacle(const std::string& roadType) {
    if (roadType == "highway") {
        return "guardrails";
    }
    if (roadType == "city street" || roadType == "residential street") {
        return "side curb";
    }
    if (roadType == "country road") {
        return "bushes";
    }
    return "side curb";
}

std::string DecisionMaker::getAdjustment(const std::string& weather, bool crashOccurred) {
    if (weather == "rain" || weather == "snow") {
        return crashOccurred
            ? " (Attempted to adjust speed for " + weather + ", but crash was unavoidable)"
            : " (Adjust speed for " + weather + ")";
    }
    if (weather == "fog") {
        return crashOccurred
            ? " (Fog reduced visibility despite adjustments)"
            : " (Turn fog lights on and adjust speed for fog)";
    }
    return "";
}

std::string DecisionMaker::traverseTree(TreeNode* node, std::queue<std::string> userAnswer) {
    if (node == nullptr) {
        return "";
    }
    if (node->left == nullptr && node->right == nullptr) {
        return node->decision;
    }
    if (userAnswer.empty()) {
        return "No answers were provided.";
    }
    std::string answer = userAnswer.front();
    userAnswer.pop();

    if (answer == "no" || answer == "empty" || answer == "left" || (answer == "middle" && node->middle == nullptr)) {
        return traverseTree(node->left, userAnswer);
    }
    if (answer == "yes" || answer == "right") {
        return traverseTree(node->right, userAnswer);
    }
    if (answer == "middle") {
        return traverseTree(node->middle, userAnswer);
    }
    return "";
}

std::string DecisionMaker::getDecision(const std::string& roadType,
                                       const std::string& obstacleType,
                                       const std::string& obstacleSuddenly,
                                       const std::string& pedestrianNearby,
                                       const std::string& vehicleNear) {
    std::queue<std::string> userAnswer;
    userAnswer.push(pedestrianNearby);
    userAnswer.push(vehicleNear);

    if (obstacleType == "empty") {
        return "Keep driving normally.";
    }

    DecisionTree tree;
    if (obstacleSuddenly == "no") {
        tree.createRegularTree(obstacleType);
    } else {
        if (vehicleNear == "no") {
            return "Swerve out of your lane to avoid crashing into the " + obstacleType + ".";
        }
        userAnswer.push(getTier(obstacleType));
        tree.createSuddenlyTree(obstacleType, getSideObstacle(roadType));
    }
    return traverseTree(tree.root, userAnswer);
}
