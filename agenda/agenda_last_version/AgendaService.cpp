#include "AgendaService.hpp"
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include "User.hpp"
#include "Meeting.hpp"
#include "Date.hpp"
#include "Storage.hpp"
using namespace std;

/**
 * constructor
 */
AgendaService::AgendaService() {
    startAgenda();
};

/**
 * destructor
 */
AgendaService::~AgendaService() {
    quitAgenda();
};

/**
 * check if the username match password
 * @param userName the username want to login
 * @param password the password user enter
 * @return if success, true will be returned
 */
bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    std::list<User> userlist = m_storage->queryUser(
        [=](const User &t)->bool {
            return (userName == t.getName() && password == t.getPassword());
        }
    );
    return userlist.size();
};

/**
 * regist a user
 * @param userName new user's username
 * @param password new user's password
 * @param email new user's email
 * @param phone new user's phone
 * @return if success, true will be returned
 */
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                  const std::string &email, const std::string &phone) {
    std::list<User> userlist = m_storage->queryUser(
        [=](const User &t)->bool{
            return (userName == t.getName());
        }
    );
    if (userlist.size()) {
        return false;
    }
    User temp(userName, password, email, phone);
    m_storage->createUser(temp);
    return true;
};

/**
 * delete a user
 * @param userName user's username
 * @param password user's password
 * @return if success, true will be returned
 */
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    int ret = m_storage->deleteUser(
        [=](const User &t)->bool{
            return (t.getName()==userName&&t.getPassword()==password);
        }
    );
    if (ret) {
        deleteAllMeetings(userName);
        std::list<Meeting> meetinglist = listAllParticipateMeetings(userName);
        for (auto i = meetinglist.begin(); i != meetinglist.end(); i++) {
            quitMeeting(userName, i->getTitle());
        }
    }
    return ret;
};

/**
 * list all users from storage
 * @return a user list result
 */
std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User &t)->bool{return true;});
};



/**
 * create a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @param startData the meeting's start date
 * @param endData the meeting's end date
 * @return if success, true will be returned
 */
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                   const std::string &startDate, const std::string &endDate,
                   const std::vector<std::string> &participator) {
    std::list<Meeting> meetinglist = m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            return (t.getTitle()==title);
        }
    );
    //cout << meetinglist.size() << endl;
    if (meetinglist.size()) {
        return false;
    }//test in case that title name tiwce
    Date start = Date::stringToDate(startDate);
    Date end = Date::stringToDate(endDate);
    if (start >= end) return false;
    //test if the time is error
    if (!(start.getYear()&&end.getYear()&&Date::isValid(start)&&Date::isValid(end))) {
        return false;
    }//test in case that the date is not valid;
    std::list<User> userlist = m_storage->queryUser([](const User &t)->bool{return true;});
    for (auto i = participator.begin(); i != participator.end(); i++) {
        int ret = 0;
        for (auto j = userlist.begin(); j != userlist.end(); j++) {
            if (*i == j->getName()) {
                ret = 1;
            }
        }
        auto j = i; j++;
        for (; j != participator.end(); j++) {
            if (*i == *j) {
                return false;
            }
        }
        if (!ret || *i == userName) {
            return false;
        }
    }//test in case that paticipantor is error;
    {
        int ret = 0;
        for (auto j = userlist.begin(); j != userlist.end(); j++) {
            if (userName == j->getName()) {
                ret = 1;
            }
        }
        if (!ret) {
            return false;
        }
    }//test in case that sponsor is error;
    std::list<Meeting> meetinglist1 = m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            int ret = 0;
            if (userName == t.getSponsor()) {
                ret = 1;
            }
            std::vector<string> v = t.getParticipator();
            for (auto i = v.begin(); i != v.end(); i++) {
            //why if there is "auto i = t.getParticipator().begin()" will be wrong
                if (userName == *i) {
                    ret = 1;
                    break;
                }
            }
            return ret; 
        }
    );
    for (auto i = meetinglist1.begin(); i != meetinglist1.end(); i++) {
        if ((start > i->getStartDate() && start < i->getEndDate())
            ||(end > i->getStartDate() && end < i->getEndDate())
            ||(i->getStartDate() > start && i->getStartDate() < end)
            ||(i->getEndDate() > start && i->getEndDate() < end)
            ||(start == i->getStartDate() && end == i->getEndDate())) {
            return false;
        }
    }//test if the sposor is error;
    for (auto i = participator.begin(); i != participator.end(); i++) {
        std::list<Meeting> meetinglist2 = m_storage->queryMeeting(
            [=](const Meeting &t)->bool{
                int ret = 0;
                if (*i == t.getSponsor()) {
                    ret = 1;
                }
                std::vector<string> v1 = t.getParticipator();
                for (auto j = v1.begin(); j != v1.end(); j++) {
                    if (*i == *j) {
                        ret = 1;
                        break;
                    }
                }
                return ret; 
            }
        );
        for (auto i = meetinglist2.begin(); i != meetinglist2.end(); i++) {
            if ((start > i->getStartDate() && start < i->getEndDate())
                ||(end > i->getStartDate() && end < i->getEndDate())
                ||(i->getStartDate() > start && i->getStartDate() < end)
                ||(i->getEndDate() > start && i->getEndDate() < end)
                ||(start == i->getStartDate() && end == i->getEndDate())) {
                return false;
            }
        }    
    }//test if paticipator is error;
    Meeting temp(userName, participator, start, end, title);
    m_storage->createMeeting(temp);
    return true;
};

