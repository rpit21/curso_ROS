#ifndef _ROS_SERVICE_demo_srv_h
#define _ROS_SERVICE_demo_srv_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mastering_ros_demo_services
{

static const char DEMO_SRV[] = "mastering_ros_demo_services/demo_srv";

  class demo_srvRequest : public ros::Msg
  {
    public:
      typedef float _x_type;
      _x_type x;
      typedef float _y_type;
      _y_type y;

    demo_srvRequest():
      x(0),
      y(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_x;
      u_x.real = this->x;
      *(outbuffer + offset + 0) = (u_x.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_x.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_x.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_x.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->x);
      union {
        float real;
        uint32_t base;
      } u_y;
      u_y.real = this->y;
      *(outbuffer + offset + 0) = (u_y.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_y.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_y.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_y.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->y);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_x;
      u_x.base = 0;
      u_x.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_x.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_x.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_x.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->x = u_x.real;
      offset += sizeof(this->x);
      union {
        float real;
        uint32_t base;
      } u_y;
      u_y.base = 0;
      u_y.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_y.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_y.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_y.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->y = u_y.real;
      offset += sizeof(this->y);
     return offset;
    }

    virtual const char * getType() override { return DEMO_SRV; };
    virtual const char * getMD5() override { return "ff8d7d66dd3e4b731ef14a45d38888b6"; };

  };

  class demo_srvResponse : public ros::Msg
  {
    public:
      typedef float _mod_type;
      _mod_type mod;

    demo_srvResponse():
      mod(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_mod;
      u_mod.real = this->mod;
      *(outbuffer + offset + 0) = (u_mod.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_mod.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_mod.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_mod.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->mod);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_mod;
      u_mod.base = 0;
      u_mod.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_mod.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_mod.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_mod.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->mod = u_mod.real;
      offset += sizeof(this->mod);
     return offset;
    }

    virtual const char * getType() override { return DEMO_SRV; };
    virtual const char * getMD5() override { return "f27c1d54385e85fd066dba092d48f7a6"; };

  };

  class demo_srv {
    public:
    typedef demo_srvRequest Request;
    typedef demo_srvResponse Response;
  };

}
#endif
