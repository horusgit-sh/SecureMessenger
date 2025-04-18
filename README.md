Peer-to-Peer terminal messenger via TCP connection.

Securing by XOR with predefined key

How to use:

Server:

`kali@root:~/Downloads$ git clone https://github.com/horusgit-sh/SecureMessenger.git`

`kali@root:~/Downloads$ cd SecureMessenger`

`kali@root:~/Downloads/SecureMessenger$ gcc messenger.c -o messenger`

`kali@root:~/Downloads/SecureMessenger$ ./messenger 4444 ` (or whatever port u want)

Client:

`kali@root:~/Downloads$ nc {server IP} {server PORT}`

