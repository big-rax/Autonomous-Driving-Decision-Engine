#pragma once
#include <string>
#include <queue>

class DecisionMaker {
public:
    struct TreeNode {
        std::string decision;
        TreeNode* left;
        TreeNode* middle;
        TreeNode* right;
        TreeNode() : decision(""), left(nullptr), middle(nullptr), right(nullptr) {}
    };

    class DecisionTree {
    public:
        TreeNode* root;
        DecisionTree();
        ~DecisionTree();

        TreeNode* createTreeNode(const std::string& decision);
        void createRegularTree(const std::string& obstacleType);
        void createSuddenlyTree(const std::string& obstacleType, const std::string& sideObstacle);

    private:
        void destroyTree(TreeNode* node);
    };

    std::string getTier(const std::string& obstacleType);
    std::string getSideObstacle(const std::string& roadType);
    std::string getAdjustment(const std::string& weather, bool crashOccurred);
    std::string traverseTree(TreeNode* node, std::queue<std::string> userAnswer);

    // Get obstacle type and user answers, create tree, and traverse tree to get a decision
    std::string getDecision(const std::string& roadType,
                            const std::string& obstacleType,
                            const std::string& obstacleSuddenly,
                            const std::string& pedestrianNearby,
                            const std::string& vehicleNear);
};
