HW 7 Design Doc

int score_letter(char letter, char **vocabulary, size_t num_words):

    - create int count
    - loop through list of strings in vocab with num_words:
        - loop through 5 letters of each string:
            - if the letter from string == letter, add 1 to count
    - return count

int score_word(char *word, int *letter_scores):
    - create int score
    - loop from 0 to 25:
        - loop through 5 letters of word:
            - if the letter = letter from first loop, add letter score to score
    - return score

size_t filter_vocabulary_gray(char letter, char **vocabulary, size_t num_words):
    create size_t filtered
    loop through list of strings in vocab with num_words:
        loop through 5 letters of each string:
            if letter from string == letter:
                free the string and set to null
                add 1 to filtered
    return filetered

size_t filter_vocabulary_yellow(char letter, int position, char **vocabulary, size_t num_words):
    create size_t filtered
    create bools for in_word and at_pos
    loop through list of strings in vocab with num_words:
        set bools to false
        loop through 5 letters of each string:
            if letter from string == letter:
                set in_word to true
                if in correct position, set at_pos to true
        if in_word is false or at_pos is true:
            free the string and set to null
            add 1 to filtered
    return filtered

size_t filter_vocabulary_green(char letter, int position, char **vocabulary, size_t num_words):
    create size_t filtered
    loop through list of strings in vocab with num_words:
        if the letter in the string position != letter:
            free the string and set to null
            add 1 to filtered
    return filtered
