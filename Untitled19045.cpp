#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <iomanip>

using namespace std;

struct Member {
    int memberID;
    string name;
    string subscriptionType; // "Monthly" ,"Yearly"
    string registrationDate;
    string expiryDate;
   
    void display() const {
        cout << "Member ID: " << memberID << endl;
        cout << "Name: " << name << endl;
        cout << "Subscription Type: " << subscriptionType << endl;
        cout << "Registration Date: " << registrationDate << endl;
        cout << "Expiry Date: " << expiryDate << endl;
        cout << "----------------------------" << endl;
    }
    

    bool isExpired() const {
        time_t now = time(0);
        tm* currentDate = localtime(&now);
        
        int expiryYear, expiryMonth, expiryDay;
        sscanf(expiryDate.c_str(), "%d-%d-%d", &expiryYear, &expiryMonth, &expiryDay);
        
        int currentYear = currentDate->tm_year + 1900;
        int currentMonth = currentDate->tm_mon + 1;
        int currentDay = currentDate->tm_mday;
        
        if (currentYear > expiryYear) return true;
        if (currentYear < expiryYear) return false;
        
        if (currentMonth > expiryMonth) return true;
        if (currentMonth < expiryMonth) return false;
        
        return currentDay > expiryDay;
    }
    
    bool expiresSoon(int days = 7) const {
        if (isExpired()) return false;
        
        time_t now = time(0);
        tm* currentDate = localtime(&now);
        
        tm currentDateCopy = *currentDate;
        time_t currentTime = mktime(&currentDateCopy);
        currentTime += days * 24 * 60 * 60;
        tm* futureDate = localtime(&currentTime);
        
        int expiryYear, expiryMonth, expiryDay;
        sscanf(expiryDate.c_str(), "%d-%d-%d", &expiryYear, &expiryMonth, &expiryDay);
        
        int futureYear = futureDate->tm_year + 1900;
        int futureMonth = futureDate->tm_mon + 1;
        int futureDay = futureDate->tm_mday;
        
        if (futureYear > expiryYear) return true;
        if (futureYear < expiryYear) return false;
        
        if (futureMonth > expiryMonth) return true;
        if (futureMonth < expiryMonth) return false;
        
        return futureDay >= expiryDay;
    }
};

class GymManagementSystem {
private:
    vector<Member> members;
    int nextMemberID;
    
    // œ«·… ·≈÷«›… √‘Â— ≈·Ï  «—ÌŒ
    string addMonthsToDate(const string& dateStr, int months) {
        int year, month, day;
        sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
        
        month += months;
        
        while (month > 12) {
            month -= 12;
            year++;
        }
		char buffer[20];
		snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
		return string(buffer);
 
    }
    
    // œ«·… ··»ÕÀ ⁄‰ ⁄÷Ê »—ﬁ„ «·⁄÷ÊÌ…
    int findMemberByID(int id) {
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i].memberID == id) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    
    // œ«·… ··»ÕÀ ⁄‰ ⁄÷Ê »«·«”„
    vector<int> findMembersByName(const string& name) {
        vector<int> indices;
        string searchName = name;
        transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
        
        for (size_t i = 0; i < members.size(); i++) {
            string memberName = members[i].name;
            transform(memberName.begin(), memberName.end(), memberName.begin(), ::tolower);
            
            if (memberName.find(searchName) != string::npos) {
                indices.push_back(static_cast<int>(i));
            }
        }
        return indices;
    }
    
    // œ«·… ··Õ’Ê· ⁄·Ï «· «—ÌŒ «·Õ«·Ì
    string getCurrentDate() {
        time_t now = time(0);
        tm* currentDate = localtime(&now);
        
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", currentDate);
        return string(buffer);
    }

