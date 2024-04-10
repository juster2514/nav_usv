; Auto-generated. Do not edit!


(cl:in-package nav_core-msg)


;//! \htmlinclude sbus_channels_msg.msg.html

(cl:defclass <sbus_channels_msg> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (channels_value
    :reader channels_value
    :initarg :channels_value
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 16 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass sbus_channels_msg (<sbus_channels_msg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <sbus_channels_msg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'sbus_channels_msg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name nav_core-msg:<sbus_channels_msg> is deprecated: use nav_core-msg:sbus_channels_msg instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <sbus_channels_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nav_core-msg:header-val is deprecated.  Use nav_core-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'channels_value-val :lambda-list '(m))
(cl:defmethod channels_value-val ((m <sbus_channels_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nav_core-msg:channels_value-val is deprecated.  Use nav_core-msg:channels_value instead.")
  (channels_value m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <sbus_channels_msg>) ostream)
  "Serializes a message object of type '<sbus_channels_msg>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) ele) ostream))
   (cl:slot-value msg 'channels_value))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <sbus_channels_msg>) istream)
  "Deserializes a message object of type '<sbus_channels_msg>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:setf (cl:slot-value msg 'channels_value) (cl:make-array 16))
  (cl:let ((vals (cl:slot-value msg 'channels_value)))
    (cl:dotimes (i 16)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:aref vals i)) (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<sbus_channels_msg>)))
  "Returns string type for a message object of type '<sbus_channels_msg>"
  "nav_core/sbus_channels_msg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'sbus_channels_msg)))
  "Returns string type for a message object of type 'sbus_channels_msg"
  "nav_core/sbus_channels_msg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<sbus_channels_msg>)))
  "Returns md5sum for a message object of type '<sbus_channels_msg>"
  "2e090f1a68ca526969f62f98dbfe335b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'sbus_channels_msg)))
  "Returns md5sum for a message object of type 'sbus_channels_msg"
  "2e090f1a68ca526969f62f98dbfe335b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<sbus_channels_msg>)))
  "Returns full string definition for message of type '<sbus_channels_msg>"
  (cl:format cl:nil "std_msgs/Header header~%uint16[16] channels_value~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'sbus_channels_msg)))
  "Returns full string definition for message of type 'sbus_channels_msg"
  (cl:format cl:nil "std_msgs/Header header~%uint16[16] channels_value~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <sbus_channels_msg>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'channels_value) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 2)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <sbus_channels_msg>))
  "Converts a ROS message object to a list"
  (cl:list 'sbus_channels_msg
    (cl:cons ':header (header msg))
    (cl:cons ':channels_value (channels_value msg))
))
