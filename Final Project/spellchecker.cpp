/*----------------------------------------------------------
 * Project: Spell Checker
 *
 * Date: 27-Nov-2024
 * Authors:
 *           A01800182 Daniel Alvarez Sil
 *           A01801044 Yael Sinuhé Grajeda Martínez
 *----------------------------------------------------------*/

#include <fstream>
#include <regex>
#include <vector>
#include <iostream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <cctype>

struct word {
    std::string text;
    int line;
    int column;
};

bool read_words(
        const std::string input_file_name,
        std::vector<word>& words)
{
    std::ifstream input_file(input_file_name);
    if (input_file.fail()) {
        return false;
    }
    std::regex reg_exp("[a-zA-Z]+");
    std::smatch match;
    std::string text;
    int line = 0;
    int column = 0;
    while (std::getline(input_file, text)) {
        ++line;
        column = 1;
        while (std::regex_search(text, match, reg_exp)) {
            column += match.position();
            words.push_back({match.str(), line, column});
            column += match.length();
            text = match.suffix().str();
        }
    }
    return true;
}

std::string lower(std::string& word){
    std::ostringstream result;
    // std::cout << word << '\n';
    for (char c : word){
        result << (char)tolower(c);
    }
    // std::cout << result.str() << '\n';
    return result.str();
}

std::string soundex(std::string& word){
    word = lower(word);
    // std::cout << word << '\n';
    std::ostringstream encoded;
    encoded << word[0];
    int counter = 1;
    for (int f = 1; f < word.size(); f++){
        if (counter == 7) return encoded.str();
        if (!(word[f] != 'a' && word[f] != 'e' && word[f] != 'i' && word[f] != 'o' && word[f] != 'u' && word[f] != 'h' && word[f] != 'w' && word[f] != 'y')){
            encoded << "";
        } else {
            if (word[f] == 'b' || word[f] == 'f' || word[f] == 'p' || word[f] == 'v')
                encoded << "1";
            else if (word[f] == 'c' || word[f] == 'g' || word[f] == 'j' || word[f] == 'k' || word[f] == 'q' || word[f] == 's' || word[f] == 'x' || word[f] == 'z')
                encoded << "2";
            else if (word[f] == 'd' || word[f] == 't')
                encoded << "3";
            else if (word[f] == 'l')
                encoded << "4";
            else if (word[f] == 'm' || word[f] == 'n')
                encoded << "5";
            else
                encoded << "6";
            counter++;
        }
    }
    for (int f = counter; f < 7; f++)
        encoded << "0";
    return encoded.str();
}

int main(){
    /*Carga del Diccionario*/
    std::string dict_name = "words.txt";
    std::vector<word> words;
    std::unordered_map<std::string, std::set<std::string>> dict_soundex;
    std::unordered_set<std::string> dict_words;
    if (read_words(dict_name, words)) {
        for (word w : words) {
            dict_soundex[soundex(w.text)].insert(lower(w.text));
            dict_words.insert(lower(w.text));
        }
    } else {
        std::cout << "Unable to read file: " << dict_name << "\n";
    }

    /*Lectura del Archivo*/
    std::string file_name = "./tests/bicycle.txt";
    std::unordered_set<std::string> wrong;
    std::vector<word> file_words;
    if (read_words(file_name, file_words)) {
        // std::cout << file_words.size();
        for (word w : file_words) {
            if (dict_words.count(lower(w.text)) == 0 && wrong.count(lower(w.text)) == 0){
                wrong.insert(lower(w.text));
                std::cout << "Unrecognized word: \"" << w.text << "\". First found at line " << w.line << ", column " << w.column << ".\n";
                if (dict_soundex.count(soundex(w.text)) != 0){
                    std::cout << "Suggestions: ";
                    for (auto it = dict_soundex[soundex(w.text)].begin(); it != dict_soundex[soundex(w.text)].end(); ++it) {
                        std:: cout << *it;
                        if (it == --dict_soundex[soundex(w.text)].end()) {
                            std::cout << '\n';
                        } else {
                            std::cout << ", ";
                        }
                    }
                } else {
                    std::cout << "No suggestions. \n";
                }
                std::cout << '\n'; 
            }


        }
    } else {
        std::cout << "Unable to read file: " << dict_name << "\n";
    }

    return 0;
}