//====================================================================================
// Name          : Evan Hodges
// UserID        : ejh0058
// Filename      : project4_ejh0058_Evan_Hodges_Phase1.cpp
// Compile
//    Production : g++ project4_ejh0058_Evan_Hodges_Phase1.cpp -o project4.out
//    Testing    : g++ project4_ejh0058_Evan_Hodges_Phase1.cpp -DUNIT_TESTING -o project4.out
// Execute       : ./project4.out
// Sources       : I did not use any external sources for this assignment
//====================================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <cassert>
using namespace std;

// Enumeration for question types.
enum QuestionType { MCQ, TF, WR };

// Structure for a single answer option (used with MCQ questions).
struct AnswerOption {
    string text;
    AnswerOption* next;
    AnswerOption(const string &t) : text(t), next(nullptr) {}
};

// Structure for a single question node in the linked list.
struct Question {
    QuestionType type;       // The type of question (MCQ, TF, or WR).
    string questionText;     // The text of the question.
    string correctAnswer;    // The correct answer (for MCQ, a letter; for TF/WR, a word).
    double pointValue;       // The point value for the question.
    AnswerOption* options;   // For MCQ: pointer to linked list of answer options.
    string userAnswer;       // The answer provided by the user during assessment.
    bool answered;           // Flag indicating if the question has been answered.
    Question* next;          // Pointer to the next question in the linked list.

    Question() : pointValue(0), options(nullptr), answered(false), next(nullptr) {}
};

// Forward declarations of functions.
void freeAnswerOptions(AnswerOption* head);
void freeQuestionList(Question* head);
string toLower(const string &s);
double getValidPointValue();
Question* createQuestionInteractive();
void appendQuestion(Question*& head, Question* newQ);
int getQuestionCount(Question* head);
Question* getQuestionByIndex(Question* head, int index);
void creationPhase(Question*& head);
void sequentialAssessment(Question* head);

/* 
   toLower: Converts a given string to all lowercase letters.
*/
string toLower(const string &s) {
    string result;
    for (char c : s)
        result.push_back(tolower(c));
    return result;
}

/* 
   freeAnswerOptions: Frees memory allocated for a linked list of answer options.
*/
void freeAnswerOptions(AnswerOption* head) {
    while (head) {
        AnswerOption* temp = head;
        head = head->next;
        delete temp;
    }
}

/* 
   freeQuestionList: Frees memory for the entire linked list of questions.
   If a question is of type MCQ, it also frees its answer options.
*/
void freeQuestionList(Question* head) {
    while (head) {
        Question* temp = head;
        head = head->next;
        if (temp->type == MCQ && temp->options)
            freeAnswerOptions(temp->options);
        delete temp;
    }
}

/* 
   getValidPointValue: Prompts the user until a valid non-negative double is entered.
*/
double getValidPointValue() {
    string input;
    double value = -1;
    while (true) {
        getline(cin, input);
        stringstream ss(input);
        if ((ss >> value) && !(ss >> input) && value >= 0)
            break;
        else
            cout << "[Not a point value, please try again!]" << endl;
    }
    return value;
}

