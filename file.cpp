#include <iostream>
#include <fstream>
using namespace std;
#define MAX 10

class Student {
    char name[25];
    int roll;
    float marks;

public:
    void getdata();
    void putdata();
    void modifyData();
    int getRoll() { return roll; }
    friend class Hash;
};

void Student::getdata() {
    cout << "Enter name: ";
    cin >> name;
    cout << "Enter roll no: ";
    cin >> roll;
    cout << "Enter marks: ";
    cin >> marks;
}

void Student::putdata() {
    cout << "Student name: " << name << ", Roll no: " << roll << ", Marks: " << marks << endl;
}

void Student::modifyData() {
    cout << "Enter new name: ";
    cin >> name;
    cout << "Enter new marks: ";
    cin >> marks;
}

class Hash {
    int hashtable[MAX][2]; // [0]=rollno,[1]=file offset
    int count;

public:
    Hash();
    void insert_wo_replacement(Student &s, long offset);
    void insert_with_replacement(Student &s, long offset);
    void display_hash();
    long search(int roll);
    void displayAllRecords();
};

Hash::Hash() {
    for(int i = 0; i < MAX; i++) {
        hashtable[i][0] = -1; // roll number init
        hashtable[i][1] = -1; // file offset init
    }
    count = 0;
}

void Hash::insert_wo_replacement(Student &s, long offset) {
    if(count >= MAX) {
        cout << "Hash table is full!" << endl;
        return;
    }

    int key = s.roll;
    int loc = key % MAX;

    if(hashtable[loc][0] == -1) {
        hashtable[loc][0] = key;
        hashtable[loc][1] = offset;
    } else {
        int i = (loc + 1) % MAX;
        while(i != loc) {
            if(hashtable[i][0] == -1) {
                hashtable[i][0] = key;
                hashtable[i][1] = offset;
                break;
            }
            i = (i + 1) % MAX;
        }
    }
    count++;
}

void Hash::insert_with_replacement(Student &s, long offset) {
    if(count >= MAX) {
        cout << "Hash table is full!" << endl;
    
        
        return;
    }

    int key = s.roll;
    int loc = key % MAX;

    if(hashtable[loc][0] == -1) {
        hashtable[loc][0] = key;
        hashtable[loc][1] = offset;
    } else {
        // Check for home location
        int existingLoc = hashtable[loc][0] % MAX;
        if(existingLoc == loc) {
            int i = (loc + 1) % MAX;
            while(i != loc) {
                if(hashtable[i][0] == -1) {
                    hashtable[i][0] = key;
                    hashtable[i][1] = offset;
                    break;
                }
                i = (i + 1) % MAX;
            }
        } else {
            //reinsert the existing element
            int tempKey = hashtable[loc][0];
            long tempOffset = hashtable[loc][1];
            hashtable[loc][0] = key;
            hashtable[loc][1] = offset;
            
            int newLoc = tempKey % MAX;
            int i = (newLoc + 1) % MAX;
            while(i != newLoc) {
                if(hashtable[i][0] == -1) {
                    hashtable[i][0] = tempKey;
                    hashtable[i][1] = tempOffset;
                    break;
                }
                i = (i + 1) % MAX;
            }
        }
    }
    count++;
}

void Hash::display_hash() {
    cout << "\nHash Table:\n";
    cout << "Index\tRoll\tOffset\n";
    for(int i = 0; i < MAX; i++) {
        cout << i << "\t" << hashtable[i][0] << "\t" << hashtable[i][1] << endl;
    }
}

long Hash::search(int roll) {
    int loc = roll % MAX;
    int originalLoc = loc;

    do {
        if(hashtable[loc][0] == roll) {
            return hashtable[loc][1];//we are returning offset
        }
        loc = (loc + 1) % MAX;
    } while(loc != originalLoc && hashtable[loc][0] != -1);

    return -1; 
}

void Hash::displayAllRecords() {
    fstream file("records.txt", ios::in | ios::binary);
    if(!file) {
        cout << "Error opening file!" << endl;
        return;
    }

    Student s;
    cout << "\nAll Records in Database:\n";
    while(file.read((char*)&s, sizeof(s))) {
        s.putdata();
    }
    file.close();
}

