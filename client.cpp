/*
	Author of the starter code
    Yifan Ren
    Department of Computer Science & Engineering
    Texas A&M University
    Date: 9/15/2024
	
	Please include your Name, UIN, and the date below
	Name: Nathan Brooks
	UIN: 632005286
	Date: 9/28/2024
*/
#include "common.h"
#include "FIFORequestChannel.h"

using namespace std;


int main (int argc, char *argv[]) {
	int opt;
	int p = -1;
	double t = -1;
	int e = -1;
	string filename = "";
	int m = MAX_MESSAGE;
	bool newchan = false;
	vector<FIFORequestChannel*> channels;

	//Add other arguments here
	while ((opt = getopt(argc, argv, "p:t:e:f:m:c")) != -1) {
		switch (opt) {
			case 'p':
				p = atoi (optarg);
				break;
			case 't':
				t = atof (optarg);	
				break;
			case 'e':
				e = atoi (optarg);
				break;
			case 'f':
				filename = optarg;
				break;
			case 'm':
				m = atoi (optarg);
				break;
			case 'c':
				newchan = true;
				break;
		}
	}

	//Task 1:
	//Run the server process as a child of the client process

    FIFORequestChannel cont_chan("control", FIFORequestChannel::CLIENT_SIDE);
	channels.push_back(&cont_chan);

	//Task 4:
	//Request a new channel
	if (newchan) {
		// send newchannel request to the server
		MESSAGE_TYPE nc = NEWCHANNEL_MSG;
		cont_chan.cwrite(&nc, sizeof(MESSAGE_TYPE));
		// create a variable to hold the name
		// create a response from the server
		// call the FIFORequestChannel constructor with the name from the server
		// push the new channel into the vector
	}

	FIFORequestChannel chan = *(channels.back());

	//Task 2:
	//Request data points	
	if (p == -1 || t == -1 || e == -1){
		char buf[MAX_MESSAGE];	
		datamsg x(p, t, e);
		
		memcpy(buf, &x, sizeof(datamsg));
		chan.cwrite(buf, sizeof(datamsg));
		double reply;
		chan.cread(&reply, sizeof(double));
		cout << "For person " << p << ", at time " << t << ", the value of ecg " << e << " is " << reply << endl;
	} else if (p != -1){
		// loop over first 1000 lines
		// send requests for ecgs 1 and 2
		//write line to received/x1.csv
	}
	
	//Task 3:
	//Request files
	filemsg fm(0, 0);
	string fname = filename;
	
	int len = sizeof(filemsg) + (fname.size() + 1);
	char* buf2 = new char[len];
	memcpy(buf2, &fm, sizeof(filemsg));
	strcpy(buf2 + sizeof(filemsg), fname.c_str());
	chan.cwrite(buf2, len);

	char* buf3 = (char*)m; // create buffer of size buff capacity (m)
	// loop over the segments in the file file_length / buff capacity(m).
	// set filemsg instance
	filemsg* file_req = (filemsg*)buf2;
	file_req->offset = //set offset in the file
	file_req->length = // set the length
	// send the request (buf2)
	chan.cwrite(buf2, len);
	// receive the response
	// cread into buf3 length file_req->length
	chan.cread(buf3, file_req->length);
	// write buf3 into file: received/filename

	delete[] buf2;
	delete[] buf3;

	// if necessary, close and delete the new channel
	if (newchan) {
		// do closes and deletes
	}

	__int64_t file_length;
	chan.cread(&file_length, sizeof(__int64_t));
	cout << "The length of " << fname << " is " << file_length << endl;
	
	//Task 5:
	// Closing all the channels
    MESSAGE_TYPE q = QUIT_MSG;
    chan.cwrite(&q, sizeof(MESSAGE_TYPE));
}
