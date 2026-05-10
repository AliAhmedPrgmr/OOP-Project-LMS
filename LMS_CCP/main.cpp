#include "LMS.h"

using namespace std;

int main() {
    try {
        LMS::getInstance().loadAll(); 
        cout << "=== Learning Management System ===" << endl;
        LMS::getInstance().run(); 
        LMS::getInstance().saveAll(); 
        cout << "Goodbye!" << endl;
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
    return 0;
}
