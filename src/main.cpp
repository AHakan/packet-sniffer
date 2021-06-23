#include <csignal>

#include "../inc/threads.h"

using namespace std;

void signalHandler( int signum ) {
  cout << "Ctrl+c pressed. Interrupt signal received." << endl;

  // cleanup and close up stuff here
  // terminate program

  exit(signum);
}

bool isValidThreadSize( int num )
{
    if( num>0 && num<=15)
        return true;
    return false;
}


int main( int argc, char *argv[] )
{
  signal( SIGINT, signalHandler );
  uint8_t threadSize;
  char * regex_pattern;

  if( argc==3 )
  {
    regex_pattern = argv[1];

    // Thread size must be between 1-15.
    if( isValidThreadSize( atoi( argv[2] ) ) )
      threadSize = atoi( argv[2] );
    else
      return 0;
  }
  else
  {
    cout << "Arguments error." << endl;
    return 0;
  }

  Threads *a = new Threads( threadSize, regex_pattern);

  return 0;
}

