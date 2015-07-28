// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

const int DEBUG = 0;

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send_string ( const std::string ) const;
  int recv_string ( std::string& ) const;

  bool send (const char* send_buff, int size) const;
  int recv(char* recv_buff, int size ) const;


  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }
 
  int m_sock;

 private:

  sockaddr_in m_addr;


};


#endif