/**
 * add a participator to a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::addMeetingParticipator(const std::string &userName,
                            const std::string &title,
                            const std::string &participator) {
    //cout << "ok" << endl;
    std::list<Meeting> meetinglist = m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            return (t.getTitle()==title);
        }
    );
    //cout << "ok" << endl;
    if (meetinglist.size() != 1) {
        return false;
    }
    if (meetinglist.begin()->getSponsor() != userName) {
        return false;
    }//test if there exist such a meetting
    if (participator == userName) {
        return false;
    }
    std::vector<string> v = meetinglist.begin()->getParticipator();
    for (auto i = v.begin(); i != v.end(); i++) {
        if (*i == participator) {
            return false;
        }
    }
    std::list<User> userlist = m_storage->queryUser([=](const User &t)->bool{return (participator == t.getName());});
    if (userlist.size() != 1) {
        return false;
    }//test if the paticipantor is error
    std::list<Meeting> meetinglist1 = m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            int ret = 0;
            if (participator == t.getSponsor()) {
                ret = 1;
            }
            std::vector<string> v = t.getParticipator();
            for (auto j = v.begin(); j != v.end(); j++) {
                if (participator == *j) {
                    ret = 1;
                    break;
                }
            }
            return ret; 
        }
    );
    Date start = meetinglist.begin()->getStartDate();
    Date end = meetinglist.begin()->getEndDate();
    for (auto i = meetinglist1.begin(); i != meetinglist1.end(); i++) {
        if ((start > i->getStartDate() && start < i->getEndDate())
            ||(end > i->getStartDate() && end < i->getEndDate())
            ||(i->getStartDate() > start && i->getStartDate() < end)
            ||(i->getEndDate() > start && i->getEndDate() < end)
            ||(start == i->getStartDate() && end == i->getEndDate())) {
            return false;
        }
    }//test if paticipator'time is error;

    return m_storage->updateMeeting(       
        [=](const Meeting &t)->bool{
            return (t.getTitle()==title);
        },
        [=](Meeting &tmp){
            std::vector<std::string> participators = tmp.getParticipator();
            participators.push_back(participator);
            tmp.setParticipator(participators);
        }
    );
};

/**
 * remove a participator from a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                               const std::string &title,
                               const std::string &participator) {
    std::list<Meeting> meetinglist = m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            return (t.getTitle()==title);
        }
    );
    if (meetinglist.size() != 1) {
        return false;
    }
    if (meetinglist.begin()->getSponsor() != userName) {
        return false;
    }//test if there exist such a meetting
    int ret = 0;
    std::vector<string> v = meetinglist.begin()->getParticipator();
    for (auto i = v.begin(); i != v.end(); i++) {
        if (participator == *i) {
            ret = 1;
            break;
        }
    }
    if (!ret) {
        return false;
    }//test if there is such paticipanter

    std::vector<std::string> participators = meetinglist.begin()->getParticipator();
    for (auto k = participators.begin(); k != participators.end(); k++) {
        if (*k == participator) {
            participators.erase(k);
            break;
        }
    }
    if (participators.size()){ 
        return m_storage->updateMeeting(       
            [=](const Meeting &t)->bool{
                return (t.getTitle()==title);
            },
            [=](Meeting &tmp){
                tmp.setParticipator(participators);
            }
        );
    } else { 
        return m_storage->deleteMeeting([=](const Meeting &t)->bool{return t.getTitle()==title;});
    }
};

/**
 * quit from a meeting
 * @param userName the current userName. need to be the participator
 * @param title the meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    std::list<Meeting> meetinglist = m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            return (t.getTitle()==title);
        }
    );
    if (meetinglist.size() != 1) {
        return false;
    }//test if there exist such a meetting
    int ret = 0;
    std::vector<string> v = meetinglist.begin()->getParticipator();
    for (auto i = v.begin(); i != v.end(); i++) {
        if (userName == *i) {
            ret = 1;
            break;
        }
    }
    if (!ret) {
        return false;
    }//test if there is such paticipanter

    std::vector<std::string> participators = meetinglist.begin()->getParticipator();
    for (auto k = participators.begin(); k != participators.end(); k++) {
        if (*k == userName) {
            participators.erase(k);
            break;
        }
    }
    if (participators.size()){ 
        return m_storage->updateMeeting(       
            [=](const Meeting &t)->bool{
                return (t.getTitle()==title);
            },
            [=](Meeting &tmp){
                tmp.setParticipator(participators);
            }
        );
    } else { 
        return m_storage->deleteMeeting([=](const Meeting &t)->bool{return t.getTitle()==title;});
    }
};

/**
 * search a meeting by username and title
 * @param uesrName the sponsor's userName
 * @param title the meeting's title
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &title) const {
    return m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            int ret = 0;
            std::vector<string> v = t.getParticipator();
            for (auto i = v.begin(); i != v.end(); i++) {
                if (userName == *i) {
                    ret = 1;
                    break;
                }
            }
            if (userName == t.getSponsor()) ret = 1;
            return (t.getTitle()==title && ret);
        }
    );
};
/**
 * search a meeting by username, time interval
 * @param uesrName the sponsor's userName
 * @param startDate time interval's start date
 * @param endDate time interval's end date
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &startDate,
                                const std::string &endDate) const {
    std::list<Meeting> a;
    Date start = Date::stringToDate(startDate);
    Date end = Date::stringToDate(endDate);
    if (!(Date::isValid(start)&&Date::isValid(end))) return a;
    if (start > end) return a;
    //test if the time is error
    return m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            int ret = 0, ret1 = 0;
            std::vector<string> v = t.getParticipator();
            for (auto i = v.begin(); i != v.end(); i++) {
                if (userName == *i) {
                    ret = 1;
                    break;
                }
            }
            if (userName == t.getSponsor()) ret = 1;
            //test if the person is the participantor
            if ((start >= t.getStartDate() && start <= t.getEndDate())
                ||(end >= t.getStartDate() && end <= t.getEndDate())
                ||(t.getStartDate() >= start && t.getStartDate() <= end)
                ||(t.getEndDate() >= start && t.getEndDate() <= end)) {
                ret1 = 1;
            }
            //test if include the time
            return (ret1 && ret);
        }
    );
};

/**
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    return m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            int ret = 0;
            std::vector<string> v = t.getParticipator();
            for (auto i = v.begin(); i != v.end(); i++) {
                if (userName == *i) {
                    ret = 1;
                    break;
                }
            }
            if (userName == t.getSponsor()) ret = 1;
            //test if the person is the participantor
            return ret;
        }
    );
};

/**
 * list all meetings the user sponsor
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
    return m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            return (userName == t.getSponsor());
        }
    );
};

/**
 * list all meetings the user take part in and sponsor by other
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllParticipateMeetings (
    const std::string &userName) const {
    return m_storage->queryMeeting(
        [=](const Meeting &t)->bool{
            std::vector<string> v = t.getParticipator();
            for (auto i = v.begin(); i != v.end(); i++) {
                if (userName == *i) {
                    return true;
                }
            }
            return false;
        }
    );
};

/**
 * delete a meeting by title and its sponsor
 * @param userName sponsor's username
 * @param title meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    return m_storage->deleteMeeting(
        [=](const Meeting &t)->bool{
            return (userName == t.getSponsor() && title == t.getTitle());
        }
    );
};

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const std::string &userName) {
    return m_storage->deleteMeeting(
        [=](const Meeting &t)->bool{
            return (userName == t.getSponsor());
        }
    );
};

/**
 * start Agenda service and connect to storage
 */
void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
};

/**
 * quit Agenda service
 */
void AgendaService::quitAgenda(void) {
    //cout << "quit" << m_storage->m_dirty << endl;
    m_storage->sync();
    
};


