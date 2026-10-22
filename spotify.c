#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h> 
#include <conio.h>

//database
struct song{
    char *songname;
    char *mood;
    char *command;  
};

void defaultplaylist(struct song data[15]);
void createplaylist(struct song data[15]);
void moodplaylist(struct song data[15]);
void queue(struct song data[15]);
void pickasong(struct song data[15]);

struct Node{
    struct song sdata;
    struct Node *next;
    struct Node *prev;
};

struct linkedlist{
    struct Node *head;
    int size;
};

void initlist(struct linkedlist *list){
    list->head = NULL;
    list->size=0;
}

void insertNode(struct linkedlist *list, struct song s) {
    struct Node *newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->sdata = s;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        struct Node *temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }
    list->size++;
}
void insertsongaftercurrent(struct linkedlist *list, struct Node* current, struct song s){
    struct Node *newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->sdata = s;
    newNode->next = NULL;
    newNode->prev = NULL;
    if(current->next!=NULL){
        newNode->next=current->next;
        newNode->prev=current;
        current->next->prev=newNode;
        current->next=newNode;
    }
    else{
        current->next=newNode;
        newNode->prev=current;
        newNode->next=NULL;
    }
}
void deleteNode(struct Node** current, struct linkedlist *list){
 
    if (current == NULL || *current == NULL || list->head == NULL) return;

    struct Node *toDelete = *current;

    // Adjust head if deleting first node
    if (toDelete == list->head) {
        list->head = toDelete->next;
    }

    // Adjust surrounding pointers
    if (toDelete->next != NULL) {
        toDelete->next->prev = toDelete->prev;
    }
    if (toDelete->prev != NULL) {
        toDelete->prev->next = toDelete->next;
    }

    // Determine what node becomes the new 'current' track
    if (toDelete->next != NULL) {
        *current = toDelete->next;
    } else {
        *current = list->head; // Fallback to start if deleting last song
    }

    free(toDelete);
    list->size--;
}


void printlist(struct linkedlist *list){
    int i=1;
    struct Node *current = list->head;
    if(current==NULL){
        printf("The playlist is empty\n");
        return;
    }
    while(current!=NULL){
        printf("%d. | %s\n",i,current->sdata.songname);
        current=current->next;
        i++;
    }
}
void playnode(struct Node *n){
    printf("Playing %s.....\n", n->sdata.songname);
    mciSendString("close music", NULL, 0, NULL);
    mciSendString(n->sdata.command, NULL, 0, NULL);
    mciSendString("play music", NULL, 0, NULL);
}
int waitforchoice(void){
    char status[32];
    while(_kbhit()) _getch();              // clear any old key presses
    while(1){
        if(_kbhit()){
            int c = _getch();
            if(c == '1' || c == '2') return c - '0';
        }
        mciSendString("status music mode", status, sizeof(status), NULL);
        if(strcmp(status, "stopped") == 0) return 0;   // song finished, no input
        Sleep(100);
    }
}
void reverseplaylist(struct Node **current){
    int rev, choice;
    struct Node *node = *current;

    while(node->next != NULL) node = node->next;   // go to last song (d)
    playnode(node);

    printf("Do you want to continue the reverse playlist? Yes(1)/No(0)\n");
    scanf("%d", &rev);
    if(rev != 1){
        mciSendString("close music", NULL, 0, NULL);
        *current = node;
        return;
    }

    while(node->prev != NULL){
        printf("\n1. Next song (reverse)\n");
        printf("2. Go to play menu\n");
        choice = waitforchoice();

        if(choice == 2){
            mciSendString("close music", NULL, 0, NULL);
            break;
        }
        // choice == 1 (pressed) or 0 (song ended): both go to the previous node
        node = node->prev;
        playnode(node);
    }
    if(node->prev == NULL) printf("Reached the first song of the playlist\n");
    *current = node;
}
//sort playlist
int comparesongs(struct song *a, struct song *b, int by) {
    if (by == 1) return _stricmp(a->songname, b->songname);
    if (by == 2) return _stricmp(a->mood, b->mood);
}

