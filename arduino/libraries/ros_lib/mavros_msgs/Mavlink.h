#ifndef _ROS_mavros_msgs_Mavlink_h
#define _ROS_mavros_msgs_Mavlink_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class Mavlink : public ros::Msg
  {
    public:
      std_msgs::Header header;
      bool is_valid;
      uint8_t len;
      uint8_t seq;
      uint8_t sysid;
      uint8_t compid;
      uint8_t msgid;
      uint16_t checksum;
      uint8_t payload64_length;
      uint64_t st_payload64;
      uint64_t * payload64;

    Mavlink():
      header(),
      is_valid(0),
      len(0),
      seq(0),
      sysid(0),
      compid(0),
      msgid(0),
      checksum(0),
      payload64_length(0), payload64(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_is_valid;
      u_is_valid.real = this->is_valid;
      *(outbuffer + offset + 0) = (u_is_valid.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->is_valid);
      *(outbuffer + offset + 0) = (this->len >> (8 * 0)) & 0xFF;
      offset += sizeof(this->len);
      *(outbuffer + offset + 0) = (this->seq >> (8 * 0)) & 0xFF;
      offset += sizeof(this->seq);
      *(outbuffer + offset + 0) = (this->sysid >> (8 * 0)) & 0xFF;
      offset += sizeof(this->sysid);
      *(outbuffer + offset + 0) = (this->compid >> (8 * 0)) & 0xFF;
      offset += sizeof(this->compid);
      *(outbuffer + offset + 0) = (this->msgid >> (8 * 0)) & 0xFF;
      offset += sizeof(this->msgid);
      *(outbuffer + offset + 0) = (this->checksum >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->checksum >> (8 * 1)) & 0xFF;
      offset += sizeof(this->checksum);
      *(outbuffer + offset++) = payload64_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < payload64_length; i++){
      union {
        uint64_t real;
        uint32_t base;
      } u_payload64i;
      u_payload64i.real = this->payload64[i];
      *(outbuffer + offset + 0) = (u_payload64i.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_payload64i.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_payload64i.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_payload64i.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->payload64[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_is_valid;
      u_is_valid.base = 0;
      u_is_valid.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->is_valid = u_is_valid.real;
      offset += sizeof(this->is_valid);
      this->len =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->len);
      this->seq =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->seq);
      this->sysid =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->sysid);
      this->compid =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->compid);
      this->msgid =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->msgid);
      this->checksum =  ((uint16_t) (*(inbuffer + offset)));
      this->checksum |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->checksum);
      uint8_t payload64_lengthT = *(inbuffer + offset++);
      if(payload64_lengthT > payload64_length)
        this->payload64 = (uint64_t*)realloc(this->payload64, payload64_lengthT * sizeof(uint64_t));
      offset += 3;
      payload64_length = payload64_lengthT;
      for( uint8_t i = 0; i < payload64_length; i++){
      union {
        uint64_t real;
        uint32_t base;
      } u_st_payload64;
      u_st_payload64.base = 0;
      u_st_payload64.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_payload64.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_payload64.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_payload64.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_payload64 = u_st_payload64.real;
      offset += sizeof(this->st_payload64);
        memcpy( &(this->payload64[i]), &(this->st_payload64), sizeof(uint64_t));
      }
     return offset;
    }

    const char * getType(){ return "mavros_msgs/Mavlink"; };
    const char * getMD5(){ return "6dd71a38b8541fdc2de89a548c7dbc2f"; };

  };

}
#endif