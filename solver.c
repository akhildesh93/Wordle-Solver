// Runs the wordle solver -- you can either specify a word for it to search
// towards, like this:
// $ ./solver <secret word>
//
// Or if you don't specify a secret word, then it will supply you with guesses
// and ask for the feedback from a different instance of wordle.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "search_util.h"

// Returns true if the guess is an exact match with the secret word, but
// more importantly, fills in the result with the following:
// - 'x' goes in a slot if the corresponding letter in the guess does not appear
//   anywhere in the secret word
// - 'y' goes in a slot if the corresponding letter in the guess appears in the
//   secret word, but not in the corresponding slot
// - 'g' goes in a slot if the corresponding letter in the guess appears in the
//   corresponding slot in the secret word.
//   You can assume that result points at enough memory for a string of length
//   5. (ie, at least 6 bytes long)
bool score_guess(char *secret, char *guess, char *result) {
  for (int i = 0; i < 5; i++) {
    result[i] = 'x';
  }

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (guess[i] == secret[j]) {
        result[i] = 'y';
      }
    }
  }

  for (int i = 0; i < 5; i++) {
    if (guess[i] == secret[i]) {
      result[i] = 'g';
    }
  }
  return strncmp(secret, guess, 5) == 0;
}

// Returns an array of strings (so, char **), where each string contains a word
// from the specified file. The file is assumed to contain 5-letter words, one
// per line.
// Also, this function sets the value pointed at by *num_words to be the number
// of words read.
// This will need to allocate enough memory to hold all of the char* pointers --
// so you will keep track of the size of your char** array and then use realloc
// to make the array larger over time, so that you have enough space for the
// dynamically-growing array of char *.
// Use fopen to open the input file for reading,
// strdup (or strndup) to make copies of each word read from that file, and
// fclose to close the file when you are done reading from it.
// Each element of the array should be a single five-letter word,
// null-terminated.
char **load_vocabulary(char *filename, size_t *num_words) {
  char **out = NULL;
  size_t n = 0;
  char buf[10];
  size_t array_size = 1;
  out = (char **)calloc(sizeof(char *), array_size);
  FILE *infile = fopen(filename, "r");
  while (fgets(buf, 10, infile) != NULL) {
    n++;
    if (n >= array_size) {
      array_size *= 2;
      out = (char **)realloc(out, sizeof(char *) * array_size);
    }
    out[n - 1] = strndup(buf, 6);
    out[n - 1][5] = '\0';
  }
  fclose(infile);
  *num_words = n;
  return out;
}


int main(int argc, char **argv) {
  char **vocabulary;
  size_t num_words;
  int num_guesses = 0;
  char *secret = NULL;
  char *guess = NULL;

  char result[10] = {0};
  bool success = false;
  size_t removed;

  if (argc == 2) {
    secret = argv[1];
  }
  vocabulary = load_vocabulary("vocabulary.txt", &num_words);

  do {
    free(guess);
    guess = get_guess(vocabulary, num_words);

    if (guess == NULL) {
      printf("ran out of ideas?\n");
      break;
    }
    num_guesses++;
    printf("GUESS #%d: %s\n", num_guesses, guess);

    if (secret != NULL) {
      success = score_guess(secret, guess, result);
    } else {
      bool sensible_result = false;

      while (!sensible_result) {
        printf("please enter result as 5 characters (g,y,x): ");
        fgets(result, 10, stdin);
        result[5] = '\0';
        sensible_result = true;
        for (int i = 0; i < 5; i++) {
          if(result[i] != 'g' && result[i] != 'y' && result[i] != 'x') {
            sensible_result = false;
          }
        }
      }
      success = (strncmp(result, "ggggg", 5) == 0);
    }

    if (!success) {
      // If we didn't get it right, filter down the vocabulary!
      for (int i = 0; i < 5; i++) {
        if (result[i] == 'x') {
          // only remove words containing this letter if it doesn't occur
          // elsewhere in the word -- ie, you might have guessed the letter
          // twice, but it only occurs once. bit of a weird edge case, but this
          // can happen. This handles the behavior or the official wordle, which
          // marks letters as gray if you guess them twice but they occur once.
          char letter = guess[i];
          bool non_gray_elsewhere = false;
          for (int j = 0; j < 5; j++) {
            char other_letter = guess[j];
            if ((j != i) && (other_letter == letter) && (result[i] != 'x')) {
              non_gray_elsewhere = true;
            }
          }
          if (!non_gray_elsewhere) {
            printf("filtering with gray letter: %c\n", guess[i]);
            removed = filter_vocabulary_gray(guess[i], vocabulary, num_words);
            printf("removed %lu words.\n", removed);
          }
        } else if (result[i] == 'y') {
          printf("filtering with yellow letter: %c\n", guess[i]);
          removed =
              filter_vocabulary_yellow(guess[i], i, vocabulary, num_words);
          printf("removed %lu words.\n", removed);
        } else if (result[i] == 'g') {
          printf("filtering with green letter: %c\n", guess[i]);
          removed = filter_vocabulary_green(guess[i], i, vocabulary, num_words);
          printf("removed %lu words.\n", removed);
        }
      }
    }
  } while (!success);

  if (success) {
    printf("correct! got it in %d guesses!\n", num_guesses);
  } else {
    printf("oh no, could not guess it -- maybe outside the vocabulary?\n");
  }

  free(guess);
  free_vocabulary(vocabulary, num_words);

  return 0;
}
