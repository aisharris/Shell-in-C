#include "headers.h"

char** tokenize(char* input, int* count) //assuming it doesnt end with a special char(except " and &)
{
    //first remove any weird spacing like consecutive spaces and tabs, or just tabs and make it a space

    //the array of strings to return:
    char** strings = malloc(sizeof(char*)*MAX_INPUT_WORDS);
    for(int i = 0; i < MAX_INPUT_WORDS; i++)
    {
        strings[i] = malloc(MAX_WORD_LENGTH);
    }

    int index = 0;          //index of array of strings
    int last_added = 0;     //last added character in the input index
    int quote_flag = 0;     //make sure to ignore letters in quotes
    int returned = 1;

    int len = strlen(input);

    for(int i = 0; i < len; i++)
    {
        if(quote_flag == 0 && input[i] == 34)
        {
            quote_flag = 1;
            //add previous section excluding this char
            if(last_added != i)
            {
                AddToStringsWithoutThisChar(strings, input, last_added, index, i, count);
                index++;
                last_added = i;                                         //not sure ??? not sure
                if(strcmp(strings[index - 1], " ") == 0 || strcmp(strings[index - 1], "\t") == 0 || strcmp(strings[index - 1], "") == 0)
                {
                    //printf("on no its a space D:\n");
                    strcpy(strings[index - 1], "");
                    index = index - 1;
                    count--;
                }
            }
            continue;
        }
        else if(quote_flag == 1 && input[i] == 34)
        {
            quote_flag = 0;
            //add previous section including this cahr
            if(i != last_added)
            {
                AddToStringsWithThisChar(strings, input,  last_added,  index,  i, count);
                index++;
                last_added = i + 1;  
                if(strcmp(strings[index - 1], " ") == 0 || strcmp(strings[index - 1], "\t") == 0 || strcmp(strings[index - 1], "") == 0)
                {
                    //printf("on no its a space D:\n");
                    strcpy(strings[index - 1], "");
                    index = index - 1;
                    count--;
                }
            }
            continue;
        }

        if(quote_flag != 1)
        {
            returned = OutsideQuotes(strings, input, &last_added, &index, &i, &len, count);
            //printf("null?\n");
            if(returned == 0)
            {
                return NULL;
            }
            else if(returned == -1)
            {
                // printf("continued\n");
                continue;
            }
        }
        else
        {
            continue;
        }
    }
    return strings;
}   

int OutsideQuotes(char** strings, char* input, int* last_added, int* index, int* i, int* len, int* count)
{

    if(input[*i] == 32 || input[*i] == 9)                         //check if tab or space
    {
        SpaceorTab(strings, input, last_added, index, i, count);
    }

    else if(input[*i] == 60 || input[*i] == 62 || input[*i] == 38 || input[*i] == 124 || input[*i] == 59) //check if special characters (<, >, &, |, ;)
    {
        int returned = SpecialCharaters(strings, input, last_added, index, i, len, count);
        if(returned == 0)
        {
            //printf("SC rreturned 0\n");
            return 0;
        }
        else if(returned == -1)
        {
            return -1;
        }
    }

    // //if its not a hyphen or letter or number show error
    // else if(input[*i] < 45 || (input[*i] > 46 && input[*i] < 48) || (input[*i] > 57 && input[*i] < 65) || (input[*i] > 90 && input[*i] < 97) || input[*i] > 122)
    // {
    //     printf("INVALID CHARACTER: '%c'\n", input[*i]);
    //     return 0;
    // } 
    
    else //letter
    {
        if(*i > 0 && (input[*i - 1] == 60 || input[*i - 1] == 62 || input[*i - 1] == 38 || input[*i - 1] == 124 || input[*i - 1] == 59))
        {
            AddToStringsWithoutThisChar(strings, input, *last_added, *index, *i, count);
            *index = *index + 1;
            *last_added = *i;                                         //not sure ??? not sure
            if(strcmp(strings[*index - 1], " ") == 0 || strcmp(strings[*index - 1], "\t") == 0 || strcmp(strings[*index - 1], "") == 0)
            {
                //printf("on no its a space D:\n");
                strcpy(strings[*index - 1], "");
                *index = *index - 1;
                *count = *count - 1;
            }
        }
    }
    
    if(*i == *len - 1)
    {
        int returned = LastLetterandAmpersand(strings, input, last_added, index, i, len, count);
        if(returned == 0)
        {
            return 0;
        }
    }

    return 1;
}

void AddToStringsWithoutThisChar(char** strings, char* input, int last_added, int index, int i, int* count)
{
    char* string = malloc(sizeof(char)*((i - last_added) + 1));
    strncpy(string, &input[last_added], i - last_added);
    string[i - last_added] = '\0';
    //printf("added: '%d', '%d'\n", i, string[0]);
    //printf("stupid fucking index: %d\n", index);
    strings[index] = string;
    *count = *count + 1;
}

void AddToStringsWithThisChar(char** strings, char* input, int last_added, int index, int i, int* count)
{
    char* string = malloc(sizeof(char)*((i + 1 - last_added) + 1));
    strncpy(string, &input[last_added], i + 1 - last_added);
    string[i + 1 - last_added] = '\0';
    //printf("added: '%d'\n", string[0]);
    strings[index] = string;
    *count = *count + 1;
}

