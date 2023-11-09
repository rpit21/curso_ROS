#ifndef _ROS_SERVICE_changecontrolledjoints_h
#define _ROS_SERVICE_changecontrolledjoints_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mastering_robot_description
{

static const char CHANGECONTROLLEDJOINTS[] = "mastering_robot_description/changecontrolledjoints";

  class changecontrolledjointsRequest : public ros::Msg
  {
    public:
      typedef int32_t _c1_type;
      _c1_type c1;
      typedef int32_t _c2_type;
      _c2_type c2;

    changecontrolledjointsRequest():
      c1(0),
      c2(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_c1;
      u_c1.real = this->c1;
      *(outbuffer + offset + 0) = (u_c1.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_c1.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_c1.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_c1.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->c1);
      union {
        int32_t real;
        uint32_t base;
      } u_c2;
      u_c2.real = this->c2;
      *(outbuffer + offset + 0) = (u_c2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_c2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_c2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_c2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->c2);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_c1;
      u_c1.base = 0;
      u_c1.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_c1.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_c1.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_c1.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->c1 = u_c1.real;
      offset += sizeof(this->c1);
      union {
        int32_t real;
        uint32_t base;
      } u_c2;
      u_c2.base = 0;
      u_c2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_c2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_c2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_c2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->c2 = u_c2.real;
      offset += sizeof(this->c2);
     return offset;
    }

    virtual const char * getType() override { return CHANGECONTROLLEDJOINTS; };
    virtual const char * getMD5() override { return "283ee42b1d51b895e07842f960389784"; };

  };

  class changecontrolledjointsResponse : public ros::Msg
  {
    public:

    changecontrolledjointsResponse()
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

    virtual const char * getType() override { return CHANGECONTROLLEDJOINTS; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class changecontrolledjoints {
    public:
    typedef changecontrolledjointsRequest Request;
    typedef changecontrolledjointsResponse Response;
  };

}
#endif
