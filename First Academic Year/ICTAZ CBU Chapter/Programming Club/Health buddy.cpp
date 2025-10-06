#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;
map<string, vector<string>> aiResponses = {
 {"greet", {"Hello! I'm HealthBuddy AI. how are you feeling?", "Hi
there! How can I help? how are you feeling?", "Welcome! Feeling unwell?
how are you feeling?"}},
 {"diagnose", {"Hmm, I think you might have %s.", "Based on symptoms,
it could be %s.", "This suggests %s."}},
 {"advice", {"You should %s", "I recommend %s", "Try %s"}},
 {"serious", {"\n WARNING: This could be serious! See a doctor!", "\n
Emergency! Seek medical help NOW!"}},
 {"unknown", {"I'm not sure. Can you describe more?", "Hmm, I need more
symptoms."}}
};
map<string, pair<vector<string>, string>> conditions = {
 // Pain
 {"headache", {{"headache", "head pain", "migraine"}, "rest in dark
room, use cold compress"}},
 {"stomach ache", {{"stomach ache", "belly pain", "abdominal pain"},
"drink peppermint tea, avoid dairy"}},
 {"back pain", {{"back pain", "spine hurt"}, "apply heat pad, do gentle
stretches"}},
 // Respiratory
 {"cold", {{"cold", "runny nose", "sneezing"}, "rest, drink warm
fluids, vitamin C"}},
 {"flu", {{"flu", "body ache", "chills"}, "rest, stay hydrated,
antiviral meds if early"}},
 {"cough", {{"cough", "chesty", "phlegm"}, "honey lemon tea, steam
inhalation"}},
 {"sore throat", {{"sore throat", "throat scratchy"}, "gargle salt
water, drink warm liquids"}},
 // Digestive
 {"diarrhea", {{"diarrhea", "loose stool","stomach", "stomach ache"},
"BRAT diet (bananas, rice, applesauce, toast)"}},
 {"constipation", {{"constipation", "can't poop"}, "more fiber, water,
and exercise"}},
 {"heartburn", {{"heartburn", "acid reflux"}, "avoid spicy food,
smaller meals"}},
 {"food poisoning", {{"food poisoning", "vomiting", "nausea"}, "stay
hydrated, electrolytes"}},
 // Other Common
 {"fever", {{"fever", "high temp"}, "paracetamol, cool compress,
fluids"}},
 {"allergies", {{"allergies", "sneezing", "itchy eyes"},
"antihistamines, avoid triggers"}},
 {"insomnia", {{"can't sleep", "insomnia"}, "reduce screen time,
bedtime routine"}},
 {"anxiety", {{"anxiety", "panic", "nervous"}, "deep breathing,
chamomile tea"}},
 {"fatigue", {{"fatigue", "tired", "exhausted"}, "check sleep,
hydration, and iron levels"}},
 // Serious (triggers warnings)
 {"serious", {{"chest pain", "can't breathe", "blood","bleeding",
"fainting", "severe pain"}, "SEEK EMERGENCY CARE IMMEDIATELY"}}
};
string getAIResponse(const string& type) {
 auto& responses = aiResponses[type];
 return responses[rand() % responses.size()];
}
bool containsKeyword(const string& input, const vector<string>& keywords)
{
 for (const string& keyword : keywords)
 if (input.find(keyword) != string::npos) return true;
 return false;
}
void diagnoseUser(const string& input) {
 bool found = false;
 for (auto& [condition, data] : conditions) {
 if (containsKeyword(input, data.first)) {
 printf((getAIResponse("diagnose") + "\n").c_str(),
condition.c_str());
 printf((getAIResponse("advice") + "\n").c_str(),
data.second.c_str());
 if (condition == "serious") cout << getAIResponse("serious")
<< endl;
 found = true;
 }
 }
 if (!found) cout << getAIResponse("unknown") << endl;
}
int main() {
 srand(time(0));
 cout << " HealthBuddy AI: " << getAIResponse("greet") << "\n(Type
'exit' to quit)\n";
 while (true) {
 cout << "\nYou: ";
 string input; getline(cin, input);
 transform(input.begin(), input.end(), input.begin(), ::tolower);
 if (input == "exit") break;
 cout << "\nAI: "; diagnoseUser(input);
 }
 cout << "\nHealthBuddy AI: Get well soon! \n";
 return 0;
}