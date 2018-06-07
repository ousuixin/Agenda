#include <functional>
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Path.hpp"
#include "Storage.hpp"
using namespace std;
shared_ptr<Storage> Storage::m_instance = NULL;

// void Storage::print (void) {
//     cout << m_dirty << endl;
//     cout << "print user:" << m_userList.size() << endl;
//     for (auto i = m_userList.begin(); i != m_userList.end(); i++) {
//         cout << i->getName() << endl;
//         cout << i->getPassword() << endl;
//         cout << i->getEmail() << endl;
//         cout << i->getPhone() << endl;
//     }
//     cout << endl;
//     cout << "print meeting:" << m_meetingList.size() << endl;
//     for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
//         cout << i->getSponsor() << endl;
//         std::vector<string> v = i->getParticipator();
//         for (auto j = v.begin(); j != v.end(); j++) {
//             cout << *j << ' ';
//         }
//         cout << endl;
//         cout << Date::dateToString(i->getStartDate()) << endl;
//         cout << Date::dateToString(i->getEndDate()) << endl;
//         cout << i->getTitle() << endl;
//     }
//     cout << endl;
// }
/**
*   default constructor
*/
Storage::Storage() {
    // cout << "ok" << endl;
    readFromFile();
    m_dirty = 0;
};

/**
*   read file content into memory
*   @return if success, true will be returned
*/

void delete_qu(string& src) {
    string temp; 
//cout << src << endl;
    unsigned i = 0;
    for (; src[i] != '"'; i++) {
        continue;
    }
    i++;
    for (; src[i] != '"'; i++) {
        temp = temp + src[i];
    }
    src = temp;
//cout << src << endl;
}

bool Storage::readFromFile(void) {
    ifstream fin_user;
    ifstream fin_meeting;

    fin_user.open(Path::userPath);
    fin_meeting.open(Path::meetingPath);
    if (fin_user && fin_meeting) {
        while (fin_user.good()) {
            string temp;
            User a;

            getline(fin_user, temp, ',');
            if (!fin_user.good()) break;
            delete_qu(temp);
//cout << temp << endl;
            a.setName(temp);

            getline(fin_user, temp, ',');
            delete_qu(temp);
//cout << temp << endl;
            a.setPassword(temp);     

            getline(fin_user, temp, ',');
            delete_qu(temp);
//cout << temp << endl;
            a.setEmail(temp);  

            getline(fin_user, temp, '\n');
            delete_qu(temp);
//cout << temp << endl;
            a.setPhone(temp);  

            m_userList.push_back(a);
        }
//cout << "ok" << endl;
        while (fin_meeting.good()) {
            string temp;
            Meeting a;

            getline(fin_meeting, temp, ',');
            if (!fin_meeting.good()) break;
            delete_qu(temp);
            a.setSponsor(temp);   
            
            getline(fin_meeting, temp, ',');
            delete_qu(temp);
            string s;
            for (unsigned i = 0; i < temp.length(); i++) {
                if (temp[i] != '&') {
                    s = s + temp[i];
                } else {
                    a.addParticipator(s);
                    s = "";
                }
            }
            a.addParticipator(s);

            getline(fin_meeting, temp, ',');
            delete_qu(temp);
            Date start = Date::stringToDate(temp);
            a.setStartDate(start);

            getline(fin_meeting, temp, ',');
            delete_qu(temp);
            Date end = Date::stringToDate(temp);
            a.setEndDate(end);

            getline(fin_meeting, temp, '\n');
            delete_qu(temp);
            a.setTitle(temp);

            m_meetingList.push_back(a);
        }
        fin_user.close();
        fin_meeting.close();
        return true;
    } else {
        fin_user.close();
        fin_meeting.close();
        return false;
    }
};

