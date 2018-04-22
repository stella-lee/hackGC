/*The Positive Interview Experience Generator: pieG*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define sentence_max 100
#define word_max 50

typedef struct{
  char word [word_max];
  int indicator;
  int matches;
}word_t;

int file_to_array(char * filename, char *** arr);
                                                  
//http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)InputOutput.html
/* return the next character from stdin without consuming it */
int peekchar(char x){
  int c;
  c = getchar();
  if (c == x)
    return 0;
  ungetc(c, stdin); /* puts it back */
  return c;
}

int isBiased(word_t sentence[sentence_max], int sentence_length,char ** pos_words,char ** neg_words, int glossary_len){
  int pos_count = 0;
  int neg_count = 0;
  for (int i = 0; i<sentence_length; i++){
    for (int j = 0; j<glossary_len; j++){
      if (memcmp(sentence[i].word, pos_words[j], 5) == 0){
        pos_count++;
        sentence[i].indicator = 1;
        break;
      }
      else if (memcmp(sentence[i].word, neg_words[j], 5) == 0){
        neg_count++;
        sentence[i].indicator = -1;
        break;
      }
      else {
        sentence[i].indicator = 0;
      }
    }
  }
  printf("Number of promotional words used: %d\nNumber of preventional words used: %d\n", pos_count, neg_count);
  return pos_count-neg_count; 
}

int findchar(char * string, char x){
  int counter = 0;
  for (int i=0; i<strlen(string); i++){
    if (string[i] == x){
      counter++;
    }
  }
  return counter;
}

int findQuestion(word_t sentence[sentence_max], int sentence_length, char ** questions, int question_len, int question_matches[]){
  for (int i=0; i<sentence_length; i++){
    if (sentence[i].indicator >= 0){
      for (int j=0; j<question_len; j++){        
        char* arr = NULL;
        int parsed_len = findchar(questions[j], ' ');
        word_t parsed[parsed_len+1];
        for (int k=0; k<=parsed_len; k++){
          char * token;
          char val [strlen(questions[j])+1];
          strcpy(val, questions[j]);
          if(arr==NULL){
            token = strtok_r(val, " ", &arr);
          }
          else{
            token = strtok_r(NULL, " ", &arr);
          }
          if(token == NULL){
            break;
          }
          strcpy(parsed[k].word, token);
        }
        for (int m=0; m<=parsed_len; m++){
          if (memcmp(sentence[i].word, parsed[m].word, 4)==0){
            question_matches[j]++;
            break;
          }
        }
      }
    }
  }
  int max = 0;
  int max_pos = 0;
  for (int i=0; i<question_len; i++){
    if (question_matches[i]>=max){
      max = question_matches[i];
      max_pos = i;
    }
  }
  return max_pos;
}
            
int main(){
  char ** neg_words;
  char ** pos_words;
  char ** questions;
  int glossary_len = file_to_array ("/home/guggilla/hackGC/positive-words.txt", &pos_words);
  file_to_array ("/home/guggilla/hackGC/negative-words.txt", &neg_words);
  int question_len = file_to_array ("/home/guggilla/hackGC/possible-questions.txt", &questions);
  int question_matches[sentence_max] = {0};
  word_t sentence[sentence_max];
  printf("Enter a question:\n");
  int counter = 0;
  while (peekchar('\n')){
    scanf(" %s", sentence[counter].word);
    counter++;
  }
  int last_char = (strlen(sentence[counter-1].word)- 1);
  if (sentence[counter-1].word[last_char] == '?'){
    sentence[counter-1].word[last_char] = '\0';
  }
  int sentence_length = counter;
  for (int i=0; i<sentence_length; i++){
    for (int j=0; j<strlen(sentence[i].word); j++){
      sentence[i].word[j] = tolower(sentence[i].word[j]);
    }    
  }
  int positivity = isBiased(sentence, sentence_length, pos_words, neg_words, glossary_len);
  if (positivity < 1){
    int alternative = findQuestion(sentence, sentence_length, questions, question_len, question_matches);
    printf("Try this question instead: %s\n",questions[alternative]);
  }
  else
    printf ("The question you entered is fine, thank you!\n");
  return 0;
}

int file_to_array(char * filename, char *** arr){
  FILE* file;
  file = fopen(filename, "r");
  int newLineLen = 0;
  while(feof(file) == 0){
    if(fgetc(file) == '\n'){
      newLineLen++;
    }
  }
  fclose(file);
  file = fopen(filename, "r");
  *arr = malloc(newLineLen*sizeof(char*));
  size_t dummy;
  for(int i = 0; i < newLineLen; i++){
    (*arr)[i] = NULL;
    getdelim(&((*arr)[i]), &dummy, '\n', file);
    int len = strlen((*arr)[i]);
    char temp[len+1];
    strcpy(temp, (*arr)[i]);
    temp[len-1] = '\0';
    strcpy((*arr)[i],temp);
  }
  fclose(file);
  return newLineLen;
}

//end
