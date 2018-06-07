#include "AgendaService.hpp"
#include "Date.hpp"
#include "User.hpp"
#include "Storage.hpp"
#include "Meeting.hpp"
#include "AgendaUI.hpp"
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <iomanip>
#include <iostream>
#include <regex>
using namespace std;

void Stop(int signo);

AgendaUI::AgendaUI() {
	user = "";
	password = "";
};

void AgendaUI::startAgenda(void){};

void AgendaUI::OperationLoop(void) { 
	signal(SIGINT, Stop);
	print_unlog();
	while (choose_func1()) {
		print_unlog();
	}
//cout << "in start" << test.m_storage->m_dirty << endl;
};

void AgendaUI::print_unlog(void) {
	cout << endl;
	cout << "-------------------------------------Agenda-------------------------------------" << endl;
	cout << "Action :                                                                        " << endl;
	cout << "l    - log in Agenda by username and password                                   " << endl;
	cout << "r    - register an Agenda acount                                                " << endl;
	cout << "q    - quit Agenda                                                              " << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << endl;
	cout << "Agenda :~$ ";
}

void AgendaUI::print_login(void) {
	cout << endl;
	cout << "-------------------------------------Agenda-------------------------------------" << endl;
	cout << "Action :                                                                        " << endl;
	cout << "o    - log out Agenda                                                           " << endl;
	cout << "dc   - delete Agenda acount                                                     " << endl;
	cout << "lu   - list all Agenda user                                                     " << endl;
	cout << "cm   - create a meeting                                                         " << endl;
	cout << "amp  - add meeting participator                                                 " << endl;
	cout << "rmp  - remove meeting participator                                              " << endl;
	cout << "rqm  - request to quit meeting                                                  " << endl;
	cout << "la   - list all meetings                                                        " << endl;
	cout << "las  - list all sponsor meetings                                                " << endl;
	cout << "lap  - list all participator meetings                                           " << endl;
	cout << "qm   - query meeting by title                                                   " << endl;
	cout << "qt   - query by time interval                                                   " << endl;
	cout << "dm   - delete meeting by title                                                  " << endl;
	cout << "da   - delete all meetings                                                      " << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << endl;
};

void AgendaUI::excute(void) {
	cout << "succeed!" << endl;
	print_login();
	cout << "Agenda@" << user << " :~# ";
	while (choose_func2()) {
		cout << "Agenda@" << user << " :~# ";
	}
//cout << "in excute" << test.m_storage->m_dirty << endl;
}

bool AgendaUI::login(void) {
	string s1,s2;
	cout << "[log in] [username]" << endl;  
	cout << "[log in] ";
	getline(cin, s1);
	cout << "[log in] [password]" << endl;  
	cout << "[log in] ";
	getline(cin, s2);
	cout << "[log in] ";
	if (test.userLogIn(s1,s2)) {
		user = s1; password = s2;
		return true;
	}
	return false;
}

int check(string src, string sort) {
	string pattern;
	if (sort == "email") {
		pattern = "\\w+@\\w+\\.com$";
	} else if (sort == "phone"||sort == "number") {
		pattern = "\\d+$";
	} 
	regex r(pattern);
	if (!regex_match(src, r)) {
		cout << "The " << sort << " you input is wrong, please input again." << endl;
		return false;
	} else {
		return true;
	}
};

bool AgendaUI::regist(void) {
	string s1,s2,s3,s4;
	cout << "[register] [username]" << endl;
	cout << "[register] ";
	getline(cin, s1);
	cout << "[register] [password]" << endl;
	cout << "[register] ";
	getline(cin, s2);
	cout << "[register] [email]" << endl;
	cout << "[register] ";
	getline(cin, s3);
	while (!check(s3, "email")) {
		cout << "[register] [email]" << endl;
		cout << "[register] ";
		getline(cin, s3);
	}
	cout << "[register] [phone]" << endl;
	cout << "[register] ";
	getline(cin, s4);
	while (!check(s4, "phone")) {
		cout << "[register] [phone]" << endl;
		cout << "[register] ";
		getline(cin, s4);
	}
	if (test.userRegister(s1,s2,s3,s4)) {
		user = s1; password = s2;
		return true;
	}
	return false;
}

