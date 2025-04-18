Peer-to-Peer terminal messenger via TCP connection.

Securing by XOR with predefined key

How to use:

Server:

`kali@root:~/Downloads$ git clone https://github.com/horusgit-sh/SecureMessenger.git`

`kali@root:~/Downloads$ cd SecureMessenger`

`kali@root:~/Downloads/SecureMessenger$ gcc server.c -o server`

`kali@root:~/Downloads/SecureMessenger$ ./server 4444 ` (or whatever port u want)

Client:

`kali@root:~/Downloads$ git clone https://github.com/horusgit-sh/SecureMessenger.git`

`kali@root:~/Downloads$ cd SecureMessenger`

`kali@root:~/Downloads/SecureMessenger$ gcc client.c -o client`

`kali@root:~/Downloads/SecureMessenger$ ./client {IP} {PORT} `

