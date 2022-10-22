#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

// Functia pentru crearea unui nou fisier
void touch (Dir* parent, char* name) {
	File* new_file;
	File* curr_file = parent -> head_children_files;
	int same_name = 0, there_are_files = 0;
	// Verificare daca exista un fisier cu numele nou
	while (curr_file != NULL) {
		there_are_files = 1;
		if (strcmp(curr_file -> name, name) == 0) {
			same_name = 1;
		}
		curr_file = curr_file -> next;
	}
	Dir* curr_director;
	curr_director = parent -> head_children_dirs;
	// Verificare daca exista deja un director cu numele nou
	while (curr_director != NULL) {
		if (strcmp(curr_director -> name, name) == 0) {
			same_name = 1;
		}
		curr_director = curr_director -> next;
	}

	if (same_name == 1) {
		// Exista deja un fisier/director cu numele nou
		printf("File already exists\n");
	} else {
		// Nu exista un fisier/director cu numele nou
		new_file = malloc(sizeof(File));
		new_file -> name = malloc((strlen(name) + 1) * sizeof(char));
		memcpy(new_file -> name, name, strlen(name) + 1);
		new_file -> parent = parent;
		new_file -> next = NULL;
		if (there_are_files == 0) { // e goala lista de fisiere
			parent -> head_children_files = new_file;
		} else {
			curr_file = parent -> head_children_files;
			while (curr_file -> next != NULL) {
				curr_file = curr_file -> next;
			}
			curr_file -> next = new_file;
		}
	}
}

// Functia pentru crearea unui nou director
void mkdir (Dir* parent, char* name) {
	Dir* new_director;
	Dir* curr_director;
	curr_director = parent -> head_children_dirs;
	int same_name = 0, there_are_dirs = 0;
	// Verificare daca exista deja un director cu numele nou
	while (curr_director != NULL) {
		there_are_dirs = 1;
		if (strcmp(curr_director -> name, name) == 0) {
			same_name = 1;
		}
		curr_director = curr_director -> next;
	}
	File* curr_file;
	curr_file = parent -> head_children_files;
	// Verificare daca exista un fisier cu numele nou
	while (curr_file != NULL) {
		if (strcmp(curr_file -> name, name) == 0) {
			same_name = 1;
		}
		curr_file = curr_file -> next;
	}
	if (same_name == 1) {
		// Exista deja un fisier/director cu numele nou 
		printf("Directory already exists\n");
	} else {
		// Nu exista un fisier/director cu numele nou
		new_director = malloc(sizeof(Dir));
		new_director -> name = malloc((strlen(name) + 1) * sizeof(char));
		memcpy(new_director -> name, name, strlen(name) + 1);
		new_director -> parent = parent;
		new_director -> head_children_dirs = NULL;
		new_director -> head_children_files = NULL;
		new_director -> next =  NULL;
		if (there_are_dirs == 0) { // e goala lista de directoare
			parent -> head_children_dirs = new_director;
		} else {
			curr_director = parent -> head_children_dirs;
			while (curr_director -> next != NULL) {
				curr_director = curr_director -> next;
			}
			curr_director -> next = new_director;
		}
	}
}

// Functie pentru printarea continutului unui director
void ls (Dir* parent) {
	Dir* curr_director = parent -> head_children_dirs;
	File* curr_file = parent -> head_children_files;
	// Afisez toate numele directoarelor din director
	while (curr_director != NULL) {
		printf("%s\n", curr_director -> name);
		curr_director = curr_director -> next;
	}
	// Afisez toate numele fisierelor din director
	while (curr_file != NULL) {
		printf("%s\n", curr_file -> name);
		curr_file = curr_file -> next;
	}
}

