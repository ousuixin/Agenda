#include <iostream>
#include <initializer_list>
#include <string>
#include <sstream>
#include "Date.hpp"
using namespace std;

/**
* @brief default constructor
*/
Date::Date() {
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
};

/**
* @brief constructor with arguments
*/
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
};

/**
* @brief constructor with a string
*/
Date::Date(const std::string &dateString) {
    int ret = 0;
    if (dateString[0]<'0'|| dateString[0] >'9') {
        ret = 1;
    } 
    if (dateString[1]<'0'|| dateString[1] >'9') {
        ret = 1;
    } 
    if (dateString[2]<'0'|| dateString[2] >'9') {
        ret = 1;
    } 
    if (dateString[3]<'0'|| dateString[3] >'9') {
        ret = 1;
    }     
    if (dateString[4] != '-') {
        ret = 1;
    }

    if (dateString[5]<'0'|| dateString[5] >'9') {
        ret = 1;
    } 
    if (dateString[6]<'0'|| dateString[6] >'9') {
        ret = 1;
    } 
    if (dateString[7] != '-') {
        ret = 1;
    }

    if (dateString[8]<'0'|| dateString[8] >'9') {
        ret = 1;
    } 
    if (dateString[9]<'0'|| dateString[9] >'9') {
        ret = 1;
    } 
    if (dateString[10] != '/') {
        ret = 1;
    }

    if (dateString[11]<'0'|| dateString[11] >'9') {
        ret = 1;
    } 
    if (dateString[12]<'0'|| dateString[12] >'9') {
        ret = 1;
    } 
    if (dateString[13] != ':') {
        ret = 1;
    }

    if (dateString[14]<'0'|| dateString[14] >'9') {
        ret = 1;
    } 
    if (dateString[15]<'0'|| dateString[15] >'9') {
        ret = 1;
    }
    if (!ret) {
        stringstream ss;
        ss << dateString;
        int a[5];
        int i = 0;
        int num = 0;
        char c;
        while (ss >> num) {
            ss.clear();
            ss >> c;
            a[i] = num;
            i++;
        }
        m_year = a[0];
        m_month = a[1];
        m_day = a[2];
        m_hour = a[3];
        m_minute = a[4];
    } else {
        m_year = 0;
        m_month = 0;
        m_day = 0;
        m_hour = 0;
        m_minute = 0;       
    }
    //may i should check whether the length of the string is bigger than 15
};
/**
* @brief return the year of a Date
* @return   a integer indicate the year of a date
*/
int Date::getYear(void) const {
    return m_year;
};

/**
* @brief set the year of a date
* @param a integer indicate the new year of a date
*/
void Date::setYear(const int t_year) {
    m_year = t_year;
};

/**
* @brief return the month of a Date
* @return   a integer indicate the month of a date
*/
int Date::getMonth(void) const {
    return m_month;
};

/**
* @brief set the month of a date
* @param a integer indicate the new month of a date
*/
void Date::setMonth(const int t_month) {
    m_month = t_month;
};

/**
* @brief return the day of a Date
* @return   a integer indicate the day of a date
*/
int Date::getDay(void) const {
    return m_day;
};

/**
* @brief set the day of a date
* @param a integer indicate the new day of a date
*/
void Date::setDay(const int t_day) {
    m_day = t_day;
};

/**
* @brief return the hour of a Date
* @return   a integer indicate the hour of a date
*/
int Date::getHour(void) const {
    return m_hour;
};

/**
* @brief set the hour of a date
* @param a integer indicate the new hour of a date
*/
void Date::setHour(const int t_hour) {
    m_hour = t_hour;
};

/**
* @brief return the minute of a Date
* @return   a integer indicate the minute of a date
*/
int Date::getMinute(void) const {
    return m_minute;
};

/**
* @brief set the minute of a date
* @param a integer indicate the new minute of a date
*/
void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
};