int main() {
    Student s;
    Hash h_wo_replace, h_with_replace;
    fstream file;
    int choice, n, roll;
    long offset;
    char ch;

    do {
        cout << "\n1. Create Database (without replacement)\n";
        cout << "2. Create Database (with replacement)\n";
        cout << "3. Display Hash Table\n";
        cout << "4. Search Record\n";
        cout << "5. Modify Record\n";
        cout << "6. Display All Records\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: {
                file.open("records.txt", ios::out | ios::binary | ios::trunc);
                if(!file) {
                    cout << "Error opening file!" << endl;
                    break;
                }

                cout << "Enter number of students to add: ";
                cin >> n;

                for(int i = 0; i < n; i++) {
                    cout << "\nEnter data for student " << (i + 1) << ":\n";
                    s.getdata();
                    offset = file.tellp();
                    file.write((char*)&s, sizeof(s));
                    h_wo_replace.insert_wo_replacement(s, offset);
                }
                file.close();
                break;
            }

            case 2: {
                file.open("records.txt", ios::out | ios::binary | ios::trunc);
                if(!file) {
                    cout << "Error opening file!" << endl;
                    break;
                }

                cout << "Enter number of students to add: ";
                cin >> n;

                for(int i = 0; i < n; i++) {
                    cout << "\nEnter data for student " << (i + 1) << ":\n";
                    s.getdata();
                    offset = file.tellp();
                    file.write((char*)&s, sizeof(s));
                    h_with_replace.insert_with_replacement(s, offset);
                }
                file.close();
                break;
            }

            case 3: {
                cout << "\nHash Table (Without Replacement):\n";
                h_wo_replace.display_hash();
                cout << "\nHash Table (With Replacement):\n";
                h_with_replace.display_hash();
                break;
            }

            case 4: {
                cout << "Enter roll number to search: ";
                cin >> roll;
                
                
                offset = h_wo_replace.search(roll);
                if(offset != -1) {
                    file.open("records.txt", ios::in | ios::binary);
                    file.seekg(offset);
                    file.read((char*)&s, sizeof(s));
                    cout << "\nRecord found (without replacement):\n";
                    s.putdata();
                    file.close();
                } else {
                    // Try with replacement next
                    offset = h_with_replace.search(roll);
                    if(offset != -1) {
                        file.open("records.txt", ios::in | ios::binary);
                        file.seekg(offset);
                        file.read((char*)&s, sizeof(s));
                        cout << "\nRecord found (with replacement):\n";
                        s.putdata();
                        file.close();
                    } else {
                        cout << "Record not found!" << endl;
                    }
                }
                break;
            }

            case 5: {
                cout << "Enter roll number to modify: ";
                cin >> roll;
                
                //without replacement first
                offset = h_wo_replace.search(roll);
                if(offset != -1) {
                    file.open("records.txt", ios::in | ios::out | ios::binary);
                    file.seekg(offset);
                    file.read((char*)&s, sizeof(s));
                    cout << "\nCurrent Record:\n";
                    s.putdata();
                    
                    cout << "\nEnter new data:\n";
                    s.modifyData();
                    
                    file.seekp(offset);
                    file.write((char*)&s, sizeof(s));
                    cout << "Record updated successfully!" << endl;
                    file.close();
                } else {
                    //with replacement
                    offset = h_with_replace.search(roll);
                    if(offset != -1) {
                        file.open("records.txt", ios::in | ios::out | ios::binary);
                        file.seekg(offset);
                        file.read((char*)&s, sizeof(s));
                        cout << "\nCurrent Record:\n";
                        s.putdata();
                        
                        cout << "\nEnter new data:\n";
                        s.modifyData();
                        
                        file.seekp(offset);
                        file.write((char*)&s, sizeof(s));
                        cout << "Record updated" << endl;
                        file.close();
                    } else {
                        cout << "Record not found!" << endl;
                    }
                }
                break;
            }

            case 6: {
                cout << "\nRecords (Without Replacement):\n";
                h_wo_replace.displayAllRecords();
                cout << "\nRecords (With Replacement):\n";
                h_with_replace.displayAllRecords();
                break;
            }

            case 7:
                exit(0);

            default:
                cout << "Invalid choice!" << endl;
        }

        cout << "\nDo you want to continue (y/n)? ";
        cin >> ch;
    } while(ch == 'y' || ch == 'Y');

    return 0;
}