// Functie pentru eliminarea unui fisier
void rm (Dir* parent, char* name) {
	File* curr_file = parent -> head_children_files;
	File* del_file;
	del_file = NULL;
	if (curr_file != NULL && strcmp(curr_file -> name, name) == 0) {
		// Verific daca primul fisier e cel cautat
		del_file = curr_file;
		parent -> head_children_files = curr_file -> next;
	} else if (curr_file != NULL && curr_file -> next != NULL) {
		// Verific daca gasesc fisierul cautat in lista de fisiere
		while (curr_file -> next != NULL) {
			if (strcmp(curr_file -> next -> name, name) == 0) {
				del_file = curr_file -> next;
				if (curr_file -> next -> next != NULL) {
					// Fisierul cautat nu se afla pe ultima pozitie
					curr_file -> next = curr_file -> next -> next;
					break;
				} else {
					// Fisierul cautat se afla pe ultima pozitie
					curr_file -> next = NULL;
					break;
				}
			}
			curr_file = curr_file -> next;
		}
	}
	if (del_file == NULL) {
		// Nu am gasit niciun fisier cu numele cautat
		printf("Could not find the file\n");
	} else {
		// Am gasit fisierul, eliberez memoria
		free(del_file -> name);
		free(del_file);
	}
}

// Functie folosita recursiv pentru eliberarea unui director
void free_director(Dir* del_director) {
	if(del_director == NULL) {
		return;
	}
	Dir* curr_director = del_director -> head_children_dirs;
	File* curr_file = del_director -> head_children_files;
	free(del_director -> name);
	// Apelez functia pentru fiecare director-copil al directorului primit ca parametru
	while (curr_director != NULL) {
		Dir* aux_d ;
		aux_d = curr_director;
		curr_director = curr_director -> next;
		free_director(aux_d);
		free(aux_d);
	}
	// Elimin fisierele din director
	while (curr_file != NULL) {
		File* aux_f ;
		aux_f = curr_file;
		curr_file = curr_file -> next;
		free(aux_f -> name);
		free(aux_f);
	}
}

// Functie care gaseste directorul cautat 
// si apeleaza functia de eliberare a directorului recursiva
void rmdir (Dir* parent, char* name) {
	Dir* curr_director = parent -> head_children_dirs;
	Dir* del_director;
	del_director = NULL;
	if (curr_director != NULL && strcmp(curr_director -> name, name) == 0) {
		// Verific daca primul director este cel cautat
		del_director = curr_director;
		parent -> head_children_dirs = curr_director -> next;
	} else if (curr_director != NULL && curr_director -> next != NULL) {
		// Verific daca directorul cautat este in lista de directoare
		while (curr_director -> next != NULL) {
			if (strcmp(curr_director -> next -> name, name) == 0) {
				del_director = curr_director -> next;
				if (curr_director -> next -> next != NULL) {
					// Directorul cautat nu se afla pe ultima pozitie
					curr_director -> next = curr_director -> next -> next;
					break;
				} else {
					// Directorul cautat se afla pe ultima pozitie
					curr_director -> next = NULL;
					break;
				}
			}
			curr_director = curr_director -> next;
		}
	}
	if (del_director == NULL) {
		// Nu am gasit niciun director cu numele cautat
		printf("Could not find the dir\n");
	} else {
		// Am gasit directorul, apelez functia recursiva
		free_director(del_director);
		free(del_director);
	}
}

// Functie schimbare director curent
void cd(Dir** target, char *name) {
	// Daca primim comanda de a merge mai sus in ierarhie
	if (strcmp(name, "..") == 0) {
		// Daca ne aflam in directorul "home"
		if ((*target) -> parent != NULL) {
			(*target) = (*target) -> parent;
		}
	} else {
		// Cautam directorul destinatie in copii directorului in care ne aflam
		Dir* curr_director = (*target) -> head_children_dirs;
		int same_name = 0;
		while (curr_director != NULL && same_name == 0) {
			if (strcmp(curr_director -> name, name) == 0) {
				same_name = 1;
				(*target) = curr_director;
				break;
			}
			curr_director = curr_director -> next;
		}
		if (same_name == 0) {
			// Nu am gasit directorul destinatie
			printf("No directories found!\n");
		}
	}
}

