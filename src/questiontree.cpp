/* Name: Nicholas Cuevas (SCPD Student)
 * Course: CS106B
 * Section Leader: Diwakar Ganesan (cganesan)
 * Assignment Description:
 * This assignment file is the cpp file of the 20Questions Assignment.
 * In this file, you will find the function implementations for the QuestionTree class.
 * Late Days Used: 0
*/

#include "questiontree.h"
#include <climits>
#include "strlib.h"

/*
 * The QuestionTree default constructor sets the root pointer to the default, new QuestionNode of "computer"
 * Initializes the private member variables gamesWonCount and gamesLostCount to zero.
 */
QuestionTree::QuestionTree() {
    root = new QuestionNode("A:computer", nullptr, nullptr);
    gamesWonCount = 0;
    gamesLostCount = 0;
}

/*
 * The QuestionTree destructor calls the removeTreeHelper method in order to remove each QuestionNode
 * and deallocate the memory.
 */
QuestionTree::~QuestionTree() {
    removeTreeHelper(root);
}

/*
 * The removeTreeHelper function passes a pointer to the rootPassed.
 * The function adds the removes and deallocates the memory of the QuestionNodes by
 * traversing the QuestionTree in post-order.
 * The root is then set to nullptr
 */
void QuestionTree::removeTreeHelper(QuestionNode* rootPassed) {
    if (rootPassed != nullptr) {
        removeTreeHelper(rootPassed->yes);
        removeTreeHelper(rootPassed->no);
        delete rootPassed;
    }

    root = nullptr;
}

/*
 * The countAnswers function calls the countAnswersHelper and sets its return value
 * to the local answersCount variable.
 * The function then returns the answersCount variable, representing the number of
 * answers (leaves) in the QuestionTree.
 */
int QuestionTree::countAnswers() const {
    int answersCount = countAnswersHelper(root);
    return answersCount;
}

/*
 * The countAnswersHelper function passes a QuestionNode pointer.
 * The function recursively traverses the QuestionTree, adding one to the count when at a leaf node.
 * Returns the total count of leaf nodes / answers in the QuestionTree
 */
int QuestionTree::countAnswersHelper(QuestionNode* rootPassed) const {
    if (rootPassed == nullptr) {
        return 0;

    } else if (rootPassed->yes == nullptr && rootPassed->no == nullptr) {
        return 1;

    } else {
        return countAnswersHelper(rootPassed->yes) + countAnswersHelper(rootPassed->no);
    }
}

/*
 * The countQuestions function calls the countQuestionsHelper and sets its return value
 * to the local questionsCount variable.
 * The function then returns the questionsCount variable, representing the number of
 * questions (non-leaves) in the QuestionTree.
 */
int QuestionTree::countQuestions() const {
    int questionsCount = countQuestionsHelper(root);
    return questionsCount;
}

/*
 * The countQuestionsHelper function passes a QuestionNode pointer.
 * The function recursively traverses the QuestionTree, adding one to the count when at a non-leaf node
 * If there are no questions in the tree, returns 0
 * Returns the total count of non-leaf nodes / questions in the QuestionTree
 */
int QuestionTree::countQuestionsHelper(QuestionNode* rootPassed) const {
    if (rootPassed == nullptr || (rootPassed->yes == nullptr && rootPassed->no == nullptr)) {
        return 0;

    } else {
        return 1 + countQuestionsHelper(rootPassed->yes) + countQuestionsHelper(rootPassed->no);
    }
}

/*
 * The gamesLost function returns the number of games lost by the computer during the session
 * The number of gamesLost is counted in the Play Game function
 * Returns the private member variable gamesLostCount
 */
int QuestionTree::gamesLost() const {
    return gamesLostCount;
}

/*
 * The gamesWon function returns the number of games won by the computer during the session
 * The number of gamesWon is counted in the Play Game function
 * Returns the private member variable gamesWonCount
 */
int QuestionTree::gamesWon() const {
    return gamesWonCount;
}

/*
 * The mergeData function passes a UI object by reference, and an istream object by reference.
 * The function creates a temporary root, passing it into the readDataHelper to create a QuestionTree with this temporary root.
 * The distinguishing question and answer are requested by the user and stored in the appropriate varaibles.
 * If the answer to their distinguishing question is yes, the current QuestionTree is set as the right (no) subtree of the distinguishing question
 * because the distinguishing question becomes the new Root of this session's QuestionTree. The temporary root with the tree of the file read in to merge is
 * set as the left (yes) subtree.
 * If the object answer is no to the distingusihing question, the subtrees are reversed.
 */