/**
*   write file content from memory
*   @return if success, true will be returned
*/
bool Storage::writeToFile(void) {
    ofstream fout_user;
    ofstream fout_meeting;
    fout_user.open(Path::userPath);
    fout_meeting.open(Path::meetingPath);
    if (fout_user.is_open() && fout_meeting.is_open()) {
        for (auto i = m_userList.begin(); i != m_userList.end(); i++) {
            fout_user << '"' << i->getName() << '"' << ','
                      << '"' << i->getPassword() << '"' << ','
                      << '"' << i->getEmail() << '"' << ','
                      << '"' << i->getPhone() << '"' << endl;
        }

        for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) { 
            fout_meeting << '"' << i->getSponsor() << '"' << ',' << '"';
            std::vector<std::string> v = i->getParticipator();
            for (auto j = v.begin(); j != v.end();) {
                fout_meeting << *j;
                j++;
                if (j != v.end()) {
                    fout_meeting << '&';
                }
            }
            fout_meeting << '"' << ',' << '"' << Date::dateToString(i->getStartDate()) << '"' << ',';
            fout_meeting << '"' << Date::dateToString(i->getEndDate()) << '"' << ',';
            fout_meeting << '"' << i->getTitle() << '"' << endl;
        }
        fout_meeting.close();
        fout_user.close();
        return true;
    } else {
        fout_meeting.close();
        fout_user.close();
        return false;
    }
};








/**
* get Instance of storage
* @return the pointer of the instance
*/
std::shared_ptr<Storage> Storage::getInstance(void) {
    if (!m_instance) {
        shared_ptr<Storage> instance(new Storage());
        m_instance = instance;
    }
    return m_instance;
};

/**
*   destructor
*/
Storage::~Storage() {
    sync();
};

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/**
* create a user
* @param a user object
*/
void Storage::createUser(const User &t_user) {
    m_dirty = 1;
    m_userList.push_back(t_user);
};
//yao fan hui xinxi biaoshi cenggong huozhe shibai
/**
* query users
* @param a lambda function as the filter
* @return a list of fitted users
*/
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
    list<User> temp;
    for (auto i = m_userList.begin(); i != m_userList.end(); i++) {
        if (filter(*i)) {
            temp.push_back(*i);
        }
    }
    return temp;
};

/**
* update users
* @param a lambda function as the filter
* @param a lambda function as the method to update the user
* @return the number of updated users
*/
int Storage::updateUser(std::function<bool(const User &)> filter,
             std::function<void(User &)> switcher) {
    int ret = 0; int count = 0;
    for (auto i = m_userList.begin(); i != m_userList.end(); i++) {
        if (filter(*i)) {
            ret = 1;
            count ++;
            switcher(*i);
        }
    }

    m_dirty = ret;
    return count;
};

/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(std::function<bool(const User &)> filter) {
    int ret = 0;
    int count = 0;
    for (auto i = m_userList.begin(); i != m_userList.end();) {
        if (filter(*i)) {
            ret = 1;
            count++;
            i = m_userList.erase(i);
        } else {
            i++;
        }
    }
    m_dirty = ret;
    return count;
};

/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting &t_meeting) {
    m_dirty = 1;
    m_meetingList.push_back(t_meeting);
};

/**
* query meetings
* @param a lambda function as the filter
* @return a list of fitted meetings
*/
std::list<Meeting> Storage::queryMeeting(
  std::function<bool(const Meeting &)> filter) const {
    list<Meeting> temp;
    for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
        if (filter(*i)) {
            temp.push_back(*i);
        }
    }
    return temp;
};

/**
* update meetings
* @param a lambda function as the filter
* @param a lambda function as the method to update the meeting
* @return the number of updated meetings
*/
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                std::function<void(Meeting &)> switcher) {
    int ret = 0; int count = 0;
    for (auto i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
        if (filter(*i)) {
            ret = 1;
            count ++;
            switcher(*i);
        }
    }

    m_dirty = ret;
    return count;
};

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
    int ret = 0;
    int count = 0;
    for (auto i = m_meetingList.begin(); i != m_meetingList.end();) {
        if (filter(*i)) {
            ret = 1;
            count++;
            i = m_meetingList.erase(i);
        } else {
            i++;
        }
    }
    m_dirty = ret;
    return count;
};

/**
* sync with the file
*/
bool Storage::sync(void) {
    if (m_dirty) {
        m_dirty = 0;
        return writeToFile();
    } else {
        return 1;
    }
};