bool is_leap(int year) {
    if (year%4 == 0) {
        if (year%100 == 0) {
            if (year%400 == 0) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    } else {
        return false;
    }
}

/**
*   @brief check whether the date is valid or not
*   @return the bool indicate valid or not
*/
bool Date::isValid(const Date &t_date) {
    if (t_date.m_year > 9999 || t_date.m_year < 1000) {
        return false;
    }
    if (t_date.m_hour > 23 || t_date.m_hour < 0) {
        return false;
    } 
    if (t_date.m_minute > 59 || t_date.m_minute < 0) {
        return false;
    }
    if (t_date.m_month > 12 || t_date.m_month < 1) {
        return false;
    }
    if (t_date.m_month == 4 || t_date.m_month == 6 || t_date.m_month == 9 || t_date.m_month == 11) {
        if (t_date.m_day > 30 || t_date.m_day < 1) {
            return false;
        }
    } else if (t_date.m_month == 2) {
        if (is_leap(t_date.m_year)) {
            if (t_date.m_day > 29 || t_date.m_day < 1) {
                return false;
            }
        } else {
            if (t_date.m_day > 28 || t_date.m_day < 1) {
                return false;
            }
        }
    } else {
        if (t_date.m_day > 31 || t_date.m_day < 1) {
            return false;
        }
    }
    return true;
};


/**
* @brief convert a string to date, if the format is not correct return
* 0000-00-00/00:00
* @return a date
*/
Date Date::stringToDate(const std::string &t_dateString) {
    Date c;
    if (t_dateString[0]<'0'|| t_dateString[0] >'9') {
        return c;
    } 
    if (t_dateString[1]<'0'|| t_dateString[1] >'9') {
        return c;
    } 
    if (t_dateString[2]<'0'|| t_dateString[2] >'9') {
        return c;
    } 
    if (t_dateString[3]<'0'|| t_dateString[3] >'9') {
        return c;
    }     
    if (t_dateString[4] != '-') {
        return c;
    }

    if (t_dateString[5]<'0'|| t_dateString[5] >'9') {
        return c;
    } 
    if (t_dateString[6]<'0'|| t_dateString[6] >'9') {
        return c;
    } 
    if (t_dateString[7] != '-') {
        return c;
    }

    if (t_dateString[8]<'0'|| t_dateString[8] >'9') {
        return c;
    } 
    if (t_dateString[9]<'0'|| t_dateString[9] >'9') {
        return c;
    } 
    if (t_dateString[10] != '/') {
        return c;
    }

    if (t_dateString[11]<'0'|| t_dateString[11] >'9') {
        return c;
    } 
    if (t_dateString[12]<'0'|| t_dateString[12] >'9') {
        return c;
    } 
    if (t_dateString[13] != ':') {
        return c;
    }

    if (t_dateString[14]<'0'|| t_dateString[14] >'9') {
        return c;
    } 
    if (t_dateString[15]<'0'|| t_dateString[15] >'9') {
        return c;
    } 
    Date b(t_dateString);
    return b;
};

/**
* @brief convert a date to string, if the format is not correct return
* 0000-00-00/00:00
*/
std::string Date::dateToString(const Date &t_date) {
    if (isValid(t_date)) {
        string s;
        stringstream ss;
        ss << t_date.m_year << '-';
        if (t_date.m_month > 9) {
            ss << t_date.m_month << '-';
        } else {
            ss << '0' << t_date.m_month << '-';
        }
        if (t_date.m_day > 9) {
            ss << t_date.m_day << '/';
        } else {
            ss << '0' << t_date.m_day << '/';
        }
        if (t_date.m_hour > 9) {
            ss << t_date.m_hour << ':';
        } else {
            ss << '0' << t_date.m_hour << ':';
        }
        if (t_date.m_minute > 9) {
            ss << t_date.m_minute;
        } else {
            ss << '0' << t_date.m_minute;
        }
        ss >> s;
        return s;
    } else {
        return "0000-00-00/00:00";
    }
    
};

/**
*  @brief overload the assign operator
*/
Date & Date::operator=(const Date &t_date) {
    m_year = t_date.m_year;
    m_month = t_date.m_month;
    m_day = t_date.m_day;
    m_hour = t_date.m_hour;
    m_minute = t_date.m_minute;
    return *this;
};

/**
* @brief check whether the CurrentDate is equal to the t_date
*/
bool Date::operator==(const Date &t_date) const {
    if (m_year==t_date.m_year && m_month==t_date.m_month 
        && m_day==t_date.m_day && m_hour==t_date.m_hour 
        && m_minute==t_date.m_minute) {
        return true;
    } else {
        return false;
    }
};

/**
* @brief check whether the CurrentDate is  greater than the t_date
*/
bool Date::operator>(const Date &t_date) const {
    if (m_year > t_date.m_year) {
        return true;
    } else if (m_year == t_date.m_year) {
        if (m_month > t_date.m_month) {
            return true;
        } else if (m_month == t_date.m_month) {
            if (m_day > t_date.m_day) {
                return true;
            } else if (m_day == t_date.m_day) {
                if (m_hour > t_date.m_hour) {
                    return true;
                } else if (m_hour == t_date.m_hour) {
                    if (m_minute > t_date.m_minute) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
};

/**
* @brief check whether the CurrentDate is  less than the t_date
*/
bool Date::operator<(const Date &t_date) const {
    if (*this > t_date || *this == t_date) {
        return false;
    } else {
        return true;
    }
};

/**
* @brief check whether the CurrentDate is  greater or equal than the t_date
*/
bool Date::operator>=(const Date &t_date) const {
    if (*this > t_date || *this == t_date) {
        return true;
    } else {
        return false;
    }
};

/**
* @brief check whether the CurrentDate is  less than or equal to the t_date
*/
bool Date::operator<=(const Date &t_date) const {
    if (*this < t_date || *this == t_date) {
        return true;
    } else {
        return false;
    }
};



