# c-leetcode-assignment

Kate Zraly, Justin Viacrusis

## Context

In this partner assignment, we used C to tackle a leetcode-style question. We search a given input text file for given keywords, and translate it into the 'BCIT language'. For example, if we detect "I", we would replace it with "Aei". Another example is if we detect "student", we would replace it with "worker". Once we detect and translate all keywords, we need to store the words on lines that are exactly 25 characters long. If a word makes the line longer than 25 characters, we would shift it to the next line. Not only must lines be 25 characters long, we need to justify the words in each line:

<pre>
|This is not justified.   |

|This     is    justified.| 
</pre>

## Our Work

C was a challenging language to work with. What seemed straight-forward with a language like Python, posed problems with C because of memory storage. Our solution was to implement a LinkedList that had Node structs for each word.

Justin handled the input file, converted to the BCIT language, and stored the result in a LinkedList. Kate determined how many words would appear on each line based on the 25 character per line rule, justified each line, and wrote the result to an output file. We considered edge cases for file input/output and for each aspect of the question.
