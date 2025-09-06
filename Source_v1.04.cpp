#include <iostream>
#include <iomanip> //setw, 
#include <string> //Using strings in the program, length(), getline(), erase()
#include <cctype> //tolower(), isalpha(), isspace()
#include <limits> //cin.ignore, 
#include <vector> //List of possible options
#include <queue> //Use user's answers to traverse the decision tree
#include <ctime> //For timestamp
#include <chrono> //For timestamp

using namespace std;


class LogData
{
    public:
        string logRoadType = "", 
        logWeather = "", 
        logPedestrianNearby = "", 
        logObstacleAhead = "", 
        logObstacles = "", 
        logObstacleType = "", 
        logObstacleSuddenly = "", 
        logVehicleNear = "",
        logDecision = "";


};

class DecisionMaker                 //Take answers gathered in the Vehicle class to get a decision
{
public:
    struct TreeNode                 //Create the struct that will be used as nodes for the decision tree
    {
        string decision = "";
        TreeNode* left = nullptr;
        TreeNode* middle = nullptr;
        TreeNode* right = nullptr;
    };

    class DecisionTree              //To create the decision tree
    {
    public:
        TreeNode* root;

        DecisionTree();                      //Constructor

        ~DecisionTree();                     //Destructor function

        void destroyTree(TreeNode*);    //Function to help delete the tree nodes

        TreeNode* createTreeNode(string);           //To instantiate nodes of the decision tree

        void createRegularTree(string);            //Create this tree if the obstacle did not appear suddenly

        void createSuddenlyTree(string, string);            //Create the tree if the obstacle appeared suddenly 
    };

    string getTier(string);             //Get the tier of obstacle to know what is okay to crash into or avoid at all cost

    string getSideObstacle(string);         //Get side of the road type obstacles

    string getAdjustment(string, bool);            //Get adjustment according to weaather conditions

    string traverseTree(TreeNode*, queue<string>);   //Pass the root of "tree" and the userAnswer queue gathered in Vehicle

    //Get obstacle type and user answers, create tree, and traverse tree to get a decision
    string getDecision(string, string, string, string, string);
};

class Vehicle
{
private:
    struct Question
    {
        string q1 = "What type of road are you driving on? (highway, city street, residential street, country road) : ";
        string q2 = "What is the weather condition? (clear, rain, fog, snow) : ";

        string q3 = "Is there an obstacle in front of you? (yes/no) : ";

        string q6 = "Is there a pedestrian nearby? (yes/no) : ";
        string q7 = "Is there another vehicle next to you? (yes/no) : ";

        string q8 = "Are there other pedestrians nearby? (yes/no) : ";
    };

    string getq4()
    {
        string additionalObstacles = "";

        if (weather == "rain")
        {
            additionalObstacles = ", pothole, fallen tree";
        }
        return "What type of obstacle is it? (animal, pedestrian, stopped car, cone" + additionalObstacles + ") : ";
    }

    string getq5(const string& obstacleType)
    {
        return "Did the " + obstacleType + " appear suddenly? (yes/no) : ";
    }

    DecisionMaker decisionMaker;
    LogData log;

public:
    bool continueSimulation = true;
    string roadType, weather, pedestrianNearby, obstacleAhead, extraObstacles, obstacleType, obstacleSuddenly, vehicleNear;
    string simulateAgain = "yes";
    Question questions;
    LogData dataLog[50]; // Create array to store data for logging
    string tempLogChoice = "";
    int loopCounter = 0;
    time_t end_time = chrono::system_clock::to_time_t(chrono::system_clock::now()); //Get the current time

    bool validateInput(const string&);              //Checks if input is empty and for any characters that are not
    //alphabetical

    bool alphaCheck(const string&);                 //Checks for non-alphabetical and non-whitespace characters

    void adjustInput(string&, const bool&);         //Adjusts the user input to all lowercase and removes whitespace 
    //characters when prompted

    string getDecision(const string&, const bool&); //Gets the decision, going through the validation process first

    string getValidChoice(const string&, const vector<string>&, const bool&); //calls getDecision and ensures "decision"
    //is not just random characters

    string returnDecision(const string&);             //Returns a valid decision 

    string returnChoice1(const string&);            //Returns a valid choice from options

    string returnChoice2(const string&);            //Returns a valid choice from options

    string returnChoice3(const string&);            //Returns a valid choice from options