/* Remove one node from its current position */
void removeNode(struct linkedlist *list, struct Node *node)
{   struct Node* current = list->head;
    while(current->next!=NULL) current=current->next;
    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;
    else
        current = node->prev;

    node->prev = NULL;
    node->next = NULL;
}

void insertBefore(struct linkedlist *list, struct Node *node, struct Node *before)
{
    node->prev = before->prev;
    node->next = before;

    if (before->prev != NULL)
        before->prev->next = node;
    else
        list->head = node;

    before->prev = node;
}


void sortlist(struct linkedlist *list, int by)
{
    struct Node *current;
    struct Node *scan;
    struct Node *smallest;

    if (list->head == NULL || list->head->next == NULL)
        return;

    current = list->head;

    while (current != NULL)
    {
        smallest = current;
        scan = current->next;
        while (scan != NULL)
        {
            if (comparesongs(&scan->sdata,&smallest->sdata,by) < 0)
                smallest = scan;

            scan = scan->next;
        }

        if (smallest != current)
        {
            removeNode(list, smallest);
            insertBefore(list, smallest, current);
        }
        else
            current = current->next;
    }
}

void play(struct linkedlist *list,struct song data[15]){
    int i,ch,sno,by;
    struct Node *current = list->head;
    if(current==NULL){
        printf("The playlist is empty\n");
        return;
    }
    
        do{
            printf("Playing %s.....\n",current->sdata.songname);
             mciSendString("close music",NULL,0,NULL);
             mciSendString(current->sdata.command,NULL,0,NULL);
             mciSendString("play music",NULL,0,NULL);
            printf("1.Next song\n");
            printf("2.Previous song\n");
            printf("3.Insert a song after current song\n");
            printf("4.Delete current song\n");
            printf("5.Reverse the playlist\n");
            printf("6.Sort the node data of list\n");//incomplete
            printf("7.Stop playing and go back to menu\n");
            printf("Please enter your choice\n");
            scanf("%d",&ch);
            switch(ch){
                case 1: if(current->next!=NULL)
                        {
                         current=current->next;
                         break;
                        }
                         else
                         printf("This is the last song of the playlist\n");
                         break;
                case 2: if(current->prev!=NULL)
                        {
                        current=current->prev;
                        break;
                        }
                        else
                        printf("This is the first song\n");
                        break;
                case 3: for(i=0;i<9;i++)
                          printf("%d. | %s\n",i+1,data[i].songname);
                          printf("Enter the serial no. of the song to be added next\n");
                          scanf("%d",&sno);
                          insertsongaftercurrent(list,current,data[sno-1]);
                          printlist(list);
                        break;
                case 4: deleteNode(&current,list);
                        printlist(list);
                        if(current==NULL) ch=7;
                        mciSendString("close music",NULL,0,NULL);
                        break;
                case 5: reverseplaylist(&current);
                        break;
                case 6: // sort alphabet order
                      printf("Sort by: 1.Song name  2.Mood \nEnter choice: ");
                      scanf("%d",&by);
                      if (by >= 1 && by <= 2) {
                      sortlist(list, by);
                      current = list->head;
                      //changed = 1;
                      printf("Playlist sorted.\n");
                      printlist(list);
                     } else {
                     printf("Invalid choice\n");
                     }
                    break;
                case 7:mciSendString("close music",NULL,0,NULL);
                       printf("Going back to main menu\n");
                       break;
                default:printf("Invalid choice\n");
                    }
        }while(ch!=7);
    
}

