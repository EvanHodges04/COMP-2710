//====================================================================================
// Name          : Evan Hodges
// UserID        : ejh0058
// Filename      : project4_ejh0058_Evan_Hodges_Phase2.cpp
// Compile
//    Production : g++ project4_ejh0058_Evan_Hodges_Phase2.cpp -o project4.out
//    Testing    : g++ project4_ejh0058_Evan_Hodges_Phase2.cpp -DUNIT_TESTING -o project4.out
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
void editQuestion(Question* head, int index);
void deleteQuestion(Question*& head, int index);
void creationPhase(Question*& head);
void assessmentPhase(Question* head);
void displayQuestionCreation(Question* q, int qnum);
void displayQuestionForAssessment(Question* q, int qnum);

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
   displayQuestionCreation: Displays the saved values of a question during creation.
*/
void displayQuestionCreation(Question* q, int qnum) {
    cout << "===============================" << endl;
    cout << "=== QUESTION " << qnum << " SAVED VALUES ===" << endl;
    cout << "===============================" << endl;
    cout << "1. Type: ";
    if(q->type == MCQ) cout << "mcq"; 
    else if(q->type == TF) cout << "tf"; 
    else cout << "wr";
    cout << endl;
    cout << "2. Question: " << q->questionText << endl;
    if (q->type == MCQ) {
        cout << "3. Answer choices:" << endl;
        AnswerOption* opt = q->options;
        char letter = 'A';
        while (opt) {
            cout << "  " << letter << ". " << opt->text << endl;
            letter++;
            opt = opt->next;
        }
        cout << "4. Correct answer: " << q->correctAnswer << endl;
    }
    else {
        cout << "3. Correct answer: " << q->correctAnswer << endl;
    }
    cout << "===============================" << endl;
}

/* 
   editQuestion: Allows the user to modify the contents of a question.
*/
void editQuestion(Question* head, int index) {
    Question* q = getQuestionByIndex(head, index);
    if (!q) {
        cout << "[That question does not exist!]" << endl;
        return;
    }
    displayQuestionCreation(q, index);
    while (true) {
        cout << endl << "Type a number to edit, or type quit(): ";
        string input;
        getline(cin, input);
        cout << endl;
        if (input == "quit()")
            break;
        int choice = atoi(input.c_str());
        switch (choice) {
            case 1:
                cout << "Question type cannot be changed." << endl;
                break;
            case 2:
                cout << "Enter a question: ";
                getline(cin, q->questionText);
                cout << "Question saved." << endl;
                break;
            case 3:
                if (q->type == MCQ) {
                    freeAnswerOptions(q->options);
                    q->options = nullptr;
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
                            }
                            break;
                        }
                        AnswerOption* newOpt = new AnswerOption(optionText);
                        if (!headOpt) { headOpt = newOpt; tailOpt = newOpt; }
                        else { tailOpt->next = newOpt; tailOpt = newOpt; }
                        optionLetter++;
                    }
                    q->options = headOpt;
                    cout << "Question saved." << endl;
                } else {
                    cout << "Editing answer choices is not applicable for this question type." << endl;
                }
                break;
            case 4:
                if (q->type == MCQ) {
                    while (true) {
                        cout << "Select correct answer: ";
                        string correct;
                        getline(cin, correct);
                        if (correct.length() == 0) continue;
                        char ansLetter = toupper(correct[0]);
                        int indexOpt = 0;
                        AnswerOption* opt = q->options;
                        bool valid = false;
                        while (opt) {
                            if ((indexOpt + 'A') == ansLetter) { valid = true; break; }
                            indexOpt++;
                            opt = opt->next;
                        }
                        if (valid) { q->correctAnswer = string(1, ansLetter); break; }
                        else { cout << "[Answer not recognized, please try again!]" << endl; }
                    }
                    cout << "Question saved." << endl;
                } else {
                    cout << "Enter correct answer: ";
                    getline(cin, q->correctAnswer);
                    cout << "Question saved." << endl;
                }
                break;
            default:
                cout << "[Invalid option, please try again!]" << endl;
        }
    }
}

/* 
   deleteQuestion: Removes a question from the linked list.
*/
void deleteQuestion(Question*& head, int index) {
    if (!head) return;
    if (index == 1) {
        Question* temp = head;
        head = head->next;
        if (temp->type == MCQ) freeAnswerOptions(temp->options);
        delete temp;
        cout << "Question 1 deleted." << endl;
        return;
    }
    Question* prev = getQuestionByIndex(head, index - 1);
    if (!prev || !(prev->next)) {
        cout << "[That question does not exist!]" << endl;
        return;
    }
    Question* toDelete = prev->next;
    prev->next = toDelete->next;
    if (toDelete->type == MCQ) freeAnswerOptions(toDelete->options);
    delete toDelete;
    cout << "Question " << index << " deleted." << endl;
}