    void simulator()
    {

        cout << setw(19) << " " << "Autonomous Vehicle Simulator\n";
        cout << string(65, '-') << endl;

        do
        {
            roadType = returnChoice1(questions.q1);                  //Returns a choice highway or city street
            weather = returnChoice2(questions.q2);                   //Returns a choice clear, rain, fog, or snow

            obstacleAhead = returnDecision(questions.q3);            //Returns true or false
            if (obstacleAhead == "yes")
            {
                obstacleType = returnChoice3(getq4());              //Returns a choice animal, pedestrian, or car accident
                obstacleSuddenly = returnDecision(getq5(obstacleType));
            }
            else
            {
                obstacleType = "empty";                               //If no obstacle, then no obstacle type
                obstacleSuddenly = "empty";                           //    or sudden obstacle appearance
            }

            if (roadType != "highway")                              //Pedestrians don't appear on highways
            {
                if (obstacleType == "pedestrian")                   //If the obstacle is a pedestrian, change q6
                {
                    pedestrianNearby = returnDecision(questions.q8);        //Retrun yes or no
                }
                else
                {
                    pedestrianNearby = returnDecision(questions.q6);         //Returns yes or no
                }
            }
            else
            {
                pedestrianNearby = "empty";                                 //If highway, then no pedestrians
            }

            vehicleNear = returnDecision(questions.q7);              //Returns true or false


            //Implement log for all data options and timestamps
            dataLog[loopCounter].logRoadType = roadType;                //Store the data in the log
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
            dataLog[loopCounter].logDecision = decision; //Store the decision in the log

            tempLogChoice = returnDecision("Display log data? (yes/no): ");
            if (tempLogChoice == "yes")
            {
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
            }
            else
            {
                cout << "Log data not displayed." << endl;
            }
            loopCounter++;




            // Ask to continue
            bool validChoice;                               //To make sure user inputs either "yes" or "no"
            do
            {
                cout << "Continue simulation? (yes/no): ";
                getline(cin, simulateAgain);


                if (!(validateInput(simulateAgain)))        //Check if it's empty or non-alphabetical characters are there
                {
                    continue;
                }

                adjustInput(simulateAgain, true);



                if (simulateAgain == "yes")                 //If yes then do another simulator
                {
                    continueSimulation = true;
                    validChoice = true;
                }
                else if (simulateAgain == "no")             //If no then stop
                {
                    continueSimulation = false;
                    validChoice = true;
                }
                else
                {
                    cout << "Error: Invalid option - Please enter \"yes\" or \"no\".\n";
                    validChoice = false;
                }

            } while (!validChoice);

        } while (continueSimulation);

    }
};


int main()
{

    Vehicle vehicle;
    vehicle.simulator();


    return 0;
}

bool Vehicle::validateInput(const string& decision)
{
    if (decision.empty())                                   //Checks if user decision is empty
    {
        cout << "Error: Cannot be empty - Please enter a valid response.\n";
        return false;
    }

    if (!(alphaCheck(decision)))                            //Checks if user decision is all alphabetical characters
    {
        cout << "Error: Non-Alphabetical characters not accepted - Please enter a valid response.\n";
        return false;
    }

    return true;
}

bool Vehicle::alphaCheck(const string& decision)
{
    for (char d : decision)                                 //Checking for all characters
    {                                                       //if it contains anything besides alphabet
        if (!isalpha(d) && !isspace(d))                      //and whitespace
        {
            return false;
        }
    }

    return true;
}

void Vehicle::adjustInput(string& decision, const bool& eraseWhitespace)
{
    string adjustedDecision = decision;                                    //For fixed decision

    if (eraseWhitespace)                                    //If true, then erase the whitespace, if false skip
    {
        for (int i = 0; i < adjustedDecision.length(); i++)
        {
            if (adjustedDecision[i] == ' ')                     //If there are spaces, erase them
            {
                adjustedDecision.erase(i, 1);
                i--;                                            //When it deletes a whitespace, it goes down one
            }                                                   //So decrement i that way the update balances it out
        }
    }

    for (int i = 0; i < adjustedDecision.length(); i++)
    {
        adjustedDecision[i] = tolower(adjustedDecision[i]);     //lowercase all characters in string decision
    }

    decision = adjustedDecision;

}

