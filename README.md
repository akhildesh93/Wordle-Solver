Wordle Solver:

Overview of game:

    6 guesses for a 5 letter word
    for each guess, signifies letter accuracy with Green (correct), Yellow (wrong position), or Gray (not in word)
    
search_util.c:

    get_guess() returns optimum guess, based on words known and the words in vocabulary
    score_letter() and score_word() finds score of letter/word based off vocabulary list for optimum letter/word
    filter_vocabulary green/yellow/gray() filters vocabulary based on letters known/misplaced/incorrect
    
solver.c:

    loads vocabulary
    calls guess function in search_util.c
    returns green, yellow, or gray for letter guesses
  
