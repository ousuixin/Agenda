#include "AgendaService.hpp"
#include "Date.hpp"
#include "User.hpp"
#include "Storage.hpp"
#include "Meeting.hpp"
#include "AgendaUI.hpp"
#include <iomanip>
#include <iostream>
using namespace std;


AgendaUI::AgendaUI() {
	user = "";
	password = "";
};

void AgendaUI::startAgenda(void){};

void AgendaUI::OperationLoop(void) {
	print_unlog();
	while (choose_func1()) {
		print_unlog();
	}
};

void AgendaUI::print_unlog(void) {
	cout << endl;
	cout << "--------------------------------Agenda--------------------------------" << endl;
	cout << "Action :                                                              " << endl;
	cout << "l    - log in Agenda by username and password                         " << endl;
	cout << "r    - register an Agenda acount                                      " << endl;
	cout << "q    - quit Agenda                                                    " << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << endl;
	cout << "Agenda :~$ ";
}

void AgendaUI::print_login(void) {
	cout << endl;
	cout << "--------------------------------Agenda--------------------------------" << endl;
	cout << "Action :                                                              " << endl;
	cout << "o    - log out Agenda                                                 " << endl;
	cout << "dc   - delete Agenda acount                                           " << endl;
	cout << "lu   - list all Agenda user                                           " << endl;
	cout << "cm   - create a meeting                                               " << endl;
	cout << "amp  - add meeting participator                                       " << endl;
	cout << "rmp  - remove meeting participator                                    " << endl;
	cout << "rqm  - request to quit meeting                                        " << endl;
	cout << "la   - list all meetings                                              " << endl;
	cout << "las  - list all sponsor meetings                                      " << endl;
	cout << "lap  - list all participator meetings                                 " << endl;
	cout << "qm   - query meeting by title                                         " << endl;
	cout << "qt   - query by time interval                                         " << endl;
	cout << "dm   - delete meeting by title                                        " << endl;
	cout << "da   - delete all meetings                                            " << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << endl;
};

void AgendaUI::excute(void) {
	cout << "succeed!" << endl;
	print_login();
	cout << "Agenda@" << user << " :~# ";
	while (choose_func2()) {
		cout << "Agenda@" << user << " :~# ";
	}
}

bool AgendaUI::login(void) {
	cout << "[log in] [username] [password]" << endl;  
	cout << "[log in] ";
	string s1,s2;
	cin >> s1 >> s2;
	cout << "[log in] ";
	if (test.userLogIn(s1,s2)) {
		user = s1; password = s2;
		return true;
	}
	return false;
}

bool AgendaUI::regist(void) {
	cout << "[register] [username] [password] [email] [phone]" << endl;
	cout << "[register] ";
	string s1,s2,s3,s4;
	cin >> s1 >> s2 >> s3 >> s4;
	cout << "[register] ";
	if (test.userRegister(s1,s2,s3,s4)) {
		user = s1; password = s2;
		return true;
	}
	return false;
}

bool AgendaUI::choose_func1(void) {
	char c;
	cin >> c;
	cout << endl;
	if (c == 'l') {
		if (login()) {
			excute();
		} else {
			cout << "Password error or user doesn't exist. So we go to start." 
				 << '\n' << endl;
		}
	} else if (c == 'r') {
		
		if (regist()) {
			excute();
		} else {
			cout << "This username has been registered. So we go to the start"
				 << '\n' << endl;
		}
	} else if (c == 'q') {
		cout << "-------------------------------Good bye-------------------------------" 
			 << '\n' << endl;
		return false;
	} else {
		cout << "[error] your input is error, then we can go to the start."
			 << '\n' << endl;
	}
	return true;
}

bool AgendaUI::choose_func2(void) {
	string s;
	cin >> s;
	cout << endl;

	if (s == "o") {
		return false;

	} else if (s == "dc") {
		cout << "[delete Agenda acount] succeed!" << endl;
		test.deleteUser(user,password);
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
		int number;
		string temp, startDate, endDate, title;
		std::vector<string> v;

		cout << "[create meeting] [the number of the participators]" << endl;
		cout << "[create meeting] ";
		cin >> number;
		for (int i = 0; i < number; i++) {
			cout << "[create meeting] [please enter the participator " << i+1 << "]" << endl;
			cout << "[create meeting] ";  
			cin >> temp;
			v.push_back(temp);
		}
		cout << "[create meeting] [title][start time(yyyy-mm-dd/hh:mm)][end time(yyyy-mm-dd/hh:mm)]"
			 << endl;
		cout << "[create meeting] ";
		cin >> title >> startDate >> endDate;

		if (test.createMeeting(user, title, startDate, endDate, v)) 
			cout << "[create meeting] succeed!" << endl;
		else cout << "[create meeting] error!" << endl;

	} else if (s == "amp") {
		string participator, title;
		cout << "[add meeting participator] [meeting title] [participator username]" << endl;
		cout << "[add meeting participator] ";
		cin >> title >> participator;
		if (test.addMeetingParticipator(user, title, participator)) 
			cout << "[add meeting participator] succeed!" << endl;
		else cout << "[add meeting participator] error!" << endl;

 	} else if (s == "rmp") {
 		string participator, title;
 		cout << "[remove meeting participator] [meeting title] [participator username]" << endl;
 		cout << "[remove meeting participator] ";
 		cin >> title >> participator;
 		if (test.removeMeetingParticipator(user, title, participator)) 
 			cout << "[remove meeting participator] succeed!" << endl;
 		else cout << "[remove meeting participator] error!" << endl;

 	} else if (s == "rqm") {
 		string title;
 		cout << "[quit meeting] [meeting title]" << endl;
 		cout << "[quit meeting] ";
 		cin >> title;
 		if (test.quitMeeting(user, title)) 
 			cout << "[quit meeting] succeed!" << endl;
 		else cout << "[quit meeting] error!" << endl;

 	} else if (s == "la") {
 		cout << "[list all meeting]" << '\n' << endl;
 		cout << "title           " << "sponsor         "
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
		cin >> title;
		std::list<Meeting> meetings = test.meetingQuery(user, title);
		lm(meetings);

	} else if (s == "qt") {
		string startDate, endDate;
		cout << "[query by time interval] [start time(yyyy-mm-dd/hh:mm)][end time(yyyy-mm-dd/hh:mm)]" << endl;
		cout << "[query by time interval] ";
		cin >> startDate >> endDate;
		std::list<Meeting> meetings = test.meetingQuery(user, startDate, endDate);
		lm(meetings);

 	} else if (s == "dm") {
 		string title;
 		cout << "[delete meeting] [ title] " << endl;
 		cout << "[delete meeting] ";
 		cin >> title;
 		if (test.deleteMeeting(user, title)) 
 			 cout << "[delete meeting] succeed!" << endl;
 		else cout << "[delete meeting] error!" << endl;

 	} else if (s == "da") {
 		cout << "[delete all meeting] succeed!" << endl;
 		test.deleteAllMeetings(user);

	} 
	return true;
}

void AgendaUI::lm(std::list<Meeting> meetings) {
    for (auto i = meetings.begin(); i != meetings.end(); i++) { 
    	cout << left << setw(16) << i->getTitle();
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


