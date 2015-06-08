// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include <iostream>
#include "Socket.h"

using namespace std;

class ServerSocket : public Socket
{
 public:

  ServerSocket ( int port );
  ServerSocket ()
  {
if (DEBUG)
{
  cout << "new server waiting for query..." << endl;
}
  };
  virtual ~ServerSocket();

  const ServerSocket& operator << ( const std::string& ) const;
  const ServerSocket& operator >> ( std::string& ) const;

//  const ServerSocket& operator << ( const char* ) const;
//  const ServerSocket& operator >> ( char* ) const;

  void accept ( ServerSocket& );

};


#endif

