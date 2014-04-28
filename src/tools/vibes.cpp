#include "vibes.h"
#include <sstream>
#include <cstdlib>

namespace vibes
{    
  FILE *channel=0;
  std::string current_fig="default";
  /**
  * Connects to the named pipe, not implemented yet.
  */
  void connect()
  {
      // Retrieve user-profile directory from envirnment variable
      char * user_dir = getenv("USERPROFILE"); // Windows
      if (!user_dir)
          user_dir = getenv("HOME"); // POSIX
      if (user_dir)
      { // Environment variable found, connect to a file in user's profile directory
          std::string file_name(user_dir);
          file_name.append("/.vibes.json");
          connect(file_name);
      }
      else
      { // Connect to a file in working directory
          connect("vibes.json");
      }
  }
  
  void connect(const std::string &fileName)
  {
    channel=fopen(fileName.c_str(),"a");
  }
  
  void disconnect()
  {
    fclose(channel);
  }
  
  void figure(const std::string &figureName)
  {
    std::string msg;
    current_fig = figureName;
    msg ="{\"action\":\"new\",\"figure\":\""+figureName+"\"}\n\n";
    fputs(msg.c_str(),channel);
  }
  
  void figure()
  {
    figure(current_fig);
  }
  void clear()
  {
    clear(current_fig);
  }
  
  void clear(const std::string &figureName)
  {
    std::string msg;
    msg="{\"action\":\"clear\",\"figure\":\""+figureName+"\"}\n\n";
    fputs(msg.c_str(),channel);
  }

  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, const std::string &figureName, const std::string &color)
  {
    std::stringstream msg;
    msg<<"{\"action\":\"draw\",\"figure\":\""+figureName+"\",\"shape\":{\"type\":\"box\",\"color\":\""<<color<<"\",\"bounds\":["<<x_lb<<","<<x_ub<<","<<y_lb<<","<<y_ub<<"]}}\n\n";
    fputs(msg.str().c_str(),channel);
  }
  
  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, const std::string &color)
  {
    drawBox(x_lb,x_ub,y_lb,y_ub,current_fig,color);
  }
  
  void saveImage(const std::string &fileName)
  {
      saveImage(fileName, current_fig);
  }

  void saveImage(const std::string &fileName, const std::string &figureName)
  {
      std::string msg;
      msg="{\"action\":\"export\",\"figure\":\""+figureName+"\",\"file\":\""+fileName+"\"}\n\n";
      fputs(msg.c_str(),channel);
  }

}