int main(){
    int choice,contd;
    //song array database
     struct song data[15]={
        {"Senorita", "Romantic", "open \"C:\\Users\\home\\Downloads\\Senorita.mp3\" type mpegvideo alias music"}, 
        {"Dandelions", "Romantic", "open \"C:\\Users\\home\\Downloads\\Dandelions.mp3\" type mpegvideo alias music"},
        {"Nattunattu","feelgood","open \"C:\\Users\\home\\Downloads\\Naatu Naatu.mp3\" type mpegvideo alias music"},
        {"I Ain't Worried", "feelgood", "open \"C:\\Users\\home\\Downloads\\I Ain't Worried.mp3\" type mpegvideo alias music"},
        {"Muskaanein Jhooti Hai", "Sad", "open \"C:\\Users\\home\\Downloads\\Muskaanein Jhooti Hai.mp3\" type mpegvideo alias music"},
        {"Why this kolavari", "Sad","open \"C:\\Users\\home\\Downloads\\Why This Kolaveri Di Official.mp3\" type mpegvideo alias music"},
        {"Dynamite","feelgood","open \"C:\\Users\\home\\Downloads\\Dynamite.mp3\" type mpegvideo alias music"},
        {"Minnale Amaran", "Romantic","open \"C:\\Users\\home\\Downloads\\Minnale Amaran.mp3\" type mpegvideo alias music"},
        {"HeartAttack","Sad", "open \"C:\\Users\\home\\Downloads\\Demi Lovato - Heart Attack.mp3\" type mpegvideo alias music"}
     };
    printf("***PITCH PERFECT***\n");
     do{
        printf("-------Menu------\n"); 
        printf("1.Default playlist\n");
        printf("2.Create your own playlist\n");
        printf("3.Select playlist based on your mood\n");
        printf("4.Queue\n");
        printf("5.Pick a Song\n");
        printf("6.Exit\n");
        scanf("%d",&choice);

        switch(choice){
            case 1: defaultplaylist(data);
            break;
            case 2: createplaylist(data);
            break;
            case 3: moodplaylist(data);
            break;
            case 4: queue(data);
            break;
            case 5: pickasong(data);
            break;
            case 6: printf("Pay your next visit to ENT Specialist\n");
            return 0;
            break;
            default: printf("Invalid Choice\n");
        }
        printf("Do you want to continue?(1/0)");
        scanf("%d",&contd);
        
     }while(contd!=0);

     printf("Thankyou for visiting\n");
     return 0;
}

void defaultplaylist(struct song data[15]){
    int i;  
    struct linkedlist mixed;
    initlist(&mixed);
    srand(time(NULL));
    for(i=0;i<5;i++)
     insertNode(&mixed, data[rand()%9]);
     printf("**MIXED PLAYLIST**\n");
     printlist(&mixed);
     play(&mixed,data);
}

void moodplaylist(struct song data[15]){
    int choice,i;
    printf("Pick a theme among the following\n");
    printf("1.Romantic\n");
    printf("2.Feel-good\n");
    printf("3. Sad\n");
    printf("Enter choice: ");
    scanf("%d",&choice);

    switch(choice){
        case 1: {struct linkedlist rom;
                initlist(&rom);
                for(i=0; i<9; i++){
                   if(strcmp(data[i].mood, "Romantic")==0) insertNode(&rom, data[i]);
                }
                play(&rom,data);
                }
        break;
        case 2: {struct linkedlist feelgood;
                initlist(&feelgood);
                for(i=0; i<9; i++){
                   if(strcmp(data[i].mood, "feelgood")==0) insertNode(&feelgood, data[i]);
                }
                play(&feelgood,data);
                }
        break;
        case 3: {struct linkedlist sad;
                initlist(&sad);
                for(i=0; i<9; i++){
                   if(strcmp(data[i].mood, "Sad")==0) insertNode(&sad, data[i]);
                }
                play(&sad,data);
                }
        break;
        default: printf("Invalid Choice\n");
        break;       
    }
}

void createplaylist(struct song data[15]){
    int i,ch,sno;
    char name[50];
    struct linkedlist mylist;
    initlist(&mylist);
    //displaying database
    printf("***Song Catalog***\n");
    for(i=0; i<9; i++)
    printf("%d. | %s\n",(i+1),data[i].songname);
    
    printf("Please enter the name of your playlist: ");
    while((getchar())!='\n' && getchar()!=EOF);
    fgets(name, sizeof(name), stdin);
    
    do{
        printf("1. Add song to playlist\n");
        printf("2. Stop adding\n");
        printf("Enter your choice: ");
        scanf("%d",&ch);

        switch(ch){
            case 1: {printf("Select the song serial number\n");
                    scanf("%d",&sno);
                    insertNode(&mylist, data[sno-1]);
                    printf("The playlist : %s\n",name);
                    printlist(&mylist);
                    }
            break;
            case 2:printf("\nCreated Successfully!\n"); 
                   printf("%s\n",name);
            break;
            default: printf("Invalid choice\n");
            break;
        }
    }while(ch!=2);
    printf("*Playing your playlist*: %s",name);
    printlist(&mylist);
    play(&mylist,data);
}

