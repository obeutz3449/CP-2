# Developer Log (DEVLOG.md)       
## Monopoly Board Simulator (Spring 2026)    

---
### Entry 1
**Date:** 2026-03-17  
I imported the templates from canvas and created the repository.
### Entry 2
**Date:** 2026-03-18    
I moved `tokenize` after the helper methods I used and I filled in the functions. To make it work, I had to rework `isOperator`.
### Entry 3
**Date:** 2026-03-19    
I made a simple function, `isValidInfix`, to determine if something is in valid infix format. I did not account for parenthesis yet though.
### Entry 4
**Date:** 2026-03-21    
I fixed the function from last time to use parenthesis correctly. Also fixed a strange indentation error.
### Entry 5
**Date:** 2026-03-22    
I wrote `isValidPostfix`. To do this, I reworked `isOperator` and created helper function `isNumber`.
### Entry 6
**Date:** 2026-03-23   
Rather than later make my functions deal with implied multiplication, I reworked `tokenize` to read implied multiplication as explicit multiplication.
### Entry 7
**Date:** 2026-03-24     
I started work on the first of many `infixToPostfix` solutions.
### Entry 8
**Date:** 2026-03-25   
In order to continue working on `infixToPostfix`, I created helper methods to create a `subVector` for me.    
This iteration worked by splitting, from left to right, the P from the MD from the AS of PEMDAS and using recursivity to do the rest.
### Entry 9
**Date:** 2026-03-26     
I filled out the `ArrayStack` template with methods.
### Entry 10
**Date:** 2026-03-29    
I made a lot of small changes to a lot of pieces of the code so they worked better. I spent a significant amount of time figuring out how I could get `subVector` to be more memory efficient.
### Entry 11
**Date:** 2026-03-30    
I first moved a lot of things around and started work on the `precedence` method, which I decided would find the index of the last mathematical operation to be performed in an infix expression.    
I made `subVector` a bit nicer for large input sizes but also managed to mess a lot of things up in the process. This took me a long time to fix.     
`isValidInfix` was getting caught on miniscule input sizes and close parenthesis, so I added guardrails.       
I made `infixToPostfix` utilize `precedence` rather than deal with PEMDAS itself which fixed a few OOP errors.    
I wrote `evaluatePostfix` which became easy when I broke it up with recursion.
### Entry 12
**Date:** 2026-03-31    
I attached `DEVLOG` to the repo.   
The edge case of the input being an expression that started with an expression with parenthesis and ends with one two would break it, so I tweaked `infixToPostfix`.   
`tokenize` was also at fault for not recognizing implied multiplication between two expressions surrounded parenthesis.        
Unfortunately, it still didn't work for nested parenthesis, so I cut the method down to only removing one layer at a time.         
Next, I diagnosed a problem with `precedence` that, when given subtraction and addition, depending on the order, adds the two numbers before subtracting them. To remedy this, I had precedence choose from right to left instead.    
Next, I noticed my program treats expressions with an extra open parenthesis as valid and runs into an error trying to evaluate them, so I added `&& priority == 0` to the return statement.     
I also made inputs consisting of just a number or of nothing at all valid as randomly infix or postfix. Unfortunately, the seed is really predictable, so it's always postfix.   
After a bit of googling, I found out how to throw exceptions like in java, so I added safety to division by 0.    
Finally, I noticed that the spirit of the instructions would not like my recursive `evaluatePostfix` functions so I remade it with two `ArrayStack`s and a new helper method to get the integer value represented by a `Token`.