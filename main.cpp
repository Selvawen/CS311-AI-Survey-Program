#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

struct SurveyQuestion {
    std::string question;
    std::vector<std::string> options;
};

struct UserResponse {
    std::vector<int> answers;
    std::string politicalParty;
};

struct PartyData {
    std::string partyName;
    std::vector<std::vector<int> > answerHistograms;
};

UserResponse gatherUserResponse(const std::vector<SurveyQuestion>& questions) {
    UserResponse response;
    response.answers.reserve(questions.size());

    for (const auto& question : questions) {
        std::cout << question.question << std::endl;
        for (size_t i = 0; i < question.options.size(); ++i) {
            std::cout << static_cast<char>('A' + i) << ". " << question.options[i] << std::endl;
        }

        char userChoice;
        std::cin >> userChoice;
        response.answers.push_back(userChoice - 'A');
    }

    std::cout << "Enter your political party affiliation: ";
    std::cin >> response.politicalParty;

    return response;
}

std::string guessPoliticalParty(const UserResponse& userResponse, const std::vector<PartyData>& partyData) {
    std::vector<double> partyScores(partyData.size(), 0.0);

    for (size_t i = 0; i < partyData.size(); ++i) {
        for (size_t j = 0; j < userResponse.answers.size(); ++j) {
            partyScores[i] += partyData[i].answerHistograms[j][userResponse.answers[j]];
        }
    }

    auto maxScoreIt = std::max_element(partyScores.begin(), partyScores.end());
    size_t maxScoreIndex = std::distance(partyScores.begin(), maxScoreIt);

    return partyData[maxScoreIndex].partyName;
}

int main() {
    // Read party data from file
    std::ifstream dataFile("party_data.txt");
    if (!dataFile.is_open()) {
        std::cerr << "Error: Could not open party_data.txt" << std::endl;
        return 1;
    }

    std::vector<PartyData> partyData;
    std::string line;
    while (std::getline(dataFile, line)) {
        PartyData party;
        party.partyName = line;
        for (int i = 0; i < 3; ++i) {
            std::getline(dataFile, line);
            std::istringstream iss(line);
            std::vector<int> histogram;
            int value;
            while (iss >> value) {
                histogram.push_back(value);
            }
            party.answerHistograms.push_back(histogram);
        }
        partyData.push_back(party);
    }
    dataFile.close();

    // Simulate user responses
    std::vector<SurveyQuestion> questions = {
        {"What should the government do to help the poor?",
         {"Make it easier to apply for assistance.",
          "Allow parents to use education funds for charter schools.",
          "Create welfare to work programs.",
          "Nothing."}},
        // Add more questions here
    };

    UserResponse userResponse = gatherUserResponse(questions);

    // Guess the political party
    std::string guessedParty = guessPoliticalParty(userResponse, partyData);
    std::cout << "Guessed Political Party: " << guessedParty << std::endl;

    return 0;
}

/*This will be first cpp commit I*/