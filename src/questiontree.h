/* Name: Nicholas Cuevas (SCPD Student)
 * Course: CS106B
 * Section Leader: Diwakar Ganesan (cganesan)
 * Assignment Description:
 * This assignment file is the header file of the 20Questions Assignment.
 * In this file, you will find the public, provided function declarations, private member variables,
 * and private function declarations for the QuestionTree class.
 * Late Days Used: 0
*/

#ifndef _questiontree_h
#define _questiontree_h

#include <iostream>
#include <string>
#include "questionnode.h"
#include "ui.h"
using namespace std;

class QuestionTree {
public:
    QuestionTree();
    ~QuestionTree();
    void answerRange(int& minDepth, int& maxDepth) const;
    int countAnswers() const;
    int countQuestions() const;
    int gamesLost() const;
    int gamesWon() const;
    void mergeData(UI& ui, istream& input);
    bool playGame(UI& ui);
    void readData(istream& input);
    void writeData(ostream& output);

private:
    QuestionNode* root;
    int gamesWonCount;
    int gamesLostCount;

    bool playGameHelper(UI& ui, QuestionNode*& rootTraversalNode);
    void removeTreeHelper(QuestionNode* rootPassed);
    int gamesWonHelper(QuestionNode* rootPassed, int winCount) const;
    int countAnswersHelper(QuestionNode* rootPassed) const;
    int countQuestionsHelper(QuestionNode* rootPassed) const;
    int answerRangeMinDepth(QuestionNode* rootTraversalNode) const;
    int answerRangeMaxDepth(QuestionNode* rootTraversalNode) const;
    void readDataHelper(istream& input, QuestionNode*& rootTraversalNode);
    void printTree(QuestionNode* rootTraversalNode) const;
    void writeDataHelper(ostream& output, QuestionNode* rootTraversalNode);

};

#endif // _questiontree_h
