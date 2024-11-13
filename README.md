***************************************************

step 1:
#To compile the server program
make -f makeserver

step 2:
#To compile the client program
make -f makeclient

step 3:
#Get two terminals(for the the server and client)

step 4:
#Run the server in one terminal
./server.out 127.0.0.1

#Run the client in 2nd terminal
./client.out 127.0.0.1

>>> Instructions:

1.Server should be in listen state, when running the client.
2.To download the file, the user should enter the relevant file number.
3.After downloading a file from the server, the client disconnects. To download another file, the client must reconnect to the server.
4.You can connect multiple clients at same time.
5.All downloaded file save in "download_path" folder.

************************************************************