/* 
   createQuestionInteractive: Interactively creates a question.
   It asks for the question type, the question text, and, depending on type, additional information such as answer options for MCQs, the correct answer, and the point value.
*/
Question* createQuestionInteractive() {
    Question* q = new Question();
    string typeInput;
    while (true) {
        cout << "Type of question [mcq/tf/wr]: ";
        getline(cin, typeInput);
        string lowerType = toLower(typeInput);
        if (lowerType == "mcq") { q->type = MCQ; break; }
        else if (lowerType == "tf") { q->type = TF; break; }
        else if (lowerType == "wr") { q->type = WR; break; }
        else { cout << "[Command not recognized, please try again!]" << endl << endl; }
    }
    cout << endl;
    cout << "Enter a question: ";
    getline(cin, q->questionText);
    
    if (q->type == MCQ) {
        cout << "[At any time, type 'quit()' to exit]" << endl << endl;
        AnswerOption* headOpt = nullptr;
        AnswerOption* tailOpt = nullptr;
        char optionLetter = 'A';
        while (true) {
            cout << "Enter choice " << optionLetter << ": ";
            string optionText;
            getline(cin, optionText);
            if (optionText == "quit()") {
                if (!headOpt) {
                    cout << "[At least one answer option is required!]" << endl;
                    continue;
                } else {
                    break;
                }
            }
            AnswerOption* newOpt = new AnswerOption(optionText);
            if (!headOpt) { headOpt = newOpt; tailOpt = newOpt; }
            else { tailOpt->next = newOpt; tailOpt = newOpt; }
            optionLetter++;
        }
        q->options = headOpt;
        cout << endl;
        string correct;
        while (true) {
            cout << "Select correct answer: ";
            getline(cin, correct);
            if (correct.length() == 0) continue;
            char ansLetter = toupper(correct[0]);
            int index = 0;
            AnswerOption* opt = q->options;
            bool valid = false;
            while (opt) {
                if ((index + 'A') == ansLetter) { valid = true; break; }
                index++;
                opt = opt->next;
            }
            if (valid) { q->correctAnswer = string(1, ansLetter); break; }
            else { cout << "[Answer not recognized, please try again!]" << endl; }
        }
    }
    else if (q->type == TF) {
        cout << endl;
        string answer;
        while (true) {
            cout << "Select correct answer: ";
            getline(cin, answer);
            string lowerAns = toLower(answer);
            if (lowerAns == "true" || lowerAns == "false") { q->correctAnswer = lowerAns; break; }
            else { cout << "[Answer not recognized, please try again!]" << endl; }
        }
    }
    else if (q->type == WR) {
        cout << endl;
        cout << "Type correct answer: ";
        getline(cin, q->correctAnswer);
    }
    cout << "Enter point value: ";
    q->pointValue = getValidPointValue();
    q->answered = false;
    q->userAnswer = "";
    return q;
}

/* 
   appendQuestion: Adds a new question to the end of the linked list.
*/
void appendQuestion(Question*& head, Question* newQ) {
    if (!head) { head = newQ; }
    else {
        Question* curr = head;
        while (curr->next) curr = curr->next;
        curr->next = newQ;
    }
}

/* 
   getQuestionCount: Returns the number of questions in the linked list.
*/
int getQuestionCount(Question* head) {
    int count = 0;
    while (head) { count++; head = head->next; }
    return count;
}

/* 
   getQuestionByIndex: Retrieves the pointer to the question at a given 1-indexed position.
*/
Question* getQuestionByIndex(Question* head, int index) {
    int count = 1;
    while (head && count < index) { head = head->next; count++; }
    return head;
}

/* 
   creationPhase: Allows the user to create a question bank.
*/
void creationPhase(Question*& head) {
    while (true) {
        cout << "\n=== QUESTION " << getQuestionCount(head) + 1 << " ===" << endl;
        Question* newQ = createQuestionInteractive();
        appendQuestion(head, newQ);
        cout << "Question saved. Continue? [y/n]: ";
        string cont;
        getline(cin, cont);
        if (toLower(cont) != "y" && toLower(cont) != "yes")
            break;
    }
    
    int totalQuestions = getQuestionCount(head);
    double totalPoints = 0;
    Question* temp = head;
    while (temp) {
        totalPoints += temp->pointValue;
        temp = temp->next;
    }
    cout << "\n=== SESSION LOG ===" << endl;
    cout << "Total questions: " << totalQuestions << endl;
    cout << "Total point values: " << totalPoints << endl;
}

