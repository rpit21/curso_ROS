#ifndef _ROS_SERVICE_changescale_h
#define _ROS_SERVICE_changescale_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mastering_robot_description
{

static const char CHANGESCALE[] = "mastering_robot_description/changescale";

  class changescaleRequest : public ros::Msg
  {
    public:
      typedef float _s_type;
      _s_type s;

    changescaleRequest():
      s(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_s;
      u_s.real = this->s;
      *(outbuffer + offset + 0) = (u_s.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_s.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_s.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_s.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->s);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_s;
      u_s.base = 0;
      u_s.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_s.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_s.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_s.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->s = u_s.real;
      offset += sizeof(this->s);
     return offset;
    }

    virtual const char * getType() override { return CHANGESCALE; };
    virtual const char * getMD5() override { return "bc08dbcb40b2ba4b05703f0913420f07"; };

  };

  class changescaleResponse : public ros::Msg
  {
    public:

    changescaleResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return CHANGESCALE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class changescale {
    public:
    typedef changescaleRequest Request;
    typedef changescaleResponse Response;
  };

}
#endif