string Vehicle::getDecision(const string& question, const bool& eraseWhitespace)
{
    string decision;
    bool validInput;

    do
    {
        validInput = true;

        cout << question;                                       //Output the current question
        getline(cin, decision);                                 //Take the decision/response

        if (!(validateInput(decision)))                         //Validates the input that it's not empty
        {                                                       //and only contains alphabet characters
            validInput = false;
        }

    } while (!validInput);

    adjustInput(decision, eraseWhitespace);                     //Remove uppercase and maybe whitespace


    return decision;

}

string Vehicle::getValidChoice(const string& question, const vector<string>& validOptions, const bool& eraseWhitespace)
{
    string decision;
    bool isValid = false;

    do
    {
        decision = getDecision(question, eraseWhitespace);                 //True to erase whitespace because it's either "yes" or "no"

        for (auto& choice : validOptions)               //If it's one of the options, then continue to while condition
        {                                               //and return decision
            if (decision == choice)
            {
                isValid = true;
                continue;
            }
        }


        if (isValid == false)
        {
            cout << "Error: Invalid option - Please enter : ";     //If it wasn't any of options it outputs

            for (int i = 0; i < validOptions.size(); i++)
            {
                cout << "\"" << validOptions[i] << "\"";

                if (i < validOptions.size() - 1)                //Make it so last choice has no comma at the end
                {
                    cout << ", or ";
                }

            }
            cout << endl;
        }


    } while (!isValid);                             //continue, until decicion == choice which turns true
    //now isValid is true so breaks and return decision

    return decision;

}

string Vehicle::returnDecision(const string& question)
{
    string decision;
    vector<string> validOptions = { "yes", "no" };              //Clarifying valid options for this

    decision = getValidChoice(question, validOptions, true);//True to erase whitespace because it's either "yes" or "no"

    cout << endl;
    return decision;                               //If decision is "yes" then the statement is true
    //If not then that means it's "no" so that statement is false
}

string Vehicle::returnChoice1(const string& question)
{
    string decision;
    vector<string> validOptions = { "highway", "city street", "residential street", "country road" };

    decision = getValidChoice(question, validOptions, false);                  //False to not erase whitespace because "city street"
    //Loop if it's not "highway" or "city street"
    cout << endl;
    return decision;
}

string Vehicle::returnChoice2(const string& question)
{
    string decision;
    vector<string> validOptions = { "clear", "rain", "fog", "snow" };

    decision = getValidChoice(question, validOptions, true);

    cout << endl;
    return decision;
}

string Vehicle::returnChoice3(const string& question)
{
    string decision;
    vector<string> validOptions = { "animal", "pedestrian", "stopped car", "cone" };

    if (weather == "rain") {
        validOptions.insert(validOptions.end(), { "pothole", "fallen tree" }); //Add obstacles specific to "rain" to the "validOptions" vector
    }

    decision = getValidChoice(question, validOptions, false);                  //False to not erase whitespace because "city street"

    cout << endl;
    return decision;
}

DecisionMaker::DecisionTree::DecisionTree()                      //Constructor
{
    root = nullptr;
}

DecisionMaker::DecisionTree::~DecisionTree()                     //Destructor function
{
    destroyTree(root);
}

void DecisionMaker::DecisionTree::destroyTree(TreeNode* node)    //Function to help delete the tree nodes
{
    if (node == nullptr) {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->middle);
    destroyTree(node->right);

    delete node;
}

DecisionMaker::TreeNode* DecisionMaker::DecisionTree::createTreeNode(string decision)           //To instantiate nodes of the decision tree
{
    DecisionMaker::TreeNode* newNode = new DecisionMaker::TreeNode();
    newNode->decision = decision;                   //add "" if the node is internal or a decision if the node is a leaf
    return newNode;
}

void DecisionMaker::DecisionTree::createRegularTree(string obstacleType)            //Create this tree if the obstacle did not appear suddenly
{
    root = createTreeNode("");                      //If the it node is an internal node, pass an empty string to it

    root->left = createTreeNode("");

    root->left->left = createTreeNode("Change lanes to bypass the " + obstacleType + ".");  //Each leaf holds a decision
    root->left->right = createTreeNode("Come to a stop to avoid hitting the " + obstacleType + " or the vehicle next to you.");

    root->right = createTreeNode("");

    root->right->left = createTreeNode("Change lanes to bypass the " + obstacleType + " and avoid hitting the pedestrian.");
    root->right->right = createTreeNode("Come to a stop to avoid hitting the pedestrian, the " + obstacleType + ", or the car nearby.");
}

