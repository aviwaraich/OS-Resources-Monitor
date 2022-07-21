#include <stdio.h> //Printf()
#include <stdlib.h> //fopen()
#include <string.h> //strncmp()
#include <sys/sysinfo.h> //Memory Info
#include <stdlib.h> //Malloc()
#include <unistd.h> //sleep()
#include <utmp.h> //session info
#include <sys/utsname.h> //System Infomrmation
#include <stdbool.h> //bool

float byte_to_gb(float bytes)
{
    /**
     This Function coverts Float bytes and returns float GB
     */
    return (bytes/(1024 * 1024 * 1024)); // Formula for converting bytes to GB
}

double cputeller()
{
    /**
     This function reads the /proc/stat and read the noumber cores there are and then prints it for user
     and it retunrs the Double total CPU Utilization at that time
     */
    printf("\033[0G----------------------------------------------------------\n");
    printf("                   ### CPU Information ###\n");
    printf("----------------------------------------------------------\n");
    printf("\033[93m");        //Bold and Yellow
    char line[1024]; //Variable to save each line of the file
    FILE *file = fopen("/proc/stat","r"); //opens the file /proc/stat and makes a pointer to it
    // Variable found in the file which will be used to find the CPU Utiliations
    long int User=0;
    long int Nice=0;
    long int System=0;
    long int Idle=0;
    long int Iowait=0;
    long int Irq=0;
    long int Softirq=0;
    int Cores=-1; //Stars the Number of Cores as -1
    fgets(line,sizeof(line),file); // Saves first line file in line
    while (strncmp(line, "cpu", 3)==0) //if the line contains CPU
    {
        if (Cores==0) //We only need first line of stats in the file
        {
            //saves the integar in file to the thier individual varible
            sscanf(line,"%*s%ld%ld%ld%ld%ld%ld%ld",&User,&Nice,&System,&Idle,&Iowait,&Irq,&Softirq);
        }
        Cores = Cores + 1; //Count the number of Cores
        fgets(line,sizeof(line),file); //keep going to next line in file and saving it
    }
    fclose(file); //when cores counted and variable saved close the file
    printf("Number of Cores: %d                                \n",Cores); //print the number of cores
    long int TotalCPU = User+Nice+System+Idle+Iowait+Irq+Softirq; //Save the total CPU
    long int IdleCPU = Idle+Iowait; //Save the Idle in CPU
    long int UsageCPU = TotalCPU - IdleCPU; //Save the total CPU Usaage Load without Idle in CPU
    double UtilizationCPU = ((double) UsageCPU/TotalCPU)*100; //Save the Ratio among the net use and the actual total
    printf("\033[91m");        //Red
    return UtilizationCPU; //Return total CPU Utiliation
}

void sessionteller(int curloc)
{
    /**
     This Function Inputs the cursor locationvand starts priting from there by ESCape Codes and
     prints the session of user in the each lab terminal
     */
    printf("\033[%d;0H----------------------------------------------------------\n",curloc);
    printf("                       ### Sessions ###\n");
    printf("----------------------------------------------------------\n");
    struct utmp *sessions; //Intialize sessions pointer to utmp from the utmp.h
    printf("Username | Device Name | Terminal Name |   Hostname\n");
    printf("\033[93m");        //Bold and Yellow
    while ((sessions=getutent()) != NULL) //getutent() returns a pointer to a structure sessions in the utmp file and it keeps going until the the NULL comes in
    {
        if(sessions->ut_type==USER_PROCESS) //Check if the ut_type is USER_PROCESS means we only need user information nothing else
        {
            printf("%s \033[10G|    %s \033[24G|  %s \033[40G| %s\n", sessions->ut_user,sessions->ut_line,sessions->ut_id,sessions->ut_host);
            // Prints the Username, Device Name of the user. Terminal Name of user and the Hostname of the user
        }
    }
    printf("\033[91m");        //Red
    endutent(); //closes the utmp file
}

void systemteller()
{
    /**
     The Function prints out the System Information such as: System Name , Machine Name , Version,
     Release and Architecture of the Terminal
     */
    struct utsname systeminformation; //Intialize systeminformation structture tag to utsname from the sys/utsname.h
    uname(&systeminformation); //Fills thesysteminformation structture tag from uname from sys/utsname.h
    printf("\033[0G----------------------------------------------------------\n");
    printf("                 ### System Information ###\n");
    printf("----------------------------------------------------------\n");
    printf("\033[93m");        //Bold and Yellow
    // Prints Out the all of the Information we need from the systeminformation filled by uname
    printf("System Name = %s\n", systeminformation.sysname);
    printf("Machine Name = %s\n", systeminformation.nodename);
    printf("Version = %s\n", systeminformation.release);
    printf("Release = %s\n", systeminformation.version);
    printf("Architecture = %s\n", systeminformation.machine);
    printf("\033[91m");        //Red
    printf("\033[0G----------------------------------------------------------\n");
}


