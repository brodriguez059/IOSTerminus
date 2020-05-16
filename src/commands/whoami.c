#include <unistd.h>
#include <pwd.h> //has passwd struct
#include <string.h>
#include <sys/types.h>

int main(){
    uid_t id = geteuid(); //gets effective user id
    struct passwd *pwd = getpwuid(id); //returns a pointer to passwd struct
    write(1, pwd->pw_name, strlen(pwd->pw_name)); // pwd->pw_name get user name from passwd struct
    write(1, "\n", 1); //To create a new line
    return 0;
}