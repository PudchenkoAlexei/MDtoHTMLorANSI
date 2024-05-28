#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


using namespace std;

string processMarkdownToHTML(const string& markdown) {
    string html;
    stringstream ss(markdown);
    string line;

    bool inPreformatted = false;
    vector<string> preformattedLines;

    while (getline(ss, line)) {
        if (line.empty()) {
            if (inPreformatted) {
                html += "</pre></p>\n";
                inPreformatted = false;
            }
            else {
                html += "<p></p>\n";
            }
            continue;
        }

        if (line.substr(0, 3) == "```") {
            if (!inPreformatted) {
                inPreformatted = true;
                preformattedLines.clear();
                html += "<p><pre>\n";
            }
            else {
                inPreformatted = false;
                for (const auto& preLine : preformattedLines) {
                    html += preLine + "\n";
                }
                html += "</pre></p>\n";
            }
        }
        else {
            if (inPreformatted) {
                preformattedLines.push_back(line);
            }
            else {
                size_t pos = 0;
                html += "<p>";
                while (pos < line.size()) {
                    if (line[pos] == '`') {
                        size_t endPos = line.find('`', pos + 1);
                        if (endPos != string::npos) {
                            html += "<tt>" + line.substr(pos + 1, endPos - pos - 1) + "</tt>";
                            pos = endPos + 1;
                        }
                        else {
                            html += line.substr(pos);
                            break;
                        }
                    }
                    else if (line[pos] == '*' && pos + 1 < line.size() && line[pos + 1] == '*') {
                        size_t endPos = line.find("**", pos + 2);
                        if (endPos != string::npos) {
                            html += "<p>" + line.substr(pos + 2, endPos - pos - 2) + "</b>";
                            pos = endPos + 2;
                        }
                        else {
                            html += line.substr(pos);
                            break;
                        }
                    }
                    else if (line[pos] == '_') {
                        size_t endPos = line.find('_', pos + 1);
                        if (endPos != string::npos) {
                            html += "<i>" + line.substr(pos + 1, endPos - pos - 1) + "</i>";
                            pos = endPos + 1;
                        }
                        else {
                            html += line.substr(pos);
                            break;
                        }
                    }
                    else {
                        html += line[pos];
                        pos++;
                    }
                }
                html += "</p>\n";
            }
        }
    }

    if (inPreformatted) {
        html += "</pre></p>\n";
    }

    return html;
}

string processMarkdownToANSI(const string& markdown) {
    string ansi;
    stringstream ss(markdown);
    string line;

    bool inPreformatted = false;

    while (getline(ss, line)) {
        if (line.empty()) {
            ansi += "\n";
            continue;
        }

        if (line.substr(0, 3) == "```") {
            if (!inPreformatted) {
                inPreformatted = true;
                ansi += "\033[7m\n"; // Inverse mode on
            }
            else {
                inPreformatted = false;
                ansi += "\033[0m\n"; // Reset mode
            }
        }
        else {
            if (inPreformatted) {
                ansi += line + "\n";
            }
            else {
                size_t pos = 0;
                while (pos < line.size()) {
                    if (line[pos] == '`') {
                        size_t endPos = line.find('`', pos + 1);
                        if (endPos != string::npos) {
                            ansi += "\033[7m" + line.substr(pos + 1, endPos - pos - 1) + "\033[0m";
                            pos = endPos + 1;
                        }
                        else {
                            ansi += line.substr(pos);
                            break;
                        }
                    }
                    else if (line[pos] == '*' && pos + 1 < line.size() && line[pos + 1] == '*') {
                        size_t endPos = line.find("**", pos + 2);
                        if (endPos != string::npos) {
                            ansi += "\033[1m" + line.substr(pos + 2, endPos - pos - 2) + "\033[0m";
                            pos = endPos + 2;
                        }
                        else {
                            ansi += line.substr(pos);
                            break;
                        }
                    }
                    else if (line[pos] == '_') {
                        size_t endPos = line.find('_', pos + 1);
                        if (endPos != string::npos) {
                            ansi += "\033[3m" + line.substr(pos + 1, endPos - pos - 1) + "\033[0m";
                            pos = endPos + 1;
                        }
                        else {
                            ansi += line.substr(pos);
                            break;
                        }
                    }
                    else {
                        ansi += line[pos];
                        pos++;
                    }
                }
                ansi += "\n";
            }
        }
    }

    if (inPreformatted) {
        ansi += "\033[0m\n"; // Reset mode
    }

    return ansi;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file> [--out <output_file>] [--format=<format>]" << endl;
        return 1;
    }

    string inputFile = argv[1];
    ifstream file(inputFile);
    if (!file.is_open()) {
        cerr << "Error: Unable to open input file" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string markdown = buffer.str();

    string format = "ansi"; // Встановлюємо ansi як формат за замовчуванням
    string outputFile = "";
    bool toFile = false;

    for (int i = 2; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--out" && i + 1 < argc) {
            outputFile = argv[++i];
            toFile = true;
            format = "html"; // Якщо вказано вихідний файл, встановлюємо формат html
        }
        else if (arg.substr(0, 9) == "--format=") {
            format = arg.substr(9);
        }
    }

    string output;

    if (format == "html") {
        output = processMarkdownToHTML(markdown);
    }
    else if (format == "ansi") {
        output = processMarkdownToANSI(markdown);
    }
    else {
        cerr << "Error: Unsupported format. Supported formats are 'html' and 'ansi'." << endl;
        return 1;
    }

    if (toFile) {
        ofstream out(outputFile);
        if (!out.is_open()) {
            cerr << "Error: Unable to open output file" << endl;
            return 1;
        }
        out << output;
        out.close();
    }
    else {
        cout << output << endl;
    }

    return 0;
}