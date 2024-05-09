#define LNAME_LENGTH 60

typedef struct playlist playlist;

struct song {
  char* style;
  char* name;
  char* singer;
  int year;
};

struct song_node {
  song* data;
  song_node* next;
  song_node* prev;
  playlist* parent;
};

struct playlist {
  int songnumber=0;
  char* name = new char[LNAME_LENGTH];
  song_node* head;
  playlist* next;
  playlist* prev;
};

//add file definitions and functions as needed.
struct list {
  int playlist_count=0;
  playlist* head;
  void create();
  void readfromfile(playlist*);
  void writetofile();
  void addplaylist(playlist*);
  void addSongNode (song_node*, playlist*);
  FILE *songbook;
  char *filename;
  playlist* printplaylist(int* );
  song_node* printsongs(playlist* ,int*);
  void removesong(int, playlist*);
  void createrandom(playlist*, playlist*);
  void removeplaylist(int );
  song_node *givenode(int, playlist*);
  void exit();
};
