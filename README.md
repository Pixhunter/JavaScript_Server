# JavaScript_Server
Server with JavaScript interpretator

This is the server that main function is to interpretate & run simple JavaScript code with poor functions.



==================================================
<br>  WHAT's my JavaScript interpretator cans???
<br>
<br>  TYPES: Integer, Float, String, Boolean
  ALSO:  Arrays
  OPERATIONS: + - * /
  LOGIC OPERATIONS: == != === !==  
                     >  <  >=  <= 
  OPERATIONS WITH STRING: + / * 
  COMMENTS: // /*...*/
  KEY WORDS: var "
             if {} else {}
             while {}
             continue
             break 

  U don't need the ; but program skip it :)
=================================================

Here are all the server files with the javascript interpreter:
(interpreter capabilities are given by srun.sh during compilation and they are in the js.cpp header and also some lines upper :D)

- server.cpp - the server itself, opens a file with an interpreter (js.cpp) via fork and waits for its execution, intercepting its std::out messages and outputting them in response to the client (processes many requests as a "live" server) + forwards to the client parameters from the server such as request length, connection type, etc.
- js.cpp - interpreter, outputs to std::out the string itself, which it processes and the result
- srun.sh - compiles server.cpp and js.cpp files and runs server.cpp, plus simulates connecting to a "real" server when you need to wait for the connection and it fails the first time (fictitiously through printing messages with a bash delay)
- client.cpp - client, sends a GET request with a string with parameters to the server and waits for a response
- script.txt - a file with a script, the processing of which is requested by the client from the server via a GET request with the file address (by default, this is the script.txt file in the same directory as the server, if not, it will return an error)



Fibonacci and factorial are implemented in the script.txt file that shows the correct working, you also can play with it.
Implemented an example of the operation of loops on the output of Fibonacci and factorial numbers through while and if / else loops, and both a single if without {} and with else c / without {} work (using comments, the text in the script is separated, \n is not implemented) .