void QuestionTree::mergeData(UI& ui, istream& input) {
    QuestionNode *tempRoot = nullptr;
    readDataHelper(input,tempRoot);

    string question = ui.readLine("Type a Y/N question to distinguish my data from this file:");
    bool answer = ui.readBoolean("And what is the answer for the new data?");

    if (answer) {
        QuestionNode* tempTree = root; //current QuestionTree of this session
        root = new QuestionNode("Q:" + question, tempRoot, tempTree); //root is this QuestionNode, yes-subtree is the already existing QuestionTree, and no-sbutree is the new, read-in file tree

    } else {
        QuestionNode* tempTree = root; //current QuestionTree of this session
        root = new QuestionNode("Q:" + question, tempTree, tempRoot); //root is this QuestionNode, no-sbutree is the new, read-in file tree, and yes-subtree is the already existing QuestionTree
    }

}

/*
 * The playGame function passes a UI object by reference.
 * The function calls the playGameHelper function and sets its return value to a local playGameResult boolean variable.
 * The stored boolean value is returned.
 * Returns true if the computer won and false if the human won.
 */
bool QuestionTree::playGame(UI& ui) {
    bool playGameResult = playGameHelper(ui, root);
    return playGameResult;
}

/*
 * The playGameHelper function passes a UI object by reference and a rootTraversalNode its address.
 * The function checks if the node is a leaf. If it is not, then it traverses the list based on the user's response to that node's question.
 * If it is a leaf node, the function asks the user if that node's data (an answer) is the correct one. If yes, then the gamesWonCount increases and
 * the computer wins the game. Otherwise, the gamesLostCount increases and the user is prompted for a new question and answer.
 * If the new answer is yes, then the new question's left (yes) child is this answer and its right (no) child is the old answer. If the new answer is no,
 * the node's children are reversed.
 * The userResponse is then returned to indicate that the human won.
 */
bool QuestionTree::playGameHelper(UI& ui, QuestionNode*& rootTraversalNode) {
    if (rootTraversalNode->yes == nullptr && rootTraversalNode->no == nullptr) {
       string nodeData = rootTraversalNode->data;
       string trimmedLine = nodeData.substr(2);
       bool userResponse = ui.readBoolean("Are you thinking of: " + trimmedLine + "?");

       if (userResponse) {
           gamesWonCount++;
           ui.print("Hooray, I win!");
           return userResponse;

       } else {
           gamesLostCount++;
           string myObject = ui.readLine("Drat, I lost. What was your object?");
           string distinguishingQuestion = ui.readLine("Type a Y/N question to distinguish " + trimmedLine + " from " + myObject + ":");
           bool objectAnswer = ui.readBoolean("And what is the answer for " + myObject + "?");

           if (objectAnswer) { //answer to the new distinguishing question is yes
               rootTraversalNode->data = "Q:" + distinguishingQuestion;
               rootTraversalNode->yes = new QuestionNode("A:" + myObject, nullptr, nullptr);
               rootTraversalNode->no = new QuestionNode(nodeData, nullptr, nullptr);

           } else { //answer to the new distinguishing question is no
              rootTraversalNode->data = "Q:" + distinguishingQuestion;
              rootTraversalNode->yes = new QuestionNode(nodeData, nullptr, nullptr);
              rootTraversalNode->no = new QuestionNode("A:" + myObject, nullptr, nullptr);
           }

           return userResponse;
       }

    } else {
        string nodeData = rootTraversalNode->data;
        string trimmedLine = nodeData.substr(2);

        //traverse tree based on user responses
        bool userResponse = ui.readBoolean(trimmedLine);
        if (userResponse) {
            playGameHelper(ui, rootTraversalNode->yes);

        } else {
            playGameHelper(ui, rootTraversalNode->no);
        }

    }

    return false;
}

/*
 * The answerRange function passes mindDepth and maxDepth integers by reference.
 * The function calls answerRangeMinDepth helper function to set its return value to the output variable minDepth.
 * It also calls answerRangeMaxDepth helper function to set its return value to the output variable maxDepth.
 */
