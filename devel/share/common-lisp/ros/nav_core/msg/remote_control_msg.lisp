; Auto-generated. Do not edit!


(cl:in-package nav_core-msg)


;//! \htmlinclude remote_control_msg.msg.html

(cl:defclass <remote_control_msg> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (ch_1
    :reader ch_1
    :initarg :ch_1
    :type cl:fixnum
    :initform 0)
   (ch_2
    :reader ch_2
    :initarg :ch_2
    :type cl:fixnum
    :initform 0)
   (key_value
    :reader key_value
    :initarg :key_value
    :type cl:fixnum
    :initform 0))
)

(cl:defclass remote_control_msg (<remote_control_msg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <remote_control_msg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'remote_control_msg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name nav_core-msg:<remote_control_msg> is deprecated: use nav_core-msg:remote_control_msg instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <remote_control_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nav_core-msg:header-val is deprecated.  Use nav_core-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'ch_1-val :lambda-list '(m))
(cl:defmethod ch_1-val ((m <remote_control_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nav_core-msg:ch_1-val is deprecated.  Use nav_core-msg:ch_1 instead.")
  (ch_1 m))

(cl:ensure-generic-function 'ch_2-val :lambda-list '(m))
(cl:defmethod ch_2-val ((m <remote_control_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nav_core-msg:ch_2-val is deprecated.  Use nav_core-msg:ch_2 instead.")
  (ch_2 m))

(cl:ensure-generic-function 'key_value-val :lambda-list '(m))
(cl:defmethod key_value-val ((m <remote_control_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nav_core-msg:key_value-val is deprecated.  Use nav_core-msg:key_value instead.")
  (key_value m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <remote_control_msg>) ostream)
  "Serializes a message object of type '<remote_control_msg>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let* ((signed (cl:slot-value msg 'ch_1)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ch_2)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'key_value)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <remote_control_msg>) istream)
  "Deserializes a message object of type '<remote_control_msg>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ch_1) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ch_2) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'key_value) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<remote_control_msg>)))
  "Returns string type for a message object of type '<remote_control_msg>"
  "nav_core/remote_control_msg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'remote_control_msg)))
  "Returns string type for a message object of type 'remote_control_msg"
  "nav_core/remote_control_msg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<remote_control_msg>)))
  "Returns md5sum for a message object of type '<remote_control_msg>"
  "8c560d5e5383d834df3135169b349720")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'remote_control_msg)))
  "Returns md5sum for a message object of type 'remote_control_msg"
  "8c560d5e5383d834df3135169b349720")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<remote_control_msg>)))
  "Returns full string definition for message of type '<remote_control_msg>"
  (cl:format cl:nil "std_msgs/Header header~%int16 ch_1~%int16 ch_2~%int16 key_value~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'remote_control_msg)))
  "Returns full string definition for message of type 'remote_control_msg"
  (cl:format cl:nil "std_msgs/Header header~%int16 ch_1~%int16 ch_2~%int16 key_value~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <remote_control_msg>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     2
     2
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <remote_control_msg>))
  "Converts a ROS message object to a list"
  (cl:list 'remote_control_msg
    (cl:cons ':header (header msg))
    (cl:cons ':ch_1 (ch_1 msg))
    (cl:cons ':ch_2 (ch_2 msg))
    (cl:cons ':key_value (key_value msg))
))