// Functie printare cale director curent
char *pwd (Dir* target) {
	if (target -> parent == NULL) {
		// Ne aflam in "home"
		char *str = malloc(6);
		memcpy(str, "/home", 5);
		return str;
	} else {
		// Nu ne aflam in "home"
		Dir* curr_director = target;
		char *str = malloc(strlen(curr_director -> name) + 2);
		memcpy(str, "/", 1);
		strcat(str, curr_director -> name);
		// Adaugam la nume directorul parinte si mergem in sus in ierarhie
		while (curr_director -> parent != NULL) {
			char *temp = malloc(strlen(curr_director -> parent -> name) + strlen(str) + 2);
			memcpy(temp, "/", 1);
			strcat(temp, curr_director -> parent -> name);
			str = realloc(str, strlen(temp)+ strlen(str) + 2);
			strcat(temp, str);
			memcpy(str, temp, strlen(temp) + 1);
			free(temp);
			curr_director = curr_director -> parent;
		}
		return str;
	}
}

// Functie de incheiere a programului care apeleaza functia recursiva de eliminare a directorului
void stop (Dir* target) {
	free_director(target);
	free(target);
}

// Functie de printare a arborelui de fisiere, apelata recursiv
void tree (Dir* target, int level) {
	Dir* next_director = target -> head_children_dirs;
	int copy_level;
	// Daca avem copii-directori, apelam functia pe acestia, afisand numele acestora
	while (next_director != NULL) {
		copy_level = 4 * level;
		while (copy_level > 0) {
			printf(" ");
			copy_level--;
		}
		printf("%s\n", next_director -> name);
		tree(next_director, level + 1);
		next_director = next_director -> next;
	}
	File* curr_file = target -> head_children_files;
	// Nu mai avem directoare, afisam numele fisierelor
	while (curr_file != NULL) {
		copy_level = 4 * level;
		while (copy_level > 0) {
			printf(" ");
			copy_level--;
		}
		printf("%s\n", curr_file -> name);
		curr_file = curr_file -> next;
	}
	
}

// Functie de schimbare a numelui unui fisier/director
void mv(Dir* parent, char *oldname, char *newname) {
	Dir* curr_director = parent -> head_children_dirs;
	File* curr_file = parent -> head_children_files;
	Dir* aux_director = NULL;
	File* aux_file = NULL;
	int aux = 0, one_file = 0, one_dir = 0;
	// Verificam daca exista deja un director cu numele nou
	while (curr_director != NULL) {
		if (strcmp(curr_director -> name, newname) == 0) {
			aux = 1;
		}
		curr_director = curr_director -> next;
	}
	// Verificam daca exista deja un fisier cu numele nou
	while (curr_file != NULL) {
		if (strcmp(curr_file -> name, newname) == 0) {
			aux = 1;
		}
		curr_file = curr_file -> next;
	}
	// Nu am gasit un fisier cu noua denumire
	if (aux == 0) {
		curr_director = parent -> head_children_dirs;
		curr_file = parent -> head_children_files;
		if (curr_director != NULL) {
			// Verificam daca primul director este cel cautat
			if (strcmp(curr_director -> name, oldname) == 0) {
				aux_director = curr_director;
				if (curr_director -> next == NULL) {
					// Este singurul director din lista
					one_dir = 1;
				} else {
					// Nu este singurul director din lista
					parent -> head_children_dirs = curr_director -> next;
				}
				aux_director -> next = NULL;
			}
			// Daca nu primul director este cel cautat, cautam in restul directoarelor
			while (curr_director -> next != NULL && aux_director == NULL) {
				if (strcmp(curr_director -> next -> name, oldname) == 0) {
					aux_director = curr_director -> next;
					aux_director -> next = NULL;
					if (curr_director -> next -> next ==  NULL) {
						// Directorul cautat este la finalul listei de directoare
						curr_director -> next = NULL;
						break;
					} else {
						// Directorul cautat nu este la finalul listei de directoare
						curr_director -> next = curr_director -> next -> next;
					}
					curr_director -> next = curr_director -> next -> next;
				}
				curr_director = curr_director -> next;
			}
		}
		if (curr_file != NULL) {
			// Verificam daca primul fisier este cel cautat
			if (strcmp(curr_file -> name, oldname) == 0) {
				aux_file = curr_file;
				if (curr_file -> next == NULL) {
					// Este singurul fisier din lista
					one_file = 1;
				} else {
					// Nu este singurul fisier din lista
					parent -> head_children_files = curr_file -> next;
				}
				aux_file -> next = NULL;
			}
			// Daca nu primul fisier este cel cautat, cautam in restul fisierelor
			while (curr_file -> next != NULL && aux_file == NULL) {
				if (strcmp(curr_file -> next -> name, oldname) == 0) {
					aux_file = curr_file -> next;
					aux_file -> next = NULL;
					if (curr_file -> next -> next ==  NULL) {
						// Fisierul cautat este la finalul listei de fisiere
						curr_file -> next = NULL;
						break;
					} else {
						// Fisierul cautat este la finalul listei de fisiere
						curr_file -> next = curr_file -> next -> next;
						break;
					}
				}
				curr_file = curr_file -> next;
			}
		}
	}
	if (aux == 1)  {
		// Am gasit un director/ fisier cu numele nou
		printf("File/Director already exists\n");
	} else if (aux_director == NULL && aux_file == NULL) {
		// Nu am gasit directorul/fisierul cu vechiul nume
		printf("File/Director not found\n");
	} else if (one_dir == 1) {
		// Exista un singur director in lista de directoare, fix cel cautat, redenumim
		memcpy(aux_director -> name, newname, strlen(newname));
		parent ->head_children_dirs = aux_director;
	} else if (one_file == 1) {
		// Exista un singur fisier in lista de fisiere, fix cel cautat, redenumim
		memcpy(aux_file -> name, newname, strlen(newname));
		parent -> head_children_files = aux_file;
	} else if (aux_director != NULL) {
		// Am gasit directorul cautat, il adaugam la finalul listei de directoare, redenumim
		curr_director = parent -> head_children_dirs;
		while (curr_director -> next != NULL) {
			curr_director = curr_director -> next;
		}
		memcpy(aux_director -> name, newname, strlen(newname));
		curr_director -> next = aux_director;
	} else if (aux_file != NULL) {
		curr_file = parent -> head_children_files;
		// Am gasit fisierul cautat, il adaugam la finalul listei de fisiere, redenumim
		while (curr_file -> next != NULL) {
			curr_file = curr_file -> next;
		}
		memcpy(aux_file -> name, newname, strlen(newname));
		curr_file -> next = aux_file;
	}
}