float memoryteller(int iteration,bool graphics,float Past_Physical_Used)
{
    /**
     This Function inputs the iteration of the sample, check if graphics are true/false and inputs the the last Physical Use of the Memory
     and interms of the Return it returns the latest Physical Used  to save it as Past_Physical_Used.
     Function also prints the Physical Used,Physical Total ,Virtual Used, Virtual Total and if graphics is True then without
     Physical Used,Physical Total ,Virtual Used, Virtual Total and the grpahics of #* and :@
     */
    printf("\033[1m \033[91m");        //Bold and Red
    printf("\033[5;0H                       ### Memory ###\n");
    printf("----------------------------------------------------------\n");
    printf("Physical Used|Physical Total -- Virtual Used|Virtual Total\n");
    printf("\033[93m");        //Bold and Yellow
    struct sysinfo info; //Intialize info structture tag to sysinfo from the sys/sysinfo.h
    sysinfo (&info); //Fills info structture tag from sysinfo from sys/utsname.h
    float Physical_Used = byte_to_gb(info.totalram-info.freeram); //Save Physical_Used by converting from byte to gb
    float Physical_Total = byte_to_gb(info.totalram); //Save Physical_Total by converting from byte to gb
    float Virtual_Used = byte_to_gb((info.totalram-info.freeram) + (info.totalswap - info.freeswap)); //Save Virtual_Used by converting from byte to gb
    float Virtual_Total  = byte_to_gb(info.totalram + info.totalswap); //Save Virtual_Total by converting from byte to gb
    if (iteration==0)
    {
        Past_Physical_Used = Physical_Used; //Saving Physical_Used as Past_Physical_Used for the first itteration
    }
    printf("\033[%d;0H   %.2f GB   |   %.2f GB    --   %.2f GB   |  %.2f GB\n",iteration+8,Physical_Used,Physical_Total,Virtual_Used,Virtual_Total);
    // Prints the Memory info
    if(graphics) //If user wants graphics
    {
        int coloumn = 60; //Add Coloumn Space by 60 after Print of the Memory info
        float difference = Physical_Used-Past_Physical_Used; //Difference between last Physical_Used and now
        printf("\033[%d;%dH|\n",iteration+8,coloumn); //Move the Cursor by some coloumn and add "|"
        coloumn = coloumn + 1; //Add coloumn by 1 becase we added | in last line
        if (difference>=0) //If Difference Is Positive
        {
            for (double j=0.009; j<difference; j=j+0.01) //Add # while it is less than difference
            {
                printf("\033[%d;%dH#\n",iteration+8,coloumn);
                coloumn = coloumn + 1; //New Column each time
            }
            printf("\033[%d;%dH*   %.2f (%.2f)\n",iteration+8,coloumn,difference,Physical_Used);
            // Print * difference (Physical_Used)
        }
        else  //If Difference Is Negative
        {
            for (double j=0.009; j<(-1*(difference)); j=j+0.01) //Add : while it is less than difference
            {
                printf("\033[%d;%dH:\n",iteration+8,coloumn);
                coloumn = coloumn + 1; //New Column each time
            }
            printf("\033[%d;%dH@   %.2f (%.2f)\n",iteration+8,coloumn,difference,Physical_Used);
            // Print @ difference (Physical_Used)
        }
    }
    printf("\033[91m");        //Red
    return Physical_Used; //Return Physical_Used to save it as Past_Physical_Used
}

void CPU_graphics(float cputotal,int iteration)
{
    /**
     This Functions Input each Sample iteration and cputotal utilization CPU and it Prints "|" for cputotal
     */
    printf("\033[93m");        //Bold and Yellow
    printf("%d  ",iteration); //Print Space for the Line
    if (iteration>=10) //if iteration is 10 since 10 takes 2 coloums shift the cursor by -1
    {
        printf("\033[1D");
    }
    for (double j=0.15; j<cputotal; j=j+(j/1.5)-(j/2.0)) //itterating until cputotal is reached by some random reasomable formula
    {
        printf("|");
    }
    printf(" %.2f %%",cputotal); // At last just print the cputotal
    printf("\033[91m");        //Red
}

