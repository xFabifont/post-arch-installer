#include "colormod.hpp"
#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

std::string parse_input(std::string);
std::string slurp(std::ifstream &in);
int menu();

int main() {

  Color::Modifier red(Color::FG_RED);
  Color::Modifier cyan(Color::FG_CYAN);
  Color::Modifier green(Color::FG_GREEN);
  Color::Modifier def(Color::FG_DEFAULT);

  std::cout << red << "##########################################" << std::endl
            << "#" << def << "           POST ARCH INSTALLER          " << red
            << "#" << std::endl
            << "##########################################" << std::endl;

  // User
  std::cout << cyan << "Creating new user" << std::endl;
  std::cout << def << "Username (default 'user'): ";
  std::string username = "user";
  username = parse_input(username);
  system(("useradd -m " + username).c_str());
  system(("passwd " + username).c_str());

  // Sudo
  std::cout << std::endl << cyan << "Adding the new user to sudoers" << def << std::endl;

  std::ifstream sudoers;
  sudoers.open("/etc/sudoers");
  std::string temp_sudoers = slurp(sudoers);
  sudoers.close();

  int pos = temp_sudoers.find("root ALL=(ALL) ALL", 0);
  temp_sudoers.insert(pos + 19, (username + " ALL=(ALL) ALL\n").c_str());

  std::ofstream new_sudoers;
  new_sudoers.open("/etc/sudoers");
  new_sudoers << temp_sudoers;
  new_sudoers.close();

  // Minimal plasma installation
  std::cout << std::endl
            << cyan << "Minimal Plasma installation" << def << std::endl;
  system("systemctl enable dhcpcd");
  system("systemctl enable NetworkManager");
  system("systemctl start dhcpcd");
  system("systemctl start NetworkManager");

  std::ofstream res;
  res.open("res.txt");
  res << "\n\n\nY";
  res.close();

  system("locale-gen");
  system("pacman -Syyu < res.txt");
  std::string packages = "plasma konsole dolphin okular spectacle git";
  system(("pacman -S " + packages + " < res.txt").c_str());
  system("systemctl enable sddm");

  // Reboot
  std::cout << std::endl << cyan << "Now you can reboot your system by typing 'reboot'" << def << std::endl;

  return 0;
}

std::string parse_input(std::string def) {
  std::string input = "";
  std::getline(std::cin, input);

  if (input.compare("") != 0)
    return input;
  else
    return def;
}

std::string slurp(std::ifstream &in) {
  std::stringstream sstr;
  sstr << in.rdbuf();
  return sstr.str();
}

int menu() { return 0; }