int main () {
	
	// Crearea directorului radacina, "home"
	Dir *this_director = malloc(sizeof(Dir));
	this_director -> name = malloc(5 * sizeof(char));
	memcpy(this_director -> name, "home", 4);
	this_director -> parent = NULL;
	this_director -> head_children_dirs = NULL;
	this_director -> head_children_files = NULL;
	this_director -> next = NULL;
	// Director auxiliar care tine adresa directorului "home"
	// pentru momentul in care se apeleaza "stop"
	Dir* home_director = this_director;
	int aux = 0;
	do
	{
		// Citire de la tastatura a comenzii si (eventual) a numelor
		char *input = malloc(sizeof(char) * MAX_INPUT_LINE_SIZE);
		char *name = malloc(sizeof(char) * MAX_INPUT_LINE_SIZE);
		scanf("%s", input);
		if (strcmp(input, "touch") == 0) {
			scanf("%s", name);
			touch(this_director, name);
		} else if (strcmp(input, "mkdir") == 0) {
			scanf("%s", name);
			mkdir(this_director, name);
		} else if (strcmp(input, "ls") == 0) {
			ls(this_director);
		} else if (strcmp(input, "rm") == 0) {
			scanf("%s", name);
			rm(this_director, name);
		} else if (strcmp(input, "rmdir") == 0) {
			scanf("%s", name);
			rmdir(this_director, name);
		} else if (strcmp(input, "cd") == 0) {
			scanf("%s", name);
			cd(&this_director, name);
		} else if (strcmp(input, "tree") == 0) {
			tree(this_director, 0);
		} else if (strcmp(input, "pwd") == 0) {
			char *name2;
			name2 = pwd(this_director);
			printf("%s\n", name2);
			free(name2);
		} else if (strcmp(input, "stop") == 0) {
			stop(home_director);
			aux = 1;
		} else if (strcmp(input, "mv") == 0) {
			char *name2 = malloc(sizeof(char) * MAX_INPUT_LINE_SIZE);
			scanf("%s", name);
			scanf("%s", name2);
			mv(this_director, name, name2);
			free(name2);
		}
		free(name);
		free(input);
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/
	} while (aux == 0);
	
	return 0;
}