int main(int argc, char *argv[])
{
    /**
     The Main Function First Checks for the Flags and then Print The Utlization Tool accoridng to the flags
     */
    int Sample=10; //Start with Default 10 Sample
    int Delay=1; //Start with Default 1 Delay
    char charholder[1024]; //Char to Save the Flags
    bool graphics_check = false; //No --Grapics by Default
    bool system_check = false; //No --System by Default
    bool user_check = false; //No --User by Default
    bool all_check = true; //No argv means print all of them
    memset(charholder, '\0', sizeof(charholder)); //Make charholder '\0' so there is always a end
    for (int i=1; i<argc; i=i+1) //To check every argv[i]
    {
        if (strcmp(argv[i],"--system")==0) //Check if System Flag Makes System True and All False
        {
            system_check = true;
            all_check = false;
        }
        else if(strcmp(argv[i],"--user")==0) //Check if User Flag Makes User True and All False
        {
            user_check = true;
            all_check = false;
        }
        else if(strcmp(argv[i],"--graphics")==0 || strcmp(argv[i],"-g")==0) //Check if Graphics or g Flag Makes graphics = True
        {
            graphics_check = true;
        }
        else if(strncmp(argv[i],"--samples=",10)==0)  //Check if Flag is Sample and Save it
        {
            for(int j=10;j<=strlen(argv[i]);j=j+1)
            {
                charholder[j-10]=argv[i][j]; //Save the Last Digit of --samples=
            }
            Sample = atoi(charholder); //atoi turns string to int
            memset(charholder, '\0', sizeof(charholder)); //Work is done make charholder all '\0' again
        }
        else if(strncmp(argv[i],"--tdelay=",9)==0) //Check if Flag is Tdelay and Save it
        {
            for(int j=9;j<=strlen(argv[i]);j=j+1)
            {
                charholder[j-9]=argv[i][j]; //Save the Last Digit of --tdelay=
            }
            Delay = atoi(charholder); //atoi turns string to int
            memset(charholder, '\0', sizeof(charholder)); //Work is done make charholder all '\0' again
        }
        else if (argv[i]!= NULL && argv[i+1]!= NULL ) //Since Last 2 are Digits
        {
            Sample = atoi(argv[i]); //Save First as Sample
            Delay = atoi(argv[i+1]); //Save Second as Delay
            break;
        }
        else if (argv[i]!= NULL && argv[i+1]== NULL ) //Since the Last one is Digits
        {
            Sample = atoi(argv[i]); //Save First as Sample
            break;
        }
    }
    printf("\033[2J");        /*  clear the screen  */
    printf("\033[H");         /*  position cursor at top-left corner */
    printf("\033[1m \033[91m");        //Bold and Red
    printf("                   Monitor Tool\n");
    printf("                  Number of Samples: %d Times\n",Sample);
    printf("                  Sample Delay Timer: %d Seconds\n",Delay);
    printf("----------------------------------------------------------\n");
    float Past_Physical_Used = 0; //To Save the Memory Past Physical Used
    float CPU_Percent = 0; //To Save CPU Utilization Percentage
    for (int i=0; i<Sample; i=i+1) //Itterate over each Sample
    {
        if(all_check) //If User want everything
        {
            Past_Physical_Used = memoryteller(i,graphics_check,Past_Physical_Used); //Save the Latest Physical Used as Past Physical Used
            sessionteller(Sample+8); //Print the Session Information Starting Cursor from Samples + 8 (Other printf's)
            CPU_Percent = cputeller(); //Save the CPU Utilization Percentage
            printf("\033[93m");        //Bold and Yellow
            printf("Total CPU USE: %.2f %%",CPU_Percent);
            if(graphics_check) //If User want the Grpahics
            {
                printf("\033[%dB",i+1); //Go to Next Line
                CPU_graphics(CPU_Percent,i+1); //Make The CPU Grpahics print
            }
        }
        else if(system_check) //If User want --system
        {
            Past_Physical_Used = memoryteller(i,graphics_check,Past_Physical_Used); //Save the Latest Physical Used as Past Physical Used
            printf("\033[%d;0H",Sample+8); //Skip to Sample+8 Line
            CPU_Percent = cputeller(); //Save the CPU Utilization Percentage
            printf("\033[93m");        //Bold and Yellow
            printf("Total CPU USE: %.2f %%",CPU_Percent);
            if(graphics_check) //If User want the Grpahics
            {
                printf("\033[%dB",i+1); //moves cursor down i+1 lines
                CPU_graphics(CPU_Percent,i+1); //Make The CPU Grpahics print
            }
        }
        else if(user_check) //If User want --user
        {
            sessionteller(5); //Print the Session Information Starting Cursor from 5 (Other printf's)
            printf("\033[1A");
        }
        sleep (Delay); //Add the Delay user wants
    }
    printf("\n"); //Skip A Line before printing the systen Info
    printf("\033[91m");        //Bold and Yellow
    systemteller(); //At last print the System Information
    printf("\033[0m");        //UnBold and UnColor
    return 0;
}