public:
    GymManagementSystem() : nextMemberID(1001) {
        initializeSampleData();
    }
    
    //  ÂÌ∆… »Ì«‰«  ⁄Ì‰…
    void initializeSampleData() {
        Member m1 = {1001, "Ahmed Ali", "Monthly", "2024-01-15", "2024-06-30"};
        Member m2 = {1002, "Sara Mohammed", "Yearly", "2023-11-20", "2024-11-19"};
        Member m3 = {1003, "Omar Khalid", "Monthly", "2024-03-10", "2024-07-10"};
        Member m4 = {1004, "Fatima Hassan", "Yearly", "2024-02-05", "2025-02-04"};
        
        members.push_back(m1);
        members.push_back(m2);
        members.push_back(m3);
        members.push_back(m4);
        nextMemberID = 1005;
    }

    // 1) ≈÷«›… ⁄÷Ê ÃœÌœ
    void addNewMember() {
        Member newMember;
        newMember.memberID = nextMemberID++;
        
        cout << "\n=== Add New Member ===" << endl;
        cout << "Auto-generated Member ID: " << newMember.memberID << endl;
        
        cin.ignore();
        cout << "Member Name: ";
        getline(cin, newMember.name);
        
        if (newMember.name.empty()) {
            cout << "Error: Name cannot be empty!" << endl;
            return;
        }
        
        cout << "Subscription Type (1 - Monthly, 2 - Yearly): ";
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            newMember.subscriptionType = "Monthly";
        } else if (choice == 2) {
            newMember.subscriptionType = "Yearly";
        } else {
            cout << "Invalid choice! Defaulting to Monthly." << endl;
            newMember.subscriptionType = "Monthly";
        }
        
        newMember.registrationDate = getCurrentDate();
        
        if (newMember.subscriptionType == "Monthly") {
            newMember.expiryDate = addMonthsToDate(newMember.registrationDate, 1);
        } else {
            newMember.expiryDate = addMonthsToDate(newMember.registrationDate, 12);
        }
        
        members.push_back(newMember);
        cout << "\nMember added successfully!" << endl;
        cout << "Expiry Date: " << newMember.expiryDate << endl;
    }
    
    // 2)  ÃœÌœ «‘ —«ﬂ ⁄÷Ê
    void renewMemberSubscription() {
        cout << "\n=== Renew Member Subscription ===" << endl;
        cout << "Enter Member ID: ";
        int id;
        cin >> id;
        
        int index = findMemberByID(id);
        if (index == -1) {
            cout << "Error: Member ID not found!" << endl;
            return;
        }
        
        cout << "\nCurrent Member Information:" << endl;
        members[index].display();
        
        cout << "\nChoose renewal duration:" << endl;
        cout << "1 - Renew for 1 month" << endl;
        cout << "2 - Renew for 1 year" << endl;
        cout << "3 - Custom duration (in months)" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        int monthsToAdd = 0;
        switch(choice) {
            case 1:
                monthsToAdd = 1;
                members[index].subscriptionType = "Monthly";
                break;
            case 2:
                monthsToAdd = 12;
                members[index].subscriptionType = "Yearly";
                break;
            case 3:
                cout << "Number of months: ";
                cin >> monthsToAdd;
                if (monthsToAdd <= 0) {
                    cout << "Error: Invalid number of months!" << endl;
                    return;
                }
                members[index].subscriptionType = "Custom";
                break;
            default:
                cout << "Error: Invalid choice!" << endl;
                return;
        }
        
        members[index].expiryDate = addMonthsToDate(members[index].expiryDate, monthsToAdd);
        
        cout << "\nSubscription renewed successfully!" << endl;
        cout << "New Expiry Date: " << members[index].expiryDate << endl;
    }
    
    // 3) Õ–› ⁄÷Ê
    void deleteMember() {
        cout << "\n=== Delete Member ===" << endl;
        cout << "Enter Member ID: ";
        int id;
        cin >> id;
        
        int index = findMemberByID(id);
        if (index == -1) {
            cout << "Error: Member ID not found!" << endl;
            return;
        }
        
        cout << "\nMember to be deleted:" << endl;
        members[index].display();
        
        cout << "Are you sure? (1 - Yes, 0 - No): ";
        int confirm;
        cin >> confirm;
        
        if (confirm == 1) {
            members.erase(members.begin() + index);
            cout << "Member deleted successfully!" << endl;
        } else {
            cout << "Deletion cancelled." << endl;
        }
    }
    
    // 4) «·»ÕÀ ⁄‰ ⁄÷Ê - *«· ’ÕÌÕ Â‰«*
    void searchMember() {
        cout << "\n=== Search for Member ===" << endl;
        cout << "1 - Search by Member ID" << endl;
        cout << "2 - Search by Name" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            cout << "Enter Member ID: ";
            int id;
            cin >> id;
            
            int index = findMemberByID(id);
            if (index == -1) {
                cout << "No member found with this ID!" << endl;
            } else {
                cout << "\nSearch Result:" << endl;
                members[index].display();
            }
        } else if (choice == 2) {
            cin.ignore();
            cout << "Enter member name (or part of it): ";
            string name;
            getline(cin, name);
            
            if (name.empty()) {
                cout << "Error: Name cannot be empty!" << endl;
                return;
            }
            
            vector<int> indices = findMembersByName(name);
            if (indices.empty()) {
                cout << "No members found with this name!" << endl;
            } else {
                cout << "\nSearch Results (" << indices.size() << " found):" << endl;
                // «·ÿ—Ìﬁ… «·√Ê·Ï: «” Œœ«„ Õ·ﬁ… for  ﬁ·ÌœÌ… («·√ﬂÀ— √„«‰«)
                for (size_t i = 0; i < indices.size(); i++) {
                    int memberIndex = indices[i];
                    members[memberIndex].display();
                }
            }
        } else {
            cout << "Error: Invalid choice!" << endl;
        }
    }
    
    // 5) ⁄—÷ Ã„Ì⁄ «·√⁄÷«¡
    void displayAllMembers() {
        cout << "\n=== All Members ===" << endl;
        cout << "Total Members: " << members.size() << endl;
        
        if (members.empty()) {
            cout << "No members registered." << endl;
            return;
        }
        
        for (size_t i = 0; i < members.size(); i++) {
            cout << "\nMember " << (i + 1) << ":" << endl;
            members[i].display();
        }
    }
    
    // 6) ⁄—÷ «·√⁄÷«¡ «·–Ì‰ «‰ ÂÏ «‘ —«ﬂÂ„
    void displayExpiredMembers() {
        cout << "\n=== Members with Expired Subscriptions ===" << endl;
        
        vector<Member> expiredMembers;
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i].isExpired()) {
                expiredMembers.push_back(members[i]);
            }
        }
        
        if (expiredMembers.empty()) {
            cout << "No members with expired subscriptions." << endl;
            return;
        }
        
        cout << "Expired Members: " << expiredMembers.size() << endl;
        for (size_t i = 0; i < expiredMembers.size(); i++) {
            cout << "\nMember " << (i + 1) << ":" << endl;
            expiredMembers[i].display();
        }
    }
    
    // 7) ⁄—÷ «·√⁄÷«¡ «·–Ì‰ ”Ì‰ ÂÌ «‘ —«ﬂÂ„ ﬁ—Ì»«
    void displayMembersExpiringSoon() {
        cout << "\n=== Members with Subscriptions Expiring Soon (within 7 days) ===" << endl;
        
        vector<Member> expiringSoonMembers;
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i].expiresSoon(7) && !members[i].isExpired()) {
                expiringSoonMembers.push_back(members[i]);
            }
        }
        
        if (expiringSoonMembers.empty()) {
            cout << "No members with subscriptions expiring soon." << endl;
            return;
        }
        
        cout << "Members expiring soon: " << expiringSoonMembers.size() << endl;
        for (size_t i = 0; i < expiringSoonMembers.size(); i++) {
            cout << "\nMember " << (i + 1) << ":" << endl;
            expiringSoonMembers[i].display();
        }
    }
    
    // 8) „⁄—›… ⁄œœ «·√⁄÷«¡ «·Õ«·ÌÌ‰
    void displayCurrentMemberCount() {
        cout << "\n=== Current Member Statistics ===" << endl;
        cout << "Total Registered Members: " << members.size() << endl;
        
        int activeMembers = 0;
        int expiredMembers = 0;
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i].isExpired()) {
                expiredMembers++;
            } else {
                activeMembers++;
            }
        }
        
        cout << "Active Members: " << activeMembers << endl;
        cout << "Expired Members: " << expiredMembers << endl;
        
        if (members.size() > 0) {
            double activePercentage = (static_cast<double>(activeMembers) / members.size()) * 100;
            cout << "Active Percentage: " << fixed << setprecision(1) << activePercentage << "%" << endl;
        }
    }
    
    // ⁄—÷ «·ﬁ«∆„… «·—∆Ì”Ì…
    void displayMenu() {
        cout << "\n=== Gym Management System ===" << endl;
        cout << "1 - Add New Member" << endl;
        cout << "2 - Renew Member Subscription" << endl;
        cout << "3 - Delete Member" << endl;
        cout << "4 - Search for Member" << endl;
        cout << "5 - Display All Members" << endl;
        cout << "6 - Display Expired Members" << endl;
        cout << "7 - Display Members Expiring Soon" << endl;
        cout << "8 - Display Member Statistics" << endl;
        cout << "0 - Exit" << endl;
        cout << "Choice: ";
    }
    
    //  ‘€Ì· «·‰Ÿ«„
    void run() {
        int choice;
        
        do {
            displayMenu();
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Error: Invalid input! Please enter a number." << endl;
                continue;
            }
            
            switch(choice) {
                case 1:
                    addNewMember();
                    break;
                case 2:
                    renewMemberSubscription();
                    break;
                case 3:
                    deleteMember();
                    break;
                case 4:
                    searchMember();
                    break;
                case 5:
                    displayAllMembers();
                    break;
                case 6:
                    displayExpiredMembers();
                    break;
                case 7:
                    displayMembersExpiringSoon();
                    break;
                case 8:
                    displayCurrentMemberCount();
                    break;
                case 0:
                    cout << "\nThank you for using Gym Management System!" << endl;
                    cout << "Goodbye!" << endl;
                    break;
                default:
                    cout << "Error: Invalid choice! Please try again." << endl;
            }
            
            cout << endl;
            
        } while (choice != 0);
    }
};

// «·œ«·… «·—∆Ì”Ì…
int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
        system("cls");
    #endif
    
    cout << "========================================" << endl;
    cout << "    GYM MANAGEMENT SYSTEM" << endl;
    cout << "    Developed for Small Gym Clubs" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    GymManagementSystem gymSystem;
    gymSystem.run();
    
    return 0;
}