void QuestionTree::answerRange(int& minDepth, int& maxDepth) const {
    minDepth = answerRangeMinDepth(root);
    maxDepth = answerRangeMaxDepth(root);
}

/*
 * The answerRangeMinDepth function passes a QuestionNode pointer.
 * The function traverses the QuestionTree, adding to the count for each answer. It then takes the maximum of the traversals and returns that value.
 * Returns 1 if there is only one answer node.
 */
int QuestionTree::answerRangeMinDepth(QuestionNode* rootTraversalNode) const {
    if (rootTraversalNode == nullptr) {
        return 0;

    } else if (rootTraversalNode->yes == nullptr && rootTraversalNode->no == nullptr) {
        return 1;

    } else {
        return min(1 + answerRangeMinDepth(rootTraversalNode->yes), 1 + answerRangeMinDepth(rootTraversalNode->no));
    }
}

/*
 * The answerRangeMaxDepth function passes a QuestionNode pointer.
 * The function traverses the QuestionTree, adding to the count for each answer. It then takes the maximum of the traversals and returns that value.
 * Returns 1 if there is only one answer node.
 */
int QuestionTree::answerRangeMaxDepth(QuestionNode* rootTraversalNode) const {
    if (rootTraversalNode == nullptr) {
        return 0;

    } else if (rootTraversalNode->yes == nullptr && rootTraversalNode->no == nullptr) {
        return 1;

    } else {
        return max(1 + answerRangeMaxDepth(rootTraversalNode->yes), 1 + answerRangeMaxDepth(rootTraversalNode->no));
    }
}

/*
 * The readData function passes an istream object by reference.
 * The function calls the removeTreeHelper function to free the memory of any previous nodes in the tree.
 * The function then calls the readDataHelper function in order to create the binary tree of QuestionNodes.
 */
void QuestionTree::readData(istream& input) {
    removeTreeHelper(root);
    readDataHelper(input, root);
}

/*
 * The readDataHelper function passes an istream object by reference and a QuestionNode object by address.
 * The function calls getline to be the base case. It will continue as long as the input file has unread lines.
 * If the root is nullptr because the QuestionTree is empty, the first line is set to root.
 * The function checks if the first character of the line is "A" signifying an Answer/leaf-node and returns after its creation.
 * Creates nodes in preorder traversal for the QuestionNode Tree.
 */
void QuestionTree::readDataHelper(istream& input, QuestionNode*& rootTraversalNode) {
    string line = "";
    if (getline(input, line)) {
        if (root == nullptr) {
           root = new QuestionNode(line, nullptr, nullptr);
        }

        if (line[0] == 'A') {
          rootTraversalNode = new QuestionNode(line, nullptr, nullptr);
          return;

        } else {
            rootTraversalNode = new QuestionNode(line, nullptr, nullptr);
            readDataHelper(input, rootTraversalNode->yes);
            readDataHelper(input, rootTraversalNode->no);
        }
    }
}

/*
 * The printTree function passes QuestionNode pointer.
 * The function prints the QuestionTree in preorder.
 * Traverses the tree recursively in preorder.
 */
void QuestionTree::printTree(QuestionNode* rootTraversalNode) const {
    if (rootTraversalNode == nullptr) return;

    cout << rootTraversalNode->data << endl;
    printTree(rootTraversalNode->yes);
    printTree(rootTraversalNode->no);
}

/*
 * The writeData function passes an ostream object by reference.
 * The function calls the writeDataHelper function.
 */
void QuestionTree::writeData(ostream& output) {
    writeDataHelper(output, root);
}

/*
 * The writeDataHelper function passes an ostream object by reference and a QuestionNode pointer.
 * The function prints the QuestionTree in preorder to the output stream (the file designated by the user).
 * Traverses the tree recursively in preorder.
 * Assumes the nodes hold data with the A: or Q: prefixes for each line of the file to be written.
 */
void QuestionTree::writeDataHelper(ostream& output, QuestionNode* rootTraversalNode) {
    if (rootTraversalNode == nullptr) return;

    output << rootTraversalNode->data << endl;
    writeDataHelper(output, rootTraversalNode->yes);
    writeDataHelper(output, rootTraversalNode->no);
}
