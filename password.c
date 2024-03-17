#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 300
#define flushBuffer while ( getchar() != '\n' )

int main() {
    char user_input[LENGTH]; 
    char master[LENGTH];
    char login[LENGTH];
    char choice;
    char getstored[LENGTH];
    char change[LENGTH];
    int c;
    int flag = 0; 
    
    
    FILE *flag_file = fopen("flag.txt", "r");
    if (flag_file != NULL) {                     // see if the user has already set his master password
        fscanf(flag_file, "%d", &flag);
        fclose(flag_file);
    }
    else {                                      // if not, we create a new file to keep its value
        flag_file = fopen("flag.txt", "w");
        fclose(flag_file);
    }
    
    FILE *MasterDefined;
    FILE *Content;
    
    if (!flag) {       // if master password hasnt been set yet:
        printf("Define Your Master Password: "); 
        scanf("%s",master);
        flushBuffer;
        MasterDefined = fopen("master.txt", "w");
        fputs(master, MasterDefined);    // write the defined password in the text file
        fclose(MasterDefined);
        flag_file = fopen("flag.txt", "a");    // set flag to 1, it means we have already set the password
        fprintf(flag_file, "%d", 1);
        fclose(flag_file);
        flag = 1;
    }   
    
    MasterDefined = fopen("master.txt", "r");  
    fgets(getstored, LENGTH, MasterDefined);    // we get the defined password in a variable, to compare with the user's password
    getstored[strcspn(getstored, "\n")] = '\0';
    fclose(MasterDefined);
    
    printf ("Enter your Password Master: ");  // password confirmation
    try:  
    scanf("%s", login);
    flushBuffer;
        
    if (!strcmp(login,getstored)){    // if it matches the password, we keep going through the code
        Content = fopen("historic.txt", "a+");
        
        while (1) {
            
            printf("\n---------------------\nPress 'x' to exit\n1. Add Password\n2. List Passwords\n3. Change Password\n4. Delete Password\n\nChoice: "); 
            scanf(" %c", &choice );
            flushBuffer;
            switch (choice){    // define new passwords for any app name
                case '1':{
                    fputs("\nApp: ", Content);
                    printf("App: ");
                    fgets(user_input, LENGTH, stdin); 
                    fputs(user_input, Content);
                    
                    fputs("Password: ", Content);
                    printf("Password: ");
                    fgets(user_input, LENGTH, stdin); 
                    fputs(user_input, Content);
                    fflush(Content);
                    break;
                }
                case '2':{    // print all the passwords the user has
                    rewind(Content);
                    if (fgetc(Content) == EOF) {
                        printf("\nYou don't have any password yet,\nPress '1' to add a Password\n");
                    }
                    rewind(Content);
                    while ((c = getc(Content)) != EOF) {
                    putchar(c);
                    }
                    break;
                }
                case '3':{    // change Master password
                    remove("master.txt");
                    FILE *NEW_MASTER = fopen("master.txt", "w");
                    printf("\nDefine your new Master password: ");
                    scanf("%s",change);
                    flushBuffer;
                    printf("\nPassword successfully set.\n");
                    fputs(change, NEW_MASTER);
                    fclose(NEW_MASTER);
                    break;
                }
                case '4': {    // delete apps and passwords
                    char delete[LENGTH];
                    char line[LENGTH];
                    printf("\nEnter the app name to delete: ");
                    fgets(delete, LENGTH, stdin);
                    delete[strcspn(delete, "\n")] = '\0';
                    FILE *tempFile = fopen("temp.txt", "w");
                    rewind(Content);
                    while (fgets(line, sizeof(line), Content)) {
                        if (strstr(line, delete) != NULL) {      
                            fgets(line, sizeof(line), Content);    
                            fgets(line, sizeof(line), Content);
                        }       
                        else {
                            fputs(line, tempFile);      
                        }
                    }
                    fclose(tempFile);
                    fclose(Content);
                    remove("historic.txt");
                    rename("temp.txt", "historic.txt");
                    Content = fopen("historic.txt", "a+");
                    printf("\nApp '%s' and its password deleted successfully.\n", delete);
                    break;
                }
                case 'x': {
                    fclose(Content);
                    return 0;
                }
                default: {    // if nothing matches, go back to the while loop
                    break;
                }
            }   
        }
    }
    else {        // if the Master password doesnt matches, retry
        printf("Try Again: ");
        goto try;
    }
    fclose(Content);
    return 0;
}