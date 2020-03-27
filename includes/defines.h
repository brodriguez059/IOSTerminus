#define ROOT 0
#define GAME 1
#define CMDS 2

#define STDIN 0
#define STDOUT 1
#define STERR 2

typedef struct {
  char name[512];
  int length;
} DirInfo;