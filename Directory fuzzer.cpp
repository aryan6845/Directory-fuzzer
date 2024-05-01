/* Make sure the wordlist file has no permission restrictions. Run this program via terminal with root privileges
# g++ <ye_code_jis_file_m_h>.cpp -o <new_filename> -lcurl
# ./<created_filename> */

#include <bits/stdc++.h>
#include <curl/curl.h>
using namespace std;

size_t write_callback(char* ptr, size_t size, size_t nmemb, string* data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

int main() {
    string domain;
    cout << "Enter the domain name: ";
    cin >> domain; // test input: replit.com

    string wordlist_filename;
    cout << "Enter the filename of the directory wordlist: ";
    cin >> wordlist_filename;

    ifstream wordlist_file(wordlist_filename);
    if (!wordlist_file.is_open()) {
        cerr << "Error opening " << wordlist_filename << endl;
        return 1;
    }

    vector<string> directories;
    string directory;
    while (getline(wordlist_file, directory)) {
        directories.push_back(directory);
    }

    cout << "Enumerating directories..." << endl;
    cout << "Valid directories for " << domain << "..." << endl;

    vector<string> valid_directories;
    for (const string& dir : directories) {
        string url = "https://" + domain + "/" + dir;
        
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            string response_data;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            CURLcode res = curl_easy_perform(curl);
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            curl_easy_cleanup(curl);
            
            if (res == CURLE_OK && response_code < 400) {
                valid_directories.push_back(url);
            }
        }
    }

    for (const string& valid_dir : valid_directories) {
        cout << valid_dir << endl;
    }

    return 0;
}