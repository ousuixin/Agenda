#ifndef AGENDAUI_H
#define AGENDAUI_H

#include <iostream>
#include <string>
#include "AgendaService.hpp"

class AgendaUI {
 public:
  AgendaUI();
  void OperationLoop(void);

 private:
  /**
   * constructor
   */
  void startAgenda(void);

  /**
   * execute the operation
   * @return if the operationloop continue
   */
  void excute(void);

  /**
   * user Login
   */
  bool login(void);

  /**
   * user regist
   */
  bool regist(void);

  void print_unlog(void);

  void print_login(void);

  bool choose_func1(void);

  bool choose_func2(void);

  friend void Stop(int signo);

  void lm(std::list<Meeting>);

  std::string user;
  std::string password;
  AgendaService test;
};

#endif
