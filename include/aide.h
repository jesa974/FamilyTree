typedef struct date {
	short day;
	short month;
	short year;
}date;

typedef struct event {
	char *location;
	date time;
}event;

typedef struct weddData {
	struct individual *husband;
	struct individual *wife;
	event wedd;
}weddData;

typedef struct weddLink {
	struct wedding *mariage;
	struct weddLink *next;
}weddLink;

typedef struct indivData {
	short num_gen;
	char *name;
	char *surname;
	char gender;
	event birth;
	event death;
}indivData;

typedef struct childLink {
	struct individual *child;
	struct childLink *next;
}childLink;

struct individual;
struct wedding;

typedef struct individual {
	indivData infos;
	struct individual *next;
	struct individual *father;
	struct individual *mother;
	childLink *firstChild;
	weddLink *firstUnion;
}individual;

typedef struct wedding {
	weddData info_wedding;
	struct wedding *next;
}wedding;
