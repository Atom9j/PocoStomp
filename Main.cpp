/*
 PocoStomp - a STOMP (Simple Text Oriented Messaging Protocol) client using PoCo
----------------------------------------------------
Copyright (c) 2012 Elias Karakoulakis <elias.karakoulakis@gmail.com>

SOFTWARE NOTICE AND LICENSE

PocoStomp is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

Thrift4OZW is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with PocoStomp.  If not, see <http://www.gnu.org/licenses/>.

for more information on the LGPL, see:
http://en.wikipedia.org/wiki/GNU_Lesser_General_Public_License
*/

// 
// Main.cpp: a demo STOMP client
// (c) 2011 Elias Karakoulakis <elias.karakoulakis@gmail.com>
//

#include <string>
#include <sstream>
#include <iostream>
#include "unistd.h"


// PocoStromp
#include "PocoStomp.h"

static STOMP::PocoStomp* stomp_client;
static string*          notifications_topic = new string("/queue/zwave/monitor");

// -------------------------------------------------------
// a callback for any STOMP Frames in subscribed channels
// -------------------------------------------------------
STOMP::pfnOnStompMessage_t subscription_callback(STOMP::Frame* _frame) {
	cout << "--Incoming STOMP Frame--" << endl;
	cout << "  Headers:" << endl;
	STOMP::hdrmap::iterator it;
	for ( it = _frame->headers.begin() ; it != _frame->headers.end(); it++ )
	    cout << (*it).first << " => " << (*it).second << endl;
	//
	cout << "  Body: (size: " << _frame->body.size() << " chars):" << endl;
	cout << _frame->body << endl;
	return 0;
}

// -----------------------------------------
int main(int argc, char *argv[]) {
// -----------------------------------------
    string  stomp_host = string("localhost");
    int     stomp_port = 61613;

    try {
    	// connect to STOMP server
        stomp_client = new STOMP::PocoStomp(stomp_host, stomp_port);
        stomp_client->connect();

        // subscribe to a channel
        stomp_client->subscribe(*notifications_topic, (STOMP::pfnOnStompMessage_t) &subscription_callback);

        // construct a headermap
        STOMP::hdrmap headers;
        headers["header1"] = string("value1").c_str();
        headers["header2"] = string("value2").c_str();
        headers["header3"] = string("value3").c_str();
        string body = string("this is the main message body");

        // add an outgoing message to the queue
        stomp_client->send(*notifications_topic, headers, body);
        sleep(1);
    } 
    catch (exception& e) 
    {
        cerr << "Error in PocoStomp: " << e.what() << "\n";
        return 1;
    } 
    
    return 0;
}
