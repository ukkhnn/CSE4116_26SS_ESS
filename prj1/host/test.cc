#include "nvme_passthru.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <unistd.h>

using namespace std;
using namespace Embedded;

#define PAGE_SIZE 4096

// Uncomment to enable Task 3
#define TASK3_ON

#ifndef TASK3_ON
// ------------------------
// Task 2: Image Write/Read
// ------------------------
static vector<uint8_t> read_file(const string &path) {
    ifstream ifs(path, ios::binary);
    if (!ifs) {
        cerr << "Failed to open input file: " << path << endl;
        exit(1);
    }
    return vector<uint8_t>((istreambuf_iterator<char>(ifs)), {});
}

static void write_file(const string &path, const vector<uint8_t> &data) {
    ofstream ofs(path, ios::binary);
    if (!ofs) {
        cerr << "Failed to open output file: " << path << endl;
        exit(1);
    }
    ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
}
#endif

int main(int argc, char *argv[]) {
#ifdef TASK3_ON
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <lab_nvme_device>" << endl;
        return -1;
    }
#else
    if (argc != 4) {
        cerr << "Usage: " << argv[0]
             << " <lab_nvme_device> <input_img> <output_img>" << endl;
        cerr << "Task 2 writes directly to the specified device. Use only the lab SSD." << endl;
        return -1;
    }
#endif

    Proj1 dev;
    if (dev.Open(argv[1]) < 0) {
        cerr << "Device open failed" << endl;
        return -1;
    }

#ifdef TASK3_ON
    // ------------------------
    // Task 3: Custom NVMe Command
    // ------------------------
    cout << "Sending custom HELLO command to device..." << endl;

    if (dev.Hello() < 0) {
        cerr << "HELLO command failed" << endl;
        return -1;
    }

    cout << "HELLO command sent successfully. Check 'tio' output for device message." << endl;
    return 0;

#else
    // ------------------------
    // Task 2: Image Write/Read
    // ------------------------
    string input_path = argv[2];
    string output_path = argv[3];

    vector<uint8_t> src_data = read_file(input_path);

    cout << "Writing " << src_data.size() << " bytes to device..." << endl;
    if (dev.ImageWrite(src_data) < 0) {
        cerr << "ImageWrite failed" << endl;
        return -1;
    }

    vector<uint8_t> dst_data;
    cout << "Reading back from device..." << endl;
    if (dev.ImageRead(dst_data, src_data.size()) < 0) {
        cerr << "ImageRead failed" << endl;
        return -1;
    }

    write_file(output_path, dst_data);

    if (src_data.size() != dst_data.size()) {
        cerr << "Size mismatch!" << endl;
        return -1;
    }
    if (memcmp(src_data.data(), dst_data.data(), src_data.size()) == 0) {
        cout << "SUCCESS: Files are identical." << endl;
    } else {
        cout << "FAIL: Files differ!" << endl;
    }

    return 0;
#endif
}
