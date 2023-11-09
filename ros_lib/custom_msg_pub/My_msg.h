#ifndef _ROS_custom_msg_pub_My_msg_h
#define _ROS_custom_msg_pub_My_msg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/PoseStamped.h"

namespace custom_msg_pub
{

  class My_msg : public ros::Msg
  {
    public:
      typedef const char* _id_type;
      _id_type id;
      typedef int32_t _number_type;
      _number_type number;
      typedef float _number2_type;
      _number2_type number2;
      typedef geometry_msgs::PoseStamped _pos_type;
      _pos_type pos;

    My_msg():
      id(""),
      number(0),
      number2(0),
      pos()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_id = strlen(this->id);
      varToArr(outbuffer + offset, length_id);
      offset += 4;
      memcpy(outbuffer + offset, this->id, length_id);
      offset += length_id;
      union {
        int32_t real;
        uint32_t base;
      } u_number;
      u_number.real = this->number;
      *(outbuffer + offset + 0) = (u_number.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_number.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_number.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_number.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->number);
      union {
        float real;
        uint32_t base;
      } u_number2;
      u_number2.real = this->number2;
      *(outbuffer + offset + 0) = (u_number2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_number2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_number2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_number2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->number2);
      offset += this->pos.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_id;
      arrToVar(length_id, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_id; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_id-1]=0;
      this->id = (char *)(inbuffer + offset-1);
      offset += length_id;
      union {
        int32_t real;
        uint32_t base;
      } u_number;
      u_number.base = 0;
      u_number.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_number.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_number.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_number.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->number = u_number.real;
      offset += sizeof(this->number);
      union {
        float real;
        uint32_t base;
      } u_number2;
      u_number2.base = 0;
      u_number2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_number2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_number2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_number2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->number2 = u_number2.real;
      offset += sizeof(this->number2);
      offset += this->pos.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return "custom_msg_pub/My_msg"; };
    virtual const char * getMD5() override { return "9a0dc2f1ddd85bbaf2d25f85b83885d8"; };

  };

}
#endif
