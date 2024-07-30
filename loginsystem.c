#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

#define ENTER 13
#define TAB 9
#define BACKSPACE 8

struct user {
    char name[100];
    char email[100];
    char user[100];
    char pass[100];
    char pass2[100];
    int phone;
};

void takeinput(char ch[100]) {
    fgets(ch, 100, stdin);
    ch[strcspn(ch, "\n")] = '\0';
}

void produceusername(char email[100], char name[100]) {
    int i;
    for(i = 0; i < strlen(email); i++) {
        if(email[i] == '@') {
            break;
        } else {
            name[i] = email[i];
        }
    }
    name[i] = '\0';
}

void takepass(char pws[100]) {
    int i = 0;
    char ch;
    while(1) {
        ch = getch();
        if(ch == ENTER || ch == TAB) {
            pws[i] = '\0';
            break;
        } else if(ch == BACKSPACE) {
            if(i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            pws[i++] = ch;
            printf("* \b");
        }
    }
}

int main() {
    FILE *fp;
    int opt;

    printf("  - - - - login page----\n");

    printf("1. Signup\n");
    printf("2. Login\n");
    printf("3. Exit\n");

    printf("Your choice: ");
    scanf("%d", &opt);
    fgetc(stdin);

    switch(opt) {
        case 1:
            system("cls");
            struct user p;

            printf("Enter your full name: \n");
            takeinput(p.user);

             printf("Enter the phone: \n");
            scanf("%d", &p.phone);
            fgetc(stdin);

            

            system("cls");

            printf("Enter the email:  \n");
            takeinput(p.email);

           

        retry:
            printf("Enter the password: \n");
            takepass(p.pass);

            printf("\nConfirm password: \n");
            takepass(p.pass2);

            if(strcmp(p.pass, p.pass2) != 0) {
                Beep(750, 350);
                printf("\nPasswords do not match\n");
                goto retry;
            }

            produceusername(p.email, p.name);
            printf("\nYour username: %s\n", p.name);
            strcpy(p.user, p.name); // Ensure user is set to the generated username

            fp = fopen("userdata.dat", "ab");
            if (fp == NULL) {
                printf("Error opening file\n");
                return 1;
            }
            if(fwrite(&p, sizeof(struct user), 1, fp) == 1) {
                printf("\nStorage success!\n");
                printf("Your username is %s\n", p.name);
            } else {
                printf("Sorry, storage failed\n");
            }
            fclose(fp);

            break;
        case 2:
            system("cls");
            char names[100], pasds[100];
            struct user q;
            int found = 0;

            printf("username: ");
            takeinput(names);

            printf("Password: ");
            takepass(pasds);

            fp = fopen("userdata.dat", "rb");
            if (fp == NULL) {
                printf("Error opening file\n");
                return 1;
            }

            while(fread(&q, sizeof(struct user), 1, fp)) {
                printf("Debug: Checking user %s with input %s\n", q.user, names); // Debug print
                if(strcmp(q.user, names) == 0) {
                    printf("Debug: username match\n"); // Debug print
                    if(strcmp(q.pass, pasds) == 0) {
                        printf("Congrats! Login Success :)\n");
                        printf("\nYour full name: %s\n", q.name);
                        printf("Your number: %d\n", q.phone);
                    } else {
                        printf("Login invalid!\n");
                    }
                    found = 1;
                    break;
                }
            }

            if(!found) {
                printf("user isn't registered\n");
            }

            fclose(fp);
            break;
        case 3:
            printf("Exiting...\n");
            exit(0);
            break;
        default:
            printf("Invalid option\n");
            break;
    }

    return 0;
}