/* 
   creationPhase: Interactive menu to create, edit, and delete questions.
*/
void creationPhase(Question*& head) {
    while (true) {
        cout << "\nDo you want to?" << endl;
        cout << "   1. Create new question." << endl;
        cout << "   2. Edit question." << endl;
        cout << "   3. Delete question." << endl;
        cout << "   4. Finish." << endl;
        cout << "Select an action: ";
        string action;
        getline(cin, action);
        if (action == "1") {
            int qCount = getQuestionCount(head) + 1;
            cout << "\n=== QUESTION " << qCount << " ===" << endl;
            Question* newQ = createQuestionInteractive();
            appendQuestion(head, newQ);
            cout << "Question saved." << endl;
        }
        else if (action == "2") {
            int count = getQuestionCount(head);
            if (count == 0) {
                cout << "[No questions available to edit.]" << endl;
                continue;
            }
            string input;
            while (true) {
                cout << endl << "Select a question to edit, or type quit() [1-" << count << "]: ";
                getline(cin, input);
                if (input == "quit()") break;
                int idx = atoi(input.c_str());
                if (idx >= 1 && idx <= count) { 
                    editQuestion(head, idx); 
                    continue; 
                }
                else { cout << "[That question does not exist!]" << endl; }
            }
        }
        else if (action == "3") {
            int count = getQuestionCount(head);
            if (count == 0) {
                cout << "[No questions available to delete.]" << endl;
                continue;
            }
            string input;
            while (true) {
                cout << endl << "Select a question to delete [1-" << count << "]: ";
                getline(cin, input);
                int idx = atoi(input.c_str());
                if (idx < 1 || idx > count) { 
                    cout << "[That question does not exist!]" << endl; 
                }
                else { 
                    deleteQuestion(head, idx);
                    break;
                }
            }
        }        
        else if (action == "4") { break; }
        else { cout << "[Command not recognized, please try again!]" << endl; }
    }
    int totalQuestions = getQuestionCount(head);
    double totalPoints = 0;
    Question* curr = head;
    while (curr) { totalPoints += curr->pointValue; curr = curr->next; }
    cout << "\n=== SESSION LOG ===" << endl;
    cout << "Total questions: " << totalQuestions << endl;
    cout << "Total point values: " << totalPoints << endl;
    cout << "\n/!\\ Begin assessment? [y/n]: ";
    string begin;
    getline(cin, begin);
    if (toLower(begin) == "y" || toLower(begin) == "yes")
        assessmentPhase(head);
}

/* 
   displayQuestionForAssessment: Displays the question text (and options if MCQ) during the assessment phase.
*/
void displayQuestionForAssessment(Question* q, int qnum) {
    cout << endl << "Question " << qnum << ": " << q->questionText << endl;
    if (q->type == MCQ) {
        AnswerOption* opt = q->options;
        char letter = 'A';
        while (opt) {
            cout << "   " << letter << ". " << opt->text << endl;
            letter++;
            opt = opt->next;
        }
    }
}

