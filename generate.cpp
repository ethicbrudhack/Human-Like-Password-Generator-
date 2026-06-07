#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

using namespace std;

const vector<string> PREFIXES = {
    "", "my", "the", "real", "official", "super", "vip", "best", "mr", "mrs",
    "ms", "dr", "x", "xx", "xxx", "pro", "top", "elite", "master", "king",
    "queen", "dark", "big", "cool", "true", "new", "its", "im", "iam", "team",
    "admin", "mod", "ultra", "mega", "hyper", "boss", "legend", "epic",
    "crazy", "official_", "real_", "the_", "x_"
};

const vector<string> SUFFIXES = {
    "", "1", "12", "123", "1234", "01", "007", "69", "777", "0", "00", "000",
    "11", "111", "1111", "22", "222", "33", "333", "99", "999", "12345",
    "123456", "321", "3210", "2024", "2025", "2026", "24", "25", "26",
    "!", "!!", "!!!", "@", "#", "69!", "123!", "007!", "7777", "888",
    "8888", "666", "6666"
};

const vector<string> SEPARATORS = {"", "_", ".", "-"};

const vector<string> COMMON_KEYWORDS = {
    "", "user", "admin", "pass", "password", "welcome", "letmein", "qwerty",
    "1q2w3e", "abc123", "administrator", "root", "guest", "test", "demo",
    "login", "passwd", "secret", "default", "master", "owner", "manager",
    "support", "service", "office", "work", "home", "internet", "system",
    "security", "company", "private", "public"
};

const vector<string> MONTHS_SHORT = {
    "", "jan", "feb", "mar", "apr", "may", "jun",
    "jul", "aug", "sep", "oct", "nov", "dec"
};

const int MIN_LEN = 6;
const int MAX_LEN = 32;

