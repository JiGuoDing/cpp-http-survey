#!/bin/bash
g++ -o executable/httplib_server httplib_server.cpp -lpthread
g++ -o executable/httplib_client httplib_client.cpp -lpthread