Author: Jacob Everett (jae0204)
Class:  CSCE3530.002
Instructor: Dr. Robin Pottathuparambil

Make file instructions

To compile:  make
To run: .\wcserver <port number>
        .\client   <port number>

	
Usage instructions

When client connects to server, server will send "Input string:" message to client. Enter a string (up to 1024 chars), 
press return, and the server will process your string and return:
1) your string with the first letter of each word capitalized 
2)the total number of words
3) the total number of letters
4) the total number of vowels

After processing your string, the server will resend the "Input string:" prompt and will process strings 
until a 'quit' message is received,at which point the connection will be closed.