void SpaceorTab(char** strings, char* input, int* last_added, int* index, int* i, int* count)
{
    if(*i > 0 && input[*i - 1] != 32 && input[*i - 1] != 9)      // check if previous also tab or space
    {
        //add everything excluding this char
        
        if(*i != *last_added) //no point adding nothing(i - last_added = null)
        {
            //printf("i: %d, la: %d\n", *i, *last_added);
            AddToStringsWithoutThisChar( strings, input, *last_added, *index,  *i, count);
            *index = *index  +1;
            *last_added = *i + 1;  
            if(strcmp(strings[*index - 1], " ") == 0 || strcmp(strings[*index - 1], "\t") == 0 || strcmp(strings[*index - 1], "") == 0)
            {
                //printf("on no its a space D:\n");
                strcpy(strings[*index - 1], "");
                *index = *index - 1;
                *count = *count - 1;
            }
            //printf("new la: %d\n", *last_added);
        }
        else
        {
            *last_added = *i + 1;
        }
        

        if(strcmp(strings[*index - 1], " ") == 0)
        {
            //printf("removed " "\n");
            strcpy(strings[*index - 1], "");
            *index = *index - 1;
        }
        //dont add spaces to strings
    }
    else
    {
        *last_added = *i + 1;
    }
}

int SpecialCharaters(char** strings, char* input, int* last_added, int* index, int* i, int* len, int* count)
{
    if(input[*i] != 62)
    {
        if(*last_added != *i)
        {
            //printf("i: %d, la: %d\n", *i, *last_added);

            AddToStringsWithoutThisChar(strings, input, *last_added, *index, *i, count);
            *index = *index + 1;
            *last_added = *i;                                         //not sure ??? not sure
            if(strcmp(strings[*index - 1], " ") == 0 || strcmp(strings[*index - 1], "\t") == 0 || strcmp(strings[*index - 1], "") == 0)
            {
                //printf("on no its a space D:\n");
                strcpy(strings[*index - 1], "");
                *index = *index - 1;
                *count = *count - 1;
            }
        }
        //printf("%d\n", strings[index - 1][0]);
        if(*index > 0 && (strings[*index - 1][0] == 60 || strings[*index - 1][0] == 38 || strings[*index - 1][0] == 124 || strings[*index - 1][0] == 59)) //if immediately before was another special character
        {
            fprintf(stderr, RED"ERROR: invalid input syntax\n"reset);
            return 0;
        }

    }
    else
    {
        if(*i < *len - 2 && input[*i + 1] == 62 && input[*i + 2] == 62) //if >>> is entered
        {
            fprintf(stderr, RED"invalid input syntax\n"reset);
            return 0;
        }

        if(*i > 0 && input[*i - 1] == 62)
        {
            //printf("'%s', '%s'\n", strings[0], strings[1]);
            return -1; // >>
        }
        else
        {
            AddToStringsWithoutThisChar(strings, input, *last_added, *index, *i, count);
            *index = *index + 1;
            *last_added = *i;  

            if(strcmp(strings[*index - 1], " ") == 0 || strcmp(strings[*index - 1], "\t") == 0 || strcmp(strings[*index - 1], "") == 0)
            {
                //printf("on no its a space D:\n");
                strcpy(strings[*index - 1], "");
                *index = *index - 1;
                *count = *count - 1;
            } 
        }
        
        if(*index > 0 && (strings[*index - 1][0] == 60 || strings[*index - 1][0] == 38 || strings[*index - 1][0] == 124 || strings[*index - 1][0] == 59))
        {
            fprintf(stderr, RED"invalid input syntax\n"reset);
            return 0;
        }
     
    }
    // if(*index > 0 && strings[*index - 1][0] == 60 || strings[*index - 1][0] == 38 || strings[*index - 1][0] == 124 || strings[*index - 1][0] == 59)
    // {
    //     printf("INVALID INPUT\n");
    //     return 0;
    // }

    //check if theres a space and symbol
    return 1;

}

int LastLetterandAmpersand(char** strings, char* input, int* last_added, int* index, int* i, int* len, int* count)
{
    if(input[*i] != 38) //check if not ampersand
    {
        
        AddToStringsWithThisChar( strings, input, *last_added, *index, *i, count);
        *index = *index + 1;
        if(strcmp(strings[*index - 1], " ") == 0 || strcmp(strings[*index - 1], "\t") == 0 || strcmp(strings[*index - 1], "") == 0)
        {
            //printf("on no its a space D:\n");
            strcpy(strings[*index - 1], "");
            *index = *index - 1;
            *count = *count - 1;
        }
        
    }
    else
    {
        AddToStringsWithoutThisChar(strings, input, *last_added, *index, *i, count);
        *index = *index + 1;
        *last_added = *i;  

        if(strcmp(strings[*index - 1], " ") == 0 || strcmp(strings[*index - 1], "\t") == 0 || strcmp(strings[*index - 1], "") == 0)
        {
            //printf("on no its a space D:\n");
            strcpy(strings[*index - 1], "");
            *index = *index - 1;
            *count = *count - 1;
        }

        char* str = malloc(sizeof(char)*2);
        str[0] = 38;
        str[1] = '\0';
        strings[*index] = str;
        *index = *index - 1;
        // printf("added: '%s'\n", str);
        *count = *count + 1;
    }
    return 1;
}