void DecisionMaker::DecisionTree::createSuddenlyTree(string obstacleType, string sideObstacle)            //Create the tree if the obstacle appeared suddenly 
{
    root = createTreeNode("");                      //If the node is an internal node, pass an empty string to it

    root->left = createTreeNode("");

    root->left->right = createTreeNode("");
    root->left->right->left = createTreeNode("Swerve into the " + sideObstacle + " to avoid crashing into the " + obstacleType + " or the vehicle nearby.");
    root->left->right->right = createTreeNode("Slow down, but run over the " + obstacleType + ".");

    root->right = createTreeNode("");

    root->right->right = createTreeNode("");
    root->right->right->left = createTreeNode("Hit the brakes, swerve out of your lane, and crash into the vehicle nearby to avoid hitting the pedestrians.");
    root->right->right->middle = createTreeNode("Hit the brakes, but crash into the " + obstacleType + ".");
    root->right->right->right = createTreeNode("Slow down, but run over the " + obstacleType + ".");
}

string DecisionMaker::getTier(string obstacleType)             //Get the tier of obstacle to know what is okay to crash into or avoid at all cost
{
    if (obstacleType == "pedestrian")
    {
        return "left";
    }
    if (obstacleType == "animal" || obstacleType == "stopped car" || obstacleType == "fallen tree")
    {
        return "middle";
    }
    if (obstacleType == "pothole" || obstacleType == "cone")
    {
        return "right";
    }
    
}

string DecisionMaker::getSideObstacle(string roadType)         //Get side of the road type obstacles
{
    if (roadType == "highway")
    {
        return "guardrails";
    }
    if (roadType == "city street" || roadType == "residential street")
    {
        return "side curb";
    }
    if (roadType == "country road")
    {
        return "bushes";
    }
}

string DecisionMaker::getAdjustment(string weather, bool crashOccurred)            //Get adjustment according to weaather conditions
{
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

string DecisionMaker::traverseTree(TreeNode* node, queue<string> userAnswer)   //Pass the root of "tree" and the userAnswer queue gathered in Vehicle
{
    if (node == nullptr)                                        //If the node points to nullptr, return an empty string
    {
        return "";
    }

    if (node->left == nullptr && node->right == nullptr)        //If the node is a leaf, return a decision
    {
        return node->decision;
    }

    if (userAnswer.empty())                                     //If the queue is empty, tell the user
    {
        return "No answers were provided.";
    }

    string answer = userAnswer.front();                         //assign the front of the queue to a variable then remove it from the queue
    userAnswer.pop();

    //Recursion to traverse the tree
    if (answer == "no" || answer == "empty" || answer == "left" || (answer == "middle" && node->middle == nullptr))
    {
        return traverseTree(node->left, userAnswer);            //Go left if the user entered "no" or the answer is "empty" or the tier is "one"
    }
    if (answer == "yes" || answer == "right")
    {
        return traverseTree(node->right, userAnswer);           //Go right if the user entered "yes" or tier is "three"
    }
    if (answer == "middle")
    {
        return traverseTree(node->middle, userAnswer);           //Go middle if the tier is "two"
    }
}

//Get obstacle type and user answers, create tree, and traverse tree to get a decision
string DecisionMaker::getDecision(string roadType, string obstacleType, string obstacleSuddenly, string pedestrianNearby, string vehicleNear)
{
    string sideObstacle = "";
    queue<string> userAnswer;                                       //Queue for tree traversal
    userAnswer.push(pedestrianNearby);
    userAnswer.push(vehicleNear);


    if (obstacleType == "empty")                                    //If obstacle is empty, the user chose no obstacles
    {
        return "Keep driving normally.";                             //return this decision, no need to traverse the tree
    }

    DecisionTree tree;
    if (obstacleSuddenly == "no")
    {
        tree.createRegularTree(obstacleType);                      //If obstacleSuddenly = "no", create regular tree                 
    }
    else
    {
        if (vehicleNear == "no")                                  //If there are no vehicles nearby, return the message bellow
        {
            return "Swerve out of your lane to avoid crashing into the " + obstacleType + ".";
        }
        userAnswer.push(getTier(obstacleType));
        tree.createSuddenlyTree(obstacleType, getSideObstacle(roadType));        //If obstacleSuddenly = "yes", create suddenly tree               
    }

    return traverseTree(tree.root, userAnswer);                     //Traverse tree and return decision
}
