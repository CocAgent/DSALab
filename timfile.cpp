#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <system_error>

using namespace std;
using namespace filesystem;


bool file_chua_noi_dung(path duong_dan, const string& noi_dung) {

    ifstream file(duong_dan, ios::binary);
    if (!file.is_open()) return false;


    string dong;
    while (getline(file, dong)) {
        if (dong.find(noi_dung) != string::npos) {
            return true;
        }
    }
    return false;
}


void quet_thu_muc_thu_cong(string thu_muc_goc, const string& noi_dung) {
    vector<string> danh_sach_cho; 
    danh_sach_cho.push_back(thu_muc_goc);

    while (!danh_sach_cho.empty()) {
        string duong_dan_hien_tai = danh_sach_cho.back();
        danh_sach_cho.pop_back();

        error_code ec;
        directory_iterator it(duong_dan_hien_tai, directory_options::skip_permission_denied, ec);
        
        if (ec) continue; 

        for (const auto& entry : it) {
            error_code error_entry;
            
            if (entry.is_directory(error_entry)) {
                danh_sach_cho.push_back(entry.path().string());
            } 
            else if (entry.is_regular_file(error_entry)) {
                if (file_chua_noi_dung(entry.path(), noi_dung)) {
                    cout << ">>> TIM THAY: " << entry.path().string() << endl;
                }
            }
            
            if (error_entry) continue; 
        }
    }
}

int main() {
    string noi_dung;
    cout << "Nhap noi dung can tim: ";
    getline(cin, noi_dung);

    if (noi_dung.empty()) return 0;

    vector<string> ds_thu_muc;

    #ifdef _WIN32
        for (char c = 'C'; c <= 'Z'; ++c) {
            string o_dia = string(1, c) + ":\\";
            error_code ec;
            if (exists(o_dia, ec)) {
                ds_thu_muc.push_back(o_dia);
            }
        }
    #else
        const char* homeDir = getenv("HOME");
        if (homeDir) ds_thu_muc.push_back(homeDir);
        else ds_thu_muc.push_back("/");
    #endif

    for (const string& thu_muc : ds_thu_muc) {
        cout << "--- Dang quet: " << thu_muc << " ---" << endl;
        quet_thu_muc_thu_cong(thu_muc, noi_dung);
    }

    cout << "Xong!" << endl;
    return 0;
}