#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

string trim(const string& str) {
    if (str.empty()) return "";
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    if (first == string::npos) return "";
    return str.substr(first, (last - first + 1));
}

bool sprawdz_pliki(string plik_moj, string plik_wzorzec) {
    ifstream m(plik_moj), w(plik_wzorzec);
    if (!m.is_open() || !w.is_open()) return false;

    set<string> set_m, set_w;
    string linia;
    
    while (getline(m, linia)) {
        string oczyszczona = trim(linia);
        if (!oczyszczona.empty()) set_m.insert(oczyszczona);
    }
    while (getline(w, linia)) {
        string oczyszczona = trim(linia);
        if (!oczyszczona.empty()) set_w.insert(oczyszczona);
    }

    return set_m == set_w;
}

int main() {
    string my_dir = "output";
    string answers_dir = "data_answers";

    int poprawne = 0;
    int bledne = 0;
    int brak_wzorcow = 0;

    cout << "POROWNYWANIE WYNIKOW Z WZORCAMI" << endl;

    if (!fs::exists(my_dir)) {
        cout << "Katalog '" << my_dir << "' nie istnieje" << endl;
        return 1;
    }

    for (const auto& entry : fs::directory_iterator(my_dir)) {
        string moj_plik = entry.path().filename().string();
        string wzorzec_nazwa = moj_plik;

        if (wzorzec_nazwa.find("D_list.txt") != string::npos) {
            wzorzec_nazwa.replace(wzorzec_nazwa.find("D_list.txt"), 10, ".txt");
        } 
        else if (wzorzec_nazwa.find("D_matrix.txt") != string::npos) {
            wzorzec_nazwa.replace(wzorzec_nazwa.find("D_matrix.txt"), 12, ".txt");
        }
        else if (wzorzec_nazwa.find("BF_list.txt") != string::npos) {
            wzorzec_nazwa.replace(wzorzec_nazwa.find("BF_list.txt"), 11, ".txt");
        } 
        else if (wzorzec_nazwa.find("BF_matrix.txt") != string::npos) {
            wzorzec_nazwa.replace(wzorzec_nazwa.find("BF_matrix.txt"), 13, ".txt");
        }

        if (wzorzec_nazwa.length() >= 5) {
            wzorzec_nazwa = "sp" + wzorzec_nazwa.substr(5);
        }

        string pelna_sciezka_moj = my_dir + "/" + moj_plik;
        string pelna_sciezka_wzec = answers_dir + "/" + wzorzec_nazwa;

        if (fs::exists(pelna_sciezka_wzec)) {
            if (sprawdz_pliki(pelna_sciezka_moj, pelna_sciezka_wzec)) {
                cout << "[ OK ] " << moj_plik << " jest zgodny z " << wzorzec_nazwa << endl;
                poprawne++;
            } else {
                cout << "[ BLAD ] " << moj_plik << " rozni sie od " << wzorzec_nazwa << endl;
                bledne++;
            }
        } else {
            cout << "[ BRAK WZORCA ] Nie znaleziono: " << wzorzec_nazwa << " (dla pliku " << moj_plik << ")" << endl;
            brak_wzorcow++;
        }
    }

    cout << "\nPODSUMOWANIE TESTOW JEDNOSTKOWYCH" << endl;
    cout << "Zgodnych plikow: " << poprawne << endl;
    cout << "Blednych plikow: " << bledne << endl;
    cout << "Nieporownanych: " << brak_wzorcow << endl;

    return 0;
}