bool AgendaUI::choose_func1(void) {
loop:
	string s1;
	getline(cin, s1);
	cout << endl;
	if (s1 == "l") {
		if (login()) {
			excute();
		} else {
			cout << "Password error or user doesn't exist. So we go to start." 
				 << '\n' << endl;
		}
	} else if (s1 == "r") {
		
		if (regist()) {
			excute();
		} else {
			cout << "This username has been registered. So we go to the start"
				 << '\n' << endl;
		}
	} else if (s1 == "q") {
		cout << "------------------------------------Good bye------------------------------------" 
			 << '\n' << endl;
		return false;
	} else if (s1 == "") {
		cout << "Agenda :~$ ";
		goto loop;
	} else {
		cout << "[error] your input is error, then we can go to the start."
			 << '\n' << endl;
	}
//cout << "in func 1" << test.m_storage->m_dirty << endl;
	return true;
}

bool AgendaUI::choose_func2(void) {
	string s;
	getline(cin, s);
	cout << endl;

	if (s == "o") {
		return false;

	} else if (s == "dc") {
		if (test.deleteUser(user,password)) {
			cout << "[delete Agenda acount] succeed!" << endl;
		} else {
			cout << "[delete Agenda acount] error!" << endl;
		}
		return false;

	} else if (s == "lu") {
		cout << "[list all Agenda user]" << '\n' << endl;
		cout << left << setw(21) << "name" << setw(21) << "email" << "phone" << endl;
		list<User> userlist = test.listAllUsers();
		for (auto i = userlist.begin(); i != userlist.end(); i++) {
			cout << left << setw(21) << i->getName() << setw(21) << i->getEmail() << i->getPhone() << endl;
		}
		cout << endl;

	} else if (s == "cm") {
		int num;
		string temp, startDate, endDate, title, number;
		std::vector<string> v;

		cout << "[create meeting] [the number of the participators]" << endl;
		cout << "[create meeting] ";
		getline(cin, number);
		while (!check(number, "number")||number.size()>2) {
			cout << "You should input a number under 100." << endl;
			cout << "[create meeting] [the number of the participators]" << endl;
			cout << "[create meeting] ";
			getline(cin, number);
		}//input the number;
		istringstream ss(number); ss >> num;
		for (int i = 0; i < num; i++) {
			cout << "[create meeting] [please enter the participator " << i+1 << "]" << endl;
			cout << "[create meeting] ";  
			getline(cin, temp);
			v.push_back(temp);
		}//input the paticipator;
		cout << "[create meeting] [title]" << endl;
		cout << "[create meeting] ";
		getline(cin, title);
		cout << "[create meeting] [start time(yyyy-mm-dd/hh:mm)]" << endl;
		cout << "[create meeting] ";
		getline(cin, startDate);
		while (!Date::isValid(Date::stringToDate(startDate))) {
			cout << "the date you input is not valid, please input a new one." << endl;
			cout << "[create meeting] [start time(yyyy-mm-dd/hh:mm)]" << endl;
			cout << "[create meeting] ";
			getline(cin, startDate);
		}//input startdate
		cout << "[create meeting] [end time(yyyy-mm-dd/hh:mm)]" << endl;
		cout << "[create meeting] ";
		getline(cin, endDate);
		while (!Date::isValid(Date::stringToDate(endDate))) {
			cout << "the date you input is not valid, please input a new one." << endl;
			cout << "[create meeting] [start time(yyyy-mm-dd/hh:mm)]" << endl;
			cout << "[create meeting] ";
			getline(cin, endDate);
		}//input enddate
		if (test.createMeeting(user, title, startDate, endDate, v)) 
			cout << "[create meeting] succeed!" << endl;
		else cout << "[create meeting] error : maybe title is used!" << endl;

	} else if (s == "amp") {
		string participator, title;
		cout << "[add meeting participator] [meeting title]" << endl;
		cout << "[add meeting participator] ";
		getline(cin, title);
		cout << "[add meeting participator] [participator username]" << endl;
		cout << "[add meeting participator] ";
		getline(cin, participator);
		if (test.addMeetingParticipator(user, title, participator)) 
			cout << "[add meeting participator] succeed!" << endl;
		else cout << "[add meeting participator] error : maybe there is no such person!" << endl;

 	} else if (s == "rmp") {
 		string participator, title;
 		cout << "[remove meeting participator] [meeting title]" << endl;
 		cout << "[remove meeting participator] ";
 		getline(cin, title);
 		cout << "[remove meeting participator] [participator username]" << endl;
 		cout << "[remove meeting participator] ";
 		getline(cin, participator);
 		if (test.removeMeetingParticipator(user, title, participator)) 
 			cout << "[remove meeting participator] succeed!" << endl;
 		else cout << "[remove meeting participator] error : there is no such participator!" << endl;

 	} else if (s == "rqm") {
 		string title;
 		cout << "[quit meeting] [meeting title]" << endl;
 		cout << "[quit meeting] ";
 		getline(cin, title);
 		if (test.quitMeeting(user, title)) 
 			cout << "[quit meeting] succeed!" << endl;
 		else cout << "[quit meeting] error : you don't have that meeting!" << endl;

 	} else if (s == "la") {
 		cout << "[list all meeting]" << '\n' << endl;
 		cout << "title                " << "sponsor         "
 			 << "startDate           " << "endDate             " << "participators" << endl;
 		std::list<Meeting> meetings = test.listAllMeetings(user);
 		lm(meetings);

 	} else if (s == "las") {
 		cout << "[list all sponsor meeting]" << '\n' << endl;
 		cout << "title           " << "sponsor         "
 			 << "startDate           " << "endDate             " << "participators" << endl;
 		std::list<Meeting> meetings = test.listAllSponsorMeetings(user);
 		lm(meetings);

 	} else if (s == "lap") {
 		cout << "[list all participator meetings]" << '\n' << endl;
 		cout << "title           " << "sponsor         "
 			 << "startDate           " << "endDate             " << "participators" << endl;
 		std::list<Meeting> meetings = test.listAllParticipateMeetings(user);
 		lm(meetings);

	} else if (s == "qm") {
		string title;
		cout << "[query meeting] [title]" << endl;
		cout << "[query meeting] ";
		getline(cin, title);
		std::list<Meeting> meetings = test.meetingQuery(user, title);
		lm(meetings);

	} else if (s == "qt") {
		string startDate, endDate;
		cout << "[query by time interval] [start time(yyyy-mm-dd/hh:mm)]" << endl;
		cout << "[query by time interval] ";
		getline(cin, startDate);
		while (!Date::isValid(Date::stringToDate(startDate))) {
			cout << "the date you input is not valid, please input a new one." << endl;
			cout << "[query by time interval] [start time(yyyy-mm-dd/hh:mm)]" << endl;
			cout << "[query by time interval] ";
			getline(cin, startDate);
		}//input startdate
		cout << "[query by time interval] [end time(yyyy-mm-dd/hh:mm)]" << endl;
		cout << "[query by time interval] ";
		getline(cin, endDate);
		while (!Date::isValid(Date::stringToDate(endDate))) {
			cout << "the date you input is not valid, please input a new one." << endl;
			cout << "[query by time interval] [end time(yyyy-mm-dd/hh:mm)]" << endl;
			cout << "[query by time interval] ";
			getline(cin, endDate);
		}//input enddate
		std::list<Meeting> meetings = test.meetingQuery(user, startDate, endDate);
		lm(meetings);

 	} else if (s == "dm") {
 		string title;
 		cout << "[delete meeting] [ title] " << endl;
 		cout << "[delete meeting] ";
 		getline(cin, title);
 		if (test.deleteMeeting(user, title)) 
 			 cout << "[delete meeting] succeed!" << endl;
 		else cout << "[delete meeting] error : you don't have such a meeting!" << endl;

 	} else if (s == "da") {
 		cout << "[delete all meeting] succeed!" << endl;
 		test.deleteAllMeetings(user);

	} else if (s == "") {
	}  else {
		cout << s << ": command not found." << endl;
	} 
	return true;
}

void AgendaUI::lm(std::list<Meeting> meetings) {
    for (auto i = meetings.begin(); i != meetings.end(); i++) { 
    	cout << left << setw(21) << i->getTitle();
        cout << setw(16) << i->getSponsor();
        cout << setw(20) << Date::dateToString(i->getStartDate());
        cout << setw(20) << Date::dateToString(i->getEndDate());
        std::vector<std::string> v = i->getParticipator();
        for (auto j = v.begin(); j != v.end();) {
            cout << *j;
            j++;
            if (j != v.end()) {
                cout << ',';
            }
        }
        cout << endl;
    }
    cout << endl;
}

