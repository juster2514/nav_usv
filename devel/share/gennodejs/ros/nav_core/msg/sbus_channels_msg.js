// Auto-generated. Do not edit!

// (in-package nav_core.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class sbus_channels_msg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.channels_value = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('channels_value')) {
        this.channels_value = initObj.channels_value
      }
      else {
        this.channels_value = new Array(16).fill(0);
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type sbus_channels_msg
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Check that the constant length array field [channels_value] has the right length
    if (obj.channels_value.length !== 16) {
      throw new Error('Unable to serialize array field channels_value - length must be 16')
    }
    // Serialize message field [channels_value]
    bufferOffset = _arraySerializer.uint16(obj.channels_value, buffer, bufferOffset, 16);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type sbus_channels_msg
    let len;
    let data = new sbus_channels_msg(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [channels_value]
    data.channels_value = _arrayDeserializer.uint16(buffer, bufferOffset, 16)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 32;
  }

  static datatype() {
    // Returns string type for a message object
    return 'nav_core/sbus_channels_msg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '2e090f1a68ca526969f62f98dbfe335b';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    std_msgs/Header header
    uint16[16] channels_value
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    string frame_id
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new sbus_channels_msg(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.channels_value !== undefined) {
      resolved.channels_value = msg.channels_value;
    }
    else {
      resolved.channels_value = new Array(16).fill(0)
    }

    return resolved;
    }
};

module.exports = sbus_channels_msg;