void pickasong(struct song data[15]){
    int i, sno, ch, res;
//display
for(i=0; i<9; i++)
    printf("%d. | %s\n",(i+1),data[i].songname);

printf("***Pick a song****\n");
scanf("%d",&sno);
printf("Playing %s....\n",data[sno-1].songname);
        mciSendString("close music",NULL,0,NULL);
        mciSendString(data[sno-1].command,NULL,0,NULL);
        mciSendString("play music",NULL,0,NULL);
         printf("1. Pause\n");
         printf("2. Stop\n");
         printf("Enter your choice: ");
         scanf("%d",&ch);

    if(ch == 1){
        mciSendString("pause music",NULL,0,NULL);

        printf("Song paused.\n");
        printf("Do you want to resume? Yes(1)/No(0)\n");
        scanf("%d",&res);

        if(res == 1){
            mciSendString("resume music",NULL,0,NULL);
        }
        else{
            mciSendString("close music",NULL,0,NULL);
            return;
        }
    }
    else if(ch == 2){
        mciSendString("close music",NULL,0,NULL);
        return;
       }
        mciSendString("close music",NULL,0,NULL);
        printf("Playback completed successfully.\n");
}

void queue(struct song data[15]){
    
    struct linkedlist mylist;
    initlist(&mylist);
    int i, sno, ch;

    // 1. Display available songs
    printf("\n--- Available Songs ---\n");
    for (i = 0; i < 9; i++) {
        printf("%d. | %s\n", i + 1, data[i].songname);
    }

    // 2. Select initial song to play immediately
    printf("\nPick a song to start playing: ");
    if (scanf("%d", &sno) != 1 || sno < 1 || sno > 9) {
        printf("Invalid choice.\n");
        return;
    }

    // Add initial song and set current pointer
    insertNode(&mylist, data[sno - 1]);
    struct Node *current = mylist.head;

    // Start playing the initial song right away
    mciSendString("close music", NULL, 0, NULL);
    mciSendString(current->sdata.command, NULL, 0, NULL);
    mciSendString("play music", NULL, 0, NULL);

    do {
        printf("\nNow Playing: %s\n", current->sdata.songname);
        printf("1. Add song to play NEXT (Queue next)\n");
        printf("2. Go to next song in queue\n");
        printf("3. View current queue\n");
        printf("4. Stop playback & return to menu\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                // Show song catalog
                printf("\n--- Song Catalog ---\n");
                for (i = 0; i < 9; i++) {
                    printf("%d. | %s\n", i + 1, data[i].songname);
                }
                printf("Enter serial no. to queue next: ");
                scanf("%d", &sno);

                if (sno >= 1 && sno <= 9) {
                    // Inserts immediately after the currently playing node
                    insertsongaftercurrent(&mylist, current, data[sno - 1]);
                    printf("Added '%s' right after current song.\n", data[sno - 1].songname);
                } else {
                    printf("Invalid song number!\n");
                }
                break;

            case 2:
                // Advance to next song in the queue
                if (current->next != NULL) {
                    current = current->next;
                    mciSendString("close music", NULL, 0, NULL);
                    mciSendString(current->sdata.command, NULL, 0, NULL);
                    mciSendString("play music", NULL, 0, NULL);
                } else {
                    printf("No more songs queued after this one!\n");
                }
                break;

            case 3:
                // Print remaining linked list queue
                printf("\n--- Upcoming Queue ---\n");
                printlist(&mylist);
                break;

            case 4:
                mciSendString("close music", NULL, 0, NULL);
                printf("Playback stopped.\n");
                break;

            default:
                printf("Invalid option!\n");
                break;
        }

    } while (ch != 4);
}
