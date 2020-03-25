#include <dirent.h>     /* Defines DT_* constants */
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>

struct linux_dirent {
    long           d_ino;
    off_t          d_off;
    unsigned short d_reclen;
    char           d_name[];
};

#define BUF_SIZE 1024
#define MAX_DIRECTORIES 64
#define MAX_FILES 64

void fill_buff(char buf[], char str[]);

int main(int argc, char *argv[])
{
    int fd, nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;
    char d_type;
    struct linux_dirent *directories[64] = { 0 };
    int dir_quant = 0;
    struct linux_dirent *files[64] = { 0 };
    
    fd = openat(AT_FDCWD, ".", O_RDONLY|O_NONBLOCK|O_DIRECTORY|O_CLOEXEC);
    if (fd == -1) {
        strcpy(buf, "Failed to open\n");
        write(2, buf, strlen(buf));
    }

    
    for ( ; ; ) {
        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
        if (nread == -1) {
            strcpy(buf, "Failed to getdents\n");
            write(2, buf, strlen(buf));
        }

        if (nread == 0)
            break;

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            d_type = *(buf + bpos + d->d_reclen - 1);
            if (d_type == DT_REG) {
                for (int i = 0; i < MAX_FILES; i++) {
                    if (files[i] == NULL) {
                        files[i] = d;
                        break;
                    }
                }
            } else if (d_type == DT_DIR) {
                for (int i = 0; i < MAX_DIRECTORIES; i++) {
                    if (directories[i] == NULL) {
                        directories[i] = d;
                        dir_quant++;
						break;
                    }
                }
            }

            bpos += d->d_reclen;
        }
    }
    
    strcpy(buf, " Locations:\n");
    write(1, buf, strlen(buf));
    for (int i = 0; i < dir_quant - 2; i++) {
        strcpy(buf, directories[i]->d_name);
		if (buf[0] != '.') {
			write(1, buf, strlen(buf));
        	write(1, "\n", 1);
		}
    }
    strcpy(buf, " Items:\n");
    write(1, buf, strlen(buf));
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i] != NULL) {
        	strcpy(buf, files[i]->d_name);
    		if (buf[0] != '.') {
				write(1, buf, strlen(buf));
        		write(1, "\n", 1);
			}
        } else {
            break;
        }
    }
    

   return 0;
}