/* 
   assessmentPhase: Handles the quiz-taking process.
*/
void assessmentPhase(Question* head) {
    if (!head) {
        cout << "[No questions to assess.]" << endl;
        return;
    }
    int totalQ = getQuestionCount(head);
    int currentIndex = 1;

    while (true) {
        bool allAnswered = true;
        Question* temp = head;
        while (temp) {
            if (!temp->answered) { allAnswered = false; break; }
            temp = temp->next;
        }
        if (allAnswered) {
            cout << "\nAll questions answered. Do you want to?" << endl;
            cout << "   1. Go to next question." << endl;
            cout << "   2. Jump to question." << endl;
            cout << "   3. Submit." << endl;
            cout << "Select an action: ";
        } else {
            cout << "\nDo you want to?" << endl;
            cout << "   1. Go to next question." << endl;
            cout << "   2. Jump to question." << endl;
            cout << "   3. Submit." << endl;
            cout << "Select an action: ";
        }
        string outerChoice;
        getline(cin, outerChoice);
        if (outerChoice == "1") {
            currentIndex++;
            if (currentIndex > totalQ) currentIndex = 1;
        }
        else if (outerChoice == "2") {
            int jumpIndex;
            while (true) {
                cout << "\nJump to question [1-" << totalQ << "]: ";
                string inp;
                getline(cin, inp);
                jumpIndex = atoi(inp.c_str());
                if (jumpIndex < 1 || jumpIndex > totalQ)
                    cout << "[Invalid question number.]" << endl;
                else
                    break;
            }
            currentIndex = jumpIndex;
        }
        else if (outerChoice == "3") {
            bool anyUnanswered = false;
            Question* temp = head;
            while (temp) {
                if (!temp->answered) { anyUnanswered = true; break; }
                temp = temp->next;
            }
            if (anyUnanswered) {
                cout << "\nThere are unanswered questions. Do you want to continue? [y/n]: ";
                string confirm;
                getline(cin, confirm);
                if (toLower(confirm) != "y" && toLower(confirm) != "yes")
                    continue;
            }
            break;
        }
        else {
            cout << "[Command not recognized, please try again!]" << endl;
            continue;
        }
        
        Question* q = getQuestionByIndex(head, currentIndex);
        if (!q->answered) {
            displayQuestionForAssessment(q, currentIndex);
            if (q->type == TF)
                cout << "Your answer [true/false]: ";
            else
                cout << "Your answer: ";
            string ans;
            getline(cin, ans);
            if (q->type == MCQ && ans.length() > 0)
                q->userAnswer = string(1, toupper(ans[0]));
            else
                q->userAnswer = toLower(ans);
            q->answered = true;
        }
        else {
            cout << endl << "Question " << currentIndex << ": " << q->questionText << endl;
            if (q->type == MCQ) {
                AnswerOption* opt = q->options;
                char letter = 'A';
                while (opt) {
                    cout << "   " << letter << ". " << opt->text << endl;
                    letter++;
                    opt = opt->next;
                }
            }
            cout << "You answered: " << q->userAnswer << endl;
            cout << "\nDo you want to?" << endl;
            cout << "   1. Edit this answer." << endl;
            cout << "   2. Return to menu." << endl;
            cout << "   3. Submit." << endl;
            cout << "Select an action: ";
            string innerChoice;
            getline(cin, innerChoice);
            if (innerChoice == "1") {
                cout << "Your new answer: ";
                string newAns;
                getline(cin, newAns);
                if (q->type == MCQ && newAns.length() > 0)
                    q->userAnswer = string(1, toupper(newAns[0]));
                else
                    q->userAnswer = toLower(newAns);
            }
            else if (innerChoice == "2") {
            }
            else if (innerChoice == "3") {
                break;
            }
            else {
                cout << "[Command not recognized, please try again!]" << endl;
                continue;
            }
        }
    }
submit:
    int correctCount = 0;
    double score = 0;
    double totalPoints = 0;
    Question* cur = head;
    int qNum = 1;
    while (cur) { totalPoints += cur->pointValue; cur = cur->next; }
    cur = head;
    cout << "\n/!\\ Assessment Complete." << endl;
    cout << "\n=== SESSION LOG ===" << endl;
    while (cur) {
        cout << "   Question " << qNum << ": " << cur->correctAnswer << endl;
        cout << "   Your answer: " << (cur->userAnswer == "" ? "No answer" : cur->userAnswer) << endl << endl;
        if (toLower(cur->userAnswer) == toLower(cur->correctAnswer))
            correctCount++;
        if (toLower(cur->userAnswer) == toLower(cur->correctAnswer))
            score += cur->pointValue;
        cur = cur->next;
        qNum++;
    }
    cout << "Correct answers: " << correctCount << "/" << totalQ << endl;
    cout << "Final score: " << score << "/" << totalPoints << endl;
    cout << "\n*** Thank you for using the testing service. Goodbye! ***" << endl;
}

// ================== UNIT TESTING CODE =====================
#ifdef UNIT_TESTING

// Unit testing code for Phase 2.
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

// Run unit tests.
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
    deleteQuestion(head, 1);
    assert(getQuestionCount(head) == 0);
    cout << "Unit Test Case 3 Passed: Deleted question successfully." << endl;

    // Testcase 4: Simulate answering a TF question.
    Question* q2 = createTestQuestion(TF, "Is the sky blue?", "true", 5);
    appendQuestion(head, q2);
    q2->userAnswer = "true";
    q2->answered = true;
    assert(toLower(q2->userAnswer) == toLower(q2->correctAnswer));
    cout << "Unit Test Case 4 Passed: TF question answered correctly." << endl;

    // Testcase 5: Multiple questions scoring.
    Question* q3 = createTestQuestion(WR, "Spell 'code'", "code", 15);
    appendQuestion(head, q3);
    q2->userAnswer = "false"; // intentionally wrong
    q3->userAnswer = "Code";  // case-insensitive match
    q2->answered = true;
    q3->answered = true;
    double totalScore = 0;
    if (toLower(q2->userAnswer) == toLower(q2->correctAnswer))
        totalScore += q2->pointValue;
    if (toLower(q3->userAnswer) == toLower(q3->correctAnswer))
        totalScore += q3->pointValue;
    assert(totalScore == 15);
    cout << "Unit Test Case 5 Passed: Multiple questions scoring works." << endl;

    freeQuestionList(head);
}

int main() {
    cout << "*** This is a debugging version (Phase 2) ***" << endl << endl;
    runUnitTests();
    cout << "\n*** End of the Debugging Version ***" << endl;
    return 0;
}

#else

// ================== PRODUCTION CODE =====================
int main() {
    cout << "\n*** Welcome to Evan's Testing Service (Phase 2) ***" << endl;
    Question* questionBank = nullptr;
    creationPhase(questionBank);
    freeQuestionList(questionBank);
    return 0;
}

#endif