vector<string> YEARS;

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string to_lower_str(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

string to_upper_str(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

string capitalize_str(string s) {
    if (s.empty()) return s;
    s = to_lower_str(s);
    s[0] = toupper(s[0]);
    return s;
}

vector<string> simple_leet(const string& word, int max_variants = 5) {
    vector<string> variants;
    unordered_set<string> seen;

    variants.push_back(word);
    seen.insert(word);

    vector<int> positions;
    int length = word.size();

    if (length >= 1) positions.push_back(0);
    if (length >= 3) positions.push_back(length / 2);
    if (length >= 2) positions.push_back(length - 1);

    for (int pos : positions) {
        char c = tolower(word[pos]);
        vector<char> replacements;

        if (c == 'a') replacements = {'4', '@'};
        else if (c == 'o') replacements = {'0'};
        else if (c == 'i') replacements = {'1', '!'};
        else if (c == 'e') replacements = {'3'};
        else if (c == 's') replacements = {'5', '$'};
        else if (c == 't') replacements = {'7'};

        for (char r : replacements) {
            string cand = word;
            cand[pos] = r;

            if (!seen.count(cand)) {
                seen.insert(cand);
                variants.push_back(cand);
            }

            if ((int)variants.size() >= max_variants)
                return variants;
        }
    }

    return variants;
}

vector<string> capitalization_forms(const string& base) {
    vector<string> forms;

    string low = to_lower_str(base);
    string cap = capitalize_str(base);
    string up = to_upper_str(base);

    for (const string& f : {low, cap, up}) {
        if (!f.empty() && find(forms.begin(), forms.end(), f) == forms.end()) {
            forms.push_back(f);
        }
    }

    return forms;
}

vector<string> join_with_prefix(const string& prefix, const string& core) {
    if (prefix.empty()) return {core};

    return {
        prefix + core,
        prefix + "_" + core,
        prefix + "." + core,
        prefix + "-" + core
    };
}

void try_write(
    ofstream& fout,
    const string& variant,
    unordered_set<string>& seen,
    int& count,
    int max_per_word,
    long long& total_count
) {
    if (count >= max_per_word) return;

    if (!seen.count(variant)) {
        seen.insert(variant);

        if ((int)variant.size() >= MIN_LEN && (int)variant.size() <= MAX_LEN) {
            fout << variant << "\n";
            total_count++;
        }

        count++;
    }
}

void generate_for_base(
    const string& base_raw,
    ofstream& fout,
    int max_per_word,
    bool enable_leet,
    long long& total_count
) {
    string base = trim(base_raw);
    if (base.empty()) return;

    unordered_set<string> seen;

    vector<string> forms = capitalization_forms(base);
    vector<string> cores;

    for (const string& f : forms) {
        cores.push_back(f);

        if (enable_leet && cores.size() < 8) {
            vector<string> leets = simple_leet(f, 3);

            for (const string& l : leets) {
                if (find(cores.begin(), cores.end(), l) == cores.end()) {
                    cores.push_back(l);
                }
            }
        }
    }

    int count = 0;

    for (const string& core : cores) {
        if (count >= max_per_word) return;

        for (int i = 0; i < 10 && count < max_per_word; i++) {
            string n = to_string(i);
            try_write(fout, core + n, seen, count, max_per_word, total_count);

            for (size_t s = 1; s < SEPARATORS.size() && count < max_per_word; s++) {
                try_write(fout, core + SEPARATORS[s] + n, seen, count, max_per_word, total_count);
            }
        }

        for (int i = 0; i < 100 && count < max_per_word; i++) {
            char buf[4];
            snprintf(buf, sizeof(buf), "%02d", i);
            string n = buf;

            try_write(fout, core + n, seen, count, max_per_word, total_count);

            for (size_t s = 1; s < SEPARATORS.size() && count < max_per_word; s++) {
                try_write(fout, core + SEPARATORS[s] + n, seen, count, max_per_word, total_count);
            }
        }

        for (int i = 0; i < 1000 && count < max_per_word; i++) {
            char buf[5];
            snprintf(buf, sizeof(buf), "%03d", i);
            string n = buf;

            try_write(fout, core + n, seen, count, max_per_word, total_count);

            for (size_t s = 1; s < SEPARATORS.size() && count < max_per_word; s++) {
                try_write(fout, core + SEPARATORS[s] + n, seen, count, max_per_word, total_count);
            }
        }

        for (const string& suf : SUFFIXES) {
            if (suf.empty()) continue;

            for (const string& sep : SEPARATORS) {
                if (count >= max_per_word) return;
                try_write(fout, core + sep + suf, seen, count, max_per_word, total_count);
            }
        }

        for (const string& pre : PREFIXES) {
            if (pre.empty()) continue;

            vector<string> variants = join_with_prefix(pre, core);

            for (const string& variant : variants) {
                if (count >= max_per_word) return;
                try_write(fout, variant, seen, count, max_per_word, total_count);
            }
        }

        for (const string& kw : COMMON_KEYWORDS) {
            if (kw.empty()) continue;

            for (const string& sep : SEPARATORS) {
                if (count >= max_per_word) return;

                try_write(fout, core + sep + kw, seen, count, max_per_word, total_count);

                if (count >= max_per_word) return;

                try_write(fout, kw + sep + core, seen, count, max_per_word, total_count);
            }
        }

        for (const string& m : MONTHS_SHORT) {
            if (m.empty()) continue;

            if (count >= max_per_word) return;
            try_write(fout, core + m, seen, count, max_per_word, total_count);

            if (count >= max_per_word) return;
            try_write(fout, m + core, seen, count, max_per_word, total_count);
        }

        for (const string& y : YEARS) {
            if (count >= max_per_word) return;
            try_write(fout, core + y, seen, count, max_per_word, total_count);

            if (count >= max_per_word) return;
            try_write(fout, y + core, seen, count, max_per_word, total_count);
        }
    }
}

void stream_process(
    const string& input_path,
    const string& output_path,
    int max_per_word,
    bool enable_leet,
    int flush_every
) {
    ifstream fin(input_path);
    ofstream fout(output_path);

    if (!fin.is_open()) {
        cerr << "Nie mogę otworzyć pliku wejściowego: " << input_path << endl;
        exit(1);
    }

    if (!fout.is_open()) {
        cerr << "Nie mogę otworzyć pliku wyjściowego: " << output_path << endl;
        exit(1);
    }

    string line;
    int idx = 0;
    long long total_count = 0;

    while (getline(fin, line)) {
        idx++;

        string base = trim(line);
        if (base.empty()) continue;

        fout << "# base: " << base << "\n";

        generate_for_base(base, fout, max_per_word, enable_leet, total_count);

        fout << "\n";

        if (idx % flush_every == 0) {
            fout.flush();
            cout << "Przetworzono " << idx << " słów, zapisano "
                 << total_count << " linii..." << endl;
        }
    }

    fout.flush();

    cout << "\nZakończono: " << total_count
         << " linii zapisano do " << output_path << endl;
}

int main(int argc, char* argv[]) {
    string input_path;
    string output_path;

    int max_per_word = 50000;
    int flush_every = 10000;
    bool enable_leet = true;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
            input_path = argv[++i];
        }
        else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            output_path = argv[++i];
        }
        else if (arg == "--max-per-word" && i + 1 < argc) {
            max_per_word = stoi(argv[++i]);
        }
        else if (arg == "--flush-every" && i + 1 < argc) {
            flush_every = stoi(argv[++i]);
        }
        else if (arg == "--no-leet") {
            enable_leet = false;
        }
    }

    if (input_path.empty() || output_path.empty()) {
        cerr << "Użycie:\n";
        cerr << argv[0] << " -i input.txt -o output.txt "
             << "--max-per-word 50000 --flush-every 10000 --no-leet\n";
        return 1;
    }

    for (int y = 1970; y <= 2026; y++) {
        YEARS.push_back(to_string(y));
    }

    stream_process(input_path, output_path, max_per_word, enable_leet, flush_every);

    return 0;
}