/* 
   sequentialAssessment: Administers the quiz sequentially.
   It presents each question in order, checks the user's answer immediately.
*/
void sequentialAssessment(Question* head) {
    if (!head) {
        cout << "[No questions to assess.]" << endl;
        return;
    }
    cout << "\n/!\\ Begin assessment? [y/n]: ";
    string begin;
    getline(cin, begin);
    if (toLower(begin) != "y" && toLower(begin) != "yes")
        return;

    int totalQ = getQuestionCount(head);
    double totalPoints = 0;
    Question* temp = head;
    while (temp) {
         totalPoints += temp->pointValue;
         temp = temp->next;
    }
    
    int correctCount = 0;
    double score = 0;
    Question* curr = head;
    int qNum = 1;
    while (curr) {
        cout << "\nQuestion " << qNum << ": " << curr->questionText << endl;
        if (curr->type == MCQ) {
            AnswerOption* opt = curr->options;
            char letter = 'A';
            while (opt) {
                cout << "   " << letter << ". " << opt->text << endl;
                letter++;
                opt = opt->next;
            }
        }
        else if (curr->type == TF) {
            cout << "[true/false]" << endl;
        }
        cout << "Your answer: ";
        string ans;
        getline(cin, ans);
        if (curr->type == MCQ && ans.length() > 0)
            curr->userAnswer = string(1, toupper(ans[0]));
        else
            curr->userAnswer = toLower(ans);
        curr->answered = true;
        if (toLower(curr->userAnswer) == toLower(curr->correctAnswer)) {
            cout << "[Your answer is correct!]" << endl;
            score += curr->pointValue;
            correctCount++;
        } else {
            cout << "[Your answer is incorrect. The correct answer is " 
                 << curr->correctAnswer << ".]" << endl;
        }
        curr = curr->next;
        qNum++;
    }
    cout << "\n/!\\ Assessment Complete." << endl;
    cout << "\n=== SESSION LOG ===" << endl;
    cout << "Correct answers: " << correctCount << "/" << totalQ << endl;
    cout << "Final score: " << score << "/" << totalPoints << endl;
    cout << "\n*** Thank you for using the testing service. Goodbye! ***" << endl;
}

#ifndef UNIT_TESTING
int main() {
    cout << "\n*** Welcome to Evan's Testing Service (Phase 1) ***" << endl;
    Question* questionBank = nullptr;
    creationPhase(questionBank);
    sequentialAssessment(questionBank);
    freeQuestionList(questionBank);
    return 0;
}
#endif

// ================== UNIT TESTING CODE =====================
#ifdef UNIT_TESTING

// Unit testing code for Phase 1.
Question* createTestQuestion(QuestionType type, const string &qText, const string &corr, double pts) {
    Question* q = new Question();
    q->type = type;
    q->questionText = qText;
    q->correctAnswer = corr;
    q->pointValue = pts;
    q->answered = false;
    q->options = nullptr;
    return q;
}

void runUnitTests() {
    // Testcase 1: Empty question bank.
    Question* head = nullptr;
    assert(getQuestionCount(head) == 0);
    cout << "Unit Test Case 1 Passed: Empty question bank." << endl;

    // Testcase 2: Add one question and verify count.
    Question* q1 = createTestQuestion(MCQ, "What is 2+2?", "A", 10);
    appendQuestion(head, q1);
    assert(getQuestionCount(head) == 1);
    cout << "Unit Test Case 2 Passed: Added one question." << endl;

    // Testcase 3: Delete the question and verify list is empty.
    freeQuestionList(head);
    head = nullptr;
    assert(getQuestionCount(head) == 0);
    cout << "Unit Test Case 3 Passed: Deleted question successfully." << endl;

    // Testcase 4: Simulate answering a TF question.
    Question* q2 = createTestQuestion(TF, "Is the sky blue?", "true", 5);
    appendQuestion(head, q2);
    q2->userAnswer = "true";
    q2->answered = true;
    assert(toLower(q2->userAnswer) == toLower(q2->correctAnswer));
    cout << "Unit Test Case 4 Passed: TF question answered correctly." << endl;

    freeQuestionList(head);
}

int main() {
    cout << "*** This is a debugging version (Phase 1) ***" << endl << endl;
    runUnitTests();
    cout << "\n*** End of the Debugging Version ***" << endl;
    return 0;
}

#endif